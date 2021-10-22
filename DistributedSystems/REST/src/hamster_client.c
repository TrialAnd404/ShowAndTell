#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hamsterrest.h"

#include "curl/curl.h"

#define POST 1
#define GET 2
#define DELETE 3
#define PUT 4

char url[1024];
char defaultURL[512];
char *defaultPath = ":8080/HamsterREST/rest";
CURLcode res;
CURL *curl;

//the only memory struct we use as a buffer
//-> we have to clear it before calling a new function
struct MemoryStruct mem;

struct MemoryStruct
{
	char *memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr)
	{
		/* out of memory! */
		//printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

char **str_split(char *a_str)
{
	char delimiter = '\n';
	char **result = 0;
	size_t count = 0;
	char *tmp = a_str;
	char *last_comma = 0;
	char delim[2];
	delim[0] = delimiter;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (delimiter == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* Add space for trailing token. */
	count += last_comma < (a_str + strlen(a_str) - 1);

	/* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char *) * count);
	if (result)
	{
		size_t idx = 0;
		char *token = strtok(a_str, delim);

		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}
	return result;
}

void hrest_init(char *hostname)
{
	//curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	//setup von curl
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);
	//setup der default URL
	////printf("putting the URL teogether...\n");
	sprintf(defaultURL, "http://%s%s", hostname, defaultPath);
	//memory objekt initialisieren
	mem.size = 0;
	mem.memory = malloc(1);
	mem.memory[0] = '\0';
}

int curlsForTheGirls(int curlType)
{
	//alten mem speicher löschen
	free(mem.memory);
	mem.memory = malloc(1);
	mem.memory[0] = '\0';
	mem.size = 0;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		switch (curlType)
		{
		case POST:
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			break;
		case DELETE:
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
			break;
		case PUT:
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
			break;
		case GET:
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
			break;
		default:
			//printf("unknown method\n");
			return -1;
		}
		////printf("url: %s\n", url);
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
			return -2;
		}
		return 0;
	}
	return -3;
}

void hrest_terminate(void)
{
	curl_easy_cleanup(curl);
	//curl_global_cleanup();

	//we need to free our mem objects memory
	free(mem.memory);
}

int hrest_helper_new(const char *owner, const char *hamster_name, uint16_t treats, int *hamster_id)
{
	int ownerID;
	////printf("attempting to call hrest_helper_new...\n");
	//zuerst: gucken ob owner vorhanden

	sprintf(url, "%s/owners?ownerName=%s", defaultURL, owner);

	curlsForTheGirls(GET);
	//printf("GET (CHECKING):\n%s\n", mem.memory);
	//mem checken, ob etwas zurückgekommen ist
	if (mem.size == 0)
	{
		//es ist nichts zuurück gekommen, user muss angelegt werden
		sprintf(url, "%s/owners?name=%s", defaultURL, owner);
		curlsForTheGirls(POST);
		sprintf(url, "%s/owners?ownerName=%s", defaultURL, owner);
		curlsForTheGirls(GET);
	}
	//printf("GET (CONFIRMING):\n%s\n", mem.memory);
	sscanf(mem.memory, "%i", &ownerID);
	//printf("ownerID: %i\n", ownerID);

	//es ist etwas zurück gekommen, hamster checken
	//das hier ist ein buggy check
	sprintf(url, "%s/hamsters?hamsterName=%s&ownerName=%s", defaultURL, hamster_name, owner);
	curlsForTheGirls(GET);

	if (mem.size != 0)
	{
		//hamster existiert bereits, return error
		return HMSTR_ERR_EXISTS;
	}

	sprintf(url, "%s/owners/%i/hamsters?name=%s&treats=%i", defaultURL, ownerID, hamster_name, treats);
	curlsForTheGirls(POST);

	sprintf(url, "%s/hamsters?hamsterName=%s&ownerName=%s", defaultURL, hamster_name, owner);
	curlsForTheGirls(GET);
	sscanf(mem.memory, "%i", hamster_id);

	return HREST_RET_OK;
}

int hrest_helper_collect(const char *owner, int16_t *price)
{
	////printf("attempting to call hrest_helper_new...\n");
	int ownerID;
	*price = 0;

	//owner id mit namen ermitteln
	sprintf(url, "%s/owners?ownerName=%s", defaultURL, owner);
	curlsForTheGirls(GET);
	if (mem.size == 0) //mem.memory string ist leer => kein owner gefunden
	{
		return HMSTR_ERR_NOTFOUND;
	}
	//nicht ins if, => owner wurde gefunden --> id parsen mit sscanf
	sscanf(mem.memory, "%i", &ownerID);

	// hamster des owners ermitteln
	sprintf(url, "%s/owners/%i/hamsters", defaultURL, ownerID);
	curlsForTheGirls(GET);
	if (mem.size == 0) //mem.memory string ist leer => keine hamster gefunden
	{
		//owner kann gelöscht werden
		sprintf(url, "%s/owners/%i", defaultURL, ownerID);
		curlsForTheGirls(DELETE);
		//theoretisch abfrage, ob user wirklich gelöscht ist
		return HREST_RET_OK;
	}
	//mem.memory enthält daten --> alle daten parsen und preise addieren
	char **hamsterIDs;

	hamsterIDs = str_split(mem.memory); //mem.memory an \n splitten in substrings
	if (hamsterIDs)
	{
		int hamsterID;
		int cost = 0;

		int i;
		for (i = 0; *(hamsterIDs + i); i++)
		{
			//einzelne HamsterID aus substring auslesen
			sscanf(*(hamsterIDs + i), "%i", &hamsterID);
			//einzelnen hamster löschen
			sprintf(url, "%s/owners/%i/hamsters/%i", defaultURL, ownerID, hamsterID);
			curlsForTheGirls(DELETE);
			if (mem.size == 0)
			{ //muss funktionieren, sonst giga-fehler in server
				return -150;
			}
			//Alles auslesen, was in den hamsterdaten steht
			sscanf(mem.memory, "costs:%i", &cost);
			*price += cost;
		}
		//printf("\n");
		free(hamsterIDs);
	}
	//jetzt müssten alle hamster gelöscht sein -> user löschen. noch ein letzter test davor

	sprintf(url, "%s/owners/%i/hamsters", defaultURL, ownerID);
	curlsForTheGirls(GET);
	if (mem.size == 0) //mem.memory string ist leer => keine hamster gefunden
	{
		//owner kann gelöscht werden
		sprintf(url, "%s/owners/%i", defaultURL, ownerID);
		curlsForTheGirls(DELETE);
		//theoretisch abfrage, ob user wirklich gelöscht ist
		return HREST_RET_OK;
	}

	//printf("something went gloriously wrong whilst deleting user with ID:%i\n", ownerID);
	return -150;
}

int hrest_helper_print_all_hamsters(void)
{
	////printf("attempting to call hrest_helper_new...\n");

	sprintf(url, "%s/hamsters", defaultURL);

	curlsForTheGirls(GET);
	//mem checken, ob etwas zurückgekommen ist
	if (mem.size == 0)
	{
		//es ist nichts zuurück gekommen, return error
		//printf("keine hamster in DB gefunden?\n");
		return HMSTR_ERR_NOTFOUND;
	}
	char **hamsterIDs;

	hamsterIDs = str_split(mem.memory);
	if (hamsterIDs)
	{
		char hamsterName[HMSTR_MAX_NAME + 1];
		char ownerName[HMSTR_MAX_NAME + 1];
		int hamsterID;
		int ownerID;
		int treats;
		int rounds;
		int cost;

		int i;
		for (i = 0; *(hamsterIDs + i); i++)
		{
			//einzelne HamsterID auslesen
			sscanf(*(hamsterIDs + i), "%i", &hamsterID);
			//einzelnen Hamster ausgeben
			sprintf(url, "%s/hamsters/%i", defaultURL, hamsterID);
			curlsForTheGirls(GET);
			//Schicker output
			sscanf(
				mem.memory,
				"name:%s\nID:%i\nownerID:%i\ntreats:%i\nrounds:%i\ncost:%i\n",
				hamsterName, &hamsterID, &ownerID, &treats, &rounds, &cost);
			sprintf(url, "%s/owners/%i", defaultURL, ownerID);
			curlsForTheGirls(GET);
			if (mem.size == 0)
			{
				//printf("owner that should be enlisted cannot be found\n");
				return -150;
			}
			sscanf(
				mem.memory,
				"name:%s\nid:%i",
				ownerName, &ownerID);

			/*printf(
				"%i\t\t%s\t\t%s\t\t%i\t\t%i €\t\t%i\n",
				hamsterID, ownerName, hamsterName, hamsterID, cost, treats);
				free(*(hamsterIDs + i));
				*/
			hrest_helper_print_hamster(hamsterID, ownerName, hamsterName, cost, treats);
		}
		//printf("\n");
		free(hamsterIDs);
	}
	return HREST_RET_OK;
}

int hrest_helper_print_owners_hamsters(const char *owner)
{
	////printf("attempting to call hrest_print_owners_hamsters..\n");
	int ownerID;
	//zuerst: gucken ob owner vorhanden

	sprintf(url, "%s/owners?ownerName=%s", defaultURL, owner);

	curlsForTheGirls(GET);
	//mem checken, ob etwas zurückgekommen ist
	if (mem.size == 0)
	{
		//es ist nichts zuurück gekommen, return error
		return HMSTR_ERR_NOTFOUND;
	}
	sscanf(mem.memory, "%i", &ownerID);
	////printf("ownerID: %i\n", ownerID);

	sprintf(url, "%s/owners/%i/hamsters", defaultURL, ownerID);
	curlsForTheGirls(GET);
	//TODO: mit scanf über mem laufen und einzelne hamsterdaten ausgeben

	char **hamsterIDs;

	hamsterIDs = str_split(mem.memory);
	if (hamsterIDs)
	{
		char hamsterName[HMSTR_MAX_NAME + 1];
		int hamsterID;
		int treats;
		int rounds;
		int cost;
		/*//printf(
			"\n============================================================================\n"
			"%s\tID: %i\n"
			"Hamstername:\t\tHamsterID:\t\tTreats:\t\tRounds:\t\tCost:\n",
			owner, ownerID);
			*/

		int i;
		for (i = 0; *(hamsterIDs + i); i++)
		{
			//einzelne HamsterID auslesen
			sscanf(*(hamsterIDs + i), "%i", &hamsterID);
			//einzelnen Hamster ausgeben
			sprintf(url, "%s/hamsters/%i", defaultURL, hamsterID);
			curlsForTheGirls(GET);
			//Schicker output
			sscanf(
				mem.memory,
				"name:%s\nID:%i\nownerID:%i\ntreats:%i\nrounds:%i\ncost:%i\n",
				hamsterName, &hamsterID, &ownerID, &treats, &rounds, &cost);

			/*
			printf(
				"%s\t\t%i\t\t%i\t\t%i\t\t%i €\n",
				hamsterName, hamsterID, treats, rounds, cost);
				free(*(hamsterIDs + i));
				*/
			hrest_helper_print_hamster(hamsterID, owner, hamsterName, cost, treats);
		}
		//printf("\n");
		free(hamsterIDs);
	}

	return HREST_RET_OK;
}
int hrest_helper_print_hamster(int id, const char *owner, const char *hamster_name, uint16_t price, uint16_t treats)
{

	printf("%-10d %-32s%-32s%-12d %-12d\n", id, owner, hamster_name, price, treats);

	return HREST_RET_OK;
}

int hrest_helper_howsdoing(int id, struct hmstr_state *state)
{
	//printf("attempting to call hrest_helper_howsdoing...\n");
	//checken ob hamster existiert

	sprintf(url, "%s/hamsters/%i", defaultURL, id);
	curlsForTheGirls(GET);

	if (mem.size == 0)
	{
		//hamster existiert nicht, return error
		return HMSTR_ERR_NOTFOUND;
	}
	char hamsterName[HMSTR_MAX_NAME + 1];
	int hamsterID;
	int ownerID;
	int treats;
	int rounds;
	int cost;

	sscanf(
		mem.memory,
		"name:%s\nID:%i\nownerID:%i\ntreats:%i\nrounds:%i\ncost:%i\n",
		hamsterName, &hamsterID, &ownerID, &treats, &rounds, &cost);

	state->cost = cost;
	state->rounds = rounds;
	state->treats_left = treats;
	return HREST_RET_OK;
}

int hrest_helper_givetreats(int id, uint16_t treats, uint16_t *treats_left)
{
	//printf("attempting to call hrest_helper_givetreats...\n");
	//gucken ob hamster existiert
	sprintf(url, "%s/hamsters/%i", defaultURL, id);
	curlsForTheGirls(GET);
	if (mem.size == 0)
	{
		//hamster existiert nicht, return error
		return HMSTR_ERR_NOTFOUND;
	}
	//hamster existiert, treats an den kopf schmeissen
	sprintf(url, "%s/hamsters/%i/treat?count=%i", defaultURL, id, treats);
	curlsForTheGirls(PUT);

	sscanf(mem.memory, "treats:%hi", treats_left);
	return HREST_RET_OK;
}

int hrest_helper_queryHamster(char *owner, char *hamster_name, int *hamster_id)
{
	//printf("attempting to call hrest_helper_queryHamster...\n");

	int ownerID;
	//ownerID ermitteln
	sprintf(url, "%s/owners?ownerName=%s", defaultURL, owner);
	curlsForTheGirls(GET);
	if (mem.size == 0)
	{
		return HMSTR_ERR_NOTFOUND;
	}
	sscanf(mem.memory, "%i", &ownerID);

	//hamsterID(s) ermitteln
	sprintf(
		url,
		"%s/hamsters?hamsterName=%s",
		defaultURL, hamster_name);
	curlsForTheGirls(GET);
	if (mem.size == 0)
	{
		//hamster existiert nicht, return error
		return HMSTR_ERR_NOTFOUND;
	}

	//hamsterID(s) durchiterieren, mit ownerID abgleichen
	char **hamsterIDs;

	hamsterIDs = str_split(mem.memory);
	if (hamsterIDs)
	{
		char hamsterName[HMSTR_MAX_NAME + 1];
		int hamsterID;
		int secondOwnerID;
		int treats;
		int rounds;
		int cost;

		int i;
		for (i = 0; *(hamsterIDs + i); i++)
		{
			//einzelne HamsterID auslesen
			sscanf(*(hamsterIDs + i), "%i", &hamsterID);
			//einzelnen Hamster anfordern
			sprintf(url, "%s/hamsters/%i", defaultURL, hamsterID);
			curlsForTheGirls(GET);
			sscanf(
				mem.memory,
				"name:%s\nID:%i\nownerID:%i\ntreats:%i\nrounds:%i\ncost:%i\n",
				hamsterName, &hamsterID, &secondOwnerID, &treats, &rounds, &cost);
			free(*(hamsterIDs + i));
			//abgleich ownerID mit ownnerID des aktuell gelesenen Hamsters
			if (secondOwnerID == ownerID)
			{
				*hamster_id = hamsterID;
				break;
			}
		}
		//printf("\n");
		free(hamsterIDs);
	}
	return HREST_RET_OK;
}