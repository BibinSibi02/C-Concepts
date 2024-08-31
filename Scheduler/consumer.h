/*BIBIN  */

#ifndef PA4_CONSUMER_H
#define PA4_CONSUMER_H


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <string>
#include <fstream>
#include <iostream>
#include <queue>

#include "consumerData.h"
#include "report.h"
#include <time.h>
#define MSPERSEC 1000
#define NSPERMS 1000000

void * consumer(void* data);


#endif //PA4_CONSUMER_H
