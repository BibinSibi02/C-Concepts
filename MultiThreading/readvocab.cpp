/* BIBIN SIBI
 * CS480
 * RED ID:825763777 */

#include "readvocab.h"
#include "shared.h"
#include <sys/stat.h>
void *readvocab(void* data){
    SHARED_DATA *data1 = (SHARED_DATA*)data;
    std::string line;
    //open vocab file
    std::ifstream infile(data1->fileName[0]);
    struct stat stat1;
    stat(data1->fileName[0],&stat1);
    //error checking
    if(!infile) {
        std::cout << "Unable to open <<vocabulary.txt>>" << std::endl;
        exit(1);
    }
    //counting total character count in the file
    data1->totalNumOfCharsInVocabFile = (stat1.st_size/sizeof(char));
    //initialize
    data1->lineCountInFile[0]=0;
    while (std::getline(infile, line)){
        //update line count
        data1->lineCountInFile[0]+=1;
    }
    infile.close();
    //substract the end character from each line
    data1->totalNumOfCharsInVocabFile -=(data1->lineCountInFile[0]);
    //start putting the vocab in the shared data vector
    std::ifstream infile2(data1->fileName[0]);
    while (std::getline(infile2, line)){
        data1->vocabVect->push_back(line);
        //update current character count
        data1->numOfCharsReadFromVocabFile += line.length();
    }
    infile.close();
}
