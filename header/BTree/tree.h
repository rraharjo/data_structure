#include "header/BTree/node.h"
#include <optional>

namespace ds{
    namespace btree{
        template <typename T>
        class BTree{
            private:
                size_t degree;
                size_t size;
                size_t depth;
                BNode<T> *root;

                std::optional<std::pair<T, size_t>> search_node(BNode<T>* node, T obj);
            public:
                std::optional<std::pair<T, size_t>> search(T obj);

                BTree(size_t degree = 2);

                ~BTree();
        };
    }
}