/**
 * Copyright (C) 2017 by Arno Nuehm
 * 
 * This software is distributed in the hope that it will be useful,
 * or at least interesting, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more details.
 * 
 * General Disclaimer
 * ==================
 * Use at your own risk. This software may damage your machine, wipe out
 * your disk or kill your hamster. Whatever happens, the authors are in
 * no way responsible.
 */

/**
 * @file	hmstr_new.c
 * @brief	New hamster function for hamster library
 * 
 *
 */
#include <stdint.h>

#include "hamsterdef.h"


/**
 * @brief	Local helper function to compute a hash value from to strings
 */
 
/* 
 * Hash parameters according to  Bernstein's function
 * See http://www.strchr.com/hash_functions
 */
#define INITIAL_VALUE	5381
#define M				33

static int make_hash(const char *s1, const char *s2)
{
	unsigned int hash;
	int i;
	char key[2*(HMSTR_MAX_NAME+1)];

	/* copy both strings into a single buffer */
	memset(key, 0, sizeof(key));
	strcpy(&key[0], s1);
	strcpy(&key[HMSTR_MAX_NAME+1], s2);

	/* compute a hash value over the buffer's contents */
	hash = INITIAL_VALUE;
	for(i = 0; i < 2*(HMSTR_MAX_NAME+1); ++i)
		hash = M * hash + key[i];

	/* make sure always get a >= 0 number */
	return hash >> 1U;
}


/**
 * @brief	Put a new hamster into the hamstercare institute
 */

int32_t hmstr_new(
	const char* owner_name,		/**< name of hamster's owner     */
	const char* hamster_name,	/**< name of hamster             */
	uint16_t treats			/**< initial privision of treats */
	)
{
	int fd;
	int ret;
	Hamsterdesc h;

	/* check if name lenghts are in range */
	if(strlen(owner_name) > HMSTR_MAX_NAME ||
	   strlen(hamster_name) > HMSTR_MAX_NAME)
	{
		return HMSTR_ERR_NAMETOOLONG;		
	}

	/* open the data file for writing (create if necessary) */
	fd = open(DATAFILE, O_RDWR|O_CREAT, 0600);
	if(fd < 0)
	{
		return HMSTR_ERR_STORE;
	}
	/*
	 *  before adding a new entry, make sure there isn't one
	 *  with the same owner and name already!
	 */
	while((ret = read(fd, &h, sizeof(h))) == sizeof(h))
	{
		if(!strcmp(h.owner, owner_name) && 
		   !strcmp(h.name, hamster_name))
		{
			close(fd);
			return HMSTR_ERR_EXISTS;
		}
	}
	/*
	 * since the file always contains an integer number of structs
	 * with constant size, read() can legally return only sizeof(h)
	 * (dealt with above) or zero at end-of-file. read() returning < 0
	 * indicates a severe error:
	 */
	if(ret < 0)
	{
		close(fd);
		return HMSTR_ERR_STORE;
	}
	/*
	 * if read() returns > 0, but < sizeof(h), it means that the file's
	 * size is not a multiple of sizeof(h), which must never happen.
	 * If it does, we conclude that the file must be damaged.
	 */
	else if(ret > 0)
	{
		close(fd);
		return HMSTR_ERR_CORRUPT;
	}
	/*
	 * Build a new entry and append it to the file:
	 */
    /*
     *  Clear the buffer, otherwise funny characters are
     *  stored in the file after the end-of-string-character.
     *  We do deal with this correctly but it potentially breaks some
     *  of the testsuites which expect to see equal data.
     */
	memset(&h, 0, sizeof(h));

	h.uid = make_hash(owner_name, hamster_name);
	gettimeofday(&h.admission_time, NULL);
	strcpy(h.owner, owner_name);
	strcpy(h.name, hamster_name);
	h.price = HMSTR_BASE_PRICE;
	h.treats_left = treats;

	if((ret = write(fd, &h, sizeof(h))) != sizeof(h))
	{
		close(fd);
		return HMSTR_ERR_STORE;
	}
	close(fd);

	return(h.uid);
}


