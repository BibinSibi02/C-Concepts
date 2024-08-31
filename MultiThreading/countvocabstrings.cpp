/* BIBIN SIBI
 * CS480
 * RED ID:825763777 */

#include "countvocabstrings.h"
#include <pthread.h>
#include "shared.h"
#include "tree.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
void *countvocabstrings(void* data){
    //output file
    std::ofstream outfile;
    outfile.open("countNumOfContainedVocab.txt");
    SHARED_DATA *data3 = (SHARED_DATA*)data;
    //busy wait till readvocab is completed
    while(!data3->taskCompleted[0] || data3->lineQueue->empty()){//while readvocab is not finished
        //do nothing
    }

    int ans;
    while (!data3->lineQueue->empty() || !data3->taskCompleted[1]){
        if(!data3->lineQueue->empty()){
            ans = 0;
            tree trie;
            std::string word;
            //unlock the lock
            if(pthread_mutex_lock(&data3->queue_mutex)==0){
                word = data3->lineQueue->front().c_str();
                data3->lineQueue->pop();
            }
            else{
                std::cout << "lock in countvocab is broken" << std::endl;
                exit(4);
            }
            //lock the lock
            if(pthread_mutex_unlock(&data3->queue_mutex)==0) {

            }
            else{
                std::cout << "unlock in countvocab is broken" << std::endl;
                exit(4);
            }

            //pop the line from the queue after reading it
            //data3->lineQueue->pop();
            //stores original length of the input string
            long stringLen = word.length();
            const char* cString = word.c_str();
            for(int i =0; i < stringLen ;i++) {
                //add input to trie tree
                trie.insert(cString);
                cString++;
            }
            const char* vword;
            //for each word in the vocab vector, search if its a substring of input
            for(const auto & i : *data3->vocabVect) {
                vword = i.c_str();
                if (trie.search(vword)) {
                    ans++;
                }
            }
            if(ans>=data3->minNumOfVocabStringsContainedForPrinting) {
                outfile << ans << std::endl;
            }
            data3->numOfProcessedLines += 1;
            //deallocate and destroy tree
            trie.destroyTree();
        }


    }
    outfile.close();


}
