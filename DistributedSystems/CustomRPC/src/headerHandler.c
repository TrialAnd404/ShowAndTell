#include "headerHandler.h"

#include <stdio.h>

//basic decoding of header
int handleHeader(uint8_t headerStream[], header* receiveHeader, header* sendHeader)
{
    printf("decoder called:\n");
    rpc_decode_octet(&headerStream[0], &receiveHeader->version);
    printf("version decoded: %d\n", receiveHeader->version);
    rpc_decode_octet(&headerStream[1], &receiveHeader->flag);
    printf("flag decoded: %d\n", receiveHeader->flag);
    rpc_decode_uint16(&headerStream[2], &receiveHeader->msgID);
    printf("msgID decoded: %d\n", receiveHeader->msgID);
    rpc_decode_uint16(&headerStream[4], &receiveHeader->pay_size);
    printf("paySize decoded: %d\n", receiveHeader->pay_size);
    rpc_decode_uint16(&headerStream[6], &receiveHeader->rpcID);
    printf("rpcID decoded: %u\n", receiveHeader->rpcID);
    sendHeader->flag = 1;
    sendHeader->msgID = receiveHeader->msgID;
    sendHeader->rpcID = receiveHeader->rpcID;
    sendHeader->version = receiveHeader->version;

    switch(sendHeader->rpcID){
        case 1: //new
            sendHeader->pay_size = 4;
            break;
        case 2: //lookup
            sendHeader->pay_size = 4;
            break;
        case 3: //directory
            sendHeader->pay_size = 8;
            break;
        case 4: //howsdoing
            sendHeader->pay_size = 16;
            break;
        case 5: //readentry
            sendHeader->pay_size = 70;
            break;
        case 6: //givetreats
            sendHeader->pay_size = 4;
            break;
        case 7: //collect
            sendHeader->pay_size = 4;
            break;
    }

    return 0;
}