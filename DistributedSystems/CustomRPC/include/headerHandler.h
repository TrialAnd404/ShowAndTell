#ifndef HEADERHANDLER_H
#define HEADERHANDLER_J

#include <inttypes.h>
#include "structs.h"
#include "hamsterrpc.h"

int handleHeader(uint8_t headerStream[], header* receiveHeader, header* sendHeader);

#endif