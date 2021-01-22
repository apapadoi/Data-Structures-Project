#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "dsHandler.h"
#define START 1
#define TOTAL_DATASET_FILES 190 
using namespace std;

void processData(); // function that processes the train dataset's files and saves to statistics.txt the number of 
                    // processed( frequencies ) words per file and the number of unique words( indexes )in the file
int calculateEND(); // function that calculates the number of the dataset files in case someone appends the statistics file
                    // by hand
void linearRegression1(double &slope,double &intercept); // function that trains the model 
void readData(ifstream &statisticsFile,int*totalIndexes,int*totalFrequencies,int END); // function that reads the data from
                                    // the statistics file and saves them in to the two arrays totalIndexes and 
                                    // total Frequencies
double rSquareError( int*totalIndexes, int*totalFrequencies ,double slope , double intercept , int END); // function that
                                    // calculates the R^2 error for the current training session
void saveLFitParameters( double slope , double intercept); /// function that saves the slope and the intercept of the best 
                        // fit line in the file with name LINE_FIT_FILE_NAME ( hashTable.h ) so we can use it later on the 
                        // constructor of the hashTable class to determine the size  of the array
void printDatasetsFilesSize(); // function that prints the size of the train dataset's files in bytes

int main()
{     
    processData(); 

    double slope=0.0,intercept=0.0;
    linearRegression1( slope , intercept );
    //saveLFitParameters( slope , intercept );
    cout<<"Linear Regression : "<<endl;
    cout<<"Slope: "<<slope<<endl;
    cout<<"Intercept: "<<intercept<<endl<<endl<<endl;
    return 0;
}

// function that prints the size of the train dataset's file in bytes
void printDatasetsFilesSize()
{
    streampos fsize = 0;
    int END = calculateEND();
    string currentFileName;
    for(int i=START;i<END+1;i++)
    {
        currentFileName=to_string(i); // converting to string
        currentFileName.shrink_to_fit(); // reducing memory use
        std::ifstream currentFile ( currentFileName + ".txt", ios::in); // opening file 
        fsize = currentFile.tellg();         
        currentFile.seekg(0, ios::end);      // calculating the size of the file using the file pointer
        fsize = currentFile.tellg() - fsize;
        currentFile.close(); // closing file
        cout<<fsize<<endl; 
    }
}

// function that saves the slope and the intercept of the best fit line in the file with name LINE_FIT_FILE_NAME so
// we can use it later on the constructor of the hashTable class to determine the size of the array
void saveLFitParameters( double slope , double intercept)
{
    ofstream lineFitFile;
    lineFitFile.open( LINE_FIT_FILE_NAME ); //opening file
    if( !lineFitFile.is_open() )
    {
        printError( COULD_NOT_OPEN_LINE_FIT_FILE );
        return;
    }
    lineFitFile<<slope<<endl; // writing data
    lineFitFile<<intercept;
    lineFitFile.close(); // closing file
}

// function that calculates the R^2 error for the current training session
double rSquareError( int*totalIndexes, int*totalFrequencies , double slope , double intercept ,int END)
{
    double yMean=0.0,xMean=0.0;
    for(int i=START;i<END+1;i++)
    {
        xMean+=totalFrequencies[i-1];
        yMean+=totalIndexes[i-1];
    }
    xMean/=END;
    yMean/=END;
    
    //calculating R^2 using 
    // Σ(yBar - yMean )^2 as numerator
    //and Σ(y - yMean )^2 as denominator
    //also using x as totalFrequencies and y as totalIndexes
    double numerator=0.0,denominator=0.0;
    for(int i=START;i<END+1;i++)
    {
        numerator+= ( slope*totalFrequencies[i-1]+intercept  - yMean) * ( slope*totalFrequencies[i-1]+intercept  - yMean);
        denominator+= ( totalIndexes[i-1] - yMean ) * ( totalIndexes[i-1] - yMean );
    }

    return numerator/denominator;
}

// function that trains the model 
void linearRegression1(double &slope,double &intercept)
{
    int END = calculateEND(); //calculating the number of files in the train dataset
    cout<<"End: "<<END<<endl;
    int*totalIndexes= new int [END]; //allocating memory for the number of unique words per file
    if( !totalIndexes )
    {
        cout<<"Could not allocate memory for totalIndexes "<<endl;
        printError( FAILED_MEMORY_ALLOCATION );
        return;
    }
    int*totalFrequencies= new int [END]; //allocating memory for the number of words per file
    if( !totalFrequencies )
    {
        cout<<"Could not allocate memory for totalFrequencies "<<endl;
        printError( FAILED_MEMORY_ALLOCATION );
        return;
    }
    for(int i=START;i<END+1;i++)
        totalFrequencies[i-1]=totalIndexes[i-1]=0; //initializing the arrays

    ifstream statisticsFile;
    statisticsFile.open( STATISTICS_FILE_NAME , ios::in );
    if( !statisticsFile.is_open() )
    {
        printError( COULD_NOT_OPEN_STATISTICS_FILE );
        return;
    }

    readData(statisticsFile,totalIndexes,totalFrequencies,END); //reading data from statistics file
    
    // x=total processed words from the text file or totalFrequencies
    // y=unique words in the text file or totalIndexes
    //calculating the mean values of the two input variables
    double xMean=0.0,yMean=0.0;
    for(int i=START;i<END+1;i++)
    {   
        xMean+=totalFrequencies[i-1];
        yMean+=totalIndexes[i-1];
    }
    xMean/=END;
    yMean/=END;
    
    //calculating the slope of the best fit line
    //using Σ(x-xMean)(y-yMean) as numerator
    //and Σ(x-xMean)^2 as denominator 
    double numerator=0.0;
    double denominator=0.0;
    for(int i=START;i<END+1;i++)
    {
        numerator+= ( totalFrequencies[i-1] - xMean ) * ( totalIndexes[i-1] - yMean );
        denominator+= ( totalFrequencies[i-1] - xMean )* ( totalFrequencies[i-1] - xMean );
    }
    slope = numerator / denominator;
    //calculating the intercept of the best fit line 
    //using yMean - slope*xMean;
    intercept = yMean - slope*xMean; 
    cout<<"R Square Error: "<<rSquareError( totalIndexes , totalFrequencies , slope , intercept , END)*100<<"%"<<endl;

    delete [] totalIndexes;
    delete [] totalFrequencies;
}

// function that reads the data from the statistics file and saves them in to the two arrays totalIndexes and 
// total Frequencies
void readData(ifstream &statisticsFile,int*totalIndexes,int*totalFrequencies,int END)
{
    string buffer;
    for(int i=START;i<END+1;i++)
    {
        statisticsFile>>buffer;
        buffer.shrink_to_fit();
        totalIndexes[i-1]=stoi( buffer );	//converting to integer
    	statisticsFile>>buffer;
    	buffer.shrink_to_fit();
        totalFrequencies[i-1]=stoi( buffer ); //converting to integer
    }
}

// function that processes the train dataset's files and saves to STATISTICS_FILE_NAME (hashTable.h) the number of 
// processed( frequencies ) words per file and the number of unique words( indexes )in the file
//function starts processing from file START e.g. for processing files from 1 to 10 we will use START=1 and 
//END=10
void processData()
{
    int END= TOTAL_DATASET_FILES; //calculating END from the #define MACRO and not from the calculateEND() function
    // because the file might be empty if we train the model for the first time
    
    string tempFileName;
    for(int i=START;i<END+1;i++) // reading and processing files with the fileHandler
    {
        tempFileName=to_string(i);
        tempFileName.shrink_to_fit();
        fileHandler currentFileHandler( tempFileName + ".txt" );
        currentFileHandler.readText();
    }
	
    ofstream outputFile;
    outputFile.open( STATISTICS_FILE_NAME , ios::app ); //opening STATISTICS_FILE_NAME file so we can print the results
    if( !outputFile.is_open() )                         // of the insertion
    {
        printError( COULD_NOT_OPEN_STATISTICS_FILE );
        return;
    }

    string buffer;
    string tempProcessedFileName;
    int*totalIndexes= new int [END]; //allocating memory
    int*totalFrequencies= new int [END];
    for(int i=START;i<END+1;i++)
        totalFrequencies[i-1]=totalIndexes[i-1]=0; // initializing the arrays

    double meanValue=0.0;
    for(int i=START;i<END+1;i++) //simulating the insertion in a hashTable
    {
        int totalWords=0;
        hashTable ht( 850000 );
        
        tempProcessedFileName=to_string(i);
        tempProcessedFileName.shrink_to_fit();
        ifstream inputFile;
        inputFile.open( tempProcessedFileName + "processed.txt"  ,ios::in); //opening the processed file
        if( !inputFile.is_open() )
        {
            cout<<i<<"processed.txt file is not processed"<<endl;
            break;
        }
        clock_t c1=clock(); //calculating the duration of the insertion so we can define the best combination of hasing functions for the 
        inputFile>>buffer;  /// hash table
        while( !inputFile.eof() ) // and then doing the insertion to the hashTable
        {
            totalWords++;
            ht.insert(buffer);
            inputFile>>buffer;
        }
        clock_t c2=clock();
        cout<<(double)( ( (double)(c2-c1) )/CLOCKS_PER_SEC )<<endl; //printing the current file's insertion time
        meanValue+= ( ( (double)(c2-c1) )/CLOCKS_PER_SEC ) / TOTAL_DATASET_FILES; // calculating the mean value of 
                                                    // the insertion times for this combination of hash functions
        inputFile.close();
        outputFile<<totalIndexes[i-1]<<endl<<totalFrequencies[i-1]<<endl; //saving data to the statistics file
    }
    cout<<"Mean Value of Insertion: "<<meanValue<<endl;
    for(int i=START;i<END+1;i++)
        cout<<totalFrequencies[i-1]<<"  "<<totalIndexes[i-1]<<endl; //printing the whole dataset's data about insertion

    if( outputFile.is_open() )
        outputFile.close();

    delete [] totalIndexes;
    delete [] totalFrequencies;
}

// function that calculates the number of the dataset files in case someone appends the statistics file by hand
int calculateEND()
{
    ifstream statisticsFile;
    statisticsFile.open( STATISTICS_FILE_NAME , ios::in ); //opening statistics file
    
    if( !statisticsFile.is_open() )
    {
        printError( COULD_NOT_OPEN_STATISTICS_FILE );
        return -1;
    }

    string buffer;
    int totalObservations=0;
    statisticsFile>>buffer;
    while( !statisticsFile.eof() ) //counting the numbers/lines that the file contains
    {
        totalObservations++;
        statisticsFile>>buffer;
    }

    if( statisticsFile.is_open() )
        statisticsFile.close();
    return totalObservations/2; // and then returning the half of these numbers/lines to find the number of files
}