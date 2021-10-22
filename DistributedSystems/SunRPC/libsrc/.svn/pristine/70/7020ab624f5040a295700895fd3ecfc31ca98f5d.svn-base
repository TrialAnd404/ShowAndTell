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
 * @file	hmstr_readentry.c
 * @brief	Read Entry function for hamster library
 * 
 *
 */

#include "hamsterdef.h"


/**
 * @brief	Get contents of an entry in the database
 * 
 */

int32_t hmstr_readentry(
	int32_t ID,		/**< Hamster's unique ID             */
	char* owner,    /**< Where to store name of Owner    */
	char* name,   	/**< Where to store name of Hamster  */
	int16_t*  price	/**< Where to store cost (up to now) */
	)
{
	int fd;
	int ret;
	Hamsterdesc h;

	fd = open(DATAFILE, O_RDONLY);
	if(fd < 0)
	{
		return HMSTR_ERR_NOTFOUND;
	}
	/* read file entries one by one */
	while((ret = read(fd, &h, sizeof(h))) == sizeof(h))
	{
		if(h.uid == ID)
		{	/*
			 * found matching hamster!
			 * 
			 */
			strcpy(owner, h.owner);
			strcpy(name, h.name);
			*price = h.price + (___get_rounds(&h.admission_time) * HMSTR_STAY_PRICE) / 1000;;
			close(fd);
			return h.treats_left;
		}
	}
	close(fd);
	return HMSTR_ERR_NOTFOUND;
}
