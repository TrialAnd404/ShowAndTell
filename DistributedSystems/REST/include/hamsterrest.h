/*
 * =====================================================================================
 *
 *       Filename:  hamsterrest.h
 *
 *    Description:  Helper functions to encapsulate the hamster REST api
 *
 *        Version:  1.0
 *        Created:  25.06.2018 08:24:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kai Beckmann (kai), kai.beckmann@hs-rm.de
 *        Company:  Hochschule RheinMain - DOPSY Labor für verteilte Systeme
 *
 * =====================================================================================
 */
#ifndef HAMSTERREST_H_INC
#define  HAMSTERREST_H_INC

#include <stdint.h>

#include "hamsterlib.h"

#define HREST_RET_OK                0       /**< Everthing is ok  */
#define HREST_RET_NOT_IMPLEMENTED   -150    /**< functionality is not implemented (yet) */


int hrest_helper_new(const char* owner, const char* hamster_name, uint16_t treats, int* hamster_id);
int hrest_helper_collect(const char* owner, int16_t* price);
int hrest_helper_print_all_hamsters(void);
int hrest_helper_print_owners_hamsters(const char* owner);
int hrest_helper_print_hamster(int id, const char* owner, const char* hamster_name, uint16_t price, uint16_t treats);
int hrest_helper_howsdoing(int id, struct hmstr_state* state);
int hrest_helper_givetreats(int id, uint16_t treats, uint16_t* treats_left );
int hrest_helper_queryHamster(char* owner, char* hamster_name, int* hamster_id);


void hrest_init(char *hostname);
void hrest_terminate(void);

#endif   /* ----- #ifndef HAMSTERREST_H_INC  ----- */
