/*BIBIN  */


#include "producer.h"
void *producer(void* data) {
    PRODUCER_DATA *data1 = (PRODUCER_DATA *) data;


    //keep looping until the total number of requests has been reached
    while (data1->sharedData->currProduced < data1->sharedData->totalRequests) {

        //production time

        /*struct timespec SleepTime;
        //setting up delay
        SleepTime.tv_sec = data1->productionTime / MSPERSEC; //seconds
        SleepTime.tv_nsec = (data1->productionTime % MSPERSEC) * NSPERMS; //nanoseconds
        //puts to sleep to simulate production
        nanosleep(&SleepTime, NULL); */

        //production time
        usleep(data1->productionTime *1000);

        //check if the inserting crypto is the only item
        bool onlyItem;

        if (data1->request == Bitcoin) {
            //acquire lock first
            if (pthread_mutex_lock(&data1->sharedData->buffer_mutex) == 0) {
                // if the buffer is full
                while ((data1->sharedData->inRequestQueue[0] + data1->sharedData->inRequestQueue[1]) >= 15) {
                    //wait for available Slots signal
                    pthread_cond_wait(&data1->sharedData->availableSlots, &data1->sharedData->buffer_mutex);
                }

                //set onlyItem to true if buffer is empty
                if ((data1->sharedData->inRequestQueue[0] + data1->sharedData->inRequestQueue[1]) <= 0) {
                    onlyItem = true;
                } else {
                    onlyItem = false;
                }
                //if max amt of bitcoins is reached (aka 6)
                while (data1->sharedData->inRequestQueue[0] >= 6) {
                    //wait for bitcoin availableBitcoinSlots signal
                    pthread_cond_wait(&data1->sharedData->availableBitcoinSlots, &data1->sharedData->buffer_mutex);
                }


                if(data1->sharedData->currProduced < data1->sharedData->totalRequests){
                    //access buffer and insert bitcoin at the end
                    data1->sharedData->buffer->push(Bitcoin);
                    //number of produced requests
                    data1->sharedData->currProduced += 1;
                    data1->sharedData->produced[0] += 1;
                    data1->sharedData->inRequestQueue[0] += 1;

                    //report the output
                    report_request_added(Bitcoin, data1->sharedData->produced, data1->sharedData->inRequestQueue);

                }
                //make sure to signal to consumers

                    //signal to consumer there are crypto to be consumed
                    pthread_cond_signal(&data1->sharedData->unconsumed);

            }
            else {
                std::cout << "lock in producer is broken" << std::endl;
                exit(3);
            }
            if (pthread_mutex_unlock(&data1->sharedData->buffer_mutex) == 0) {
                //successfully unlocked
            } else {
                std::cout << "unlock in producer is broken" << std::endl;
                exit(3);
            }

        }
        else if (data1->request == Ethereum) {
            //acquire lock first
            if (pthread_mutex_lock(&data1->sharedData->buffer_mutex) == 0) {

                //set onlyItem to true if buffer is empty
                if ((data1->sharedData->inRequestQueue[0] + data1->sharedData->inRequestQueue[1]) <= 0) {
                    onlyItem = true;
                } else {
                    onlyItem = false;
                }

                if(data1->sharedData->currProduced < data1->sharedData->totalRequests){
                    //access buffer and insert etherium at the end
                    data1->sharedData->buffer->push(Ethereum);
                    //number of produced requests
                    data1->sharedData->currProduced += 1;
                    data1->sharedData->produced[1] += 1;
                    data1->sharedData->inRequestQueue[1] += 1;

                    //report the output
                    report_request_added(Ethereum, data1->sharedData->produced, data1->sharedData->inRequestQueue);
                }

                //make sure to signal to consumers

                    //signal to consumer there are crypto to be consumed
                    pthread_cond_signal(&data1->sharedData->unconsumed);


            }
            else {
                std::cout << "lock in producer is broken" << std::endl;
                exit(3);
            }
            if (pthread_mutex_unlock(&data1->sharedData->buffer_mutex) == 0) {
                //successfully unlocked
            } else {
                std::cout << "unlock in producer is broken" << std::endl;
                exit(3);
            }

        }
    }

}


