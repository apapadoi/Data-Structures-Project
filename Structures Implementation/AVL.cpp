#include "AVL.h"
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

//default constructor
avl::avl() : bst()
{ }

//destruction method
avl::~avl()
{ }

//method that returns max out of 2 numbers
int avl::max(int a,int b)
{
    return ( (a > b) ? a : b );
}

//method that performs right rotation
treeNode* avl::rightRotation(treeNode* current)
{
    treeNode* temp;
    temp = current->left;
	
	if( temp )
    	( current->left ) = temp->right;
	else
		( current->left ) = NULL;
	
	if( temp )
    	( temp->right ) = current;

    ( current->height ) = max( getHeight(current->left) , getHeight(current->right) ) + 1;

    if( temp )
		( temp->height ) = max( getHeight(temp->left) , getHeight(temp->right) ) + 1;
    return temp;
	
}

//method that performs left rotaion
treeNode* avl::leftRotation(treeNode* current)
{
    treeNode* temp;
    temp = current->right;
    if( temp )
		( current->right ) = temp->left;
    else
		( current->right ) = NULL;

	if( temp )
		( temp->left ) = current;
	
    ( current->height ) = max( getHeight(current->left) , getHeight(current->right) ) + 1;
    
	if( temp)
		( temp->height ) = max( getHeight(temp->left) , getHeight(temp->right) ) + 1;
    
	return temp;
}

//method that performs left right rotation
treeNode* avl::lrRotation(treeNode* current)
{
    ( current->left ) = leftRotation( current->left );
    return rightRotation(current);
}

//methot that performs right left rotation
treeNode* avl::rlRotation(treeNode* current)
{
    ( current->right ) = rightRotation( current->right );
    return leftRotation(current);
}

//method that inserts the key string to the AVL tree
bool avl::insert(const string &key)
{
    return insertion(key,this->root);
}

// method that public insert method uses to insert a word
bool avl::insertion(const string &key,treeNode* &current)
{
    if(!current)
    {
        current = new treeNode(key); //if current node is NULL allocate new space to save the key string
        if(!current) //return false is there is no space to allocate
            return false;
        //else return true
        return true;
    }
    else 
        if( key < current->word ) // if the current node's word is greater than the key word we insert the key 
        {
            insertion(key,current->left); // string in the left subtree recursively
            if( getHeight(current->left) - getHeight(current->right) == 2 ) 
            {  //if the node is not balanced then we have to fix the tree
                if( key < current->left->word ) //if current node's left child's word is greater than the key we perform a right rotation to fix the AVL tree
                    current=rightRotation(current);
                else //else we perform a left right rotation to fix the AVL tree
                    current=lrRotation(current);
            }
        }
        else // if the key word is greater than the current node's word we insert the key
            if ( key > current->word )
            {
                insertion(key,current->right); // string in the right subtree recursively
                if( getHeight(current->left) - getHeight(current->right) == -2 )
                { //if the node is not balanced then we have to fix the tree
                    if( key > current->right->word ) //if the key is greater than the current node's right child's word we perform a left rotation to fix the AVL tree
                        current=leftRotation(current);
                    else //else we perform a right left rotation to fix the AVL tree
                        current=rlRotation(current);
                }
            }
            else //if current node's word is equal to key's word
                ( current->frequency )++; //increasing current node's frequency

    ( current->height ) = max( getHeight(current->left) , getHeight(current->right) ) + 1; //updating height and return true
    return true;
}

// method that deletes the key string from the avl tree
bool avl::Delete(const string &key)
{
	return deletion(this->root,key);
}

// method that public Delete method uses to delete a word
bool avl::deletion(treeNode* &current,const string &key)
{
	if (!current)  // if the avl tree with root==current is empty return false
        return false;  

    if ( key < current->word )  // if the current node's word is greater than the key word we delete the key
        deletion(current->left, key);  // string in the left subtree recursively
    else if( key > current->word )  // if the key word is greater than the current node's word we delete the key 
        deletion(current->right, key);  // string in the right subtree recursively
    else // current->word == key
    {  
        if( !(current->left) || !(current->right) )  // the node that is going to be deleted has 1 child or no child at all
        {  
            treeNode *temp; // a temporary node to save the address of the node we want to delete in case it has a 
                            // left or right child
			if( current->left ) // if there is a left child we set temp equal to this left child 
				temp = current->left;
			else // otherwise we set temp equal to the right child if the current node has no children at all then 
				temp = current->right; // the current->right equals NULL so still works in the general case

            if (!temp)  // if the current has no children
            {  
                temp = current;  // we save current's address to temp, set current equal to NULL and then delete temp
                current = NULL;  
            }  
            else // otherwise the current has a child so we save temp's ( child's ) data to current and then delete temp
            	*current = *temp; 

            delete temp;
			temp = NULL;
        }  
        else // the node that is going to be deleted has both left and right child 
        {  
            treeNode* temp; // so we find the node that is next to current node in inOrder traversal of the avl tree
			temp = nextOrdered(current->right);  

            current->word = temp->word;   // copy the data from the next Ordered node in the current node 
			current->frequency = temp->frequency;

           	deletion(current->right,temp->word);  // and then delete the next Ordered node. Because the nextOrdered is 
                                                  // the node with the smaller value in the right subtree of current 
                                                  // it means that it has 1 or 0 child so when deletion is called 
                                                  // recursively here the above if condition will be true
        }  
    }  

    if (!current)  // if current is equal to NULL it means that the subtree with root the node current is Empty after
    	return true; // the delete of the key word
    // after the deletion we update the height of the current node
    current->height = 1 + max(getHeight(current->left),getHeight(current->right));  
  
    int balance = getBalanceFactor(current); // saving balance factor of the current node so we get rid of some 
                                            // function calls
    // and then doing the essential rotations in the nodes that are included in the path from the root to the node 
    // we deleted in reversed order because there is a possibility that if a rotation is made in a child of a node
    // then the balance of this node will be fixed too
    if ( balance > 1 )  // if balance is greater than 1 it means that we need to fix the left subtree
    {
		if( getBalanceFactor(current->left) >= 0 ) // if the balance of the current's left child is greater or equal to
	    	current = rightRotation(current);      // 0 then again we need to fix the left subtree.So we have a left left imbalance
		else                                       // and we fix this ll imbalance with a right rotation
			current = lrRotation(current); // otherwise we have a left right imbalance and we fix this lr imbalance with a left-right rotation
	}
	else if( balance < -1 ) // if balance is less than -1 it means that we need to fix the right subtree
	{
		if( getBalanceFactor(current->right) <= 0 ) // if the balance of the current's right child is less or equal to
			current = leftRotation(current);        // 0 then we also need to fix the right subtree.So we have a right right imbalance
		else                                        // amd we fix this rr imbalance with a left rotation
			current= rlRotation(current); // otherwise we have a right left imbalance and we fix this rl imbalance with a right-left rotation 
	}

    return true;  
}

// method that returns the balance factor of a specific node
int avl::getBalanceFactor(treeNode *current)  
{  
    if ( !current )  
        return 0;  
    return getHeight(current->left) - getHeight(current->right);  
}  

// method that finds the next node of a specific node in in order traversal
treeNode* avl::nextOrdered(treeNode* current)  
{  
    treeNode* node = current;  

    while (node->left != NULL)  
        node = node->left;  
  
    return node;  
}

//method that prints preOrder traversal of the avl tree in the file named AVL_PRE_ORDER_FILE_NAME
bool avl::preOrder()
{
	ofstream outputFile;
	outputFile.open(AVL_PRE_ORDER_FILE_NAME);
	if( outputFile.is_open() )
	{
		outputFile<<"\t\t\tPre Order Traversal:"<<endl<<endl;
		bst::preOrder(this->root,outputFile);
		outputFile.close();
		return true;
	}

	return false;
}

//method that prints inOrder traversal of the avl tree in the file named AVL_IN_ORDER_FILE_NAME
bool avl::inOrder()
{
	ofstream outputFile;
	outputFile.open(AVL_IN_ORDER_FILE_NAME);
	if( outputFile.is_open() )
	{
		outputFile<<"\t\t\tIn Order Traversal:"<<endl<<endl;
		bst::inOrder(this->root,outputFile);
		outputFile.close();
		return true;
	}

	return false;
}

//method that prints postOrder traversal of the avl tree in the file named AVL_POST_ORDER_FILE_NAME
bool avl::postOrder()
{
	ofstream outputFile;
	outputFile.open(AVL_POST_ORDER_FILE_NAME);
	if( outputFile.is_open() )
	{
		outputFile<<"\t\t\tPost Order Traversal:"<<endl<<endl;
		bst::postOrder(this->root,outputFile);
		outputFile.close();
		return true;
	}

	return false;
}