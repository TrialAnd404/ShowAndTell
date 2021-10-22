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
 * @file	hmstr_givetreats.c
 * @brief	Give treats function for hamster library
 * 
 *
 */

#include <stdint.h>

#include "hamsterdef.h"


/**
 * @brief	Give treats to my hamster
 * 
 * 
 */

int32_t hmstr_givetreats(
	int32_t ID,	/**< Hamster's unique ID */
	uint16_t treats	/**< How many treats to feed */
	)
{
	int fd;
	int ret;
	Hamsterdesc h;

	fd = open(DATAFILE, O_RDWR);
	if(fd < 0)
	{	/* if the file cannot be opened, it's definitly a NOTFOUND error,
		 * (why should it be a STORE error?)
		 */
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
			if(treats > h.treats_left)
			{
				treats -= h.treats_left;
				h.treats_left = 0;
				h.price += treats * HMSTR_FEED_PRICE;
			}
			else
			{
				h.treats_left -= treats;
			}

			/* write entry (with modified price) back to file: */
			if(lseek(fd, -sizeof(h), SEEK_CUR) ==(off_t)-1)
			{
				close(fd);
				return HMSTR_ERR_STORE;
			}
			if(write(fd, &h, sizeof(h)) != sizeof(h))
			{
				close(fd);
				return HMSTR_ERR_STORE;
			}
			close(fd);
			return h.treats_left;
		}
	}
	close(fd);
	return HMSTR_ERR_NOTFOUND;
}

