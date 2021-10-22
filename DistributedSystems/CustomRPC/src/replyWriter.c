#include "../include/replyWriter.h"

#include <stdio.h>
// #include <inttypes.h>


#include "structs.h"
#include "hamsterrpc.h"
#include "hamsterlib.h"

void constructErrorMsg(uint8_t* errorBuffer, header* sendHeader, int32_t* retValue){
    sendHeader->flag=2;
    sendHeader->pay_size=4;
    encodeHeader(errorBuffer, sendHeader);
    encodeFailure(errorBuffer, retValue);
}

void encodeHeader(uint8_t* sendBuffer, header* sendHeader){
    printf("encoding version\n");
    rpc_encode_octet(&sendBuffer[0], sendHeader->version);
    printf("encoding flag\n");
    rpc_encode_octet(&sendBuffer[1], sendHeader->flag);
    printf("encoding msgID: %d\n", sendHeader->msgID);
    rpc_encode_uint16(&sendBuffer[2], sendHeader->msgID);
    rpc_encode_uint16(&sendBuffer[4], sendHeader->pay_size);
    printf("encoding payload size\n\n");
    rpc_encode_uint16(&sendBuffer[6], sendHeader->rpcID);
    printf("encoding rpcID: %d\n", sendHeader->rpcID);
    
};

void encodeReplyNew(uint8_t* sendBuffer, int32_t* newReturn){
    printf("encoding return value of new: %d\n", *newReturn);
    rpc_encode_int32(&sendBuffer[8], *newReturn);
}

void encodeReplyLookUp(uint8_t* sendBuffer, int32_t* hamsterID){
    printf("encoding return value of lookup\n");
    rpc_encode_int32(&sendBuffer[8], *hamsterID);
}

void encodeReplyHowsDoing(uint8_t* sendBuffer, int32_t* retValue, struct hmstr_state* state){
    printf("encoding return value of howsdoing\n");
    rpc_encode_int32(&sendBuffer[8], *retValue);
    rpc_encode_uint16(&sendBuffer[12], state->treats_left);
    rpc_encode_uint32(&sendBuffer[14], state->rounds);
    rpc_encode_int16(&sendBuffer[18], state->cost);
}

void encodeReplyDirectory(uint8_t* sendBuffer, int32_t* retValue, int32_t* fd){
    printf("encoding return value of directory\n");
    rpc_encode_int32(&sendBuffer[8], *retValue);
    rpc_encode_int32(&sendBuffer[12], *fd);
}

void encodeReplyReadEntry(uint8_t* sendBuffer, int32_t* retValue, char* owner, char* hamster, int16_t* price){
    printf("encoding return value of readentry\n");
    rpc_encode_int32(&sendBuffer[8], *retValue);
    rpc_encode_sstring(&sendBuffer[12], owner, 32);
    rpc_encode_sstring(&sendBuffer[44], hamster, 32);
    rpc_encode_int16(&sendBuffer[76], *price);
}

void encodeReplyGiveTreats(uint8_t* sendBuffer, int32_t* retValue){
    printf("encoding return value of givetreats\n");
    rpc_encode_int32(&sendBuffer[8], *retValue);
}

void encodeReplyCollect(uint8_t* sendBuffer, int32_t* retValue){
    printf("encoding return value of collect\n");
    rpc_encode_int32(&sendBuffer[8], *retValue);
}

void encodeFailure(uint8_t* failBuffer, int32_t* retValue){
    printf("encoding failure buffer\n");
    rpc_encode_int32(&failBuffer[8], *retValue);
}