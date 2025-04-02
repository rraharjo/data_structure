#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <limits>
namespace ds
{
    namespace btree
    {
        /*General structure of a BNode
        c0 | k0 | c1 | k1 | c2 | k2 | ... | cn | kn | c(n+1)
        */
        template <typename T>
        class BNode
        {
        public:
            std::vector<T> keys;
            std::vector<BNode<T> *> children;
            int degree, size;
            bool leaf;

            void insert_this(T obj);

        public:
            /*Minimum degree is 2. Each node must have at least (degree - 1) keys,
            but no more than (2*degree - 1) keys. Number of children cannot exceed
            twice the size of degree*/
            BNode(int degree);

            ~BNode();

            int get_degree();

            int capacity();

            int get_size();

            bool is_leaf();

            bool full();

            /*Split the child at index idx with the assumption that this node is not full*/
            void split_child(int idx);

            void insert(T obj);

            std::string to_string();
        };
    }
}

template <typename T>
void shift_vector(std::vector<T> &v, size_t idx)
{
    for (int i = v.size() - 1; i > idx; i--)
    {
        v[i] = v[i - 1];
    }
}
namespace ds
{
    namespace btree
    {
        template <typename T>
        BNode<T>::BNode(int degree) : degree(degree), leaf(false), size(0)
        {
            if (degree < 2)
            {
                throw std::invalid_argument("Degree size cannot be lower than 2");
            }
            if (degree > std::numeric_limits<int>::max() / 2)
            {
                throw std::invalid_argument("This clas only support a maximum degree of " +
                                            std::to_string(std::numeric_limits<int>::max() / 2));
            }
            this->children.reserve(2 * this->degree);
            this->children.assign(2 * this->degree, nullptr);
            this->keys.reserve(2 * this->degree - 1);
            this->keys.assign(2 * this->degree - 1, T());
        }

        template <typename T>
        int BNode<T>::capacity()
        {
            return this->degree * 2 - 1;
        }

        template <typename T>
        int BNode<T>::get_size()
        {
            return this->size;
        }

        template <typename T>
        int BNode<T>::get_degree()
        {
            return this->degree;
        }

        template <typename T>
        bool BNode<T>::is_leaf()
        {
            return this->leaf;
        }

        template <typename T>
        bool BNode<T>::full()
        {
            return this->get_size() == this->capacity();
        }

        template <typename T>
        void BNode<T>::split_child(int idx)
        {
            // if (this->full())
            // {
            //     throw std::invalid_argument("This node is full");
            // }
            if (this->children[idx] == nullptr)
            {
                std::string err_msg = "children at index " + std::to_string(idx) + " is null";
                throw std::invalid_argument(err_msg);
            }
            // if (((BNode<T> *)this->children[idx])->full())
            // {
            //     std::string err_msg = "Children at index " + std::to_string(idx) + " is not full";
            //     throw std::invalid_argument(err_msg);
            // }
            BNode<T> *new_node = new BNode<T>(this->degree);
            BNode<T> *to_split = this->children[idx];
            new_node->leaf = to_split->leaf;
            for (int i = this->degree; i < 2 * this->degree - 1; i++)
            {
                new_node->keys[i - this->degree] = to_split->keys[i];
                to_split->keys[i] = T();
            }
            
            for (int i = this->degree; i < 2 * this->degree; i++)
            {
                new_node->children[i - this->degree] = ((BNode<T> *)to_split->children[i]);
                to_split->children[i] = nullptr;
            }
            to_split->size = this->degree - 1;
            new_node->size = this->degree - 1;
            shift_vector(this->keys, idx);
            shift_vector(this->children, idx + 1);
            this->keys[idx] = to_split->keys[degree - 1];
            to_split->keys[this->degree - 1] = T();
            this->children[idx + 1] = new_node;
            this->size++;
        }

        template <typename T>
        void BNode<T>::insert_this(T obj){
            if (this->full()){
                throw std::invalid_argument("Can't insert to this node: full");
            }
            int idx = this->get_size() - 1;
            while (this->keys[idx] > obj && idx >= 0){
                this->keys[idx + 1] = this->keys[idx];
                idx--;
            }
            idx++;
            this->keys[idx] = obj;
            this->size++;
        }

        template <typename T>
        void BNode<T>::insert(T obj)
        {
            if (this->is_leaf())
            {
                this->insert_this(obj);
            }
            else
            {
                int last_idx = this->get_size() - 1;
                while (obj < this->keys[last_idx] && last_idx >= 0)
                {
                    last_idx--;
                }
                last_idx++;
                if (((BNode<T> *)this->children[last_idx])->full())
                {
                    this->split_child(last_idx);
                    if (obj > this->keys[last_idx]){
                        last_idx++;
                    }
                }
                ((BNode<T> *)this->children[last_idx])->insert(obj);
            }
        }

        template <typename T>
        BNode<T>::~BNode()
        {
            for (BNode *child : this->children)
            {
                delete child;
            }
        }

        template <typename T>
        std::string BNode<T>::to_string()
        {
            std::string to_ret = "Degree: " + std::to_string(this->degree) + "\n";
            to_ret += "Size: " + std::to_string(this->get_size()) + "\n";
            to_ret += "Capacity: " + std::to_string(this->capacity()) + "\n";
            to_ret += "Keys: ";
            for (T t : this->keys)
            {
                to_ret += std::to_string(t) + " ";
            }
            to_ret += "\n";
            // for (BNode<T> *c : this->children)
            // {
            //     to_ret += std::to_string((int)c);
            // }
            // to_ret += "\n";
            return to_ret;
        }
    }
}
