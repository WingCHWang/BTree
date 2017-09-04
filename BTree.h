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
void deleteKey(Node *node, int key);
void gothrough(Node *node, int i);