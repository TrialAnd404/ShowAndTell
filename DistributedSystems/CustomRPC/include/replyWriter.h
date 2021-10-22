#ifndef REPLYWRITER_H
#define REPLYWRITER_H


// #include <stdio.h>
#include <inttypes.h>


#include "structs.h"
#include "hamsterrpc.h"
#include "hamsterlib.h"

void constructErrorMsg(uint8_t* errorBuffer, header* sendHeader, int32_t* retValue);

void encodeHeader(uint8_t* sendBuffer, header* sendHeader);

void encodeFailure(uint8_t* failBuffer, int32_t* retValue);

void encodeReplyNew(uint8_t* sendBuffer, int32_t* newReturn);
void encodeReplyLookUp(uint8_t* sendBuffer, int32_t* hamsterID);
void encodeReplyHowsDoing(uint8_t* sendBuffer, int32_t* retValue, struct hmstr_state* state);
void encodeReplyDirectory(uint8_t* sendBuffer, int32_t* retValue, int32_t* fd);
void encodeReplyReadEntry(uint8_t* sendBuffer, int32_t* retValue, char* owner, char* hamster, int16_t* price);
void encodeReplyGiveTreats(uint8_t* sendBuffer, int32_t* retValue);
void encodeReplyCollect(uint8_t* sendBuffer, int32_t* retValue);

#endif