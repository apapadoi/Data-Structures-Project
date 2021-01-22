#ifndef DS_HANDLER_H
#define DS_HANDLER_H
#include "fileHandler.h"
#include "hashTable.h"
#include "AVL.h"
// class that handles all the data structures
class dsHandler
{
    public:
        // constructor of the class that does also the insertion of the Q set inside the data structures
        dsHandler(fileHandler &currentFileHandler,hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string Q[],int numChosenWords,int &totalTextWords);
        // constructor of the class that does not give access to totalTextWords 
        dsHandler(fileHandler &currentFileHandler,hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string Q[],int numChosenWords);
        // method that searches all the Q set words in the data structures and writes to benchmark file the time it
        // took for searching
        bool search( fileHandler &currentFileHandler, hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string* Q);
        // method that deletes all the Q set words from the data structures and writes to benchmark file the time it
        // took for deleting
        bool Delete( fileHandler &currentFileHandler, hashTable &currentHashTable,bst &currentBST,avl &currentAVL,string* Q);
        int getQsize() { return Qsize; };
        void setQsize( int newQsize ) { Qsize = newQsize; };
    private:
        int Qsize; // saving actual Qsize in case there were read less words than the user's choice
};
#endif //DS_HANDLER_H