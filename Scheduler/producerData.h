/*BIBIN  */

#ifndef PA4_PRODUCERDATA_H
#define PA4_PRODUCERDATA_H

#include "tradecrypto.h"
#include "shared.h"

typedef struct {
    RequestType request;//wether it is a bitcoin or etherium producer
    unsigned int productionTime;  //production time
    COMMON_DATA *sharedData; //access to shared data

} PRODUCER_DATA;

#endif //PA4_PRODUCERDATA_H
