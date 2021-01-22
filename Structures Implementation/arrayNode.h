#ifndef ARRAY_NODE_H
#define ARRAY_NODE_H
#include <string>
using std::string;
class arrayNode
{
    public:
        arrayNode(); //default constructor
        arrayNode(string word); //constructor that initialises private members to correct values
        bool isValid() const { return valid; }; 
        bool isDeleted() const; 
        arrayNode operator=(const arrayNode &rightObject); // overloading operator =
        friend class hashTable;
    private:
        string word; // the word that a specific node saves
        int frequency; // the word's frequency
        bool valid; // shows if the node is occupied
        unsigned int deleted : 1; // shows if the node is deleted
        void swap(arrayNode &rightObject); // private method that swaps the data of two nodes
};
#endif //ARRAY_NODE_H