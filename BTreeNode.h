#pragma once
#include <iostream>

class BTreeNode {
    private:
        int order;
        bool isLeaf;
        int* keys;
        int keysNum;
        BTreeNode** children;
    public:
        BTreeNode(int order, bool isLeaf);
        ~BTreeNode();
        void SplitChild(int childIndex, BTreeNode* childToBeSplitted);
        void InsertToThisNotFullNode(int newKey);
        void Print() const;
        bool Contains(const int key) const;
    private:
        void AddNewKey(int newKey);
        void AddNewKeyToChild(int newKey);
    friend class BTree;
};
