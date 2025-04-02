#include <iostream>
#include "BTree/tree.h"
using namespace ds::btree;
#define DEG 4
int main(){
    BTree<int> *tree = new BTree<int>(4);
    for (int i = 1 ; i < 20 ; i++){
        tree->insert(i);
    }
    std::cout << tree->to_string() << std::endl;
    delete tree;
}