#pragma once
#include "BTreeNode.h"

class BTree {
    private:
        int order;
        BTreeNode* root;
    public:
        BTree(int order) : order(order), root(nullptr) { }
        ~BTree();
        void Add(const int key);
        void Search(const int key) const;
        void Print() const;
        void Load();
        void Save() const;
        void Remove(const int key);
    private:
        void Add_FirstElement(const int key);
        void Add_ToFullRoot(const int key);
};