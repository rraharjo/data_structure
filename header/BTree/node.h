#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <limits>
#include <optional>
/*Merge child number (idx + 1) to child number idx*/
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
            std::vector<std::optional<T>> keys;
            std::vector<BNode<T> *> children;
            int degree, size;
            bool leaf;

            void insert_this(T obj);

            void remove_1(T obj, int i);

            void remove_2(T obj, int i);

            void remove_3(T obj);

            void merge(int idx);

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

            void remove(T obj);

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
            this->keys.assign(2 * this->degree - 1, std::nullopt);
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
            if (this->children[idx] == nullptr)
            {
                std::string err_msg = "children at index " + std::to_string(idx) + " is null";
                throw std::invalid_argument(err_msg);
            }
            BNode<T> *new_node = new BNode<T>(this->degree);
            BNode<T> *to_split = this->children[idx];
            new_node->leaf = to_split->leaf;
            for (int i = this->degree; i < 2 * this->degree - 1; i++)
            {
                new_node->keys[i - this->degree] = to_split->keys[i];
                to_split->keys[i] = std::nullopt;
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
            to_split->keys[this->degree - 1] = std::nullopt;
            this->children[idx + 1] = new_node;
            this->size++;
        }

        template <typename T>
        void BNode<T>::insert_this(T obj)
        {
            if (this->full())
            {
                throw std::invalid_argument("Can't insert to this node: full");
            }
            int idx = this->get_size() - 1;
            while (this->keys[idx] > obj && idx >= 0)
            {
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
                    if (obj > this->keys[last_idx])
                    {
                        last_idx++;
                    }
                }
                ((BNode<T> *)this->children[last_idx])->insert(obj);
            }
        }

        template <typename T>
        void BNode<T>::remove_1(T obj, int i)
        {
            while (i < this->get_size() - 1)
            {
                this->keys[i] = this->keys[i + 1];
                i++;
            }
            this->keys[this->get_size() - 1] = std::nullopt;
            this->size--;
        }

        template <typename T>
        void BNode<T>::remove_2(T obj, int i)
        {
            BNode<T> *pre = this->children[i], *suc = this->children[i + 1];
            if (pre->get_size() >= this->degree)
            {
                std::optional<T> obj_p = pre->keys[pre->get_size() - 1];
                this->keys[i] = obj_p;
                pre->remove(obj_p.value());
            }
            else if (suc->get_size() >= this->degree)
            {
                std::optional<T> obj_p = suc->keys[0];
                this->keys[i] = obj_p;
                suc->remove(obj_p.value());
            }
            else
            {
                int pre_size = pre->get_size();
                this->merge(i);
                pre->remove(obj);
            }
        }

        template <typename T>
        void BNode<T>::remove_3(T obj)
        {
            if (this->is_leaf()){
                return;
            }
            int i = 0;
            while (i < this->get_size() && obj > this->keys[i])
            {
                i++;
            }
            BNode<T> *c_i = (BNode<T> *)this->children[i];
            if (c_i->get_size() < this->degree)
            {
                if (i > 0 && ((BNode<T> *)this->children[i - 1])->get_size() >= this->degree)
                {
                    for (int j = c_i->get_size() - 1; j >= 0; j--)
                    {
                        c_i->keys[j + 1] = c_i->keys[j];
                    }
                    c_i->keys[0] = this->keys[i - 1];
                    c_i->size++;
                    BNode<T> *c_i_min_one = (BNode<T> *)this->children[i - 1];
                    this->keys[i - 1] = c_i_min_one->keys[c_i_min_one->get_size() - 1];
                    c_i_min_one->keys[c_i_min_one->get_size() - 1] = std::nullopt;
                    c_i_min_one->size--;
                    c_i->remove(obj);
                }
                else if (i < this->get_size() && ((BNode<T> *)this->children[i + 1])->get_size() >= this->degree)
                {
                    BNode<T> *c_i_plus_one = (BNode<T> *)this->children[i + 1];
                    c_i->keys[c_i->get_size()] = this->keys[i];
                    c_i->size++;
                    this->keys[i] = c_i_plus_one->keys[0];
                    for (int j = 0; j < c_i_plus_one->get_size() - 1; j++)
                    {
                        c_i_plus_one->keys[j] = c_i_plus_one->keys[j + 1];
                    }
                    c_i_plus_one->keys[c_i_plus_one->get_size() - 1] = std::nullopt;
                    c_i_plus_one->size--;
                    c_i->remove(obj);
                }
                else if (i > 0)
                { // merge c_i to c_{i-1}
                    this->merge(i - 1);
                    ((BNode<T> *)this->children[i - 1])->remove(obj);
                }
                else
                { // i == 0 merge c_{i + 1} to c_i
                    this->merge(i);
                    c_i->remove(obj);
                }
            }
            else
            {
                c_i->remove(obj);
            }
        }

        template <typename T>
        void BNode<T>::remove(T obj)
        {
            int idx = -1;
            for (int i = 0; i < this->get_size(); i++)
            {
                if (this->keys[i] == obj)
                {
                    idx = i;
                    break;
                }
            }
            if (idx >= 0 && this->is_leaf())
            {
                this->remove_1(obj, idx);
            }
            else if (idx >= 0)
            {
                this->remove_2(obj, idx);
            }
            else
            {
                this->remove_3(obj);
            }
        }

        template <typename T>
        void BNode<T>::merge(int i)
        {
            BNode<T> *c_i_plus_one = (BNode<T> *)this->children[i + 1];
            BNode<T> *c_i = (BNode<T> *)this->children[i];
            int to_add_idx = c_i->get_size();
            c_i->keys[to_add_idx++] = this->keys[i];
            for (int j = 0; j < c_i_plus_one->get_size(); j++)
            {
                c_i->keys[to_add_idx++] = c_i_plus_one->keys[j];
            }
            to_add_idx = c_i->get_size() + 1;
            for (int j = 0; j <= c_i_plus_one->get_size(); j++)
            {
                c_i->children[to_add_idx++] = c_i_plus_one->children[j];
                c_i_plus_one->children[j] = nullptr;
            }
            c_i->size += 1 + c_i_plus_one->get_size();

            for (int j = i; j < this->get_size() - 1; j++)
            {
                this->keys[j] = this->keys[j + 1];
            }
            for (int j = i + 1; j < this->get_size(); j++)
            {
                this->children[j] = this->children[j + 1];
            }
            this->keys[this->get_size() - 1] = std::nullopt;
            this->children[this->get_size()] = nullptr;
            this->size--;
            delete c_i_plus_one;
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
            to_ret += "Leaf node: " + std::to_string(this->is_leaf()) + "\n";
            to_ret += "Keys: ";
            for (std::optional<T> t : this->keys)
            {
                to_ret += t.has_value() ? std::to_string(t.value()) + " " : " ";
            }
            to_ret += "\n";
            return to_ret;
        }
    }
}