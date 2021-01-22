//Including essential libraries
#include "textProcessor.h"
#include "fileHandler.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::ios;
using std::getline;
//=============================

//constructor of the class
fileHandler::fileHandler(string inputFileName,string outputFileName)
{
    ( this->inputFileName ) = inputFileName;
    ( this->outputFileName ) = outputFileName;
    totalTextWords=0;
}

fileHandler::fileHandler()// constructor that reads the input/output file name from the user's keyboard
{
    string buffer;
    cout<<"Type the name of the input file: ";
    cin>>buffer;
    ( this->inputFileName )= buffer;
    cout<<"Type the name of the output benchmark file: ";
    cin>>buffer;
    ( this->outputFileName )= buffer;
    totalTextWords=0;
}

fileHandler::fileHandler(string inputFileName)
{
    ( this->inputFileName ) = inputFileName;
    ( this->outputFileName ) = OUTPUT_FILE_NAME;
    totalTextWords=0;
}

fileHandler::~fileHandler()
{
    string fileName = this->getOutputProcessedFileName();
    char*buffer;
    buffer = new char [fileName.length() + 1];
    int i;
    for(i=0;fileName[i]!='\0';i++)
        buffer[i]=fileName[i];
    buffer[i]='\0';
    remove( buffer );
    delete [] buffer;
}

bool fileHandler::readText()   //function that reads the initial text file and uses textProcessor to edit the text
                               //and write the processed words in a new text file
{
    clock_t c1=clock();
    ifstream currentInputFile( this->inputFileName, ios::in); //opening input file
    
    if( !currentInputFile.is_open() ) //checking if the file is open
    {
        printError( COULD_NOT_OPEN_INPUT_FILE ); //and if it is not then we print the error in log file and return false
        return false; 
    }
    
    textProcessor currentTextProcessor; //declaring a text processor
    if( !currentTextProcessor.createOutputProcessedFileName( this->inputFileName ) ) // creating the processed output
    {                                                                                // text file name 
        printError( COULD_NOT_CREAT_PROCESSED_OUTPUT_FILE_NAME ); // and if an error occurs we write it in the log
        return false;                                                           // file and return false
    }
    
    if( !currentTextProcessor.openOutputProcessedFile() ) //opening processed output text file
    {
        printError( COULD_NOT_OPEN_OUTPUT_PROCESSED_FILE ); // and if an error occurs we write it in the log file
        return false;                                             // and return false
    }

    string* processedWords=new string [BUFFER_ARRAY_SIZE]; //declaring a string array that will save processed  
                                                                 //words before they'll be written in the output file 
    
    if( !processedWords ) //checking if the memory for the array is allocated successfully and if not 
    {
        printError( FAILED_MEMORY_ALLOCATION ); //we write the error in the log file 
        return false;                                 //and return false
    }
    
    int currentBufferWords=0; //a variable to store how much words we've got in the current buffer 
    string buffer; //declaring a string so we can read the text until a space or Enter is read and then process it
    while( !currentInputFile.eof() ) //checking if we are at the end of the file, eof=END OF FILE
    {
        getline( currentInputFile , buffer ); //reading from input file
        
        currentTextProcessor.process(buffer,processedWords,currentBufferWords, totalTextWords); //using the text processor to process
                                            //  the text and save the words in the processedWords array and if the buffer
                                            // has been fully processed we write the words in the processed output text file
    }

    currentInputFile.close(); //closing input text file
    currentTextProcessor.closeOutputProcessedFile(); //closing processed output text file
    
    clock_t c2=clock();
    //printing the time it took to read and process the input file
    ofstream outputFile;
    outputFile.open(  this->getOutputBenchmarkFileName() );
    if( !outputFile.is_open() )
        printError( COULD_NOT_OPEN_OUTPUT_BENCHMARK_FILE );
    else
    {
        outputFile<<"Reading and Processing text file took: "<<( (double)(c2-c1) )/CLOCKS_PER_SEC<<endl<<endl;
        outputFile.close();
    }

    return true;
}

void printError(int errorCode)  //function that writes what error occurred in the LOG_FILE_NAME file
{
        time_t timeStampError;  //creating an instance of time_t struct
        timeStampError=time(NULL);  //saving the time stamp the the error occurred

        cerr<<endl<<"An error occurred!"<<endl; //printing in the screen that an error
        cerr<<"Check log Files folder about the problem."<<endl;    //occurred
        cerr<<"Time Stamp: "<<ctime(&timeStampError)<<endl; //and also the time stamp that it occurred

        ofstream logFile( LOG_FILE_NAME ,ios::app); //opening log file to write the error problem and the time stamp
        if( !logFile.is_open() )    //checking if the log file is open
        {
            cerr<<"Could not open log file with path: "<< LOG_FILE_NAME <<endl; //printing an error in case log file is
            return;                                               // not open and we return from the function                  
        }
        
        //printing the error in log file and then closing it
        switch( errorCode ) 
        {                   
            case 1:
                logFile<<"Could not open input text file. Time stamp: "<<ctime(&timeStampError)<<endl; 
                break;
            case 2:
                logFile<<"Could not write in the file that contains processed words. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 3:
                logFile<<"Could not create the output processed file name. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 4:
                logFile<<"Could not open the output processed file. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 5:
                logFile<<"Dynamic Memory Allocation Failed. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 6:
                logFile<<"Could not open output benchmark file. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 7:
                logFile<<"Could not open statistic observations file. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 8:
                logFile<<"Could not open best line fit file. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 9:
                logFile<<"Could not open prime numbers file. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 10:
                logFile<<"Could not insert the key to hash table. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            case 11:
                logFile<<"Could not find the rehashable word in the resize method of hashTable. Time stamp: "<<ctime(&timeStampError)<<endl;
                break;
            default:
                logFile<<"An unknown error occurred. Time stamp: "<<ctime(&timeStampError)<<endl;
        }
        logFile.close();
        //================================================== 
}

string fileHandler::getOutputProcessedFileName() //function that returns the processed text file name
{
    string bufferFileName=inputFileName; //we create a copy of input file name

    int pos=bufferFileName.find(".txt"); //we find the occurrance of .txt extension 
    bufferFileName.resize( ( bufferFileName.length() + 10) * sizeof(char) ); //resizing the string's space 
    bufferFileName.replace(pos,13,"processed.txt",0,13); // and replacing the previous name with processed key word 
                                                         // in the end
                                                         //e.g if input file name is "test.txt" then the output is
                                                         // "testprocessed.txt"
    return bufferFileName;    
}

string fileHandler::getOutputBenchmarkFileName()
{
    return outputFileName;
}