#include <iostream>
#include "BTree.h"
using namespace std; 

int main(int argc, char** args) {
    Tree *tree = new Tree;
    create(tree);
    for(int i = 0; i<100; i++) {
        insert(tree, i);  
    }
    cout << "insert\n";
    gothrough(tree->root, 0);
    
    for(int i = 0; i<100; i++) {
        cout<< "delete" << i << "\n";
        deleteKey(tree, i);  
        
    }
    gothrough(tree->root, 0);
    
}