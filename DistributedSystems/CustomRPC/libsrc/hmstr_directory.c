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
 * @file	hmstr_directory.c
 * @brief	Directory function for hamster library
 * 
 *
 */


#include "hamsterdef.h"


/**
 * @brief	Get a directory of entries in the database
 * 
 */

int32_t hmstr_directory(
	int32_t* fdptr,	/**< pointer where to store the open file handle across calls */
	const char* owner_name,		/**< name of hamster's owner */
	const char* hamster_name	/**< name of hamster         */
	)
{
	int ret;
	Hamsterdesc h;

	/* check if name lenghts are in range */
	if((owner_name && strlen(owner_name) > HMSTR_MAX_NAME) ||
	   (hamster_name && strlen(hamster_name) > HMSTR_MAX_NAME))
	{
		return HMSTR_ERR_NAMETOOLONG;
	}

	if(*fdptr < 0)
	{
		*fdptr = open(DATAFILE, O_RDONLY);
		if(*fdptr < 0)
		{
			return HMSTR_ERR_NOTFOUND;
		}
	}

	/* read file entries one by one */
	while((ret = read(*fdptr, &h, sizeof(h))) == sizeof(h))
	{
		if((!owner_name || !strcmp(h.owner, owner_name)) &&
		   (!hamster_name || !strcmp(h.name, hamster_name)))
		{	/* found matching hamster! */
			return h.uid;
		}
	}
	close(*fdptr);
	*fdptr = -1;
	if(ret == 0)		
	{
		return HMSTR_ERR_NOTFOUND;
	}
	/*
	 * if read() returns > 0, but < sizeof(h), it means that the file's
	 * size is not a multiple of sizeof(h), which must never happen.
	 * If it does, we conclude that the file must be damaged.
	 */
	else
	{
		return HMSTR_ERR_CORRUPT;
	}
}
