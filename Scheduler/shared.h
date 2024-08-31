/*BIBIN  */
#ifndef PA4_SHARED_H
#define PA4_SHARED_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "semaphore.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include "tradecrypto.h"

//shared contents between threads

#define MAX_BUFFER = 15;// max number of objects in buffer
#define MAX_BITBUFFER = 6; //max number of bitcoins in buffer


typedef struct {

    int totalRequests;// total amt of requests
    int currProduced;//current amt of requests made
    int currConsumed;//current amt of requests consumed
    std::queue <RequestType> *buffer; //the buffer queue containing requests
    pthread_mutex_t buffer_mutex; //buffer mutex lock
    pthread_cond_t unconsumed; // for consumer to know there is room to consume
    pthread_cond_t availableSlots; //for producer to know there is room to produce
    pthread_cond_t availableBitcoinSlots; //for bitcoin producer to know there is room to producce bitcoins
    //int numEtherium;//number of etherium requests in the buffer
    //int numBitcoin;//number of bitcoin requests in the buffer


    //allocated in order of bitcoin being at position 0 and etherium at position 1
    unsigned int produced[2] = {0,0};
    unsigned int inRequestQueue[2] = {0,0};
    unsigned int consumed[2] = {0,0};
    //2d array
    unsigned int *report[2];

    sem_t barrier;

    //produced and consumed arrays

    //barrier semaphore initialized to zero



} COMMON_DATA;

#endif //PA4_SHARED_H
