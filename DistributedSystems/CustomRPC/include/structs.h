#ifndef STRUCTS_H
#define STRUCTS_H

#include <inttypes.h>

typedef struct{
    uint8_t version;
    uint8_t flag;
    uint16_t msgID;
    uint16_t pay_size;
    uint16_t rpcID;
} header;

#endif