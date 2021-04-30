#include <iostream>
#include "BTree.h"

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
        case 'S': tree->Save(); break;
        case 'X': quit = true; break;
        }
    }

    delete tree;
    
    return 0;
}