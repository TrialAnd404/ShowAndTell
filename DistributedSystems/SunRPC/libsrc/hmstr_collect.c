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
 * @file	hmstr_collect.c
 * @brief	Collect function for hamster library
 * 
 *
 */

#include "hamsterdef.h"


/* 
 * Hash parameters according to  Bernstein's function
 * See http://www.strchr.com/hash_functions
 */
#define INITIAL_VALUE	5381
#define M				33

/**
 * @brief	Collect all my hamsters and pay the bill
 * 
 */

int hmstr_collect(
	const char *owner_name		/**< name of hamster owner */
	)
{
	int fd, fdi;
	int ret;
	int total_price = 0;
	int found = 0;
	Hamsterdesc h;

	/* check for NULL pointer */
	if(owner_name == NULL)
	{
		return HMSTR_ERR_NOTFOUND;
	}
	/* check if name length is in range */
	if(strlen(owner_name) > HMSTR_MAX_NAME)
	{
		return HMSTR_ERR_NAMETOOLONG;		
	}

	if(rename(DATAFILE, INTERMEDIATE_FILE) < 0)
	{
		return HMSTR_ERR_STORE;
	}

	fdi = open(INTERMEDIATE_FILE, O_RDONLY);
	if(fdi < 0)
	{
		return HMSTR_ERR_STORE;
	}
	fd = open(DATAFILE, O_RDWR|O_CREAT, 0600);
	if(fd < 0)
	{
		return HMSTR_ERR_STORE;
	}
	/* read file entries from intermediate file one by one */
	while((ret = read(fdi, &h, sizeof(h))) == sizeof(h))
	{
		if(!strcmp(h.owner, owner_name))
		{	/*
			 * Found a matching entry:
			 * add cost of stay (= HMSTR_STAY_PRICE * rounds/1000)
			 */
			total_price += h.price + (___get_rounds(&h.admission_time) * HMSTR_STAY_PRICE) / 1000;
			found = 1;
		}
		else
		{	/* other owner: write it to new file */
			if(write(fd, &h, sizeof(h)) != sizeof(h))
			{
				close(fd);
				close(fdi);
				return HMSTR_ERR_STORE;
			}
		}
	}
	close(fd);
	close(fdi);
	if(ret < 0)
	{
		return HMSTR_ERR_STORE;
	}
	else if(ret > 0)
	{
		return HMSTR_ERR_CORRUPT;
	}
	/* delete intermediate file */
	if(unlink(INTERMEDIATE_FILE) < 0)
	{
		return HMSTR_ERR_STORE;
	}

	if(found)
		return total_price;
	else
		return HMSTR_ERR_NOTFOUND;
}







