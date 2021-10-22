#include "../include/hamsterrpc.h"

/**
 * helper functions to encode primitive data types
 */
int rpc_encode_octet(uint8_t* buffer, uint8_t data){
    buffer[0] = data;
    return 1;
};

int rpc_encode_uint16(uint8_t* buffer, uint16_t data){
    buffer[0] = data >> 8;
    buffer[1] = data;
    return 2;
};

int rpc_encode_int32(uint8_t* buffer, int32_t data){
    buffer[0] = data >> 24;
    buffer[1] = data >> 16;
    buffer[2] = data >>  8;
    buffer[3] = data;

    return 4;
};

int rpc_encode_uint32(uint8_t* buffer, uint32_t data){
    buffer[0] = data >> 24;
    buffer[1] = data >> 16;
    buffer[2] = data >>  8;
    buffer[3] = data;
    return 4;
};

int rpc_encode_int16(uint8_t* buffer, int16_t data){
    buffer[0] = data >> 8;
    buffer[1] = data;
    return 2;
};

int rpc_encode_sstring(uint8_t* buffer, char* str, size_t size){
    for(int i=0; i< size; i++){
        buffer[i] = (uint8_t)str[i];
    }
    return size;
};

// decode functions
int rpc_decode_sstring(uint8_t* buffer, char* str, size_t size){
    for(int i=0; i< size; i++){
        str[i] = (char)buffer[i];
    }
    return size;
};

int rpc_decode_int32(uint8_t* buffer, int32_t* data){
    *data = ((int32_t)buffer[0] << 24)  | 
            ((int32_t)buffer[1] << 16)  |
            ((int32_t)buffer[2] << 8)   |
            ((int32_t)buffer[3] << 0);
    return 4;
};

int rpc_decode_uint32(uint8_t* buffer, uint32_t* data){
    *data = ((uint32_t)buffer[0] << 24)  | 
            ((uint32_t)buffer[1] << 16)  |
            ((uint32_t)buffer[2] << 8)   |
            ((uint32_t)buffer[3] << 0);
    return 4;
};

int rpc_decode_uint16(uint8_t* buffer, uint16_t* data){
    *data = ((uint16_t)buffer[0] << 8)   |
            ((uint16_t)buffer[1] << 0); 
    return 2;
};

int rpc_decode_octet(uint8_t* buffer, uint8_t* data){
    *data = buffer[0];
    return 1;
};