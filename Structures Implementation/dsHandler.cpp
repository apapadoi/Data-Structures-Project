#include "dsHandler.h"
#include <ctime>
using namespace std;
// constructor of the class that does also the insertion of the Q set inside the data structures
dsHandler::dsHandler(fileHandler &currentFileHandler,hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string Q[],int numChosenWords,int &totalTextWords)
{
    if( !Q )
        Q = new string [ numChosenWords ];
    //checking if totalTextWords counter is 0 and if it is not we change it to 0
    if( totalTextWords )
        totalTextWords=0;
    
    //opening input processed file
    ifstream processedInputFile( currentFileHandler.getOutputProcessedFileName() ,std::ios::in);
    
    //reading processed file and saving words whenever the number of total words is a multiple of the
    //MODULO_FACTOR and then inserting the current word inside the structures
    int i=0;
    string buffer;
    clock_t hc1,hc2,BSTc1,BSTc2,AVLc1,AVLc2;
    double htTime=0.0,bstTime=0.0,avlTime=0.0;
    
    processedInputFile>>buffer;
    while( !processedInputFile.eof() )
    {
        if( !( rand()%MODULO_FACTOR ) && ( i<numChosenWords ) )
        {
            buffer.shrink_to_fit();
            Q[i].resize( ( buffer.length() + 1)*sizeof(char) );
            Q[i]=buffer;
            i++;
        }
        totalTextWords++;

        hc1=clock();
        currentHashTable.insert(buffer);
        hc2=clock();
        htTime+=(double)(hc2-hc1);
       
        BSTc1=clock();
        currentBST.insert(buffer);
        BSTc2=clock();
        bstTime+=(double)(BSTc2-BSTc1);
        
        AVLc1=clock();
        currentAVL.insert(buffer);
        AVLc2=clock();
        avlTime+=(double)(AVLc2-AVLc1);
        
        processedInputFile>>buffer;
    }
    
    processedInputFile.close();
    Qsize=i; //saving Qsize in case the number of chosen words was a greater number than the multiples of the 
             // MODULO_FACTOR so we can use the array afterwards
    //==================================================

    //opening output benchmark file and writing the time it took for the insertion
    ofstream outputFile;
    outputFile.open(  currentFileHandler.getOutputBenchmarkFileName() , ios::app );
    if( !outputFile.is_open() )
        printError( COULD_NOT_OPEN_OUTPUT_BENCHMARK_FILE );
    else
    {
        time_t timeStamp=time(NULL);
        outputFile<<"Q set Size: "<<Qsize<<endl<<endl;
        outputFile<<"\t\t\tINSERTION BENCHMARKS:"<<endl<<endl;
        outputFile<<"\t\tTime Stamp: "<<ctime(&timeStamp)<<endl;
        outputFile<<"Hash Table Insertion took: "<<htTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;
        outputFile<<"Binary Search Tree Insertion took: "<<bstTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;
        outputFile<<"AVL Tree Insertion took: "<<avlTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;

        outputFile.close();
    }
    //===============================   
}

dsHandler::dsHandler(fileHandler &currentFileHandler,hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string Q[],int numChosenWords)
{
    int temp=0;
    dsHandler tempDsHandler(currentFileHandler,currentHashTable,currentBST,currentAVL,Q,numChosenWords,temp);

    ( this->Qsize ) = tempDsHandler.Qsize;
}

//method that searches all the Q set words in the data structures and writes to the benchmark file and the screen the time it
// took for searching
bool dsHandler::search( fileHandler &currentFileHandler, hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string* Q )
{    
    if( !Q )
        return false;
    //opening output benchmark file
    ofstream outputFile;
    outputFile.open( currentFileHandler.getOutputBenchmarkFileName() , ios::app );
    if( !outputFile.is_open() ) // if the benchmark file is not open we print an error and return false
    {
        printError( COULD_NOT_OPEN_OUTPUT_BENCHMARK_FILE );
        return false;
    }
    
    outputFile<<"\t\t\t"<<"SEARCHING BENCHMARKS:"<<endl<<endl;

    clock_t hc1,hc2,BSTc1,BSTc2,AVLc1,AVLc2;
    double htTime=0.0,bstTime=0.0,avlTime=0.0;
    int htWordFrequency=0;
    int bstWordFrequency=0;
    int avlWordFrequency=0;
    for(int i=0;i<Qsize;i++) // searching the words that Q set contains in the data structures and then printing the results in benchmark file
    {
        hc1=clock();
        currentHashTable.search( Q[i] , htWordFrequency );
        hc2=clock();
        htTime+=(double)(hc2-hc1); 
        
        BSTc1=clock();
        currentBST.search( Q[i] , bstWordFrequency );
        BSTc2=clock();
        bstTime+=(double)(BSTc2-BSTc1);
        
        AVLc1=clock();
        currentAVL.search( Q[i] , avlWordFrequency );
        AVLc2=clock();
        avlTime+=(double)(AVLc2-AVLc1);
        
        outputFile<<"Word: "<<Q[i]<<" Frequency: \t\t\t"<<htWordFrequency<<"\t"<<bstWordFrequency<<"\t"<<avlWordFrequency<<endl;

        if( (htWordFrequency!=bstWordFrequency) || (htWordFrequency!=avlWordFrequency) || (bstWordFrequency!=avlWordFrequency) )
        {
            cout<<"Did not found same frequency in the three structures. keyString: "<<Q[i]<<endl;
        }
    }
    
    time_t timeStamp=time(NULL);
    outputFile<<endl<<"\t\tTime Stamp: "<<ctime(&timeStamp)<<endl<<endl;
    outputFile<<"Searching "<<Qsize<<" words in hashTable took "<<htTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;   
    outputFile<<"Searching "<<Qsize<<" words in Binary Search Tree took "<<bstTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;   
    outputFile<<"Searching "<<Qsize<<" words in AVL Tree took "<<avlTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;

    outputFile.close(); //closing benchmark file
    return true;
}

// method that deletes all the Q set words from the data structures and writes to benchmark file the time it
// took for deleting
bool dsHandler::Delete( fileHandler &currentFileHandler, hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string* Q)
{
    if( !Q )
        return false;
    clock_t hc1,hc2,BSTc1,BSTc2,AVLc1,AVLc2;
    double htTime=0.0,bstTime=0.0,avlTime=0.0;
    bool htDuplicate,bstDuplicate,avlDuplicate;
    int duplicates=0;
    
    for(int i=0;i<Qsize;i++) //deleting the words that Q set contains from the data structures
    {
        htDuplicate=false;
        hc1=clock();
        if( !currentHashTable.Delete(Q[i]) )
        {
            duplicates++;               
            htDuplicate=true;
        }
        hc2=clock();
        htTime+=(double)(hc2-hc1);
        
        bstDuplicate=false;
        BSTc1=clock();
        if( !currentBST.Delete(Q[i]) )
            bstDuplicate=true;
        BSTc2=clock();
        bstTime+=(double)(BSTc2-BSTc1);
        
        avlDuplicate=false;
        AVLc1=clock();
        if( !currentAVL.Delete(Q[i]) )
            avlDuplicate=true;
        AVLc2=clock();
        avlTime+=(double)(AVLc2-AVLc1);
    }
    
    //opening output benchmark file and writing the results of deleting operation
    ofstream outputFile;
    outputFile.open( currentFileHandler.getOutputBenchmarkFileName() , ios::app );
    if( !outputFile.is_open() )
    {
        printError( COULD_NOT_OPEN_OUTPUT_BENCHMARK_FILE );
        return false;
    }
    else
    {
        time_t timeStamp=time(NULL);
        outputFile<<"\t\t\t"<<"DELETING BENCHMARKS:"<<endl<<endl;
        outputFile<<"\t\tTime Stamp: "<<ctime(&timeStamp)<<endl<<endl;
        outputFile<<"Deleting "<<Qsize<<" words in hashTable took "<<htTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;
        outputFile<<"Deleting "<<Qsize<<" words in Binary Search Tree took "<<bstTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;
        outputFile<<"Deleting "<<Qsize<<" words in AVL Tree took "<<avlTime/CLOCKS_PER_SEC<<" seconds"<<endl<<endl;
        outputFile<<"Duplicates: "<<duplicates<<endl<<endl;

        outputFile.close();
    }
    return true;    
}