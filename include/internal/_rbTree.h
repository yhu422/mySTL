//
// Created by Yile Hu on 10/1/24.
//

#ifndef MYSTL__RBTREE_H
#define MYSTL__RBTREE_H
#include <iostream>
namespace mystl {
    namespace internal {
        enum Color {
            BLACK,
            RED,
        };

        template<class T>
        struct _rbTreeNode {
            _rbTreeNode *parent;
            _rbTreeNode *left;
            _rbTreeNode *right;
            Color color;
            T value;

            _rbTreeNode<T> *_min_node() noexcept {
                auto tmp = this;
                while (tmp->left != nullptr) {
                    tmp = tmp->left;
                }
                return tmp;
            }

            _rbTreeNode<T> *_max_node() noexcept {
                auto tmp = this;
                while (tmp->right != nullptr) {
                    tmp = tmp->right;
                }
                return tmp;
            }

            _rbTreeNode<T> *_next_node() {
                auto tmp = this;
                if (tmp->right != nullptr) {
                    return tmp->right->_min_node();
                }
                while (tmp->parent != nullptr && tmp == tmp->parent->right) {
                    tmp = tmp->parent;
                }
                return tmp->parent;
            }

            _rbTreeNode<T> *_prev_node() {
                auto tmp = this;
                if (tmp->left != nullptr) {
                    return tmp->left->max_node();
                }
                while (tmp->parent != nullptr && tmp == tmp->parent->left) {
                    tmp = tmp->parent;
                }
                return tmp->parent;
            }
        };


        template<class T>
        class _rbTreeNodeIterator {

        public:
            _rbTreeNodeIterator(_rbTreeNode<T> *n) : node(n) {};

            T &operator*() {
                return node->value;
            }

            _rbTreeNodeIterator &operator++() {
                node = node->_next_node();
                return *this;
            }

            _rbTreeNodeIterator &operator--() {
                node = node->_prev_node();
                return *this;
            }

            bool operator==(const _rbTreeNodeIterator<T> it) {
                return node == it.node;
            }

        private:
            _rbTreeNode<T> *node;
        };

        template<class T>
        struct _rbTree {
            using Node = _rbTreeNode<T>;
            using iterator = _rbTreeNodeIterator<T>;


            _rbTreeNode<T> *root = nullptr;

            iterator begin() noexcept {
                if (root == nullptr) {
                    return iterator(nullptr);
                } else {
                    return iterator(root->_min_node());
                }
            }

            static iterator end() noexcept {
                return iterator(nullptr);
            }

            iterator find(T value) {
                Node *current = root;
                while (current) {
                    if (current->value > value) {
                        current = current->left;
                    } else if (current->value < value) {
                        current = current->right;
                    } else {
                        break;
                    }
                }
                return iterator(current);
            }

            const iterator find(T value) const {
                Node *current = root;
                while (current) {
                    if (current->value > value) {
                        current = current->left;
                    } else if (current->value < value) {
                        current = current->right;
                    } else {
                        break;
                    }
                }
                return iterator(current);
            }

            void rotate_left(Node *node) {
                Node *parent = node->parent;
                Node *right = node->right;
                if (node->parent != nullptr) {
                    if (node == node->parent->left) {
                        node->parent->left = right;
                    } else {
                        node->parent->right = right;
                    }
                }
                if (node->parent == nullptr) {
                    root = right;
                }
                node->right = right->left;
                if (node->right != nullptr)
                    node->right->parent = node;
                right->left = node;
                node->parent = right;
                right->parent = parent;
            }

            void rotate_right(Node *node) {
                Node *parent = node->parent;
                Node *left = node->left;
                if (node->parent != nullptr) {
                    if (node == node->parent->left) {
                        node->parent->left = left;
                    } else {
                        node->parent->right = left;
                    }
                }
                if (node->parent == nullptr) {
                    root = left;
                }
                node->left = left->right;
                node->left->parent = node;
                left->right = node;
                left->parent = parent;
                node->parent = left;
            }

            void fix_violation(Node *node) {
                Node *uncle;
                Node *parent = node->parent;
                if (parent == nullptr) {
                    node->color = BLACK;
                    return;
                }
                if ((node->color == RED && parent->color == BLACK) || (node->color == BLACK && parent->color == RED)) {
                    return;
                }
                Node *grandpa = node->parent->parent;
                if (grandpa == nullptr) {
                    node->color = BLACK;
                    return;
                }
                if (grandpa->left == parent) {
                    uncle = grandpa->right;
                } else {
                    uncle = grandpa->left;
                }
                if (uncle != nullptr && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandpa->color = RED;
                    fix_violation(grandpa);
                } else {
                    bool is_node_left = parent->left == node;
                    bool is_parent_left = grandpa->left == parent;
                    if (is_node_left && is_parent_left) {
                        LL:
                        rotate_right(grandpa);
                        std::swap(grandpa->color, parent->color);
                    } else if (!is_node_left && !is_parent_left) {
                        RR:
                        rotate_left(grandpa);
                        std::swap(grandpa->color, parent->color);
                    } else if (!is_node_left && is_parent_left) {
                        LR:
                        rotate_left(parent);
                        std::swap(grandpa->color, node->color);
                        goto LL;
                    } else if (is_node_left && !is_parent_left) {
                        RL:
                        rotate_right(parent);
                        std::swap(grandpa->color, node->color);
                        goto RR;
                    }
                }
            }

            std::pair<iterator, bool> insert(T value) {
                Node *node = new Node;
                node->value = value;
                node->left = nullptr;
                node->right = nullptr;
                node->parent = nullptr;
                node->color = RED;
                Node **p_rbTreeNode = &root;
                while (*p_rbTreeNode) {
                    node->parent = *p_rbTreeNode;
                    if ((*p_rbTreeNode)->value > value) {
                        p_rbTreeNode = &((*p_rbTreeNode)->left);
                    } else if ((*p_rbTreeNode)->value < value) {
                        p_rbTreeNode = &((*p_rbTreeNode)->right);
                    } else {
                        delete node;
                        return {iterator(*p_rbTreeNode), false};
                    }
                }
                *p_rbTreeNode = node;
                fix_violation(node);
                return {iterator(node), true};
            }

        };
    }
}

#endif //MYSTL__RBTREE_H
