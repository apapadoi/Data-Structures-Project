#ifndef TREENODE_H
#define TREENODE_H
#include <string>
using std::string;

class treeNode
{
    public:
        treeNode(string newString); //constructor
        friend class avl;
        friend class bst;
    private:
        string word; //nodes's word
        int frequency; //shows how many times a word was inserted in the tree
        int height; //shows node's height
        treeNode* left; //node's left child
        treeNode* right; //node's right child
};
#endif //TREENODE_H