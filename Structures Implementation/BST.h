#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include "treeNode.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define BST_PRE_ORDER_FILE_NAME "BSTpreOrderOutput.txt"
#define BST_IN_ORDER_FILE_NAME "BSTinOrderOutput.txt"
#define BST_POST_ORDER_FILE_NAME "BSTpostOrderOutput.txt"

class bst
{
	public:
		bst(); //default constructor
		~bst(); //destructor
		virtual bool insert(string newString); //method that inserts the string word in the BST
		virtual bool Delete(string keyString); //method that deletes the string word from the BST
		bool search(string keyString,int &frequency); //method that searches the word keyString (if the word is found,it returns the words's frequency using reference variable)
		virtual bool preOrder(); //method that prints tree's word using preOrder
		virtual bool inOrder(); //method that prints tree's word using inOrder
		virtual bool postOrder(); //method that prints tree's word using postOrder
	private:
		bool deleteNode(treeNode* &current,treeNode* &parent,int p); //method that public Delete method uses to delete a word from BST
	protected:	
		bool preOrder(treeNode* current,ofstream &outputFile); //method that public preOrder uses to print tree's words
		bool inOrder(treeNode* current,ofstream &outputFile); //method that public inOrder uses to print tree's words
		bool postOrder(treeNode* current,ofstream &outputFile); //method that public postOrder uses to print tree's words
		treeNode* root; //tree's root
		void destruction(treeNode* &current); //method that destructor uses to delete the whole tree
		int getHeight(const treeNode* current); //method that returns node's height if it is !NULL else returns 0
};
#endif //BINARYSEARCHTREE_H