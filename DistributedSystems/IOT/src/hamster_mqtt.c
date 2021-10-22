#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>

#include "MQTTClient.h"

/**
 * @brief Debug support macros
 *
 * Output messages if @c verbose is higher than a given level.
 *
 * NOTE: Usage @c debug((<printf-params>)); i.e. use @e two brackets
 *       to enclose the printf parameter list!
 */
#ifndef DEBUG
#define debug(x)        \
	do                  \
	{                   \
		printf x;       \
		fflush(stdout); \
	} while (0)
#else
#define debug(x)
#endif

#define ADDRESS "tcp://hamsteriot.vs.cs.hs-rm.de"
#define CLIENTID "ExampleClientSub" //wird durch hash ersetzt

#define PENSIONTOPIC "/pension"
#define LIVESTOCKTOPIC "/livestock"
#define ROOMTOPIC "/room"
#define HAMSTERTOPIC "/hamster"
#define STATETOPIC "/state"
#define POSITIONTOPIC "/position"
#define PUNISHTOPIC "/punish"
#define FONDLETOPIC "/fondle"
#define WHEELSTOPIC "/wheels"

#define RUN "RUNNING"
#define SLP "SLEEPING"
#define MAT "MATEING"
#define EAT "EATING"

#define QOS 1
#define TIMEOUT 10000L

#define HMSTR_MAX_NAME 31
static char address[1000];
static char pensionTopic[19];
static char clientid[33];
static char roomTopic[16];
static char stateTopic[56];
static char positionTopic[59];
static char fondleTopic[57];
static char punishTopic[57];
static char wheelsTopic[57];
static char roundsString[12];

/*
 * Code stolen and adapted from 5/sunrpc/hamster_cli.c: 
 */
static void cli_commands()
{
	printf("\nCommands:\n");
	printf("        A                           - Go to room A\n");
	printf("        B                           - Go to room B\n");
	printf("        C                           - Go to room C\n");
	printf("        D                           - Go to room D\n");
	printf("        r                           - change state to RUNNING\n");
	printf("        s                           - change state to SLEEPING\n");
	printf("        e                           - change state to EATING\n");
	printf("        m                           - change state to MATING\n");
	printf("        q                           - terminate hamster\n");
	printf("> ");
	fflush(stdout);
}

static void flushinput(void)
{
	while (getchar() != '\n')
		;
}

void rtfm(char **argv)
{
	printf("Usage: %s hamster_client_id {<Options>}\n", argv[0]);
	printf("Function: Hamster instrumentation device software\n");
	printf("Options:\n");
	printf("     -p <port>                      - port of the mqtt server (default (no tls): 1883\n");
	printf("     -s <IP address>                - IP address to run the server on (default: 127.0.0.1\n");
	printf("     -v                             - Connect with Certificate based SSL/TLS Support to the MQTT server \n");
	printf("     -V                             - Connect with Certificate based client authentification to the MQTT server \n");
	printf("     -o <owner name>                - Hamster owner's name (default: $USER)\n");
	printf("     -n <hamster name>              - Hamster's name (default: myhamster)\n");
	printf("     -i <hamster ID>                - Hamster's ID (overrides owner & hamster names)\n");
	printf("     -h                             - This help \n");
}

/*
 * Declare these static global to suppress compiler warnings
 * about variables being "set but not used". Ultimately, your
 * program *should* use these variables so you should be able
 * move these into main() without warnings.
 */
static int doAuthenticate = 0;
static int doEncrypt = 0;
static unsigned int port = 1883;
static char *ipaddr = "127.0.0.1";
int fondles = 0;
int punishes = 0;

static int make_hash(const char *s1, const char *s2)
{
	unsigned int hash;
	int i;
	char key[2 * (HMSTR_MAX_NAME + 1)];

	/* copy both strings into a single buffer */
	memset(key, 0, sizeof(key));
	strcpy(&key[0], s1);
	strcpy(&key[HMSTR_MAX_NAME + 1], s2);

	/* compute a hash value over the buffer's contents */
	hash = 5381;
	for (i = 0; i < 2 * (HMSTR_MAX_NAME + 1); ++i)
		hash = 33 * hash + key[i];

	/* make sure always get a >= 0 number */
	return hash >> 1U;
}

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;
	char *payloadptr;
	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");
	payloadptr = message->payload;
	for (i = 0; i < message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');

	if (strstr(topicName, "punish") != NULL)
	{
		punishes++;
		printf("punishcount: %d\n", punishes);
	}
	if (strstr(topicName, "fondle") != NULL)
	{
		fondles++;
		printf("fondlecount: %d\n", fondles);
	}

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

/*
int publishPos(MQTTClient client,char *pubTopic,MQTTClient_message pubmsg,MQTTClient_deliveryToken &token){

	return 0;
}
*/

int main(int argc, char **argv)
{
	int hamster_id = -1;
	int rounds = 0;

	time_t startRun, endRun;
	double seconds;
	bool running = true;

	char *owner = getenv("USER");
	char *hamster = "myhamster";

	//char *subTopic = "mySubTopic";
	char cmd;

	/*
	* parse args:
	*/
	while ((cmd = getopt(argc, argv, "?p:i:s:o:n:hVv")) != -1)
	{
		switch (cmd)
		{
		case 'p':
		{
			char *end;
			port = strtoul(optarg, &end, 0);
			if (optarg == end)
			{
				printf("%s: Not a number: %s\n", argv[0], optarg);
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 'i':
		{
			char *end;
			hamster_id = strtoul(optarg, &end, 0);
			if (optarg == end)
			{
				printf("%s: Not a number: %s\n", argv[0], optarg);
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 's':
			ipaddr = optarg;
			break;
		case 'v':
			doEncrypt = 1;
			break;
		case 'V':
			doAuthenticate = 1;
			doEncrypt = 1;
			break;
		case 'h':
		case '?':
			rtfm(argv);
			return EXIT_SUCCESS;
			break;
		case 'o':
		{
			int l = strlen(optarg);
			if (l > 0 && l < HMSTR_MAX_NAME)
			{
				owner = optarg;
			}
			else
			{
				printf("%s: owner name empty or too long: %s\n", argv[0], optarg);
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 'n':
		{
			int l = strlen(optarg);
			if (l > 0 && l < HMSTR_MAX_NAME)
			{
				hamster = optarg;
			}
			else
			{
				printf("%s: owner name empty or too long: %s\n", argv[0], optarg);
				exit(EXIT_FAILURE);
			}
			break;
		}
		}
	}

	if (hamster_id == -1)
	{
		/* Hier ist was zu tun: */
		//random ID generieren

		debug(("Calling: hamster_id = make_hash(\"%s\", \"%s\");", owner, hamster));

		hamster_id = make_hash(owner, hamster);
	}
	sprintf(clientid, "%d", hamster_id);
	printf("** Using ID %d for %s's Hamster %s **\n", hamster_id, owner, hamster);

	printf("setting up topics\n");
	sprintf(pensionTopic, "%s%s", PENSIONTOPIC, LIVESTOCKTOPIC);
	sprintf(stateTopic, "%s%s/%s%s", PENSIONTOPIC, HAMSTERTOPIC, clientid, STATETOPIC);
	sprintf(punishTopic, "%s%s/%s%s", PENSIONTOPIC, HAMSTERTOPIC, clientid, PUNISHTOPIC);
	sprintf(fondleTopic, "%s%s/%s%s", PENSIONTOPIC, HAMSTERTOPIC, clientid, FONDLETOPIC);
	sprintf(roomTopic, "%s%s/#", PENSIONTOPIC, ROOMTOPIC);
	sprintf(positionTopic, "%s%s/%s%s", PENSIONTOPIC, HAMSTERTOPIC, clientid, POSITIONTOPIC);
	sprintf(wheelsTopic, "%s%s/%s%s", PENSIONTOPIC, HAMSTERTOPIC, clientid, WHEELSTOPIC);
	/*
	* connect to MQTT broker
	* 
	*/

	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	MQTTClient_deliveryToken token;
	MQTTClient_SSLOptions sslOptions = MQTTClient_SSLOptions_initializer;

	if (doEncrypt)
	{
		sprintf(address, "ssl://%s:%d", ipaddr, port);
		printf("Doing Encryption!\n");
		sslOptions.trustStore = "certs/mqtt_ca.crt";
		if (doAuthenticate)
		{
			sslOptions.privateKey = "certs/priv.key";
			sslOptions.keyStore = "certs/cert.crt";
			printf("Doing Authentication!\n");
		}
		conn_opts.ssl = &sslOptions;
	}
	else
	{
		sprintf(address, "tcp://%s:%d", ipaddr, port);
	}

	printf("connecting to: %s\n", address);
	MQTTClient_create(&client, address, clientid,
					  MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.username = "hamster";

	printf("setting up connection to broker..\n");
	int connRc;
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
	if ((connRc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", connRc);
		exit(EXIT_FAILURE);
	}

	//Subscriptions bei fondle und punish

	/* Hier ist was zu tun: */

	/*
	* Get admission time, start RUNNING in room A
	*/
	printf("settign defaults, room: A state: running\n");
	char *room = "A";
	char *state = RUN;
	time(&startRun);

	printf("publishing to topic %s\n", pensionTopic);
	deliveredtoken = 0;

	MQTTClient_publish(client, pensionTopic, strlen(clientid), clientid, 2, 1, &token);
	printf("Waiting for publication of %s\n"
		   "on topic %s for client with ClientID: %s\n",
		   clientid, pensionTopic, clientid);
	while (deliveredtoken != token)
		;

	deliveredtoken = 0;
	MQTTClient_publish(client, stateTopic, strlen(RUN), RUN, 1, 0, &token);
	printf("Waiting for publication of %s\n"
		   "on topic %s for client with ClientID: %s\n",
		   clientid, stateTopic, clientid);
	while (deliveredtoken != token)
		;

	deliveredtoken = 0;
	MQTTClient_publish(client, positionTopic, strlen("A"), "A", 1, 0, &token);
	printf("Waiting for publication of %s\n"
		   "on topic %s for client with ClientID: %s\n",
		   clientid, positionTopic, clientid);
	while (deliveredtoken != token)
		;

	MQTTClient_subscribe(client, roomTopic, 0);
	MQTTClient_subscribe(client, fondleTopic, 2);
	MQTTClient_subscribe(client, punishTopic, 2);

	printf("connection to broker at %s established!", address);
	/*
	* Main Command loop
	*/
	do
	{
		cli_commands();
		cmd = getchar();
		flushinput();
		switch (cmd)
		{
		case 'A': /* going to room A */
			room = "A";
			MQTTClient_publish(client, positionTopic, strlen(room), room, 1, 0, &token);
			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   room, positionTopic, clientid);
			while (deliveredtoken != token)
				;

			break;
		case 'B': /* going to room B */

			/* Hier ist was zu tun: */
			room = "B";
			MQTTClient_publish(client, positionTopic, strlen(room), room, 1, 0, &token);

			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   room, positionTopic, clientid);
			while (deliveredtoken != token)
				;

			break;
		case 'C': /* going to room C */

			/* Hier ist was zu tun: */
			room = "C";
			MQTTClient_publish(client, positionTopic, strlen(room), room, 1, 0, &token);

			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   room, positionTopic, clientid);
			while (deliveredtoken != token)
				;

			break;
		case 'D': /* going to room D */
			/* Hier ist was zu tun: */
			room = "D";
			MQTTClient_publish(client, positionTopic, strlen(room), room, 1, 0, &token);
			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   room, positionTopic, clientid);
			while (deliveredtoken != token)
				;
			break;
		case 'r': /* change to "RUNNING" state */
			if (!running)
			{
				running = true;
				time(&startRun);
				printf("started running again!\n");
			}
			printf("%s->", state);

			/* Hier ist was zu tun: */
			state = RUN;
			printf("%s\n", state);
			printf("we runnin now!\n");
			MQTTClient_publish(client, stateTopic, strlen(state), state, 1, 0, &token);

			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   RUN, stateTopic, clientid);
			while (deliveredtoken != token)
				;

			break;
		case 's': /* change to "SLEEPING" state */
			if (running)
			{
				time(&endRun);
				running = false;
				seconds = difftime(endRun, startRun);
				printf("stopped running after %f seconds\n", seconds);
				rounds += (seconds / 60) * 25;
				sprintf(roundsString, "%d", rounds);
				MQTTClient_publish(client, wheelsTopic, strlen(roundsString), roundsString, 0, 0, &token);

				printf("Waiting for publication of %s\n"
					   "on topic %s for client with ClientID: %s\n",
					   EAT, wheelsTopic, clientid);
				while (deliveredtoken != token)
					;
			}
			/* Hier ist was zu tun: */
			state = SLP;
			printf("we sleepin now!\n");
			MQTTClient_publish(client, stateTopic, strlen(state), state, 1, 0, &token);

			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   SLP, stateTopic, clientid);
			while (deliveredtoken != token)
				;

			break;
		case 'e': /* change to "EATING" state */
			if (!(strcmp(state, RUN)))
			{
				time(&endRun);
				running = false;
				seconds = difftime(endRun, startRun);
				printf("stopped running after %f seconds\n", seconds);
				rounds += (seconds / 60) * 25;
				sprintf(roundsString, "%d", rounds);
				MQTTClient_publish(client, wheelsTopic, strlen(roundsString), roundsString, 0, 0, &token);

				printf("Waiting for publication of %s\n"
					   "on topic %s for client with ClientID: %s\n",
					   EAT, wheelsTopic, clientid);
				while (deliveredtoken != token)
					;
			}
			/* Hier ist was zu tun: */
			state = EAT;
			printf("we eatin now!\n");
			MQTTClient_publish(client, stateTopic, strlen(state), state, 1, 0, &token);

			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   EAT, stateTopic, clientid);
			while (deliveredtoken != token)
				;

			break;
		case 'm': /* change to "MATEING" state */
			if (!(strcmp(state, RUN)))
			{
				time(&endRun);
				running = false;
				seconds = difftime(endRun, startRun);
				printf("stopped running after %f seconds\n", seconds);
				rounds += (seconds / 60) * 25;
				sprintf(roundsString, "%d", rounds);

				MQTTClient_publish(client, wheelsTopic, strlen(roundsString), roundsString, 0, 0, &token);

				printf("Waiting for publication of %s\n"
					   "on topic %s for client with ClientID: %s\n",
					   EAT, wheelsTopic, clientid);
				while (deliveredtoken != token)
					;
			}
			/* Hier ist was zu tun: */
			state = MAT;
			printf("we matin now!\n");

			MQTTClient_publish(client, stateTopic, strlen(state), state, 1, 0, &token);

			printf("Waiting for publication of %s\n"
				   "on topic %s for client with ClientID: %s\n",
				   MAT, stateTopic, clientid);
			while (deliveredtoken != token)
				;

			break;
		case 'q':
			printf("quit\n");
			break;
		default:
			printf("unknown command\n\n");
		}
	} while (cmd != EOF && cmd != 'q');

	/* Hier ist was zu tun: */
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);

	printf("connection closed\n");

	return 0;
}
