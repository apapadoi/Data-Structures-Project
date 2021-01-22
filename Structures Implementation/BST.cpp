#include <iostream>
#include "BST.h"
#include <string>
#include <cmath>
using namespace std;

bst::bst() //default constructor
{
	( this->root ) = NULL; //sets root to NULL pointer
}

bst::~bst() //destructor
{
	destruction(this->root); //calling destruction method to delete whole tree
}

int bst::getHeight(const treeNode* current) //method that returns node's height if it is !NULL else returns 0
{
    if( current ) //if root !=NULL pointer
        return ( current->height ); //returns current nodes's height
    //else returns 0
    return 0;
}

void bst::destruction(treeNode* &current) //method that destructor uses to delete the whole tree
{
	if( current ) //if root !=NULL pointer
	{ 	//deletes whole tree using postOrder traversal but instead of printing,it deletes the node
		destruction( current->left ); //calling destruction for left subtree
		destruction( current->right ); //calling destruction for right subtree
		delete current; //deletes current node
	}
}

bool bst::insert(string newString) //method that inserts the word newString in the BST
{
	if (!this->root) //if root is NULL allocate new space to save the key string
	{
		treeNode* newNode=new treeNode(newString); //allocating space and calling default treeNode's constructor to do the rest
		this->root=newNode;
		return true; //returns true
	}
	else //if root != NUll pointer
	{
		treeNode* current=this->root;
		while(current) //creating a loop to find where the key word has to be inserted
		{
			if (current->word<newString) //if key word is greater than the current node's word
			{
				if (!current->right) //if current node's right subtree is NULL
				{
					treeNode* newNode; //allocating space
					newNode=new treeNode(newString); //calling default treeNode's constructor to save the word
					if (!newNode) //if allocation failed return false
						return false;
					current->right=newNode; //current node's right subtree points to newNode for which we just allocated space and saved newString word
					return true;
				}
				else
					current=current->right; //we continue to right subtree to find where the key word has to be inserted
			}
			else if (current->word>newString) //if current node's word is greater than the key word
			{
				if(!current->left) //if current node's left subtree is NULL
				{
					treeNode* newNode; //allocating space
					newNode=new treeNode(newString); //calling default treeNode's constructor to save the word
					if (!newNode) //if allocation failed return false
						return false;
					current->left=newNode; //current node's left subtree points to newNode for which we just allocated space and saved newString word
					return true;
				}
				else 
					current=current->left; //we continue to left subtree to find where the key word has to be inserted
			}
			else //if current node's word is equal to key word
			{
				current->frequency++; //increasing node's frequency
				return true; //return true
			}
		}
		return true; //return true
	}
}

bool bst::search(string newString, int& frequency) //method that searches the word keyString (if the word is found,it returns the words's frequency using reference variable)
{
	if (!this->root) //if root is NULL 
	{
		frequency = 0; //setting frequency to be returned to 0
		return false; //return false
	}
	treeNode* current=this->root; //current==root
	while (current) //loop to find the newString
	{
		if (current->word == newString) //if current node's word is equal to key word
		{
			frequency = current->frequency; //setting frequency to be returned equal to current node's frequency
			return true; //return true
		}
		else if (current->word < newString) //if key word is greater than current node's word
		{
			current=current->right; //we continue to right subtree to find the key word 
		}
		else //current->word > newString, if current node's word is greater than key word
			current=current->left; //we continue to left subtree to find the key word

	}
	frequency = 0; //if loop breaks,set frequency to be returned equal to 0 and return false because the word newString was not found
	return false;
}

bool bst::Delete(string keyString) //method that deletes the string word from the BST
{
	if (!this->root) //if root is NULL we return false
		return false;
	treeNode* current=this->root; //current==root
	treeNode* parent=NULL;//parent=NULL
	int p; //shows if the node that is going to be deleted was parent's left child=1 or right child=2
	while(current && current->word!=keyString) //if current != NULL and current node's word != key word keep going
	{	
		if (current->word>keyString) //if current node's word is greater than key word
		{
			parent=current;
			p=1; //we moved left from the parent to left child
			current=current->left; //we continue to left subtree to find the key word
		}
		else//current->word<keyString
		{
			parent=current;
			p=2; //we moved right from the parent to right child
			current=current->right; //we continue to right subtree to find the key word
		}	
	}
	if(!current) //current is NULL
		return false; //return false
	
	return deleteNode(current,parent,p); //deletes the key word using deleteNode method
}

bool bst::deleteNode(treeNode* &current,treeNode* &parent,int p) //method that public Delete method uses to delete a word from BST
{
	if (!current->left && !current->right) //current has no child
	{
		delete(current); //deleting current
		if (!parent) //parent is NULL
			this->root=NULL; //setting root to NULL
		else
		{
			if (p==1) //if the node to be deleted is parent's left child
				parent->left=NULL; //setting parent's left child to NULL
			else //else if node to be deleted is parent's right child
				parent->right=NULL; //setting parent's right child to NULL
		}
		return true;
	}
	else if (current->left && !current->right)//current has only left child
	{
		if (!parent) //if parent is NULL
			root=current->left; //setting root equal to current's left child
		else //parent != NULL pointer
		{
			if (p==1) //if the node to be deleted is parent's left child
				parent->left=current->left; //setting parent's left child to current node's left child
			else //else if node to be deleted is parent's right child
				parent->right=current->left; //setting parent's right child to current node's left child
		}
		delete (current); //deleting current and return true
		return true;
	}
	else if (!current->left && current->right) //current has only right child
	{
		if (!parent) //if parent is NULL
			root=current->right; //setting root equal to current's right child
		else //parent != NULL pointer
		{
			if (p==1) //if the node to be deleted is parent's left child
				parent->left=current->right; //setting parent's left child to current node's right child
			else //if the node to be deleted is parent's right child
				parent->right=current->right; //setting parent's right child to current node's right child
		}
		delete(current); //deleting current and return true
		return true;
	}
	else //current has 2 children
	{
		p=2; //we find the node that is next to current node in in order traversal
		treeNode* nextOrdered=current->right; //creating new node and setting it equal to current node's right child
		while(nextOrdered->left) //finds the next node
		{
			parent=nextOrdered;
			nextOrdered=nextOrdered->left;
			p=1;
		}
		current->word=nextOrdered->word; //copying data
		current->frequency=nextOrdered->frequency; //copying data
		
		if (p==2) //if node to be deleted is right child of current node
		{
			current->right=nextOrdered->right; //setting current node's right child equal to the next node's right child
			delete(nextOrdered); //and delete the next node
		}
		else //if node to be deleted is not right child of current node ( it means that while loop was executed at least once )
		{
			parent->left=nextOrdered->right; //setting parent's left child equal to the next node's right child
			delete(nextOrdered); //and delete the next node
		}	
	}
	//return true 
	return true;
}

//method that prints preOrder traversal of the bst tree in the file named BST_PRE_ORDER_FILE_NAME
bool bst::preOrder()
{
	ofstream outputFile;
	outputFile.open(BST_PRE_ORDER_FILE_NAME);
	if( outputFile.is_open() )
	{
		outputFile<<"\t\t\tPre Order Traversal:"<<endl<<endl;
		preOrder(this->root,outputFile);
		outputFile.close();
		return true;
	}

	return false;
}

//method that public preOrder uses to print tree's words
bool bst::preOrder(treeNode* current,ofstream &outputFile)
{
	if (current)
	{
		outputFile<<current->word<<endl;
		preOrder(current->left,outputFile);
		preOrder(current->right,outputFile);
	}

	return true;
}

//method that prints inOrder traversal of the bst tree in the file named BST_IN_ORDER_FILE_NAME
bool bst::inOrder()
{
	ofstream outputFile;
	outputFile.open(BST_IN_ORDER_FILE_NAME);
	if( outputFile.is_open() )
	{
		outputFile<<"\t\t\tIn Order Traversal:"<<endl<<endl;
		inOrder(this->root,outputFile);
		outputFile.close();
		return true;
	}

	return false;
}

//method that public inOrder uses to print tree's words
bool bst::inOrder(treeNode* current,ofstream &outputFile)
{
	if (current)
	{
		inOrder(current->left,outputFile);
		outputFile<<current->word<<endl;
		inOrder(current->right,outputFile);
	}

	return true;
}

//method that prints postOrder traversal of the bst tree in the file named BST_POST_ORDER_FILE_NAME
bool bst::postOrder()
{
	ofstream outputFile;
	outputFile.open(BST_POST_ORDER_FILE_NAME);
	if( outputFile.is_open() )
	{
		outputFile<<"\t\t\tPost Order Traversal:"<<endl<<endl;
		postOrder(this->root,outputFile);
		outputFile.close();
		return true;
	}

	return false;
}

//method that public postOrder uses to print tree's words
bool bst::postOrder(treeNode* current,ofstream &outputFile)
{
	if (current)
	{
		postOrder(current->left,outputFile);
		postOrder(current->right,outputFile);
		outputFile<<current->word<<endl;
	}

	return true;
}