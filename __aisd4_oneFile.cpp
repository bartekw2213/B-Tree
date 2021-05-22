#include <iostream>

class StringToIntConverter {
    private:
        char* string;
        int capacity;
        int length;
    public:
        StringToIntConverter() : string(new char[20]), capacity(20), length(0) { };
        ~StringToIntConverter() { delete[] string; };
        void AddInt(const char newInt);
        int GetInt(); 
    private:
        void Resize();
};

void StringToIntConverter::AddInt(const char newInt) {
    if(length == capacity)
        Resize();

    string[length++] = newInt;
    string[length] = '\0';
}

int StringToIntConverter::GetInt() {
    length = 0;
    return atoi(string);
}

void StringToIntConverter::Resize() {
    char* temp = new char[capacity*2];
    for(int i = 0; i <= length; i++)
        temp[i] = string[i];
    delete[] string;
    string = temp;
    capacity *= 2;
}

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

BTreeNode::BTreeNode(int order, bool isLeaf) {
    this->order = order;
    this->isLeaf = isLeaf;
    keysNum = 0;

    // alokacja pamieci na maksymalna ilosc wartosci i dzieci
    keys = new int[2 * order - 1];
    children = new BTreeNode*[2 * order];
}

BTreeNode::~BTreeNode() {
    if(!isLeaf && keysNum > 0)
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

BTreeNode* BTreeNode::Contains(const int key) {
    int i = 0;

    while(i < keysNum && keys[i] < key)
        i++;
    
    if(keys[i] == key && i < keysNum)
        return this;
    else if(!isLeaf)
        return children[i]->Contains(key);
    
    return nullptr;
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

void BTreeNode::Remove(const int keyToDelete) {
    int removedKeyIndex = FindKeyIndex(keyToDelete);

    if(removedKeyIndex != -1) {
        if(isLeaf)
            RemoveFromLeaf(removedKeyIndex);
        else
            RemoveFromInternalNode(removedKeyIndex);
        return;
    } else if(!isLeaf) {
        int childIndexThatCanHaveKey = FindChildIndexThatCanHaveKey(keyToDelete);
        bool isThisChildLast = (childIndexThatCanHaveKey == keysNum) ? true : false;

        if(children[childIndexThatCanHaveKey]->keysNum == order - 1)
            MakeChildNoMinKeysNode(childIndexThatCanHaveKey);
  
        // jesli ostatnie dziecko zostalo polaczone z poprzednikiem
        if(isThisChildLast && childIndexThatCanHaveKey > keysNum)
            children[childIndexThatCanHaveKey - 1]->Remove(keyToDelete);
        else
            children[childIndexThatCanHaveKey]->Remove(keyToDelete);
    }
}

void BTreeNode::RemoveFromLeaf(const int removedKeyIndex) {
    for(int i = removedKeyIndex; i < keysNum - 1; i++)
        keys[i] = keys[i+1];
    keysNum--;
}

void BTreeNode::RemoveFromInternalNode(const int removedKeyIndex) {
    BTreeNode* leftChild = children[removedKeyIndex];
    BTreeNode* rightChild = children[removedKeyIndex + 1];

    if(leftChild->keysNum > order - 1) {
        int predecessor = GetPredecessor(removedKeyIndex);
        keys[removedKeyIndex] = predecessor;
        leftChild->Remove(predecessor);
    } else if(rightChild->keysNum > order - 1) {
        int successor = GetSuccessor(removedKeyIndex);
        keys[removedKeyIndex] = successor;
        rightChild->Remove(successor);
    } else {
        int keyToRemove = keys[removedKeyIndex];
        MergeIntoSingleNode(removedKeyIndex, removedKeyIndex + 1);
        children[removedKeyIndex]->Remove(keyToRemove);
    }
}

int BTreeNode::GetPredecessor(const int keyIndex) {
    BTreeNode* current = children[keyIndex];
    while(!current->isLeaf)
        current = current->children[current->keysNum];
    return current->keys[keysNum - 1];
}

int BTreeNode::GetSuccessor(const int keyIndex) {
    BTreeNode* current = children[keyIndex+1];
    while(!current->isLeaf)
        current = current->children[0];
    return current->keys[0];
}

void BTreeNode::MakeChildNoMinKeysNode(int childIndex) {
    if(childIndex != 0 && children[childIndex-1]->keysNum > order - 1)
        BorrowLargestKeyFromSibling(children[childIndex], children[childIndex - 1]);
    else if(childIndex != keysNum && children[childIndex + 1]->keysNum > order - 1)
        BorrowSmallestKeyFromSibling(children[childIndex], children[childIndex + 1]);
    else if(childIndex == keysNum) 
        MergeIntoSingleNode(childIndex - 1, childIndex);
    else 
        MergeIntoSingleNode(childIndex, childIndex+1);
}

void BTreeNode::BorrowLargestKeyFromSibling(BTreeNode* childThatGetKey, BTreeNode* childThatGiveKey) {
    int childThatGetKeyIndex = FindChildIndex(childThatGetKey);
    
    for(int i = childThatGetKey->keysNum; i > 0; i--)
        childThatGetKey->keys[i] = childThatGetKey->keys[i-1];
    
    childThatGetKey->keys[0] = keys[childThatGetKeyIndex - 1];

    if(!childThatGetKey->isLeaf)
        for(int i = childThatGetKey->keysNum + 1; i > 0; i--)
            childThatGetKey->children[i] = childThatGetKey->children[i-1];
    
    if(!childThatGiveKey->isLeaf)
        childThatGetKey->children[0] = childThatGiveKey->children[childThatGiveKey->keysNum];

    keys[childThatGetKeyIndex - 1] = childThatGiveKey->keys[childThatGiveKey->keysNum - 1];

    childThatGiveKey->keysNum--;
    childThatGetKey->keysNum++; 
}

void BTreeNode::BorrowSmallestKeyFromSibling(BTreeNode* childThatGetKey, BTreeNode* childThatGiveKey) {
    int childThatGetKeyIndex = FindChildIndex(childThatGetKey);

    childThatGetKey->keys[childThatGetKey->keysNum] = keys[childThatGetKeyIndex];
    childThatGetKey->keysNum++;

    if(!childThatGetKey->isLeaf)
        childThatGetKey->children[keysNum] = childThatGiveKey->children[0];

    keys[childThatGetKeyIndex] = childThatGiveKey->keys[0];

    for(int i = 0; i < childThatGiveKey->keysNum - 1; i++)
        childThatGiveKey->keys[i] = childThatGiveKey->keys[i+1];

    if(!childThatGiveKey->isLeaf)
        for(int i = 0; i < childThatGiveKey->keysNum; i++)
            childThatGiveKey->children[i] = childThatGiveKey->children[i+1];

    childThatGiveKey->keysNum--;
}

void BTreeNode::MergeIntoSingleNode(int firstNodeIndex, int secondNodeIndex) {
    BTreeNode* firstNode = children[firstNodeIndex];
    BTreeNode* secondNode = children[secondNodeIndex];

    firstNode->keys[firstNode->keysNum++] = keys[firstNodeIndex];
    
    for(int i = 0; i < secondNode->keysNum; i++)
        firstNode->keys[firstNode->keysNum + i] = secondNode->keys[i];
    
    if(!firstNode->isLeaf)
        for(int i = 0; i <= secondNode->keysNum; i++)
        firstNode->children[firstNode->keysNum + i] = secondNode->children[i];
    
    // Przesuniecie kluczy i dzieci obecnego wezla
    for (int i = firstNodeIndex; i < keysNum - 1; i++)
        keys[i] = keys[i+1];
    for (int i = secondNodeIndex; i < keysNum; i++)
        children[i] = children[i+1];

    keysNum--;
    firstNode->keysNum += secondNode->keysNum;
    secondNode->keysNum = 0;

    delete secondNode;
}

int BTreeNode::FindKeyIndex(const int key) {
    int index = -1;
    for(int i = 0; i < keysNum; i++) {
        if(keys[i] > key)
            break;
        else if(keys[i] == key)
            index = i;
    }
    return index;
}

int BTreeNode::FindChildIndexThatCanHaveKey(const int key) {
    int index = 0;
    while (index < keysNum && keys[index] < key)
        ++index;
    return index;
}

int BTreeNode::FindChildIndex(BTreeNode* child) {
    int i = 0;
    int childLastKey = child->keys[child->keysNum - 1];
    while(keys[i] < childLastKey && i < keysNum)
        i++;
    return i;
}

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

void BTree::Remove(const int removedKey) {
    if(root == nullptr)
        return;

    root->Remove(removedKey);

    if(root->keysNum > 0) 
        return;

    BTreeNode* oldRootToRemove = root;

    if(root->isLeaf)
        root = nullptr;
    else
        root = root->children[0];

    delete oldRootToRemove;
}

int ScanValue() {
    int value;
    std::cin >> value;
    return value;
}

int main() {
    char a;
    int value;
    bool quit = false;
    BTree* tree = nullptr;
    
    while(!quit) {
        std::cin >> a;

        switch (a) {
        case 'I': tree = new BTree(ScanValue()); break;
        case 'A': tree->Add(ScanValue()); break;
        case '?': tree->Search(ScanValue()); break;
        case 'P': tree->Print(); break;
        case 'L': tree = new BTree(ScanValue()); tree->Load(); break;
        case 'S': tree->Save(); break;
        case 'R': tree->Remove(ScanValue()); break;
        case 'X': quit = true; break;
        }
    }

    delete tree;
    
    return 0;
}
