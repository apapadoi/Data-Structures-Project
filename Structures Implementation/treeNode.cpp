#include "treeNode.h"

treeNode::treeNode(string newString) //constructor
{
    (this->word) = newString; //setting node's word equal to newString
    (this->frequency) = 1; //setting node's word frequency equal to 1
    (this->height) = 0; //setting node's height equal to 0
    (this->right) = NULL; //setting node's right child equal to NULL pointer
    (this->left) = NULL; //setting node's left child equal to NULL pointer
}