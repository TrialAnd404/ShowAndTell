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

#ifndef __HAMSTERPROTOCOL_H__
#define __HAMSTERPROTOCOL_H__

#include <stdint.h>

#include "hamsterlib.h"

/** 
 * @file	hamsterprotocol.h
 * @defgroup rpcprot hamster RPC protocol
 * @brief	Protocol specification for the hamster RPC
 * 
 * This file is to be included by all applications implementing the hamster
 * RPC protocol. 
 * It defines necessary IDs and the protocol itself.
 */
 
 
 
/**
 * 
 * Message Header Format
 * @ingroup rpcprot
 *
 *@{*/

#define HAMSTER_RPC_PROT_VERSION 0xB6    /**< \brief Current Version of the hamster RPC protocol */
    
#define HAMSTER_RPC_PROT_HEADER_SIZE 8   /**< \brief Size in byte of the RPC protocol header */   

#define HAMSTER_RPC_PROT_HEADER_VERSION_OFFSET	0   /**< \brief Offset of Version field the RPC protocol header */
#define HAMSTER_RPC_PROT_HEADER_FLAGS_OFFSET	1   /**< \brief Offset of Version field the RPC protocol header */
#define HAMSTER_RPC_PROT_HEADER_MSGID_OFFSET	2   /**< \brief Offset of Version field the RPC protocol header */
#define HAMSTER_RPC_PROT_HEADER_PAYLOADLENGTH_OFFSET 4 /**< \brief Offset of Version field the RPC protocol header */
#define HAMSTER_RPC_PROT_HEADER_CALLID_OFFSET	6   /**< \brief Offset of Version field the RPC protocol header */

/**@}*/

/**
 * Bit masks for the protocol flags
 * @ingroup rpcprot
 *
 *@{*/
#define HAMSTER_RPC_MASK_FLAG_ERROR     0x02    /**< \brief Bit mask for the error flag (set if the message is an error) **/
#define HAMSTER_RPC_MASK_FLAG_RESPONSE  0x01    /**< \brief Bit mask for the response flag (set if the message is an response) **/
#define HAMSTER_RPC_MASK_FLAG_REQUEST   0x00    /**< \brief Bit mask if message is an normal request message **/
/**@}*/

/**
 * Specification of the RPC-Call-IDs for the
 * supported hamsterlib functions
 * 
 * @ingroup rpcprot
 * 
 *
 *@{*/
#define HAMSTER_RPC_FUNCID_NEW          0x0001  /**< \brief id for the int32_t hmstr_new() RPC call **/
#define HAMSTER_RPC_FUNCID_LOOKUP       0x0002  /**< \brief id for the int32_t hmstr_lookup() RPC call **/
#define HAMSTER_RPC_FUNCID_DIRECTORY    0x0003  /**< \brief id for the int32_t hmstr_directory() RPC call **/
#define HAMSTER_RPC_FUNCID_HOWSDOING    0x0004  /**< \brief id for the int32_t hmstr_howsdoing() RPC call **/
#define HAMSTER_RPC_FUNCID_READENTRY    0x0005  /**< \brief id for the int32_t hmstr_readentry() RPC call **/
#define HAMSTER_RPC_FUNCID_GIVETREATS   0x0006  /**< \brief id for the int32_t hmstr_givetreats() RPC call **/
#define HAMSTER_RPC_FUNCID_COLLECT      0x0007  /**< \brief id for the int32_t hmstr_collect() RPC call **/
/**@}*/

/**
 * @page rpcprot_page The hamster RPC protocol specification
 * @ingroup rpcprot
 * 
 * @tableofcontents
 * 
 *  
 * @section rpc-intro Introduction
 * 
 * This file specifices the protocol to offer the functionality of the
 * hamster libary over an RPC mechanism for remote users. 
 * 
 * The RPC server provides access to the hamster lib functionality and offers 
 * the hamster lib functions as RPC calls. 
 * 
 * One successfull RPC call consists of one request message send by the client
 * and a response message send by the server.
 * 
 * The request message identifies the requested hamster lib call and provides
 * the necessary \em in parameters of the function.
 * 
 * The server unmarshals the request and the provided parameters and calls the 
 * requested hamster lib function with the given parameters. The server creates
 * a response message, marshals the return and all \em out parameters of
 * the called hamster lib function and sends it to the client.
 * 
 * The following sequence diagram gives an abstract overview of the procedure
 * for the function hmstr_new()
 * 
 * @verbatim
+-------+         +-----------+              +-----------+            +-------------+
| Main  |         | RPCClient |              | RPCServer |            | HamsterLib  |
+-------+         +-----------+              +-----------+            +-------------+
    |                   |                          |                         |
    | hmstr_new()       |                          |                         |
    |------------------>|                          |                         |
    |                   | createRequestMsg()       |                         |
    |                   |-------------------       |                         |
    |                   |                  |       |                         |
    |                   |<------------------       |                         |
    |                   | sendRequest              |                         |
    |                   |------------------------->|                         |
    |                   |                          | unmarshalRequest        |
    |                   |                          |-----------------        |
    |                   |                          |                |        |
    |                   |                          |<----------------        |
    |                   |                          | hmstr_new()             |
    |                   |                          |------------------------>|
    |                   |                          |                         |
    |                   |                          |                  return |
    |                   |                          |<------------------------|
    |                   |                          |                         |
    |                   |                          | marshallResponse        |
    |                   |                          |-----------------        |
    |                   |                          |                |        |
    |                   |                          |<----------------        |
    |                   |             sendResponse |                         |
    |                   |<-------------------------|                         |
    |                   | unmarshalResponse        |                         |
    |                   |------------------        |                         |
    |                   |                 |        |                         |
    |                   |<-----------------        |                         |
    |            return |                          |                         |
    |<------------------|                          |                         |
    |                   |                          |                         |

 @endverbatim
 * 
 * If the called hamster lib function return an error, a special error message 
 * is constructed and send to the client as response!
 * 
 * @section rpc-overal Overall protocol structure
 * 
 * The messages of the hamster RPC protocol consists of a common header and 
 * the encoded payload. For each primitive data type and the necessary
 * composite data types an encoding is defined in the following subsections.
 * 
 * 
 * @section rpc-encoding Encoding of the data types
 * 
 * 
 * @subsection rpc-enc-int int32_t
 * 
 * An signed integer has the size of 32 bit. It is encoded in the normal
 * network byte order (Big Endian).
 * 
 * 
 * @verbatim
  	       (MSB)                   (LSB)
         +-------+-------+-------+-------+
         |byte 0 |byte 1 |byte 2 |byte 3 |          int32_t
         +-------+-------+-------+-------+
         <------------32 bits------------>
 @endverbatim
 * 
 * 
 * @subsection rpc-enc-uint uint32_t
 * 
 * An unsigned integer has the size of 32 bit. It is encoded in the normal
 * network byte order (Big Endian).
 * 
 * @verbatim
           (MSB)                   (LSB)
         +-------+-------+-------+-------+
         |byte 0 |byte 1 |byte 2 |byte 3 |          uint32_t
         +-------+-------+-------+-------+
         <------------32 bits------------>
 @endverbatim
 * 
 * @subsection rpc-enc-short int16_t
 * 
 * An signed short has the size of 16 bit. It is encoded in the normal
 * network byte order (Big Endian).
 * 
 * 
 * @verbatim
        (MSB)         (LSB)
         +-------+-------+
         |byte 0 |byte 1 |                          int16_t
         +-------+-------+
         <----16 bits---->
 @endverbatim
 * 
* @subsection rpc-enc-ushort uint16_t
 * 
 * An unsigned short has the size of 16 bit. It is encoded in the normal
 * network byte order (Big Endian).
 * 
 * 
 * @verbatim
        (MSB)         (LSB)
         +-------+-------+
         |byte 0 |byte 1 |                          uint16_t
         +-------+-------+
         <----16 bits---->
 @endverbatim
 * 
 * 
 * @subsection rpc-enc-string Fixed length strings
 * 
 * Strings are transmitted as US-ASCII with one byte per character.
 * 
 * Since the hamster lib uses strings with a fixed size it is necessary to
 * always transmit the maximum amount of bytes, even if the number of
 * actually valid characters is less.
 * 
 * This means if the string "Hello" with 5 + 1 characters (+1 for the string 
 * terminating zero byte) is to be transmitted and if the fixed string size 
 * is 32 bytes, then the whole 32 bytes must transmitted.  
 * 
 * IMPORTANT: all remaining bytes not used for characters must be set to zero!
 * 
 * @verbatim
          0        1     ...
      +--------+--------+...+--------+--------+...+--------+
      | byte 0 | byte 1 |...|byte n-1|    0   |...|    0   |
      +--------+--------+...+--------+--------+...+--------+
      |<-----------n bytes---------->|<------r bytes------>|
      |<-----------n+r (where (n+r) = fixed length)------------>|
                                                    FIXED-LENGTH STRING
 @endverbatim
 * 
 * 
 * For example the string "Harald" with a fixed 
 * length of 32 (31 + at least one \0) should be transmitted as
 * @verbatim
    0x48 0x61 0x72 0x61 
    0x6c 0x64 0x00 0x00 
    0x00 0x00 0x00 0x00 
    0x00 0x00 0x00 0x00 
    0x00 0x00 0x00 0x00 
    0x00 0x00 0x00 0x00 
    0x00 0x00 0x00 0x00 
    0x00 0x00 0x00 0x00
 @endverbatim
 * @subsection rpc-enc-struct Structs
 * 
 * The members of a c struct are encoded sequentially in the same order as 
 * within the struct with their appropriate encoding method.
 * A padding must not be added. If the native representation of a struct
 * within memory includes a padding, it has to be removed.
 * 
 * structs are implicitly typed, therefore the identifyer is not 
 * transmitted. 
 * 
 * @verbatim
         struct {
            component-declaration-A;
            component-declaration-B;
            ...
         } identifier;
 @endverbatim
 * 
 * 
 * @verbatim
         +-------------+-------------+...
         | component A | component B |...           STRUCTURE
         +-------------+-------------+..
 @endverbatim
 * 
 * 
 * @section rpc-header Protocol header
 * 
 * The header of the hamster RPC protocol has a fixed size of 8 bytes. The header
 * is the same for all RPC requests as well as for responses.
 * 

 * 
 * 
 * @verbatim
    0                   1                   2                   3  
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +---------------------------------------------------------------+
    |    Version    |     Flags     |             Msg-ID            |
    +---------------------------------------------------------------+
    |        Payload-Length         |           RPC-Call-ID         |
    +---------------------------------------------------------------+
    |                                                               ~
    ~             Payload (Payload-Length * bytes)                  |
    +---------------------------------------------------------------+
 @endverbatim
 * 
 * @subsection rpc-header-version Version
 * @subsection rpc-header-flags Flags
 * 
 * - Bit 1 - Response     (set if msg is an response)
 * - Bit 2 - Error        (set if error occurred)
 * - Bit 3 - reserved
 * - Bit 4 - reserved
 * - Bit 5 - reserved
 * - Bit 6 - reserved
 * - Bit 7 - reserved
 * - Bit 8 - reserved
 * 
 * The second byte of the header is used for flags.
 *
 * The first bit indicates the type of a message. If it is not set (and no 
 * error flag), then the message is a request. If it is set, the message is
 * a reponse. 
 * 
 * The second bit indicates an error message. If it is set, the reponse contains
 * an error code. For a request the error bit shall not be set.
 * 
 * The remaining bits are reserved for future use and shall not be used.
 * 
 * @subsection rpc-header-msg-id Message ID
 * 
 * Each request shall have an ID. The corresponding response shall use
 * the same ID. Seperate requests, at least in one session, should not
 * have the same ID.
 * 
 * The message ID has the size of 16 bits and is an unsigned integer.
 * 
 * @subsection rpc-header-payload-length Payload length
 * 
 * The length of the payload following the header is encoded in an
 * unsigned 16 bit integer. Therefore it is not possible to send 
 * messages with more than 65535 bytes of payload.
 * 
 * The size of the header is not part of the encoded payload lenght!
 * 
 *  
 * @subsection rpc-header-rpc-call-id RPC Call ID
 * 
 * Each of the hamster libary functions has an assigned ID. This ID is used
 * to specify the nature of the request and the corresponding reponse.
 * 
 * If a client wants to call a hamster libary function by RPC, the 
 * ID of this function must be used. The server's response must use the 
 * same ID.
 * 
 * @subsection rpc-header-rpc-payload Payload
 * 
 * The payload is marshalled after the header into the RPC message. It contains
 * the arguments of the called hamster libary functions.
 * 
 * The arguments are marshalled in the order of the parameter list of the 
 * function prototype. The corresponding encodings for the data type must be
 * used. The parameter name is not part of the payload!
 * 
 * This applies to request messages as well as to response messages!
 * It is necessary to differenciate between "in", "out" and "inout" parameters.
 * 
 * A request message contains all the arguments with an in and inout 
 * direction assigned. 
 * 
 * A response message starts with the result value! After the return value
 * the out and inout arguments are marshalled in the order of their position 
 * in the parameter list of the function call. If there is no out or inout 
 * parameter, only the return value is part of the payload!
 * 
 * The data of the payload is not aligned, padding must not be used!
 * 
 * 
 * @section rpc-request RPC requests
 * 
 * An RPC request is send by the client to the server and contains the
 * message header and the encoded arguments of the hamster libary function to
 * be called.
 * 
 * As an example the function hmstr_howsdoing shall be called with the argument
 * 23 for the ID parameter. The parameter state is an output parameter and
 * therefore not part of the request message!
 * 
 * int32_t hmstr_howsdoing(int32_t ID, struct hmstr_state* state)
 * 
 * There should be separate encoding functions for each possible datatype.
 * These should return the size of the encoded data in bytes. This can be used 
 * to simplify the encoding sequence.
 * If "b" is the pointer to the byte buffer to store the message and
 * "size" contains the amount of encoded bytes so far, the encoding functions 
 * can be called in the following manner:
 *@code
size += encode_foo(b+size, foo)
 @endcode
 * 
 * The following figure is an abstract example of a possible (simplified!) call 
 * sequence to create a request message.
 * 
 * @verbatim

+-------+               +-----------+                               +---------+ +-----------+
| Main  |               | RPCClient |                               | Helper  | | RPCServer |
+-------+               +-----------+                               +---------+ +-----------+
    |                         |                                          |            |
    | hmstr_howsdoing(ID, &s) |                                          |            |
    |------------------------>|                                          |            |
    |                         | createHowsDoingRequestMsg()              |            |
    |                         |----------------------------              |            |
    |                         |                           |              |            |
    |                         |<---------------------------              |            |
    |                         |                                          |            |
    |                         | encode_octet(b,0xB6) // header version   |            |
    |                         |----------------------------------------->|            |
    |                         |                           return size = 1|            |
    |                         |<-----------------------------------------|            |
    |                         | encode_octet(b,0) // flags               |            |
    |                         |----------------------------------------->|            |
    |                         |                          return size = 1 |            |
    |                         |<-----------------------------------------|            |
    |                         | encode_uint16(b,msg-id)                  |            |
    |                         |----------------------------------------->|            |
    |                         |                          return size = 2 |            |
    |                         |<-----------------------------------------|            |
    |                         |                                          |            |
    |                         | encode_uint16(b,4) //payload len = 4     |            |
    |                         |----------------------------------------->|            |
    |                         |                          return size = 2 |            |
    |                         |<-----------------------------------------|            |
    |                         | encode_uint16(b,HAMSTER_RPC_FUNCID_HOWS) |            |
    |                         |----------------------------------------->|            |
    |                         |                           return size = 2|            |
    |                         |<-----------------------------------------|            |
    |                         | encode_int32(b,ID) // payload in-para    |            |
    |                         |----------------------------------------->|            |
    |                         |                          return size = 4 |            |
    |                         |<-----------------------------------------|            |
    |                         |                                          |            |
    |                         | sendRequest                              |            |
    |                         |------------------------------------------------------>|
    |                         |                                          |            |

 @endverbatim
 * On the receiver side the request message has to be unmarshalled. There
 * should be seperate decoding functions returning the size for the data 
 * types as well.
 * 
 * The following abstract (and simplified!!) diagram shows the sequence
 * of the decoding.
 * 
 * @verbatim
+-----------+       +-----------+                             +---------+
| RPCClient |       | RPCServer |                             | Helper  |
+-----------+       +-----------+                             +---------+
      |                   |                                        |
      | sendRequest       |                                        |
      |------------------>|                                        |
      |                   | receiveBytes(8) // receive header      |
      |                   |----------------                        |
      |                   |               |                        |
      |                   |<---------------                        |
      |                   | decode_octet(b,&version)               |
      |                   |--------------------------------------->|
      |                   |                        return size = 1 |
      |                   |<---------------------------------------|
      |                   | decode_octet(b,&flags)                 |
      |                   |--------------------------------------->|
      |                   |                        return size = 1 |
      |                   |<---------------------------------------|
      |                   | decode_uint16(b,&msg_id)               |
      |                   |--------------------------------------->|
      |                   |                        return size = 2 |
      |                   |<---------------------------------------|
      |                   | decode_uint16(b,&pay_size)             |
      |                   |--------------------------------------->|
      |                   |                        return size = 2 |
      |                   |<---------------------------------------|
      |                   | decode_uint16(b,&rpc_func_id)          |
      |                   |--------------------------------------->|
      |                   |                        return size = 2 |
      |                   |<---------------------------------------|
      |                   | receiveBytes(pay_size) // payload      |
      |                   |----------------------------------      |
      |                   |                                 |      |
      |                   |<---------------------------------      |
      |                   | decodeHowsDoing                        |
      |                   |----------------                        |
      |                   |               |                        |
      |                   |<---------------                        |
      |                   | decode_int31(b,&hmst_id)               |
      |                   |--------------------------------------->|
      |                   |                         return size = 4|
      |                   |<---------------------------------------|
      |                   |                                        |

 @endverbatim 
 * @section rpc-response RPC responses
 * 
 * After the server has executed the requested hamster libary function call,
 * it assembles a response message and sends it to the requesting client.
 * 
 * To continue the example the call of hmstr_howsdoing() shall have returned
 * a struct with following content:
 *@code
struct hmstr_state state = 
{
	.treats_left = 23,
	.rounds = 42,		
	.cost = 1337		
};
 @endcode
 * 
 * Since the parameter state is an out paramater it has to be part of the payload
 * 
 * The call of the function was sucessfull, therefore the return value is "0".
 * The return value have to be the first encoded data in the payload!
 * 
 * The following abstract (and simplified!!) diagram shows the sequence
 * of the encoding.
 * 
 *@verbatim
+-----------+        +-----------+                              +---------+
| RPCClient |        | RPCServer |                              | Helper  |
+-----------+        +-----------+                              +---------+
      |                    |                                         |
      |                    | handleHowsDoing                         |
      |                    |----------------                         |
      |                    |               |                         |
      |                    |<---------------                         |
      |                    | createHowsDoingRespMsg()                |
      |                    |-------------------------                |
      |                    |                        |                |
      |                    |<------------------------                |
      |                    | encode_octet(b,0xB6) // version         |
      |                    |---------------------------------------->|
      |                    |                         return size = 1 |
      |                    |<----------------------------------------|
      |                    | encode_octet(b,HAMS..._FLAG_RESPONSE)   |
      |                    |---------------------------------------->|
      |                    |                         return size = 1 |
      |                    |<----------------------------------------|
      |                    | encode_uint16(b,msg_id) //msg id of req |
      |                    |---------------------------------------->|
      |                    |                         return size = 2 |
      |                    |<----------------------------------------|
      |                    | encode_uint16(b,12) // size payload     |
      |                    |---------------------------------------->|
      |                    |                         return size = 2 |
      |                    |<----------------------------------------|
      |                    | encode_uint16(b,HAMSTER_RPC_FUNCID_HOWS)|
      |                    |---------------------------------------->|
      |                    |                         return size = 2 |
      |                    |<----------------------------------------|
      |                    | encode_int32(b,0) // return code value  |
      |                    |---------------------------------------->|
      |                    |                         return size = 4 |
      |                    |<----------------------------------------|
      |                    | encode_uint16(b,23) //state.treats_left |
      |                    |---------------------------------------->|
      |                    |                         return size = 2 |
      |                    |<----------------------------------------|
      |                    | encode_uint32(b,42) //state.rounds      |
      |                    |---------------------------------------->|
      |                    |                        return size = 4  |
      |                    |<----------------------------------------|
      |                    | encode_int16(b,1337)//state.cost        |
      |                    |---------------------------------------->|
      |                    |                         return size = 2 |
      |                    |<----------------------------------------|
      |                    |                                         |
      |       sendResponse |                                         |
      |<-------------------|                                         |
      |                    |                                         |
 @endverbatim
 * 
 * The steps to be taken for decoding at the client side is analogous. 
 * 
 * @section rpc-error-messages RPC Error message
 * 
 * In case th the hamster libary function returned an error, an error message
 * is created an returned.
 * 
 * The difference to an normal response message is:
 * 
 * 1. The error flag is set in addition to the reponse message flag
 * 
 * 2. The only payload is the return code value
 */


#endif /* __HAMSTERPROTOCOL_H__ */
