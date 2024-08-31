
#include <unistd.h>
#include <pthread.h>
#include "shared.h"
#include <string>
#include <iostream>
/* BIBIN SIBI
 * CS480
 * RED ID:825763777 */

// thread for reading words from vocabulary file
#include "readvocab.h"
// thread for reading lines from a test file and adding them to a queue 
#include "readlines.h"
// thread for removing lines from the line queue, and 
// for each line, count number of contained vocab strings and print to
// an output file
#include "countvocabstrings.h"
using namespace std; // Make sure std components accessible
#define NORMAL_EXIT 0
/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv)
{
    
  bool verbose = false;  // chatty or quiet program
  int option;   /* command line switch */
  int idx;  // general purpose index variable
  int progressMarks = 50 ; //for optional progress mark commands (-n) with default value of 50
  int nth = 10;//for optional nth mark commands (-m) with default value of 10
  int minVocab = 0; //for optional argument for minimum number of vocab with default of 0
  // other stuff (e.g. declarations)

  /*
   * This example uses the standard C library getopt (man -s3 getopt)
   * It relies on several external variables that are defined when
   * the getopt.h routine is included. On POSIX2 compliant machines
   * <getopt.h> is included in <unistd.h> and only the unix standard
   * header need be included.
   *
   * getopt takes:
   *    argc - Number of strings in argv
   *    argv - Strings for each token (contiguous characters in the
   *           command line. Example:
   *           ./program -n 56 -v boo bear
   *           argv[0] = "a.out", argv[1] = "-n", argv[2] = "56"
   *           argv[3] = "-v", argv[4] = "boo", argv[5] = "bear"
   *    optionstr - String indicating optional arguments to process
   *           Options with a : after them expect some type of value
   *           after them.  Example:  "n:o:v" n and o expect arguments,
   *           v does not
   */
  while ( (option = getopt(argc, argv, "p:m:v:o")) != -1) {
    /* If the option has an argument, optarg is set to point to the 
     * argument associated with the option.  For example, if
     * -n is processed, optarg points to "56" in the example above.
     */
    switch (option) {
    case 'p':
        //for progress marks
      if(atoi(optarg)>=10) {
          progressMarks = atoi(optarg);
      }
      else{
          std::cout << "Number of progress marks must be a number and at least 10" <<std::endl;
          exit(NORMAL_EXIT);
      }
      break;

    case 'm': //for nth progress bar

    if((atoi(optarg)<=10) | (atoi(optarg)>0)) {
        nth= atoi(optarg);
    }
    else{
        std::cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << std::endl;
        exit(NORMAL_EXIT);
    }
      break;

    case 'v':  //for minimum number of contained vocab strings
        if(atoi(optarg)>=0) {
            minVocab = atoi(optarg);
            break;
        }
        else{
            std::cout << "Minimum number of vocab strings must be greater than or equal to 0" << std::endl;
        }

    case 'o': //optarg is undefined
        verbose = true;
        break;

    default:
      // print something about the usage and exit
      exit(NORMAL_EXIT); // BADFLAG is an error # defined in a header
    }

  }

  /*
   * Once the getop loop is done external variable optind contains
   * a number.  This is the first argument of argv to process
   * after all options have been processed.
   * argv[optind] is the next mandatory argument.
   */

  idx = optind;

  /* If idx < argc, there are mandatory arguments to process */


  // sample pthread code
  //pthread_attr_t	pthread_attr_default;

  //pthread_t thread1;
  //char *message1 = "Hello World";

  //pthread_attr_init(&pthread_attr_default);
  //pthread_create( &thread1, &pthread_attr_default,
	//	  (void *) &print_message_function, (void *) message1);
          
  // shared data to be used for communication between threads
  // main thread, readvocab, readlines, countvocabstrings
    SHARED_DATA sharedData;
  //initialize *fileName
        //infile contains the filepath to vocabulary.txt
        sharedData.fileName[0] = (argv[idx]);
        sharedData.fileName[1] = (argv[idx+1]);


  //OR
  //SHARED_DATA *sharedData = new SHARED_DATA();
  //SHARED_DATA *sharedData = (SHARED_DATA*) malloc(sizeof(SHARED_DATA));

  //IMPORTANT: initialize ALL data in the sharedData, you can have a helper to do it

  //initialize shared data here;
    sharedData.hashmarkInterval = nth;
    sharedData.minNumOfVocabStringsContainedForPrinting = minVocab;
    sharedData.numOfProgressMarks = progressMarks;
    std::vector <std::string> vocab;
    sharedData.vocabVect = &vocab;
    std::queue <std::string> queue;
    sharedData.lineQueue = &queue;
    sharedData.totalNumOfCharsInVocabFile = 0;
    sharedData.numOfCharsReadFromVocabFile = 0;
    sharedData.numOfProcessedLines = 0;
    //initialize the lock
    pthread_mutex_init(&sharedData.queue_mutex, NULL);






                 
  // readvocab thread, put more comments here of what this thread does
  pthread_t readvocabThread;
  // readlines thread, put more comments here of what this thread does
  pthread_t readlinesThread;
  // countvocabstrings thread, put more comments here of what this thread does
  pthread_t countvocabstringsThread;

  // create and start readvocab thread
  // readvocab is the thread function executed by the readvocabThread
  // **put** the declaration and definition of readvocab function into 
  // a **separate** pair of .h and .cpp files
  int works1 = pthread_create(&readvocabThread, NULL,
                              &readvocab, &sharedData);
  if (works1!=0)
  {//error checking
    std::cout << "readVocab Thread Not Created";
    exit(1);
  }
  // create and start readlines thread
  // readlines is the thread function executed by the readlinesThread
  // **put** the declaration and definition of readlines function into 
  // a **separate** pair of .h and .cpp files
  int works2 = pthread_create(&readlinesThread, NULL, &readlines, &sharedData);
  if (works2!=0)
  {//error checking
      std::cout << "readLines Thread Not Created";
      exit(1);
  }

  // create and start countvocabstrings thread
  //....

    // countvocabstrings is the thread function executed by the countvocabstringsThread
    // **put** the declaration and definition of countvocabstrings function into
    // a **separate** pair of .h and .cpp files
    int works3 = pthread_create(&countvocabstringsThread, NULL,
                                &countvocabstrings, &sharedData);
    if (works3!=0)
    {//error checking
        std::cout << "countvocabstrings Thread Not Created";
        exit(1);
    }

    // print progress bar for readvocab thread
    int curProgressBar = 0;
    while(curProgressBar<(int)sharedData.numOfProgressMarks)
    {
        //get percentage completed
        double percent = (double)sharedData.numOfCharsReadFromVocabFile/ (double)sharedData.totalNumOfCharsInVocabFile;
        //current progress bar should be at percent * number of total progress marks
        int progress = (int) (percent*sharedData.numOfProgressMarks);
        //print out the missing progress marks if current progress bar is less than expected
        while(curProgressBar<progress){
            //if the current bar is just before an expected #, then next bar should be a #
            if((curProgressBar)%((int)sharedData.hashmarkInterval)== ((int)sharedData.hashmarkInterval-1)){
                std::cout<<"#";
            }
            //else print -
            else{
                std::cout<<"-";
            }
            cout.flush();
            //increment the count for the progress bar
            curProgressBar+=1;
        }
    }
    //indent
    std::cout <<""<<std::endl;
    //print the number of lines in file
    std::cout <<"There are "<< sharedData.lineCountInFile[0]<< " lines in " << sharedData.fileName[0]<<std::endl;
    //reset for use in countvocabstrings
    curProgressBar = 0;
    //set taskCompleted[readvocabindex] to true
    sharedData.taskCompleted[0] = true;
     // use the SHARED_DATA
    // print progress bar for countvocabstring thread
    // note this needs to wait for the completion of readvocab thread
    while(!sharedData.taskCompleted[0]||!sharedData.taskCompleted[1]){
        //busy wait for the progress bar for readvocab and readlines to finish

    }
    //what if it is an empty file and lineCountInFile[1] = 0??
    if(sharedData.lineCountInFile[1]==0){
        //just print the whole progress bar
        while(curProgressBar<(int)sharedData.numOfProgressMarks)
        {
                //if the current bar is just before an expected #, then next bar should be a #
                if((curProgressBar)%((int)sharedData.hashmarkInterval)== ((int)sharedData.hashmarkInterval-1)){
                    std::cout<<"#";
                }
                    //else print -
                else{
                    std::cout<<"-";
                }
                cout.flush();
                //increment the count for the progress bar
                curProgressBar+=1;
            }

    }
    else { //if it is not an empty file
        //normal progress bar printing for countvocabstrings
        while (curProgressBar < (int) sharedData.numOfProgressMarks) {
            //get percentage completed
            double percent = (double) sharedData.numOfProcessedLines / (double) sharedData.lineCountInFile[1];
            //current progress bar should be at percent * number of total progress marks
            int progress = (int) (percent * sharedData.numOfProgressMarks);
            //print out the missing progress marks if current progress bar is less than expected
            while (curProgressBar < progress) {
                //if the current bar is just before an expected #, then next bar should be a #
                if ((curProgressBar) % ((int) sharedData.hashmarkInterval) == ((int) sharedData.hashmarkInterval - 1)) {
                    std::cout << "#";
                }
                    //else print -
                else {
                    std::cout << "-";
                }
                cout.flush();
                //increment the count for the progress bar
                curProgressBar += 1;
            }
        }
    }
    //indent
    std::cout <<""<<std::endl;
    //print the number of lines in file
    std::cout <<"There are "<< sharedData.lineCountInFile[1]<< " lines in " << sharedData.fileName[1]<<std::endl;
// Destroy mutex lock
        pthread_mutex_destroy(&sharedData.queue_mutex);

  // use the SHARED_DATA

  exit(NORMAL_EXIT); // Normal Exit! NORMALEXIT would be zero here.
}


