//Including essential libraries
#include <iostream>
#include "fileHandler.h"
#include "textProcessor.h"
#include <cstdlib>
#include <string>
#include <fstream>
using std::cout;
using std::endl;
using std::ios;
using std::string;
using std::ios;
//=============================

//   Non-Member Functions
bool isEnglishLetter(char element) //function that checks if a character belongs to english alphabet
{
	if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z'))
		return true;
	
	return false;
}

void convertToLower(string &currentWord) //function that converts upper case english characters in lower
{
    int i=0;
    while( currentWord[i]!='\0' )
    {
        if( ( currentWord[i] >= 'A' ) && ( currentWord[i] <= 'Z' ) )
                currentWord[i]+=32;
        i++;
    }
}
//=======================================================

//function that takes the text in it's initial state, process it and then writes the processed words in the output processed text file
bool textProcessor::process(string &buffer,string* processedWords,int &currentWords,int&totalTextWords)
{
    convertToLower( buffer ); //if the buffer contains any upper case english characters they will be converted in to lower case
    
    int i=0,j=0,startingIndex=0,enLettersCounter=0; //declaring essential variables
    while( buffer[i]!='\0' ) //we move through the buffer characters until the escape sequence '\0'
        if( isEnglishLetter( buffer[i] ) ) //we find the first occurrance of an english character 
        {
            startingIndex=i; // saving first occurrance's of an english character index
            enLettersCounter=0; // and then start counting how many english characters are after the first occurrance

            while( isEnglishLetter( buffer[i] ) ) //until we reach a non-english character
            {
                enLettersCounter++;
                i++;
            }
            
            j=startingIndex; // j starts from the first occurrance's index and moves through buffer enLettersCounter positions
            processedWords[currentWords].resize( ( enLettersCounter ) *sizeof(char) ); //resizing string's size 
            for(int k=0; k < enLettersCounter ; k++)
                processedWords[currentWords][k]=buffer[j++]; //and then copying the characters from buffer to the processedWords array

            currentWords++;
            totalTextWords++; 
        }
        else
            i++;
    
    if( !this->write(processedWords,currentWords) ) //if the buffer is fully processed we write the processed words in the output processed text
    {                                             //file
        printError( COULD_NOT_WRITE_FILE );//if an error occurs we write it in the log file and return false
        return false;
    }
    
    return true;
}

bool textProcessor::write(const string* processedWords,int &currentWords)//function that writes the processed words in the output processed text file
{
    if( !outputProcessedFile.is_open() ) //if the output file is not open we return false
        return false;
    
    for(int i=0;i<currentWords;i++) //writing the words in the output processed text file
        outputProcessedFile<<processedWords[i]<<endl;

    currentWords=0;//reseting currentWords that are saved in the processedWords array
    return true;
}

bool textProcessor::createOutputProcessedFileName(const string &inputFileName) // function that creates the output processed file name 
{
    string bufferFileName=inputFileName; //we create a copy of input file name

    int pos=bufferFileName.find(".txt"); //we find the occurrance of .txt extension 
    bufferFileName.resize( ( bufferFileName.length() + 10) * sizeof(char) ); //resizing the string's space 
    bufferFileName.replace(pos,13,"processed.txt",0,13);// and replacing the previous name with processed key word 
                                                         // in the end
                                                         //e.g if input file name is "test.txt" then the output is
                                                         // "testprocessed.txt"

    outputProcessedFileName=bufferFileName; //and in the end we save the output processed file name in the current text Processor
    return true;
}

bool textProcessor::openOutputProcessedFile() //function that opens outputprocessed file 
{
    outputProcessedFile.open( outputProcessedFileName );
    
    if( outputProcessedFile.is_open() ) //checking if the file is open
        return true;
    
    return false; //and if not we return false
}

void textProcessor::closeOutputProcessedFile() //function that closes outputprocessed file
{
    if( outputProcessedFile.is_open() ) //checking if the file is open 
        outputProcessedFile.close(); //and then closing it
}