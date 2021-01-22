#include "arrayNode.h"

//default constructor
arrayNode::arrayNode()
{
    ( this->word ) = "";
    ( this->frequency ) = 0;
    ( this->valid ) = false;
    ( this->deleted ) = 0;
}

//constructor that initialises private members to correct values
arrayNode::arrayNode(string word)
{
    ( this->word ) = word;
    ( this->valid ) = true;
    ( this->frequency ) = 1;
    ( this->deleted ) = 0;
}

// private method that swaps the data of two nodes
void arrayNode::swap(arrayNode &rightObject)
{
    string tempString;
    tempString= this->word;
    ( this->word )=rightObject.word;
    rightObject.word=tempString;

    int tempFrequency;
    tempFrequency= this->frequency;
    ( this->frequency )=rightObject.frequency;
    rightObject.frequency=tempFrequency;

    bool tempValid;
    tempValid= this->valid;
    ( this->valid )=rightObject.valid;
    rightObject.valid=tempValid;

    unsigned int tempDeleted;
    tempDeleted= this->deleted;
    ( this->deleted )=rightObject.deleted;
    rightObject.deleted=tempDeleted;
}

// overloading operator =
arrayNode arrayNode::operator=(const arrayNode &rightObject)
{
    ( this->word )=rightObject.word;
    ( this->word ).shrink_to_fit();
    ( this->frequency )=rightObject.frequency;
    ( this->valid )=rightObject.valid;
    ( this->deleted )=rightObject.deleted;

    return *this;
}

bool arrayNode::isDeleted() const
{
    return ( this->deleted ) == 1;
}