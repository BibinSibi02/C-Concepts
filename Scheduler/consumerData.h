/*BIBIN  */

#ifndef PA4_CONSUMERDATA_H
#define PA4_CONSUMERDATA_H

#include "tradecrypto.h"
#include "shared.h"

typedef struct {
    ConsumerType blockchain; //wether its blockchain x or y
    unsigned int consumptionTime; //consumption time
    COMMON_DATA * sharedData; //access to shared data
} CONSUMER_DATA;
#endif //PA4_CONSUMERDATA_H
