/* BIBIN SIBI
 * CS480
 * RED ID:825763777 */

#include "readlines.h"
#include <pthread.h>
#include "shared.h"

void *readlines(void* data){
    SHARED_DATA *data2 = (SHARED_DATA*)data;
    std::string line;
    //open test file
    std::ifstream infile(data2->fileName[1]);
    //lock to start locked
    //error checking
    if(!infile) {
        std::cout << "Unable to open <<testfile.txt>>" << std::endl;
        exit(0);
    }

    //initialize
    data2->lineCountInFile[1]=0;

    //counting total line count in the file
    //start putting the vocab in the shared data vector

    //unlock the lock


        //access the queue
        while (std::getline(infile, line)){
                //keep running till the lock is unlocked
                if(pthread_mutex_lock(&data2->queue_mutex) ==0){
                //access the queue
                    data2->lineQueue->push(line);
                    data2->lineCountInFile[1] += 1;}
                else{
                    std::cout << "lock in readlines is broken" << std::endl;
                    exit(3);
                }
                if(pthread_mutex_unlock(&data2->queue_mutex) ==0){}
                else{
                    std::cout << "unlock in readlines is broken" << std::endl;
                    exit(3);
                }

        }


    //set taskCompleted[readlinesindex] to true
    data2->taskCompleted[1] = true;
    infile.close();
}
