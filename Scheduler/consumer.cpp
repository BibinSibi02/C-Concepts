/*BIBIN SIBI
 * CS480
 * RED ID: 825763777 */


#include "consumer.h"
void * consumer(void* data) {
    CONSUMER_DATA *data2 = (CONSUMER_DATA *) data;
    //keep looping until the total number of requests has been reached
    while (data2->sharedData->currConsumed < data2->sharedData->totalRequests) {

        //lock critical region
        if (pthread_mutex_lock(&data2->sharedData->buffer_mutex) == 0) {

            /* //check if producers are waiting to produce
            bool atCapacity;
            if((data2->sharedData->inRequestQueue[0] + data2->sharedData->inRequestQueue[1]) >= 15){
                atCapacity = true;
            }
            else{
                atCapacity = false;
            }

            //check if producers are waiting to produce bitcoin
            bool bitCapacity;
            if(data2->sharedData->inRequestQueue[0]  >= 6){
                bitCapacity = true;
            }
            else{
                bitCapacity = false;
            }  */

            // if the buffer is empty
            while((data2->sharedData->inRequestQueue[0] + data2->sharedData->inRequestQueue[1]) <= 0) {
                //wait for unconsumed signal
                pthread_cond_wait(&data2->sharedData->unconsumed, &data2->sharedData->buffer_mutex);
                //std::cout << "waiting for unconsumed signal"<<std::endl;
            }

            //if bitcoin is being consumed
            if (data2->sharedData->buffer->front() == Bitcoin) {
                //access buffer and consume bitcoin
                data2->sharedData->buffer->pop();
                //number of bitcoins in buffer
                //data2->sharedData->numBitcoin -= 1;
                data2->sharedData->consumed[0] += 1;
                data2->sharedData->inRequestQueue[0] -= 1;
                data2->sharedData->currConsumed += 1;
                data2->sharedData->report[data2->blockchain][0]+=1;

                //report the output
                report_request_removed(data2->blockchain, Bitcoin, data2->sharedData->consumed,
                                       data2->sharedData->inRequestQueue);
                //signal to producer there is room to produce
                pthread_cond_signal(&data2->sharedData->availableSlots);
                //signal to bitcoin producer there is room to produce
                pthread_cond_signal(&data2->sharedData->availableBitcoinSlots);


                /*
                if(atCapacity) {
                    //signal to producer there is room to produce
                    pthread_cond_signal(&data2->sharedData->availableSlots);
                }
                if(bitCapacity) {
                    //signal to bitcoin producer there is room to produce
                    pthread_cond_signal(&data2->sharedData->availableBitcoinSlots);
                }*/
            }
                //if it is a etherium being consumed
            else if ((data2->sharedData->buffer->front()) == Ethereum) {
                    //access buffer and consume etherium
                    data2->sharedData->buffer->pop();
                    //number of bitcoins in buffer
                    //data2->sharedData->numEtherium -= 1;
                    data2->sharedData->consumed[1] +=1;
                    data2->sharedData->inRequestQueue[1] -=1;
                data2->sharedData->currConsumed += 1;
                data2->sharedData->report[data2->blockchain][1]+=1;

                    //report output
                report_request_removed(data2->blockchain, Ethereum, data2->sharedData->consumed, data2->sharedData->inRequestQueue);
               /*
                if(atCapacity) {
                    //signal to producer there is room to produce
                    pthread_cond_signal(&data2->sharedData->availableSlots);
                }*/

                //signal to producer there is room to produce
                pthread_cond_signal(&data2->sharedData->availableSlots);

                }
            }
        else {
            std::cout << "lock in producer is broken" << std::endl;
            exit(3);
        }
        if (pthread_mutex_unlock(&data2->sharedData->buffer_mutex) == 0) {
            //successfully unlocked
        } else {
            std::cout << "unlock in readlines is broken" << std::endl;
            exit(3);
        }
        //consumption time
        usleep(data2->consumptionTime *1000);



        /*
        struct timespec SleepTime;
        //setting up delay
        SleepTime.tv_sec = data2->consumptionTime / MSPERSEC; //seconds
        SleepTime.tv_nsec = (data2->consumptionTime % MSPERSEC) * NSPERMS; //nanoseconds
        //puts to sleep to simulate production
        nanosleep(&SleepTime, NULL);
         */
        }

    //signal barrier
    sem_post(&data2->sharedData->barrier);

    }



