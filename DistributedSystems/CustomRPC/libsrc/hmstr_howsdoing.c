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
 * @file	hmstr_howsdoing.c
 * @brief	Hamster status function for Hamster library
 * 
 *
 */



#include "hamsterdef.h"

/**
 * @brief	Internal helper function to compute the number of hamster wheel
 *          revolutions my hamster did since admission time
 * 
 */
int ___get_rounds(struct timeval *admission_time)
{
	struct timeval now, duration;
	int rounds;

	gettimeofday(&now, NULL); 			/* get current time */
	timersub(&now, admission_time, &duration);
	assert(duration.tv_sec >= 0);	/* now must be later than admission time! */
	/*
	 * compute rounds assuming hamster did HMSTR_RPM rounds per
	 * minute of time spent in the asylum
	 */
	rounds = (duration.tv_sec * HMSTR_RPM) / 60;
	return rounds;
}
 

/**
 * @brief	How is my hamster doing?
 * 
 */

int32_t hmstr_howsdoing(
	int32_t ID,	/**<  Hamster's unique ID */
	struct hmstr_state* state	/**< pointer to data structure where to store information */
	)
{
	int fd;
	int ret;
	Hamsterdesc h;
	
	fd = open(DATAFILE, O_RDWR);
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
			 * Collect the state information and put
			 * it into caller's struct:
			 */
			state->treats_left = h.treats_left;
			/*
			 * compute rounds assuming hamster did HMSTR_RPM rounds per
			 * minute of time spent in the asylum
			 */
			state->rounds = ___get_rounds(&h.admission_time);
			/*
			 * Increase the price as hmstr_howsdoing() has been called
			 */
			h.price += HMSTR_CARE_PRICE;
			/*
			 * add cost of stay (= HMSTR_STAY_PRICE * rounds/1000)
			 */
			state->cost = h.price + (state->rounds * HMSTR_STAY_PRICE) / 1000;

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
			return 0;
		}
	}
	close(fd);
	return HMSTR_ERR_NOTFOUND;
}

