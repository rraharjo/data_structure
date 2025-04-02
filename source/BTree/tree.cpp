#include "header/BTree/tree.h"
namespace ds
{
    namespace btree
    {
        template <typename T>
        BTree<T>::BTree(size_t degree) : degree(degree), size(0), depth(0)
        {
            this->root = new BNode<T>(this->degree);
        }
        template <typename T>
        BTree<T>::~BTree()
        {
            delete this->root;
        }

        template <typename T>
        std::optional<std::pair<T, size_t>> BTree<T>::search_node(BNode<T> *node, T obj)
        {
            int cur_idx = 0;
            // c0 k0 c1 k1 c2 k2 c3 k3 c4
            while (cur_idx < node->keys.size() && obj > node->children[cur_idx]){
                cur_idx++;
            }
            if (cur_idx < node->keys.size() && obj == node->children[cur_idx]){
                return std::make_pair(node->keys[cur_idx], cur_idx);
            }
            else if (node->depth == this->depth){
                return std::nullopt;
            }
            else{
                return search_node(node->children[cur_idx], obj);
            }
        }

        template <typename T>
        std::optional<std::pair<T, size_t>> BTree<T>::search(T obj)
        {
            return this->search_node(this->root, obj);
        }
    }
}