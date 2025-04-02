#pragma once
#include <vector>
#include <stdexcept>
#include <string>
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
        private:
            std::vector<T> keys;
            std::vector<BNode *> children;
            size_t degree;
            bool is_leaf;

        public:
            /*Minimum degree is 2. Each node must have at least (degree - 1) keys,
            but no more than (2*degree - 1) keys. Number of children cannot exceed
            twice the size of degree*/
            BNode(size_t degree);

            ~BNode();

            size_t get_degree();

            size_t children_size();

            size_t capacity();

            size_t size();

            bool is_leaf();

            bool full();

            /*Split the child at index idx with the assumption that this node is not full*/
            void split_child(size_t idx);
        };
    }
}
