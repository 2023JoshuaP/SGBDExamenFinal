#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <fstream>
using namespace std;

class Node {
    public:
        bool isLeaf;
        vector<int> keys;
        Node* ptr2next;
        
        union ptr {
            vector<FILE*> dataPtr;
            vector<Node*> ptr2Tree;
            ptr() {};
            ~ptr() {};
        } ptr2TreeOrData;
        friend class BPlusTree;
    public:
        Node();
};

class BPlusTree {
    private:
        int maxIntChildLimit;
        int maxLeafNodeLimit;
        Node* root;
        void insertInternal(int key, Node* parent, Node* child);
        Node** findParent(Node* cursor, Node* child);
        Node* firstLeftNode(Node* cursor);
    public:
        BPlusTree();
        BPlusTree(int degreeInternal, int degreeLeadf);
        Node* getRoot();
        int getMaxIntChildLimit();
        int getMaxLeafNodeLimit();
        void setRoot(Node*);
        void display(Node* cursor);
        void seqDisplay(Node* cursor);
        string search(int key);
        void insert(int key, FILE* filePtr, const string& additionalData);
        void removeKey(int key);
        void removeInternal(int x, Node* cursor, Node* child);
        void generateDOT(Node* cursor, ofstream &file);
        void generateDOTFile(const string& filename);
};

#endif