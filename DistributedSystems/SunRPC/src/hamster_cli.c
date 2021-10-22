/**
 * @file	hamster_cli.c
 * @brief	Hamster management via text-based menu
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "../include/hamsterlib.h"



/**
 * If this program is compiled with -DUMMY_IPC and linked against
 * libhamster.a, then it can be used as a stand-alone, non-RPC program.
 * In this case, we have no use for the two functions hmstr_rpc_init()
 * and hmstr_rpc_terminate(), so we declare them as empty dummies here.
 * Otherwise, if linked against hamster_client.c, it makes uses of
 * sunRPC to talk to a server. In that latter case, hamster_client.c must
 * provide the two functions, so we declare them as external prototypes.
 */
#ifdef UMMY_RPC

void hmstr_rpc_init(char *hostname) {}
void hmstr_rpc_terminate(void) {}

#else

extern void hmstr_rpc_init(char *hostname);
extern void hmstr_rpc_terminate(void);

#include "hamster.h"
#endif


/**
 * @brief Debug support macros
 *
 * Output messages if @c verbose is higher than a given level.
 *
 * NOTE: Usage @c debug((<printf-params>)); i.e. use @e two brackets
 *       to enclose the printf parameter list!
 */
#ifdef EBUG
static int verbose = 0;
#define debug(x) do { printf x ; fflush(stdout);  } while (0)
#define debug1(x) do { if(verbose >= 1) { printf x ; fflush(stdout); } } while (0)
#define debug2(x) do { if(verbose >= 2) { printf x ; fflush(stdout); } } while (0)
#define debug3(x) do { if(verbose >= 3) { printf x ; fflush(stdout); } } while (0)
#define debug4(x) do { if(verbose >= 4) { printf x ; fflush(stdout); } } while (0)
#else
#define debug(x)
#define debug1(x)
#define debug2(x)
#define debug3(x)
#define debug4(x)
#endif

static void rtfm(char* progname)
{
	printf("Usage: %s {<Option>} <param1> \n", progname);
	printf("Function: Hamster C CLI Client\n");
	printf("Optionen:\n");
	printf("     -h {<hostname>}                - hostname of the server\n");
}

static void cli_commands() {
	printf("\nCommands:\n");
	printf("        q                           - quit\n");
	printf("        n                           - check in new hamster\n");
	printf("        b                           - collect hamsters\n");
	printf("        l                           - list all hamsters\n");
	printf("        s                           - show hamster status\n");
	printf("        f                           - feed hamster\n");
	printf("> ");
	fflush(stdout);
}


static void flushinput(void)
{
	while(getchar() != '\n')
		;	
}

/*
 * Helper function for reading a name
 */
static void getname_helper(char *name, size_t size)
{
	fflush(stdout);
	memset(name, 0, size);
	fgets(name, HMSTR_MAX_NAME, stdin);
	if(name[(size = (strlen(name)-1))] == '\n')
		name[size] = '\0';
}

/*
 * Helper function for reading a number
 */
static void getnumber_helper(int *num)
{
	fflush(stdout);
	while(scanf("%d", num) != 1)
	{
		printf("Not a valid number, please try again: ");
		flushinput();
	}
	flushinput();
}


/*
 * Error checking
 */
int check_error(int32_t rc, const char *funcname)
{
	if(HMSTR_ERR_IS_CATASTROPHIC(rc))
	{
		perror(funcname);
		exit(EXIT_FAILURE);
	}
	else if(rc < 0)
	{
		switch(rc)
		{
			case HMSTR_ERR_NAMETOOLONG:
				printf("In function %s: Error: the specified name is too long\n", funcname);
				break;
			case HMSTR_ERR_EXISTS:
				printf("In function %s: Error: a hamster by that owner/name already exists\n", funcname);
				break;
			case HMSTR_ERR_NOTFOUND:
				printf("In function %s: Error: A hamster or hamster owner could not be found\n", funcname);
				break;
			default:
				assert(0);	/* this should never happen */
				break;
		}
		return 1;
	}
	return 0;
}



int main(int argc, char*argv[])
{
	/*
	 * predefined hostname and port
	 */
	char *hostname = "localhost";
	int  cmd;
	int  ret;
	int  any = 0;
	char owner[HMSTR_MAX_NAME+2];
	char name[HMSTR_MAX_NAME+2];
	int  treats = 0;
	int  id = 0;
	int32_t status;
	int16_t price;
	struct hmstr_state state;

	/*
	 * parse command line arguments
	 */
    while ((cmd = getopt(argc, argv, "?h:v")) != -1)
    {
        switch(cmd)
        {
            case '?':
                rtfm(argv[0]);
                return EXIT_SUCCESS;
                break;
            case 'h':
                hostname = optarg;
                break;
#ifdef EBUG
            case 'v':
                ++verbose;
                break;
#endif
        }
    }

	/*
	 * connect to server
	 * 
	 * Invoke rpcgen-generated code here
	 */
	hmstr_rpc_init(hostname);

	/*
	 * Main Command loop
	 */
	 do {
		cli_commands();
		cmd = getchar();
		flushinput();
		switch (cmd) {
			case 'n':
				printf("checking in new hamster:\n");
				printf("        owner: ");
				getname_helper(owner, sizeof(owner));

				printf("        hamster name: ");
				getname_helper(name, sizeof(name));

				printf("        treats: ");
				getnumber_helper(&treats);

				ret = hmstr_new(owner, name, treats);

				if(!check_error(ret, "hmstr_new()"))
					printf("OK\n");

				break;
			case 'b':
				printf("collecting hamsters:\n");
				printf("        owner: ");
				getname_helper(owner, sizeof(owner));

				ret = hmstr_collect(owner);
				if(!check_error(ret, "hmstr_collect()"))
					printf("return code: %d\n", ret);

				break;
			case 'l':
				any = 0;
				status = -1;
				while((ret = hmstr_directory(&status, NULL, NULL)) >= 0)
				{
					id = ret;
					if(!any)
					{
						printf("====================================================================================================\n");
						printf("ID         Owner Name                      Hamster Name                    Price â‚¬      Treats left\n");
						any = 1;
					}
					treats = ret = hmstr_readentry(id, &owner[0], &name[0], &price);
					if(!check_error(ret, "hmstr_readentry()"))
						printf("%-10d %-32s%-32s%-12d %-12d\n", id, owner, name, price, treats);
				}
				if(ret == HMSTR_ERR_NOTFOUND)
				{
					if(!any)
						printf("No hamsters matching criteria found\n");
				}
				else if(!check_error(ret, "hmstr_directory()"))
					printf("OK\n");
			
				break;
			case 's':
				printf("        ID: ");
				getnumber_helper(&id);
				ret = hmstr_howsdoing(id, &state);
				if(!check_error(ret, "hmstr_howsdoing()"))
				{
					printf("return code: %d\n", ret);
					printf("         Hamster State\n");
					printf("==================================\n");
					printf("treats left: %d\n", state.treats_left);
					printf("rounds:      %d\n", state.rounds);
					printf("cost:        %d\n", state.cost);
				}
				break;
			case 'f':
				printf("        ID: ");
				getnumber_helper(&id);
				
				printf("        treats: ");
				getnumber_helper(&treats);

				ret = hmstr_givetreats(id, treats);
				if(!check_error(ret, "hmstr_givetreats()"))
				{
					printf("Done! %d treats remaining in store\n", ret);
				}					
				break;
			case 'q':
				printf("quit\n");
				break;
			default:
				printf("unknown command\n\n");
		}
	}
	while(cmd != EOF && cmd != 'q');

	hmstr_rpc_terminate();
}
