//Including essential libraries
#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESS_H
#include <iostream>
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;
//=============================

// class that process text and then writes the processed words to a new file
class textProcessor
{
    public:
        //function that takes the text in it's initial state, process it and then writes the processed words in the output processed text file
        bool process(string &buffer,string* processedWords,int &currentWords,int &totalTextWords);
        //===================================================================================================================================
        bool write(const string* processedWords,int &currentWords); //function that writes the processed words in the output processed text file
        bool openOutputProcessedFile(); //function that opens outputprocessed file 
        void closeOutputProcessedFile(); //function that closes outputprocessed file
        string getOutputProcessedFileName()  { return outputProcessedFileName; }; //function that returns the output processed file name
        bool createOutputProcessedFileName(const string &inputFileName); //function that creates the output processed file name 
    private:
        ofstream outputProcessedFile;//the file that we will write the processed text word by word
        string outputProcessedFileName;//and it's name
};

//Non-Member Functions
void convertToLower(string &currentWord); //function that converts upper case characters in lower
bool isEnglishLetter(char element); //function that checks if a character belongs to english alphabet
#endif //TEXT_PROCESSOR_H