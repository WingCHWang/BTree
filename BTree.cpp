#include <iostream>
#include "BTree.h"
using namespace std; 


Node *alloc_node() {
    Node *node = new Node;
    node->n = 0;
    for(int i = 0; i < 2*t-1; i++) {
        node->key[i] = 0;
        node->child[i] = NULL;
    }
    node->child[2*t-1] = NULL;
    node->leaf = true;
}

void free_node(Node *node) {
    for(int i = 0; i < 2*t-1; i++) {
        node->key[i] = 0;
        node->child[i] = NULL;
    }
    node->child[2*t-1] = NULL;
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
    
    for(int j =0; j<t-1; j++) {
        z->key[j] = y->key[t+j];
    }
    if(!y->leaf) {
        for(int j = 0; j<t; j++) {
            z->child[j] = y->child[t+j];
            y->child[t+j] = NULL;
        }
    }
    y->n = t-1;
    z->n = t-1;
    for(int j = x->n; j>=i+1; j--) {
        x->child[j+1] = x->child[j];
    }
    x->child[i+1] = z;
    for(int j = x->n-1; j>=i; j--) {
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

int indexOf(Node *node, int key) {
    for(int i = 0; i < node->n; i++) {
        if(node->key[i] == key) {
            return i;
        }
    }
    return -1;
}

int contains(Node *node, int key) {
    Pos pos;
    for(int i = 0; i <= node->n; i++) {
        pos = search(node->child[i], key);
        if(pos.node != NIL) {
            return i;
        }
    }
    return -1;
}


void deleteKey(Tree *T, int key) {
    Node *root = T->root;
    if(!root->leaf && root->n == 1 && root->child[0]->n < t && root->child[1]->n < t) {
        Node *left = root->child[0];
        Node *right = root->child[1];
        left->key[t-1] = root->key[0];
        for(int i = t; i < 2*t-1; i++) {
            left->key[i] = right->key[i-t];
        }
        if(!left->leaf) {
            for(int i = t; i <= 2*t-1; i++) {
                left->child[i] = right->child[i-t];
            }
        }
        
        left->n = 2*t-1;
        free_node(right);
        free_node(root);
        T->root = left;       
    }
    deleteKey_inner(T->root, key);
}


void deleteKey_inner(Node *node, int key) {
    int index = indexOf(node, key);
    if(index > -1) {
        if(node->leaf) {
            for(int i = index; i < node->n-1; i++) {
                node->key[i] = node->key[i+1];
            }
            node->n--;
        }
        else {
            Node *preChild = node->child[index];
            Node *postChild = node->child[index+1];
            if(preChild->n >= t) {
                Node *temp = preChild;
                while(!temp->leaf) {
                    temp = temp->child[temp->n];
                }
                int preKey = temp->key[temp->n-1];
                node->key[index] = preKey;
                deleteKey_inner(preChild, preKey);
            }
            else if(postChild->n >= t){
                Node *temp = preChild;
                while(!temp->leaf) {
                    temp = temp->child[0];
                }
                int postKey = temp->key[0];
                node->key[index] = postKey;
                deleteKey_inner(postChild, postKey);
            }
            else {
                preChild->key[t-1] = key;
                for(int i = t; i < 2*t-1; i++) {
                    preChild->key[i] = postChild->key[i-t];
                }
                for(int i = t; i <= 2*t-1; i++) {
                    preChild->child[i] = postChild->child[i-t];
                }
                preChild->n = 2*t-1;
                free_node(postChild);
                for(int i = index; i < node->n-1; i++) {
                    node->key[i] = node->key[i+1];
                    node->child[i+1] = node->child[i+2];
                }
                node->n--;
                deleteKey_inner(preChild, key);
            }
        }
    }
    else {
        int childIndex = contains(node, key);
        
        if(childIndex > -1) {
            Node *child = node->child[childIndex];
            if(child->n < t) {
                int left = childIndex > 0 ? childIndex-1 : 0;
                int right = childIndex < node->n ? childIndex+1 : childIndex;
                Node *leftBrother = node->child[left];
                Node *rightBrother = node->child[right];
                if(leftBrother->n >= t) {
                    for(int i = leftBrother->n; i > 0; i--) {
                        child->key[i] = child->key[i-1];
                    }
                    for(int i = leftBrother->n+1; i > 0; i--) {
                        child->child[i] = child->child[i-1];
                    }
                    child->key[0] = node->key[childIndex-1];
                    child->child[0] = leftBrother->child[leftBrother->n];
                    child->n++;
                    node->key[childIndex-1] = leftBrother->key[leftBrother->n-1];
                    leftBrother->n--;
                }else if(rightBrother->n >= t) {
                    child->key[child->n] = node->key[childIndex];
                    child->child[child->n+1] = rightBrother->child[0];
                    node->key[childIndex] = rightBrother->key[0];
                    child->n++;

                    for(int i = 0; i < rightBrother->n-1; i++) {
                        rightBrother->key[i] = rightBrother->key[i+1];
                    }
                    for(int i = 0; i < rightBrother->n; i++) {
                        rightBrother->child[i] = rightBrother->child[i+1];
                    }
                    rightBrother->n--;
                }else {
                    if(childIndex > 0) {
                        leftBrother->key[t-1] = node->key[childIndex-1];
                        for(int i = t; i < 2*t-1; i++) {
                            leftBrother->key[i] = child->key[i-t];
                        }
                        for(int i = t; i <= 2*t-1; i++) {
                            leftBrother->child[i] = child->child[i-t];
                        }
                        leftBrother->n = 2*t-1;
                        free_node(child);
                        for(int i = childIndex; i < node->n-1; i++) {
                            node->key[i-1] = node->key[i];
                            node->child[i] = node->child[i+1];
                        }
                        node->n--;
                        child = leftBrother;
                    }
                    else {
                        child->key[t-1] = node->key[childIndex];
                        for(int i = t; i < 2*t-1; i++) {
                            child->key[i] = rightBrother->key[i-t];
                        }
                        for(int i = t; i <= 2*t-1; i++) {
                            child->child[i] = rightBrother->child[i-t];
                        }
                        child->n = 2*t-1;           
                        free_node(rightBrother);
                        for(int i = childIndex; i < node->n-1; i++) {
                            node->key[i] = node->key[i+1];
                            node->child[i+1] = node->child[i+2];
                        }
                        node->n--;
                        
                    }
                }
            }
            deleteKey_inner(child, key);
        }
    }
}

void gothrough(Node *node, int i) {
    if(i == 0) {
        cout << "\n----------------------\n\n";
    }
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
 

