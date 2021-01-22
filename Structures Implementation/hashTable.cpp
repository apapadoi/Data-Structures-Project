//Including essential libraries
#include "hashTable.h"
#include <string>
#include <cmath>
#include "fileHandler.cpp"
#include <iostream>
#include <cstdlib>
using std::string;
using std::cerr;
using std::endl;
using std::cout;
using std::stod;
using std::stoi;
//=============================

//constructors of the class using the linear regression model
hashTable::hashTable(int totalTextWords)
{
    int size=calculateSize( totalTextWords ); //using the linear model to calculate the size of the hash table
    ( this->array ) = new arrayNode [ size ];
    if( !array )
        printError( FAILED_MEMORY_ALLOCATION );
    else
    {
        arraySize= size;    
        loadFactor=0.0;
    }
}

// default constructor of the class
hashTable::hashTable()
{
    int size = 30011;
    ( this->array ) = new arrayNode [ size ];
    if( !array )
        printError( FAILED_MEMORY_ALLOCATION );
    else
    {
        arraySize=size;
        loadFactor=0.0;
    }
}

//destructor of the class
hashTable::~hashTable()
{
    if( array )
        delete [] array;
}
//======================

// method that inserts the key string in the hash table
bool hashTable::insert(const string &key)
{
    if( this->isEmpty() )
        return false;
    
    if( loadFactor>=0.5 ) // checking if the hash table needs to be resized
    {
        if( ! ( this->resize() ) )
        {    
            printError( FAILED_MEMORY_ALLOCATION );
            return false;
        }
    }

    int pos=h1(key); // hashing with the first hash function the string we are going to insert 
    int initalPosition=pos;
    arrayNode keyNode(key); // creating a temporary arrayNode containing the key string so we can afterwards use the
                            // swap method of this class

    while( array[pos].isValid() ) // we move through the array while the current position of the array is occupied
    {
        if( array[pos].word == keyNode.word ) // if we find a position that contains the same word as the key string
                                              // we increase the frequency by 1 and assign 0 to deleted member of the current position
                                              // in case this specific word was deleted and then inserted again 
        {
            ( array[pos].deleted ) = 0;
            ( array[pos].frequency )++;
            return true;
        }
        else
            if( keyNode.word < array[pos].word ) // otherwise if the key string is smaller than the current position's 
                array[pos].swap( keyNode );      // word we swap the data of the two positions and continue with the 
                                                 // insertion of the word that was saved previously there 
                                                 // In this way we keep the sequence of a specific key sorted and we 
                                                 // can optimize a little bit the search method for a key that the hash
                                                 // table does not contain ( approximately we achieve the time of a
                                                 // successful search for a word the hash table does not contain )
        pos=( pos + h2(keyNode.word) ) % arraySize;   // if the control of the program reaches this line it means that
                                                      // we should continue searching a position to insert the keyNode
                                                      // word (there is a possibility that the initial key word might
                                                      // have changed so we have to call the second hash function in 
                                                      // every step in comparison with the way we implemented the search
                                                      // method in which we saved just the second hash code in a variable
                                                      // in order to get rid of the function calls )
        if( pos == initalPosition ) // if we return to the initial position that means that we can not insert the
        {                           // key string in the hash table 
            printError( COULD_NOT_INSERT_IN_HASH_TABLE );
            return false;
        }
    }

    array[pos]=keyNode; // if the control of the program reaches this line it means that we found an empty position so we just
                        // save the key word in this specific position and return true 
    loadFactor+= 1.0/arraySize;
    
    return true;
}

// method that searches the key string in the hash table and 
// also returns the frequency using the second parameter
bool hashTable::search(const string &key,int &frequency)
{
    if( this->isEmpty() )
        return false;
    
    int pos=h1(key); // hashing with the first hash function the string we are going to search 
    int initialPosition=pos; // saving initial position
    int step=h2(key); // hashing with the second hash function the string we are going to search and saving it to a variable 
                      //in this way we reduce the function calls
    
    while( array[pos].isValid() ) // we move through the array while the current position of the array is occupied
    {   
        if( array[pos].word == key ) // if we find a position that contains same word as the key string and this position is not deleted we 
                                     // save the word's frequency in the second parameter and return true otherwise we return false
        {
            frequency = array[pos].frequency;
            return !array[pos].deleted; 
        }
        else
            if( array[pos].word > key ) // also if we find a position that contains a word that is greater than the key string we return false 
                                        // because a specific search sequence is keeped sorted and that means that the hash table does not 
                                        // contain the key string so we return false
            {
                frequency=0;
                return false;
            }
        pos = ( pos + step ) % arraySize; // if the control of the program reaches this line it means that we should continue searching the 
                                          // key string inside the hash table 
        if( pos == initialPosition ) // if we return to the initial position that means that the hash table does not
        {                            // contain the key
            frequency=0;
            return false;
        }
    }

    // if the program reaches this line it means that we found an empty position and that means that the hash table does not contain the key
    // string so we return false
    frequency=0;
    return false; 
}

// method that deletes the key string from the hash table
bool hashTable::Delete(const string &key)
{
    if( this->isEmpty() )
        return false;
    
    int pos=h1(key); // hashing with the first hash function the string we are going to search 
    int initialPosition=pos; // saving initial position
    int step=h2(key); // hasing with the second hash function the string we are going to search and in this way we reduce the function calls

    while( array[pos].isValid() ) // we move through the array while the current position of the array is occupied
        if( array[pos].word < key ) // if the current position's word is smaller than the key string we continue 
                                    // moving through the array
        {   
            pos = ( pos + step ) % arraySize;
            if( pos == initialPosition ) // if we return to the initial position that means that the hash table does not
                                         // contain the key
                return false;
        }
        else
            break;                  // otherwise the loop breaks 

    if( array[pos].isValid() ) // if the program reaches this line is means that either we found an empty position 
                               // or the key string or a greater word 
    {
        if( array[pos].word == key ) // so if the position's word is equal to key string 
        {
            if( array[pos].deleted ) // and the position is deleted we return false because the word is already deleted
                return false;
            
            array[pos].deleted=1; // otherwise we set the deleted member of current position's node to 1
            array[pos].frequency=0; // and frequency to 0
            return true; // and return true
        }
    }
    return false; // if the control of the program reaches this line it means that the hash table does not contain the key string so we return
                  // false 
}

// checks if the constructors allocated memory for the array
bool hashTable::isEmpty() const
{
    if( this->array )
        return false;
    return true;
}
//=======================================================

// Private Methods
// Hash Functions
int hashTable::h1(const string &key) // first hash function
{
    unsigned long long hash =0;
    int i=0;
    while ( key[i]!='\0' )
        hash = ((int)key[i++]) + ( hash<<6 ) + ( hash<<16 ) - hash;
    return hash % arraySize;
}

int hashTable::h2(const string &key) // second hash function
{
    unsigned long long int hash = 5381;
    int i=0;
    while ( key[i]!='\0' )
        hash = ( ( hash<<5 ) + hash ) + ((int)key[i++]);

    return ( hash % arraySize ) + 1;    
}

// function for search that is used in resize method
bool hashTable::search(const string &key,arrayNode* &searchingNode)
{
    if( this->isEmpty() )
        return false;
    
    int pos=h1(key); // hashing with the first hash function the string we are going to search 
    int initialPosition=pos; // saving initial position
    int step=h2(key); // hasing with the second hash function the string we are going to search and saving it to a variable 
                      // in this way we reduce the function calls
    
    while( array[pos].isValid() ) // we move through the array while the current position of the array is occupied
    {   
        if( array[pos].word == key ) // if we find a position that contains same word as the key string we return the node's address
        {
            searchingNode=&array[pos];
            return true;
        }
        else
            if( array[pos].word > key ) // if we find a position that contains a word that is greater than the key string we return false 
                                        // because a specific search sequence is keeped sorted and that means that the hash table does not 
                                        // contain the key string so we return false
            {
                searchingNode=NULL;
                return false;
            }
        pos = ( pos + step ) % arraySize; // if the control of the program reaches this line it means that we should continue searching the 
                                          // key string inside the hash table 
        if( pos == initialPosition ) // if we return to the initial position that means that the hash table does not
        {                            // contain the key
            searchingNode=NULL;
            return false;
        }
    }

    // if the program reaches this line it means that we found an empty position and that means that the hash table does not contain the key
    // string so we return false
    searchingNode=NULL;
    return false; 
}

// function that resizes the hash table
bool hashTable::resize()
{
    if( this->isEmpty() )
        return false;
    
    int approxSize=2*(this->arraySize); // calculating the size of the new array
    int newSize;
    if( approxSize > 1001003 ) // if the approximate size is greater than the largest prime 
        newSize=2500009; // we set the newSize to a specific value
    else
        newSize=calculateNextPrime( approxSize ); // otherwise we find the next prime which is greater or equal to
                                                  // approxSize
    arrayNode* tempArray = new arrayNode [ newSize ];
    if( !array )
    {
        printError( FAILED_MEMORY_ALLOCATION );
        return false;
    }

    //and then we resize the words that are not deleted to the new array
    swap( this->array , tempArray );
    swap( this->arraySize , newSize );
    ( this->loadFactor ) = 0.0;
    
    arrayNode* currentNode;
    for(int i=0;i<newSize;i++)
        if( ( tempArray[i].isValid() ) && ( !( tempArray[i].isDeleted() ) ) )
        {
            if( !( (*this).insert( tempArray[i].word ) ) )
            {
                printError( COULD_NOT_INSERT_IN_HASH_TABLE );
                return false;
            }

            if( !(*this).search( tempArray[i].word , currentNode ) ) // checking if the word was inserted 
            {                                                        // successfully
                printError( COULD_NOT_FIND_REHASHABLE_WORD_HT );     // and getting the address of the position it
                return false;                                        // was inserted
            }
            ( currentNode->frequency ) += tempArray[i].frequency - 1; // updating the frequency of tempArray[i].word
        }                                                             // in the new array

    delete [] tempArray;
    return true;
}

//Non-Member Functions
// function that calculates the size of the hash table depending the best fit line determined 
// from the train dataset
int calculateSize( int totalTextWords )
{
    string buffer;
    ifstream lineFitFile;
    lineFitFile.open( LINE_FIT_FILE_NAME ); //opening LINE_FIT_FILE_NAME
    if( !lineFitFile.is_open() )
    {
        printError( COULD_NOT_OPEN_LINE_FIT_FILE );
        return 0;
    }

    double slope,intercept; // and then reading the slope and the intercept of the model 
    lineFitFile>>buffer;
    buffer.shrink_to_fit();
    slope=stod(  buffer ); // converting to double
    lineFitFile>>buffer;
    buffer.shrink_to_fit();
    intercept=stod( buffer ); // converting to double
    int approxSize=2*( (int)ceil( slope*totalTextWords + intercept ) ); // double up the result of the model 
    lineFitFile.close(); 

    return calculateNextPrime( approxSize ); // and then find the next prime of the approxSize
}

// function that calculates the next prime of the approxSize
int calculateNextPrime( int approxSize )
{
    string buffer;
    ifstream primesFile;
    primesFile.open( PRIMES_FILE_NAME ); //opening the file named PRIMES_FILE_NAME
    if( !primesFile.is_open() )
    {
        printError( COULD_NOT_OPEN_PRIMES_FILE ); // if the file does not open we return 0;
        return 0;
    }
    
    int currentPrime=0;
    primesFile>>buffer;
    buffer.shrink_to_fit();
    currentPrime=stoi( buffer ); //converting to integer
    do
    {
        if( currentPrime >= approxSize ) // reading primes until we reach a greater or equal than the approxSize
        {
            primesFile.close();
            return currentPrime;
        }
        primesFile>>buffer;
        buffer.shrink_to_fit();
        currentPrime=stoi( buffer ); //converting to integer
    }while( !primesFile.eof() );
    primesFile.close();
    return 2500009; // if there is no prime greater or equal than the approxSize we return a specific prime
                    // and the resize method will do the resize of the hash table
}