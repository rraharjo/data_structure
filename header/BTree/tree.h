#include "BTree/node.h"
#include <optional>

namespace ds
{
    namespace btree
    {
        template <typename T>
        class BTree
        {
        private:
            int degree;
            int depth;
            BNode<T> *root;

            std::optional<std::pair<T, int>> search_node(BNode<T> *node, T obj);

        public:
            std::optional<std::pair<T, int>> search(T obj);

            BTree(int degree = 2);

            ~BTree();
        };
    }
}

namespace ds
{
    namespace btree
    {
        template <typename T>
        BTree<T>::BTree(int degree): degree(degree), depth(1){
            this->root = new BNode<T>();
            this->root->leaf = true;
        }

        template <typename T>
        BTree<T>::~BTree(){
            delete this->root;
        }
        
        template <typename T>
        std::optional<std::pair<T, int>> BTree<T>::search_node(BNode<T> *node, T obj)
        {
            for (int i = node->get_size() - 1; i >= 0; i--)
            {
                if (node->keys[i] == obj)
                {
                    return std::make_pair<T, int>(obj, i);
                }
                if (obj > node->keys[i])
                {
                    return this->search_node((BTree<T> *)this->children[i + 1], obj);
                }
            }
            return this->search_node((BTree<T> *)this->children[0], obj)
        }

        template <typename T>
        std::optional<std::pair<T, int>> BTree<T>::search(T obj){
            return this->search_node(this->root, obj);
        }
    }
}