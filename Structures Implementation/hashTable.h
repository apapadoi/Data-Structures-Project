//Including essential libraries
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "arrayNode.h"
#include <string>
#include <iostream>
using std::string;
//============================

// defining the name of text file in which we will search prime numbers for the size of the array
// the name of the file that the data from the train dataset will be saved
// and the name of the file that we will read the slope and the intercept of the best line fit 
#define PRIMES_FILE_NAME "primes.txt"
#define STATISTICS_FILE_NAME "statistics.txt"
#define LINE_FIT_FILE_NAME "lineFit.txt"
//=======================================

class hashTable
{
    public:
        hashTable(int totalTextWords); //constructor of the class that takes as input the total words of a text file
        hashTable(); // default constructor of the class
        ~hashTable(); // destructor of the class
        bool insert(const string &key); // method that inserts the key string in the hash table
        bool search(const string &key,int &frequency); // method that searches the key string in the hash table and 
                                                       // also returns the frequency using the second parameter
        bool Delete(const string &key); // method that deletes the key string from the hash table
        double getLoadFactor() const { return loadFactor; }; //getter for load factor of the hash table
        bool isEmpty() const; // checks if the constructors allocated memory for the array
        int getArraySize() const { return arraySize; }; //getter for the size of the hashTable 
    private:
        int arraySize; // size of the array
        double loadFactor; // load factor of the array
        arrayNode* array; 
        int h1(const string &key); // first hash function
        int h2(const string &key); // second hash function
        bool resize(); // function that resizes the hash table
        bool search(const string &key,arrayNode* &searchingNode); // function for search that is used in resize method
};

//Non-Member Functions
int calculateSize( int totalTextWords ); // function that calculates the size of the hash table using the best fit line determined 
                     // from the train dataset
int calculateNextPrime( int approxSize ); // function that calculates the next prime of the approxSize

template <typename T>
void swap(T &a,T &b)
{
    T temp;
    temp=a;
    a=b;
    b=temp;
}
#endif //HASH_TABLE_H