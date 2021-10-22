#ifndef PAYLOADHANDLER_H
#define PAYLOADHANDLER_H

#include "structs.h"
#include "hamsterlib.h"
#include "replyWriter.h"
#include "hamsterrpc.h"

int32_t executeNew(uint8_t payloadStream[]);

int32_t executeLookUp(uint8_t payloadStream[]);

int32_t executeHowsDoing(uint8_t payloadStream[], struct hmstr_state* state);

int32_t executeDirectory(uint8_t payloadStream[], int32_t* fd);

int32_t executeReadEntry(uint8_t payloadStream[], char* owner, char* hamster, int16_t* price);

int32_t executeGiveTreats(uint8_t payloadStream[]);

int32_t executeCollect(uint8_t* payloadStream);
//Handler funktionen
//#######

int32_t handleNew(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader);

int32_t handleLookUp(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader);

int32_t handleHowsDoing(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader);

int32_t handleDirectory(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader);

int32_t handleReadEntry(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader);

int32_t handleGiveTreats(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader);

int32_t handleCollect(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader);

//handler to call requested functions
//###################################

int32_t handlePayload(uint8_t payloadStream[], uint8_t sendBuffer[], header* receiveHeader, header* sendHeader);


#endif