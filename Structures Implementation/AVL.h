#ifndef AVL_H
#define AVL_H
#include "treeNode.h"
#include "BST.h"

#define AVL_PRE_ORDER_FILE_NAME "AVLpreOrderOutput.txt"
#define AVL_IN_ORDER_FILE_NAME "AVLinOrderOutput.txt"
#define AVL_POST_ORDER_FILE_NAME "AVLpostOrderOutput.txt"

class avl : public bst
{
    public:
        avl(); //default constructor
        ~avl(); //destructor
        bool insert(const string &key); //method that inserts the key string in the avl tree
        bool Delete(const string &key); // method that deletes the key string from the avl tree
    	bool preOrder(); //method that prints tree's words in preOrder traversal
		bool inOrder(); //method that prints tree's words in inOrder traversal
		bool postOrder(); //method that prints tree's words in postOrder traversal
	private:
        bool insertion(const string &key,treeNode* &current); //method that public insert method uses to insert a word
        bool deletion(treeNode* &current,const string &key); // method that public Delete method uses to delete a word
        treeNode* rightRotation(treeNode* current); //method that performs right rotation
        treeNode* leftRotation(treeNode* current); //method that performs left rotation
        treeNode* rlRotation(treeNode* current); //method that performs right left rotation 
        treeNode* lrRotation(treeNode* current); //method that performs left right rotation
        treeNode* nextOrdered(treeNode* current); // method that finds the next node of current node in in order traversal
        int max(int a,int b); // method that returns the max of two integers
        int getBalanceFactor(treeNode *current); // method that returns the balance factor of a specific node
};
#endif //AVL_H