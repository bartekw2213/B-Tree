#pragma once
#include <iostream>
#include "StringToIntConvereter.h"

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
        BTreeNode* Contains(const int key);
        void Save() const;
        void LoadNode();
    private:
        void AddNewKey(int newKey);
        void AddNewKeyToChild(int newKey);
    friend class BTree;
};
