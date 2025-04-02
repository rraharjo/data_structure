#include "header/BTree/node.h"
template <typename T>
void shift_vector(std::vector<T>& v, size_t idx){
    for (int i = v.size() - 1 ; i > idx ; i--){
        v[i] = v [i - 1];
    }
}
namespace ds
{
    namespace btree
    {
        template <typename T>
        BNode<T>::BNode(size_t degree) : degree(degree), is_leaf(false)
        {
            if (degree < 2)
            {
                throw std::invalid_argument("Degree size cannot be lower than 2");
            }
            if (degree > std::numeric_limits<size_t>::max() / 2)
            {
                throw std::invalid_argument("This clas only support a maximum degree of " +
                                            std::to_string(std::numeric_limits<size_t>::max() / 2));
            }
            this->keys.reserve(2 * degree - 1);
            this->children.reserve(2 * degree);
            this->keys.assign(2 * degree - 1, T());
            this->children.assign(2 * degree, nullptr);
        }

        template <typename T>
        size_t BNode<T>::children_size()
        {
            return this->children.capacity();
        }

        template <typename T>
        size_t BNode<T>::capacity()
        {
            return this->keys.capacity();
        }

        template <typename T>
        size_t BNode<T>::size(){
            return this->keys.size();
        }

        template <typename T>
        size_t BNode<T>::get_degree()
        {
            return this->degree;
        }

        template <typename T>
        bool BNode<T>::is_leaf()
        {
            return this->is_leaf;
        }

        template <typename T>
        bool BNode<T>::full()
        {
            return this->size() == this->capacity();
        }

        template <typename T>
        void BNode<T>::split_child(size_t idx)
        {
            if (this->full())
            {
                throw std::invalid_argument("This node is full");
            }
            if (this->children[idx] == nullptr)
            {
                throw std::invalid_argument("children at index " + idx + " is null");
            }
            if (((BNode<T> *)this->children[idx])->full())
            {
                throw std::invalid_argument("Children at index " + idx + " is not full");
            }
            BNode<T> *new_node = new BNode<T>(this->degree);
            BNode<T> *to_split = this->children[idx];
            to_split->is_leaf = this->is_leaf;
            // 0 ... 2t - 2
            // max index is 2t - 2
            // move up is at index t - 1
            // index [0, t - 2] stay t - 1
            // index [t, 2t - 2] goes to right 2t - 2 - t + 1 = t - 1
            for (int i = this->degree; i < 2 * this->degree - 1; i++)
            {
                new_node->children.push_back((BNode<T> *)to_split->children[i]);
                to_split->children[i] = nullptr;
            }
            shift_vector(this->children, this->degree);
            shift_vector(this->keys, this->degree);
        }

        template <typename T>
        BNode<T>::~BNode()
        {
            for (BNode &*children : this->children)
            {
                delete children;
            }
        }
    }
}