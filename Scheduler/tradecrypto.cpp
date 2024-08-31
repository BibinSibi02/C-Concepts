/*BIBIN SIBI
 * CS480
 * RED ID: 825763777 */

#include <unistd.h>
#include <pthread.h>
#include <string>
#include <iostream>

#include "tradecrypto.h"
#include "report.h"
#include "producer.h"
#include "consumer.h"
#include "shared.h"
#include "producerData.h"
#include "consumerData.h"
#include <queue>
using namespace std;

#define NORMAL_EXIT 0
/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv){

    bool verbose = false;//chatty or quiet
    int option;//command line switch
    int totalTrade = 120; //total number of trade requests with default as 120 set by -n
    int blockXTime = 0; //number of milliseconds for processing a trade request for blockchain X with a default of 0
    int blockYTime = 0; //number of milliseconds for processing a trade request for blockchain y with a default of 0
    int bitcoinT= 0; //number of milliseconds for producing a bitcoin request with a default of 0
    int etheriumT = 0; //number of milliseconds for producing an etherium request with a default of 0

    //Processing optional arguments
    while ( (option = getopt(argc, argv, "n:x:y:b:e:o")) != -1){
        switch(option){
            case 'n':
                //for number of trade requests
                if(atoi(optarg)>=0){
                    totalTrade = atoi(optarg);
                }
                else{
                    std::cout <<"Total number of trade requests cant be negative"<<std::endl;
                    exit(NORMAL_EXIT);
                }
                break;
            case 'x':
                //for number of milliseconds for blockchain x
                if(atoi(optarg)>=0){
                    blockXTime = atoi(optarg);
                }
                else{
                    std::cout <<"Total number of milliseconds for blockchain X cant be negative"<<std::endl;
                    exit(NORMAL_EXIT);
                }
                break;
            case 'y':
                //for number of milliseconds for blockchain y
                if(atoi(optarg)>=0){
                    blockYTime = atoi(optarg);
                }
                else{
                    std::cout <<"Total number of milliseconds for blockchain Y cant be negative"<<std::endl;
                    exit(NORMAL_EXIT);
                }
                break;
            case 'b':
                //for number of milliseconds for bitcoin production
                if(atoi(optarg)>=0){
                    bitcoinT = atoi(optarg);
                }
                else{
                    std::cout <<"Total number of milliseconds bitcoin production cant be negative"<<std::endl;
                    exit(NORMAL_EXIT);
                }
                break;
            case 'e':
                //for number of milliseconds for etherium production
                if(atoi(optarg)>=0){
                    etheriumT = atoi(optarg);
                }
                else{
                    std::cout <<"Total number of milliseconds for etherium production cant be negative"<<std::endl;
                    exit(NORMAL_EXIT);
                }
                break;
            case 'o':
                //optarg is undefined
                verbose = true;
                break;
            default:
            exit(NORMAL_EXIT);

        }
    }

    //INITIALIZE SHARED DATA
    COMMON_DATA sharedData;
    sharedData.totalRequests = totalTrade; //initialize the maximum amt of trades
    std::queue <RequestType> buff;//create a buffer queue
    sharedData.buffer = &buff; //pass the buffer queue refrence to shared data
   // sharedData.numBitcoin = 0; //initialize current amt of bitcoins in buffer to zero
    //sharedData.numEtherium = 0;//iniialize current amt of etherium in buffer to zero
    sharedData.currProduced = 0; //initialize the current amt of trades to zero
    sharedData.currConsumed = 0; //initialize the current amt of trades to zero
    pthread_mutex_init(&sharedData.buffer_mutex,NULL); //initialize the buffer mutex lock
    sharedData.unconsumed = PTHREAD_COND_INITIALIZER;
    sharedData.availableSlots = PTHREAD_COND_INITIALIZER;
    sharedData.availableBitcoinSlots = PTHREAD_COND_INITIALIZER;
    //initialize barrier to zero
    sem_init(&sharedData.barrier, 0, 0);
    //intialize report array

    sharedData.report[0] = new unsigned int [2];
    sharedData.report[1] = new unsigned int [2];
    sharedData.report[0][0] = 0;
    sharedData.report[0][1] = 0;
    sharedData.report[1][0] = 0;
    sharedData.report[1][1] = 0;


    //initi to zero

    //INITIALIZE THE BITCOIN PRODUCER SHARED DATA
    PRODUCER_DATA bitcoinProducData;
    bitcoinProducData.request = Bitcoin;//set it to produce bitcoins
    bitcoinProducData.productionTime = bitcoinT; //set the bitcoin production time
    bitcoinProducData.sharedData = &sharedData; //pass the shared data access
    //INITIALIZE THE ETHERIUM PRODUCER SHARED DATA
    PRODUCER_DATA etheriumProducData;
    etheriumProducData.request = Ethereum; //set it to produce etherium
    etheriumProducData.productionTime = etheriumT; //set the etherium production time
    etheriumProducData.sharedData = &sharedData; //pass the shared data access

    //INITIALIZE THE CONSUMER SHARED DATA FOR BLOCKCHAIN X
    CONSUMER_DATA blockchain_X;
    blockchain_X.sharedData = &sharedData; //pass the shared data access
    blockchain_X.blockchain = BlockchainX; //initialize it to be blockchainX
    blockchain_X.consumptionTime = blockXTime; //initialize the time of consumption
    //INITIALIZE THE CONSUMER SHARED DATA FOR BLOCKCHAIN Y
    CONSUMER_DATA blockchain_Y;
    blockchain_Y.sharedData = &sharedData; //pass the shared data access
    blockchain_Y.blockchain = BlockchainY; //initialize it to be blockchainX
    blockchain_Y.consumptionTime = blockYTime; //initialize the time of consumption


    //Bitcoin thread
    pthread_t bitcoinThread;
    //Etherium thread
    pthread_t etheriumThread;
    //Blockchain X thread
    pthread_t blockchainXThread;
    //Blockchain Y thread
    pthread_t blockchainYThread;

    //create and start bitcoin thread if created
    //producer is the thread function created by bitcoinThread
    int works1 = pthread_create(&bitcoinThread, NULL, &producer, &bitcoinProducData);
    //check if it is created
    if(works1!=0){
        std::cout<<"bitcoin thread not created" <<std::endl;
        exit(1);
    }
    //create and start etherium thread if created
    //producer is the thread function created by etheriumThread
    int works2 = pthread_create(&etheriumThread, NULL, &producer, &etheriumProducData);
    //check if it is created
    if(works2!=0){
        std::cout<<"etherium thread not created" <<std::endl;
        exit(1);
    }

    //create and start blockchain X thread
    //consumer is the thread function created by blockchainXThread
    int works3 = pthread_create(&blockchainXThread, NULL, &consumer, &blockchain_X);
    //check if it is created
    if(works3!=0){
        std::cout<<"blockchainX thread not created" <<std::endl;
        exit(1);
    }
    //create and start blockchain Y thread
    //consumer is the thread function created by blockchainYThread
    int works4 = pthread_create(&blockchainYThread, NULL, &consumer, &blockchain_Y);
    //check if it is created
    if(works4!=0){
        std::cout<<"blockchainY thread not created" <<std::endl;
        exit(1);
    }

    //wait for barrier complete signal
    sem_wait(&sharedData.barrier);
    report_production_history(sharedData.produced, sharedData.report);






    //IMPLEMENT THE BARRIER AND PRODUCTION HISTORY(pls help)
}