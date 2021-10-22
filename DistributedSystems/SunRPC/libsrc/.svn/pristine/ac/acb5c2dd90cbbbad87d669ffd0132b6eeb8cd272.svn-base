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
 * @file	hamsterdef.h
 * @brief	Library-internal definitions
 * 
 * This file contains definitions which are global within the hamster
 * library but are not used (and should not be visible) to users of the
 * library.
 *
 */

#ifndef __HAMSTERDEF_H__
#define __HAMSTERDEF_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

#include "../include/hamsterlib.h"


#define DATAFILE			"hamsterfile.dat"
#define INTERMEDIATE_FILE	"hamsterfile.bak"


#define HMSTR_RPM	25	/* Hamster does 25 rounds per Minute in its hamster wheel */

/**
 * @brief Structure of an entry in the database
 */
 
typedef struct hmstr
{
	int32_t  uid;						/**< Unique ID                 */
	struct timeval admission_time;	/**< time when hamster arrived */
	char owner[HMSTR_MAX_NAME+1];	/**< Name of Owner             */
	char name[HMSTR_MAX_NAME+1];	/**< Name of Hamster           */
	int16_t  price;						/**< cost (up to now)          */
	uint16_t treats_left;		/**< number of treats in stock */
} Hamsterdesc;

/**
 * @brief	Internal helper function to compute the number of hamster wheel
 *          revolutions my hamster did since admission time
 * 
 */
int ___get_rounds(struct timeval *admission_time);

/**
 * @brief Debug support macros
 *
 * Output messages if @c verbose is higher than a given level.
 *
 * NOTE: Usage @c debug((<printf-params>)); i.e. use @e two brackets
 *       to enclose the printf parameter list!
 */
#define debug1(x) do{if(verbose >= 1){ printf x ;fflush(stdout);}}while(0)
#define debug2(x) do{if(verbose >= 2){ printf x ;fflush(stdout);}}while(0)
#define debug3(x) do{if(verbose >= 3){ printf x ;fflush(stdout);}}while(0)
#define debug4(x) do{if(verbose >= 4){ printf x ;fflush(stdout);}}while(0)





#endif /* __HAMSTERDEF_H__ */
