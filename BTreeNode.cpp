#include "BTreeNode.h"

BTreeNode::BTreeNode(int order, bool isLeaf) {
    this->order = order;
    this->isLeaf = isLeaf;
    keysNum = 0;

    // alokacja pamieci na maksymalna ilosc wartosci i dzieci
    keys = new int[2 * order - 1];
    children = new BTreeNode*[2 * order];
}

BTreeNode::~BTreeNode() {
    if(!isLeaf)
        for(int i = 0; i < keysNum + 1; i++)
            delete children[i];
    delete[] children;
    delete[] keys;
}

void BTreeNode::SplitChild(int childIndex, BTreeNode* childToBeSplitted) {
    BTreeNode* newChild = new BTreeNode(childToBeSplitted->order, childToBeSplitted->isLeaf);
    newChild->keysNum = order - 1;
    childToBeSplitted->keysNum = order - 1;

    for(int i = 0; i < order - 1; i++)
        newChild->keys[i] = childToBeSplitted->keys[i + order];
    if(!newChild->isLeaf)
        for(int i = 0; i < order; i++)
            newChild->children[i] = childToBeSplitted->children[i + order];

    // zrob miejsce i wstaw nowe dziecko do obecnego wezla
    for(int i = keysNum; i > childIndex; i--)
        children[i+1] = children[i];
    children[childIndex + 1] = newChild;

    // zrob miejsce i wstaw nowy klucz do obecnego wezla
    for(int i = keysNum - 1; i >= childIndex; i--)
        keys[i+1] = keys[i];
    keys[childIndex] = childToBeSplitted->keys[order - 1];

    keysNum++;
}

void BTreeNode::InsertToThisNotFullNode(int newKey) {
    if(isLeaf) {
        AddNewKey(newKey);
    } else {
        AddNewKeyToChild(newKey);
    }  
}

void BTreeNode::AddNewKey(int newKey) {
    int i = keysNum - 1;

    while(i >= 0 && keys[i] > newKey) {
        keys[i+1] = keys[i];
        i--;
    }

    keys[i+1] = newKey;
    keysNum++;
}

void BTreeNode::AddNewKeyToChild(int newKey) {
    int i = keysNum - 1;

    while(i >= 0 && keys[i] > newKey)
        i--;

    int indexOfChildThatGetNewKey = i + 1;

    if(children[i+1]->keysNum == (2 * order - 1)) {
        SplitChild(i+1, children[i+1]);
        if(keys[i+1] < newKey)
            indexOfChildThatGetNewKey++;
    }

    children[indexOfChildThatGetNewKey]->InsertToThisNotFullNode(newKey);
}

void BTreeNode::Print() const {
    int i;

    for(i = 0; i < keysNum; i++) {
        if(!isLeaf)
            children[i]->Print();
        std::cout << keys[i] << " ";
    }

    if(!isLeaf)
        children[i]->Print();
}

bool BTreeNode::Contains(const int key) const {
    int i = 0;

    while(i < keysNum && keys[i] < key)
        i++;
    
    if(keys[i] == key)
        return true;
    else if(!isLeaf)
        return children[i]->Contains(key);
    
    return false;
}

void BTreeNode::Save() const {
    int i;
    std::cout << "( ";
    for(i = 0; i < keysNum; i++) {
        if(!isLeaf)
            children[i]->Save();
        std::cout << keys[i] << " ";
    }

    if(!isLeaf)
        children[i]->Save();

    std::cout << ") ";
}

void BTreeNode::LoadNode() {
    // Klasa odpowiadajaca za wczytywaniu znaku za znakiem i przekonwertowaniu
    // otrzymanego lancuchu na liczbe
    StringToIntConverter stringInt;
    bool previousCharWasNumber = false;

    char input = getchar();

    while(input != '\n' && input != ')') {
        if(input == '(') {
            isLeaf = false;
            BTreeNode* newNode = new BTreeNode(order, true);
            newNode->LoadNode();
            children[keysNum] = newNode;
        } else if((input >= '0' && input <= '9') || input == '-') {
            stringInt.AddInt(input);
            previousCharWasNumber = true;
        } else if(input == ' ' && previousCharWasNumber) {
            keys[keysNum++] = stringInt.GetInt();
            previousCharWasNumber = false;
        }
        input = getchar();
    }
}