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
        void Remove(const int key);
        void RemoveFromLeaf(const int removedKeyIndex);
        void RemoveFromInternalNode(const int removedKeyIndex);
        void MergeIntoSingleNode(int firstNodeIndex, int secondNodeIndex);
    private:
        void AddNewKey(int newKey);
        void AddNewKeyToChild(int newKey);
        void MakeChildNoMinKeysNode(int childIndex);
        int FindKeyIndex(int key);
        int FindChildIndexThatCanHaveKey(int key);
        int FindChildIndex(BTreeNode* child);
        int GetPredecessor(const int keyIndex);
        int GetSuccessor(const int keyIndex);
        void BorrowLargestKeyFromSibling(BTreeNode* child, BTreeNode* sibling);
        void BorrowSmallestKeyFromSibling(BTreeNode* child, BTreeNode* sibling);
    friend class BTree;
};
