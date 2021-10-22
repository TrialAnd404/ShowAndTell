#include "payloadHandler.h"

#include <stdio.h>
#include <string.h>

// #include "structs.h"
// #include "hamsterlib.h"
// #include "replyWriter.h"
// #include "hamsterrpc.h"

int32_t executeNew(uint8_t payloadStream[]){
    char owner[32];
    char hamster[32];
    memset ( owner, '\0', 32 );
    memset ( hamster , '\0', 32 );
    uint16_t treats;

    rpc_decode_sstring(&payloadStream[0], owner, 32);
    rpc_decode_sstring(&payloadStream[32], hamster, 32);
    rpc_decode_uint16(&payloadStream[64], &treats);

    //new ausführen
    // result =  hmstr_new()
    uint32_t result = hmstr_new(owner, hamster, treats);
    return result;
}

int32_t executeLookUp(uint8_t payloadStream[]){
    char owner[32];
    char hamster[32];
    memset ( owner, '\0', 32 );
    memset ( hamster , '\0', 32 );

    rpc_decode_sstring(&payloadStream[0], owner, 32);
    rpc_decode_sstring(&payloadStream[32], hamster, 32);

    //lookup ausführen
    // result = hmstr_lookup(); <----- wenn result <0, lookup fehlgeschlagen
    int32_t result = hmstr_lookup(owner, hamster);
    
    return result;
}

int32_t executeHowsDoing(uint8_t payloadStream[], struct hmstr_state* state){
    int32_t hamsterID;

    rpc_decode_int32(&payloadStream[0], &hamsterID);

    // result = hmstr_lookup(); <----- wenn result <0, lookup fehlgeschlagen

    int32_t result = hmstr_howsdoing(hamsterID, state); 
    
    return result;
}

int32_t executeDirectory(uint8_t payloadStream[], int32_t* fd){
    char owner[32];
    char hamster[32];
    memset ( owner,'\0', 32 );
    memset ( hamster ,'\0', 32 );
    char* ownerptr = &owner[0];
    char* hmstrptr = &hamster[0];

    rpc_decode_int32(&payloadStream[0], fd);
    rpc_decode_sstring(&payloadStream[4], owner, 32);
    rpc_decode_sstring(&payloadStream[36], hamster, 32);

    printf("owner: %s\n", owner);
    if(owner[0] == '\0')
        ownerptr = NULL;
    if(hamster[0] == '\0')
        hmstrptr = NULL;
        
    //lookup ausführen
    // result = hmstr_lookup(); <----- wenn result <0, lookup fehlgeschlagen
    int32_t result = hmstr_directory(fd, ownerptr, hmstrptr); 
    
    printf("after reading: fd=%d\n", *fd);

    return result;
}

int32_t executeReadEntry(uint8_t payloadStream[], char* owner, char* hamster, int16_t* price){
    
    int32_t hamsterID;
    int32_t treats; //<<--- return value of readentry

    rpc_decode_int32(&payloadStream[0], &hamsterID);

    // readentry ausführen
    // owner = "test";
    // hamster = "test";
    // *price = 5;

    treats = hmstr_readentry(hamsterID, owner, hamster, price);

    return treats;
}

int32_t executeGiveTreats(uint8_t payloadStream[]){
    int32_t leftovers; //<<-- return value of feed
    int32_t hamsterID;
    uint16_t treats;

    rpc_decode_int32(&payloadStream[0], &hamsterID);
    rpc_decode_uint16(&payloadStream[4], &treats);

    leftovers = hmstr_givetreats(hamsterID, treats);
    
    return leftovers;
}

int32_t executeCollect(uint8_t* payloadStream){
    int32_t totalPrice;
    char owner[32];
    memset ( owner, '\0', 32 );

    rpc_decode_sstring(&payloadStream[0], owner, 32);

    totalPrice = hmstr_collect(owner);
    return totalPrice;
}

//Handler funktionen
//#######

int32_t handleNew(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader){
    //handle + execute new hamster
    int32_t retValue;

    retValue = executeNew(payloadStream);
    if(retValue >= 0){
        printf("encoding header\n");
        encodeHeader(sendBuffer, sendHeader);
        printf("encoding payload: %d\n", retValue);
        encodeReplyNew(sendBuffer, &retValue);
    }
    return retValue;
}

int32_t handleLookUp(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader){
    //handle and execute howsdoing
    int32_t retValue;

    retValue = executeLookUp(payloadStream);
    printf("hamsterID: %d\n", retValue);
    if(retValue >= 0){
        printf("encoding header\n");
        encodeHeader(sendBuffer, sendHeader);
        printf("encoding payload\n");
        encodeReplyLookUp(sendBuffer, &retValue);
    }
    return retValue;
}

int32_t handleHowsDoing(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader){
    //handle and execute howsdoing
    struct hmstr_state state;

    int32_t retValue = executeHowsDoing(payloadStream, &state);
    if(retValue >= 0){
        printf("encoding header\n");
        encodeHeader(sendBuffer, sendHeader);
        printf("encoding payload\n");
        encodeReplyHowsDoing(sendBuffer, &retValue, &state);
    }
    return retValue;
}

int32_t handleDirectory(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader){
    //handle and execute howsdoing
    int32_t fileDescriptor; //im ersten Aufruf = -1

    int32_t retValue = executeDirectory(payloadStream, &fileDescriptor);
    if(retValue >= 0){
        printf("encoding header\n");
        encodeHeader(sendBuffer, sendHeader);
        printf("encoding payload\n");
        encodeReplyDirectory(sendBuffer, &retValue, &fileDescriptor);
    }
    return retValue;
}

int32_t handleReadEntry(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader){
    //handle and execute read entry
    char owner[32];
    char hamster[32];
    memset ( owner, '\0', 32 );
    memset ( hamster , '\0', 32 );
    int16_t price;

    int32_t retValue = executeReadEntry(payloadStream, owner, hamster, &price);
    if(retValue >= 0){
        printf("encoding header\n");
        encodeHeader(sendBuffer, sendHeader);
        printf("encoding payload\n");
        encodeReplyReadEntry(sendBuffer, &retValue, owner, hamster, &price);
    }
    return retValue;
}

int32_t handleGiveTreats(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader){
    //handle and execute give treats

    int32_t retValue = executeGiveTreats(payloadStream);
    if(retValue >= 0){
        printf("encoding header\n");
        encodeHeader(sendBuffer, sendHeader);
        printf("encoding payload\n");
        encodeReplyGiveTreats(sendBuffer, &retValue);
    }
    return retValue;
}

int32_t handleCollect(uint8_t* payloadStream, uint8_t sendBuffer[], header* sendHeader){
    //handle and execute collect

    int32_t retValue = executeCollect(payloadStream);
    if(retValue >= 0){
        printf("encoding header\n");
        encodeHeader(sendBuffer, sendHeader);
        printf("encoding payload\n");
        encodeReplyCollect(sendBuffer, &retValue);
    }
    return retValue;
}

//handler to call requested functions
//###################################

int32_t handlePayload(uint8_t payloadStream[], uint8_t sendBuffer[], header* receiveHeader, header* sendHeader){
    switch(receiveHeader->rpcID){
        case 1:
            return handleNew(payloadStream, sendBuffer, sendHeader);
            break;
        case 2:
            return handleLookUp(payloadStream, sendBuffer, sendHeader);
            break;
        case 3:
            return handleDirectory(payloadStream, sendBuffer, sendHeader);
            break;
        case 4:
            return handleHowsDoing(payloadStream, sendBuffer, sendHeader);
            break;
        case 5:
            return handleReadEntry(payloadStream, sendBuffer, sendHeader);
            break; 
        case 6:
            return handleGiveTreats(payloadStream, sendBuffer, sendHeader);
            break;
        case 7:
            return handleCollect(payloadStream, sendBuffer, sendHeader);
            break;
    }
    return 0;
}