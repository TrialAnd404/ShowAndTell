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
 
#ifndef __HAMSTERRPC_H__
#define __HAMSTERRPC_H__

#include <stdint.h>
#include <stddef.h>

/**
 * helper functions to encode primitive data types
 */
int rpc_encode_octet(uint8_t* buffer, uint8_t data);
int rpc_encode_uint16(uint8_t* buffer, uint16_t data);
int rpc_encode_int32(uint8_t* buffer, int32_t data);
int rpc_encode_uint32(uint8_t* buffer, uint32_t data);
int rpc_encode_int16(uint8_t* buffer, int16_t data);
int rpc_encode_sstring(uint8_t* buffer, char* str, size_t size);

// decode functions
int rpc_decode_sstring(uint8_t* buffer, char* str, size_t size);
int rpc_decode_int32(uint8_t* buffer, int32_t* data);
int rpc_decode_uint32(uint8_t* buffer, uint32_t* data);
int rpc_decode_uint16(uint8_t* buffer, uint16_t* data);
int rpc_decode_octet(uint8_t* buffer, uint8_t* data);


#endif /* __HAMSTERRPC_H__*/
