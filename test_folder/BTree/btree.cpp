#include <iostream>
#include "BTree/tree.h"
using namespace ds::btree;
#define DEG 4
int main(){
    BTree<int> *tree = new BTree<int>(2);
    for (int i = 1 ; i < 11 ; i++){
        tree->insert(i);
    }
    tree->remove(6);
    tree->remove(1);
    tree->remove(3);
    tree->remove(2);
    tree->remove(2);
    tree->remove(2);
    tree->remove(8);
    std::cout << tree->to_string() << std::endl;
    delete tree;
}