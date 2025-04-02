// #include "BTree/node.h"
// /*Shifting right all member with index >= idx 1 position*/
// template <typename T>
// void shift_vector(std::vector<T> &v, size_t idx)
// {
//     for (int i = v.size() - 1; i > idx; i--)
//     {
//         v[i] = v[i - 1];
//     }
// }
// namespace ds
// {
//     namespace btree
//     {
//         template <typename T>
//         BNode<T>::BNode(size_t degree) : degree(degree), leaf(false)
//         {
//             if (degree < 2)
//             {
//                 throw std::invalid_argument("Degree size cannot be lower than 2");
//             }
//             if (degree > std::numeric_limits<size_t>::max() / 2)
//             {
//                 throw std::invalid_argument("This clas only support a maximum degree of " +
//                                             std::to_string(std::numeric_limits<size_t>::max() / 2));
//             }
//             this->keys.reserve(2 * degree - 1);
//             this->children.reserve(2 * degree);
//             this->keys.assign(2 * degree - 1, T());
//             this->children.assign(2 * degree, nullptr);
//         }

//         template <typename T>
//         size_t BNode<T>::children_size()
//         {
//             return this->children.capacity();
//         }

//         template <typename T>
//         size_t BNode<T>::capacity()
//         {
//             return this->keys.capacity();
//         }

//         template <typename T>
//         size_t BNode<T>::size()
//         {
//             return this->keys.size();
//         }

//         template <typename T>
//         size_t BNode<T>::get_degree()
//         {
//             return this->degree;
//         }

//         template <typename T>
//         bool BNode<T>::is_leaf()
//         {
//             return this->is_leaf;
//         }

//         template <typename T>
//         bool BNode<T>::full()
//         {
//             return this->size() == this->capacity();
//         }

//         template <typename T>
//         void BNode<T>::split_child(size_t idx)
//         {
//             if (this->full())
//             {
//                 throw std::invalid_argument("This node is full");
//             }
//             if (this->children[idx] == nullptr)
//             {
//                 std::string err_msg = "children at index " + std::to_string(idx) + " is null";
//                 throw std::invalid_argument(err_msg);
//             }
//             if (((BNode<T> *)this->children[idx])->full())
//             {
//                 std::string err_msg = "Children at index " + std::to_string(idx) + " is not full";
//                 throw std::invalid_argument(err_msg);
//             }
//             BNode<T> *new_node = new BNode<T>(this->degree);
//             BNode<T> *to_split = this->children[idx];
//             to_split->is_leaf = this->is_leaf;
//             for (int i = this->degree; i < 2 * this->degree - 1; i++)
//             {
//                 new_node->keys.push_back(to_split->keys[i]);
//                 to_split->keys.erase(to_split->keys.begin() + i);
//             }
//             /*t = 4
//             i = 0 1 2 3 4 5 6 7
//             K = a b c d e f g
//             C = 1 2 3 4 5 6 7 8*/
//             for (int i = this->degree; i < 2 * this->degree; i++)
//             {
//                 new_node->children.push_back((BNode<T> *)to_split->children[i]);
//                 to_split->children.erase(to_split->children.begin() + i);
//             }
//             shift_vector(this->keys, idx);
//             shift_vector(this->children, idx + 1);
//             this->keys[idx] = to_split->keys[degree - 1];
//             this->children[idx + 1] = new_node;
//         }

//         template <typename T>
//         BNode<T>::~BNode()
//         {
//             for (BNode *child : this->children)
//             {
//                 delete child;
//             }
//         }

//         template <typename T>
//         std::string BNode<T>::to_string()
//         {
//             std::string to_ret = "Degree: " + std::to_string(this->degree) + "\n";
//             to_ret += "Size: " + std::to_string(this->size) + "\n";
//             to_ret += "Capacity: " + std::to_string(this->capacity) + "\n";
//             to_ret += "Keys: ";
//             for (T t : this->keys)
//             {
//                 to_ret += std::to_string(t) + " ";
//             }
//             to_ret += "\n";
//             for (BNode<T> *c : this->children)
//             {
//                 to_ret += std::to_string(c);
//             }
//             to_ret += "\n";
//             return to_ret;
//         }
//     }
// }