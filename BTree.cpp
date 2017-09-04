#include <iostream>
#include <string>
using namespace std; 

#define t 5
#define NIL 0

typedef struct Node{
    int n;
    int key[2*t-1];
    bool leaf;
    struct Node *child[2*t];
    string data;
}Node;

typedef struct {
    Node *root;
}Tree;

typedef struct {
    Node* node;
    int index;
}Pos;

Node *alloc_node();
void free_node(Node *node);
Pos search(Node* node, int key);
void create(Tree *tree);
void splitChild(Node* x, int i);
void insertNonFull(Node* node, int key);
void insert(Tree* T, int key);
void gothrough(Node *node, int i);


Node *alloc_node() {
    return new Node();
}

void free_node(Node *node) {
    delete node;
}

Pos search(Node* node, int key) {
    int i = 0;
    while(i < node->n && key > node->key[i]) {
        i = i+1;
    }
    if(i < node->n && key == node->key[i]) {
        Pos pos = {node, i};
        return pos;
    }    
    else if(node->leaf) {
        Pos pos = {NIL, i};
        return pos;
    }    
    else
        return search(node->child[i], key);
}

void create(Tree *tree) {
    Node *root = alloc_node();
    root->leaf = true;
    root->n = 0;
    tree->root = root;
    cout << "create\n";
    
}

void splitChild(Node* x, int i) {
    Node *z = alloc_node();
    Node *y = x->child[i];
    z->leaf = y->leaf;
    z->n = t-1;
    for(int j =0; j<t-1; j++) {
        z->key[j] = y->key[t+j];
    }
    if(!y->leaf) {
        for(int j = 0; j<t-1; j++)
            z->child[j] = y->child[t+j];
    }
    y->n = t-1;
    for(int j = x->n+1; j>=i+1; j--) {
        x->child[j+1] = x->child[j];
    }
    x->child[i+1] = z;
    for(int j = x->n; j>=i; j--) {
        x->key[j+1] = x->key[j];
    }
    x->key[i] = y->key[t-1];
    x->n ++;
}

void insertNonFull(Node* node, int key) {
    int i = node->n-1;
    if(node->leaf) {
        while(i>=0 && key<node->key[i]) {
            node->key[i+1] = node->key[i];
            i--;
        }
        node->key[i+1] = key;
        node->n ++;
    }
    else {
        while(i>=0 && key<node->key[i]) {
            i--;
        }
        i++;
        if(node->child[i]->n == 2*t-1) {
            splitChild(node, i);
            if(key > node->key[i])
                i++;
        }
        insertNonFull(node->child[i], key);
    }
}

void insert(Tree* T, int key) {
    Node *root = T->root;
    if(root->n == 2*t-1) {
        Node *newRoot = alloc_node();
        newRoot->leaf = false;
        newRoot->n = 0;
        newRoot->child[0] = root;
        T->root = newRoot;
        splitChild(newRoot, 0);
        insertNonFull(newRoot, key);
    }
    else {
        
        insertNonFull(root, key);
        
    }
        
}

void gothrough(Node *node, int i) {
    cout << i << "th\n";
    for(int j = 0; j<node->n; j++) {
        cout << node->key[j] << "|";
    }
    cout << "\n";
    if(!node->leaf) {
        for(int j = 0; j<node->n+1; j++) {
            gothrough(node->child[j], i+1);
        }
    }
}
 
int main(int argc, char** args) {
    Tree *tree = new Tree();
    create(tree);
    for(int i = 0; i<50; i++) {
        cout << i << "\n";
        insert(tree, i);  
    }
    gothrough(tree->root, 0);
}
