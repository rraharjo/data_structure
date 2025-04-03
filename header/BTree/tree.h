#include "BTree/node.h"
#include <queue>
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

            void insert(T obj);

            void remove(T obj);

            BTree(int degree = 2);

            ~BTree();

            std::string to_string();
        };
    }
}

namespace ds
{
    namespace btree
    {
        template <typename T>
        BTree<T>::BTree(int degree): degree(degree), depth(1){
            this->root = new BNode<T>(this->degree);
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
                    if (node->is_leaf()){
                        return std::nullopt;
                    }
                    return this->search_node((BTree<T> *)this->children[i + 1], obj);
                }
            }
            if (node->is_leaf()){
                return std::nullopt;
            }
            return this->search_node((BTree<T> *)this->children[0], obj);
        }

        template <typename T>
        std::optional<std::pair<T, int>> BTree<T>::search(T obj){
            return this->search_node(this->root, obj);
        }

        template <typename T>
        void BTree<T>::insert(T obj){
            if (this->root->full()){
                BNode<T> *new_node = new BNode<T>(this->degree);
                BNode<T> *root = this->root;
                this->root = new_node;
                this->root->children[0] = root;
                this->root->split_child(0);
                this->root->insert(obj);
            }
            else{
                this->root->insert(obj);
            }
        }

        template <typename T>
        void BTree<T>::remove(T obj){
            this->root->remove(obj);
        }

        template <typename T>
        std::string BTree<T>::to_string(){
            std::queue<BNode<T> *> nexts;
            std::string to_ret = "";
            if (this->root != nullptr){
                nexts.push(this->root);
            }
            while (!nexts.empty()){
                BNode<T> *cur = nexts.front();
                nexts.pop();
                for (BNode<T> *child : cur->children){
                    if (child != nullptr){
                        nexts.push(child);
                    }
                }
                to_ret += cur->to_string() + "\n";
            }
            return to_ret;
        }
    }
}