

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

#ifndef __HAMSTERLIB_H__
#define __HAMSTERLIB_H__

#include <stdint.h>

/** 
 * @file	hamsterlib.h
 * @defgroup libfuncs Library functions
 * @brief	API-Definitions for the hamster library
 * 
 * This file is to be included by all applications making use
 * of any of the hamster library's functions. It declares prototypes
 * for all user-callable functions of the library.
 * It __does not__ contain library-internal definitions.
 *
 */

/**
 * @mainpage
 * 
 * @section Introduction
 * 
 * Hamsterlib is an open source library implementing a simple database
 * for keeping pet rodents in a care center while their owners are
 * abroad or otherwise unable to look after their pets. Hamsters
 * can be brought into the care center where they will be looked after
 * and fed. Owners may call at any time to request information about
 * the well-being of their favourite hamsters and they can decide to
 * give them little treats to increase their happiness. Owners are
 * charged a service fee as they pick up their hamster from the facility.
 * 
 * @section Pricing
 * 
 * - The base price for full pension of one hamster incurring upon
 *   admission to the facility is 17 €.
 * - In addition, a staying fee is charged per hamster which amounts
 *   to 5 € per 1000 revolutions of the hamster's running wheel.
 * - Every hamster owner inquiry about the condition of their hamster
 *   is charged at 1 €,
 * - Hamsters can be provided with a stock of treats upon admission to 
 *   the asylum. Owners can request treats to be fed to their hamster. 
 *   If a hamster runs out of stock, the care center will dispense 
 *   additional treats at a price of 2 € per treat.
 * 
 * @section Assumptions
 * 
 * A hamster is assumed to run in its running wheel while in the facility.
 * As a result if this obsession, the hamster's wheel revolves at a speed
 * of 25 RPM (rounds per minute)
 */

/**
 * @brief Global definitions
 */


/** Max name length (both owner and Hamster) */
#define HMSTR_MAX_NAME	31	

/**
 * Pricing information:
 * --------------------
 */

/** Base price for full pension */
#define HMSTR_BASE_PRICE			17
/** 5€ per 1000 revolutions of the hamster wheel */
#define HMSTR_STAY_PRICE			5
/** 1€ per call made to hmstr_howsdoing() */
#define HMSTR_CARE_PRICE			1
/** 2€ per treat dispensed by hamstercare institute */
#define HMSTR_FEED_PRICE			2


/**
 * @brief	Possible error Codes
 * 
 * Note that these are all negative numbers to distinguish them from
 * a successful completion result. All library functions return a
 * value >= 0 in case of successful completion.
 * 
 * Note also that there is a distinction between 'benign' errors
 * and 'catastrophic' ones: Your application should be prepared to
 * deal with benign errors while it may simply bail out (e.g.
 * call exit()) in case of catastrophic errors.
 */
 
#define HMSTR_ERR_NAMETOOLONG	-1	/**< the specified name is too long */
#define HMSTR_ERR_EXISTS		-2	/**< a hamster by that owner/name already exists */
#define HMSTR_ERR_NOTFOUND		-3	/**< A hamster or hamster owner (given by UUID or by name) could not be found */

#define HMSTR_ERR_STORE			-100 /**< storage error */
#define HMSTR_ERR_CORRUPT		-101 /**< database is corrupted */

/** is error catastrophic? */
#define HMSTR_ERR_IS_CATASTROPHIC(err)	((err)<=HMSTR_ERR_STORE)

/**
 * @brief	Hamster status structure
 */

struct hmstr_state
{
	uint16_t treats_left;	/**< How many treats are left in stock? */
	uint32_t rounds;		/**< Number of hamster wheel revolutions */
	int16_t cost;			/**< expenses accumulated so far */
};


/**
 * @ingroup libfuncs
 * @brief	Put a new hamster into the hamstercare institute
 * 
 * This function makes a new entry into the hamster database. It returns
 * a unique ID by which the hamster can later be referenced. A Hamster
 * may be given a number of treats to feed.
 * 
 * 
 * @return If successful: Unique ID (always >= 0) of the new entry
 * @return On error: Error code (always < 0)
 * 
 */

int32_t hmstr_new(
	const char* owner_name,		/**< [in] name of hamster's owner     */
	const char* hamster_name,	/**< [in] name of hamster             */
	uint16_t treats			/**< [in] initial provision of treats */
	);

/**
 * @ingroup libfuncs
 * @brief	Find hamster in the hamstercare institute
 * 
 * This function locates an entry in the hamster database. It returns
 * a unique ID by which the hamster can be referenced. A Hamster is
 * uniquely identified by the combination of the owner's name and the
 * hamster's name.
 * 
 * @return If successful: Unique ID (always >= 0) of the entry
 * @return On error: Error code (always < 0)
 * 
 */

int32_t hmstr_lookup(
	const char* owner_name,		/**< [in] name of hamster's owner */
	const char* hamster_name	/**< [in] name of hamster         */
	);

/**
 * @ingroup libfuncs
 * @brief	Get a directory of entries in the database
 * 
 * This function enables a "wildcard search" of the database. It delivers
 * UIDs of matching entries. The caller may specify an owner name or a
 * hamster name, thus specifying a particular entry (in this case the
 * function is similar to hmstr_lookup()). However, it is also possible
 * to specify __only__ an owner name or __only__ a hamster name by passing
 * a NULL value for the name that should not be specified. In this
 * case, the function delivers UIDs of __all__ entries matching the
 * specified name. If both names are passed as NULL, the function delivers
 * the UIDs of __all__ entries in the database.
 * 
 * The function delivers __one__ __UID__ __per__ __call__. In order to
 * keep the context across calls, the caller must pass a pointer to an
 * int variable. This variable (__not__ the pointer!) must be initialized
 * with -1 before making the first call.
 * 
 * @return If successful: Unique ID (always >= 0) of the next matching entry
 * @return On error: Error code (always < 0)
 */

int32_t hmstr_directory(
    int32_t* fdptr,	/**< [in/out] pointer where to store the open file handle across 
    * calls */
	const char* owner_name,		/**< [in] name of hamster's owner or NULL if not specified */
	const char* hamster_name	/**< [in] name of hamster or NULL if not specified        */
	);

/**
 * @ingroup libfuncs
 * @brief	How is my hamster doing?
 * 
 * This function checks upon hamster (at a cost!) identified by ID. It returns
 * the hamster's state in the given data structure.
 * 
 * @return If successful: 0, state of hamster stored in state struct
 * @return On error: Error code (always < 0)
 * 
 */

int32_t hmstr_howsdoing(
	int32_t ID,	/**< [in]  Hamster's unique ID */
	struct hmstr_state* state	/**< [out] pointer to data structure where to store information */
	);

/**
 * @ingroup libfuncs
 * @brief	Get contents of an entry in the database
 * 
 * This function delivers details of a hamster identified by UID. The
 * price is __not__ changed by a call to this function.
 * 
 * @return If successful: number of treats left in hamster' store
 * @return On error: Error code (always < 0)
 */

int32_t hmstr_readentry(
	int32_t ID,		/**< [in] Hamster's unique ID             */
	char* owner,    /**< [out] Where to store name of Owner    */
	char* name,   	/**< [out] Where to store name of Hamster  */
	int16_t* price	/**< [out] Where to store cost (up to now) */
	);

/**
 * @ingroup libfuncs
 * @brief	Give treats to my hamster
 * 
 * This function gives treats to the hamster identified by ID. The
 * Hamster's stock of treats will be used up first. If stock is
 * insufficient, more treats will be dispensed (at a cost!) and the
 * function returns a benign error.
 * 
 * @return If successful: number of treats left in stock (always >=0)
 * @return On error: Error code (always < 0)
 * 
 */

int32_t hmstr_givetreats(
	int32_t ID,		/**< [in] Hamster's unique ID */
	uint16_t treats	/**< [in] How many treats to feed */
	);
	
/**
 * @ingroup libfuncs
 * @brief	Collect all my hamsters and pay the bill
 * 
 * This function collects (i.e. deletes from the database) all
 * hamsters owned by the specified owner and sums up all their
 * expenses to produce a final bill.
 * 
 * @return If successful: number of euros to pay
 * @return On error: Error code (always < 0)
 * 
 */

int32_t hmstr_collect(
	const char* owner_name		/**< [in] name of hamster owner */
	);

#endif /* __HAMSTERLIB_H__ */

