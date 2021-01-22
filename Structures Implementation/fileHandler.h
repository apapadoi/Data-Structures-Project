//Including essential libraries
#ifndef FILE_HANDLER_H
#define FILE_HANDLE_H
#include <iostream>
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;
//=============================

#define BUFFER_ARRAY_SIZE 500 //this will be the size of the array that saves the processed words before they will
                                   //be written in the new processed text file

//defining file names,Q size and modulo factor when choosing words for Q set
#define TEST_FILE_NAME "small-file.txt"
#define OUTPUT_FILE_NAME "output.txt"
#define LOG_FILE_NAME "log.txt"
#define CHOSEN_WORDS_NUMBER 230000 //Initial Q size
#define MODULO_FACTOR 2
//======================================================================

//defining error codes to write in log file in case en error occurs
#define COULD_NOT_OPEN_INPUT_FILE 1
#define COULD_NOT_WRITE_FILE 2
#define COULD_NOT_CREAT_PROCESSED_OUTPUT_FILE_NAME 3
#define COULD_NOT_OPEN_OUTPUT_PROCESSED_FILE 4
#define FAILED_MEMORY_ALLOCATION 5
#define COULD_NOT_OPEN_OUTPUT_BENCHMARK_FILE 6
#define COULD_NOT_OPEN_STATISTICS_FILE 7
#define COULD_NOT_OPEN_LINE_FIT_FILE 8
#define COULD_NOT_OPEN_PRIMES_FILE 9
#define COULD_NOT_INSERT_IN_HASH_TABLE 10
#define COULD_NOT_FIND_REHASHABLE_WORD_HT 11
//=================================================================

// class that handles the text files
class fileHandler
{
    public:
        fileHandler(string inputFileName,string outputFileName); // constructor of the class 
        fileHandler(); // constructor that reads the input/output file name from the user's keyboard
        fileHandler(string inputFileName);
        ~fileHandler();
        bool readText(); //function that reads the initial text file and uses textProcessor to edit the text and
                               //write the processed words in a new text file
        string getOutputProcessedFileName(); //function that returns the processed text file name 
        string getOutputBenchmarkFileName(); //function that returns the output benchmark text file that contains the 
                                             // time it took for the operations in the structures
        int getTotalTextWords() const { return totalTextWords; };
    private:
        string inputFileName;//file name from which we will read the initial text
        string outputFileName;  //file name to which we will write the time it took to search the words 
                                //that will be chosen randomly 
        int totalTextWords; // member of the class that saves how many words the processed file has
};
//Non-Member Function
void printError(int errorCode); //function that writes what error occurred in the LOG_FILE_NAME file
#endif //FILE_HANDLER_H