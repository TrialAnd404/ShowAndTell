#ifndef PAYLOADDECODER_H
#define PAYLOADDECODER_H

static int decodePayload(uint8_t payloadStream[], uint16_t* rpcID){
    switch(*rpcID){
        case 1:
            decodeNew(payloadStream);
            break;
        case 2:
            decodeHowsDoing(payloadStream);
            break;
        case 3:
            decodeFeed(payloadStream);
            break;
        case 4:
            decodeRetrieve(payloadStream);
            break;
        case 5:
            decodeList(payloadStream);
            break;
    }
    //test input
}

static uint8_t decodeNew(uint8_t payloadStream[]){
    return 1;
};

static int decodeHowsDoing(uint8_t payloadStream[]){
    return 2;
};

static int decodeFeed(uint8_t payloadStream[]){
    return 3;
};

static int decodeRetrieve(uint8_t payloadStream[]){
    return 4;
};

static int decodeList(uint8_t payloadStream[]){
    return 5;
};



#endif