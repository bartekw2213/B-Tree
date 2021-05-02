#include "BTree.h"

BTree::~BTree() {
    delete root;
}

void BTree::Add(int newKey) {
    if(root == nullptr)
        Add_FirstElement(newKey);
    else if(root->keysNum == (2 * order - 1))
        Add_ToFullRoot(newKey);
    else
        root->InsertToThisNotFullNode(newKey);
}

void BTree::Add_FirstElement(const int newKey) {
    root = new BTreeNode(order, true);
    root->keys[0] = newKey;
    root->keysNum += 1;
}

void BTree::Add_ToFullRoot(const int newKey) {
    BTreeNode* newRoot = new BTreeNode(order, false);
    newRoot->children[0] = root;
    newRoot->SplitChild(0, root);

    int indexOfChildThatWillHaveNewKey = (newRoot->keys[0] < newKey) ? 1 : 0;
    newRoot->children[indexOfChildThatWillHaveNewKey]->InsertToThisNotFullNode(newKey);
    
    root = newRoot;
}

void BTree::Print() const {
    root->Print();
    std::cout << "\n";
}

void BTree::Search(const int key) const {
    char sign = '+';
    if(root->Contains(key) == nullptr)
        sign = '-';
    std::cout << key << ' ' << sign << '\n';
}

void BTree::Load() {
    // zabranie z wejscia '\n' i znaku nawiasu rozpoczynajacego drzewo
    getchar();
    getchar();
    root = new BTreeNode(order, true);
    root->LoadNode();
}

void BTree::Save() const {
    std::cout << order << '\n';
    root->Save();
    std::cout << '\n';
}