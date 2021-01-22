#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "dsHandler.h"
using std::cout;
using std::endl;
using std::clock;
using std::cin;

int main()
{
    srand(time(NULL));//initiating srand so we can use it afterwards when choosing words for Q set
    
    //READING TEXT FILE
    fileHandler currentFileHandler( TEST_FILE_NAME , OUTPUT_FILE_NAME );
    if( !currentFileHandler.readText() )
        return 1;

    //Insertions in the ds
	hashTable ht( currentFileHandler.getTotalTextWords() );
    bst BST;
    avl testAVL;   
    string*Q = new string [ CHOSEN_WORDS_NUMBER ];
    /*int totalTextWords=0;*/ // in this main function we dont want to save the number of total words the txt file
                              // contains
	dsHandler tester( currentFileHandler , ht , BST , testAVL , Q , CHOSEN_WORDS_NUMBER /*, totalTextWords*/ );	
    
    //searching
    tester.search( currentFileHandler , ht , BST , testAVL , Q );
    
	testAVL.preOrder();
    testAVL.inOrder();
    testAVL.postOrder();
	BST.preOrder();
	BST.inOrder();
	BST.postOrder();
			
    //deleting
    tester.Delete( currentFileHandler , ht , BST , testAVL , Q );

    //searching deleted words
	tester.search( currentFileHandler , ht , BST , testAVL , Q );
    
    ht.insert("the");
    BST.insert("the");
    testAVL.insert("the");
    
    ht.insert("the");
    BST.insert("the");
    testAVL.insert("the");
    
    ht.insert("britain");
    BST.insert("britain");
    testAVL.insert("britain");
    
    tester.search( currentFileHandler , ht , BST , testAVL , Q );
    
    delete [] Q;
    return 0;
}