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
 * @file	hmstr_lookup.c
 * @brief	Lookup function for hamster library
 * 
 *
 */



#include "hamsterdef.h"


/**
 * @brief	Find hamster in the hamstercare institute
 * 
 */

int32_t hmstr_lookup(
	const char* owner_name,		/**< name of hamster's owner */
	const char* hamster_name	/**< name of hamster         */
	)
{
	int fd;
	int ret;
	Hamsterdesc h;

	/* skip reading the file if no useful names have been passed */
	if (owner_name == NULL || hamster_name == NULL) {
		return HMSTR_ERR_NOTFOUND;
	}
	/* check if name lenghts are in range */
	if(strlen(owner_name) > HMSTR_MAX_NAME ||
	   strlen(hamster_name) > HMSTR_MAX_NAME)
	{
		return HMSTR_ERR_NAMETOOLONG;		
	}

	fd = open(DATAFILE, O_RDONLY);
	if(fd < 0)
	{
		return HMSTR_ERR_NOTFOUND;
	}
	/* read file entries one by one */
	while((ret = read(fd, &h, sizeof(h))) == sizeof(h))
	{
		if(!strcmp(h.owner, owner_name) &&
		   !strcmp(h.name, hamster_name))
		{	/* found matching hamster! */
			close(fd);
			return h.uid;
		}
	}
	close(fd);
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

