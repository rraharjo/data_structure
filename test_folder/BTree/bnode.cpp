#include "BTree/node.h"
#include <iostream>
#define DEG 4
using namespace ds::btree;
int main()
{
    BNode<int> *par = new BNode<int>((size_t) DEG);
    std::vector<int> keys = {1, 5, 34, 100, 245, 300};
    for (int i = 0 ; i < keys.size() ; i++)
    {
        par->keys[i] = keys[i];
    }
    std::cout << par->to_string() <<  std::endl;
    BNode<int> *chld = new BNode<int>((size_t)DEG);
    std::vector<int> c_keys = {35, 36, 37, 38, 39, 40, 41};
    for (int i = 0 ; i < c_keys.size() ; i++){
        chld->keys[i] = c_keys[i];
    }
    par->children[3] = chld;
    par->split_child(3);
    std::cout << par->to_string() << std::endl;
    std::cout << par->children[3]->to_string() << std::endl;
    std::cout << par->children[4]->to_string() << std::endl;
}