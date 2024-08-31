/*BIBIN SIBI
 * CS480
 * RED ID: 825763777 */

#ifndef PA4_PRODUCER_H
#define PA4_PRODUCER_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <string>
#include <fstream>
#include <iostream>
#include <queue>

#include "producerData.h"
#include "report.h"
#include "shared.h"
#include <time.h>
#define MSPERSEC 1000
#define NSPERMS 1000000
void * producer(void* data);

#endif //PA4_PRODUCER_H
