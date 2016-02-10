#ifndef __BST_H__
#define __BST_H__

#include <iostream>
#include "node.hpp"

namespace trees {

        /* rooted binary search tree class definition */
        template <typename KEY, typename VALUE> class BST {

                protected:
                        Node<KEY,VALUE> *root_;
                        Node<KEY,VALUE> *leaf_;

                public:
                        /* default constructor */
                        BST() {
                                root_ = NULL;
                                leaf_ = NULL;
                        }

                        /* destructor */
                        ~BST() {
                                traverseTreeInternal(root_, &this->deleteNode);
                                if (leaf_) delete leaf_;
                        }

                        /* insert a new node with key k and value v */
                        void insertKey(const KEY & k, const VALUE & v) {
                                insertKeyInternal(k, v, &root_, root_);
                        }

                        /* delete the node with key k */
                        void deleteKey(const KEY & k) {
                                /*
                                 * find and replace node having
                                 * key k with either the node having
                                 * max key > k in the left branch or the
                                 * node having min key < k in the left.
                                 * return the pointer to the min/max
                                 * node.
                                 */
                                Node<KEY,VALUE> *child = deleteKeyInternal(k);
                                Node<KEY,VALUE> *parent;

                                /* if key not present */
                                if (child == NULL)
                                        return;

                                parent = child->parent_;

                                /* deleting the root */
                                if (parent == NULL) {
                                        goto delete_node;
                                }

                                /* delete min/max node */
                                if (parent->left_ == child) {
                                        if (child->left_ != leaf_)
                                                parent->left_ = child->left_;
                                        else if (child->right_ != leaf_)
                                                parent->left_ = child->right_;
                                        else
                                                parent->left_ = leaf_;
                                }
                                else {
                                        if (child->right_ != leaf_)
                                                parent->right_ = child->right_;
                                        else if (child->left_ != leaf_)
                                                parent->right_ = child->left_;
                                        else
                                                parent->right_ = leaf_;
                                }
delete_node:
                                deleteNode(child);
                        }

                        /* return node with key k */
                        Node<KEY,VALUE> * searchKey(const KEY & k) {
                                return searchKeyInternal(k, root_);
                        }

                        /* find the maximum */
                        Node<KEY,VALUE> * findMaxKey() {
                                Node<KEY,VALUE> * max;

                                if (root_ == NULL)
                                        return NULL;

                                max = root_;
                                while (max->right_ != leaf_)
                                        max = max->right_;
                                return max;
                        }

                        /* find the minumum */
                        Node<KEY,VALUE> * findMinKey() {
                                Node<KEY,VALUE> * min;

                                if (root_ == NULL)
                                        return NULL;

                                min = root_;
                                while (min->left_ != leaf_)
                                        min = min->left_;
                                return min;
                        }

                        /* traverse tree */
                        void traverseTree(void (*function)(Node<KEY,VALUE> *)) {
                                traverseTreeInternal(root_, function);
                        }

                        static void print_node(trees::Node<KEY,VALUE> * node) {
                                std::cout << "key: " << node->getKey()
                                          << ", value: " << node->getValue()
                                          << ", colour: " << node->colour_
                                          << std::endl;
                        }

                protected:
                        /* insert node internal used to implement recursion */
                        void insertKeyInternal(const KEY & k, const VALUE & v, Node<KEY,VALUE> ** node, Node<KEY,VALUE> * parent) {
                                Node<KEY,VALUE> * p;

                                if ((*node == leaf_) || (*node == NULL)) {
                                        *node = new Node<KEY,VALUE>(k, v);
                                        (*node)->parent_ = parent;
                                        (*node)->left_ = leaf_;
                                        (*node)->right_ = leaf_;
                                        return;
                                }

                                if (k < (*node)->key_) {
                                        insertKeyInternal(k, v, &(*node)->left_, *node);
                                }
                                else if (k > (*node)->key_) {
                                        insertKeyInternal(k, v, &(*node)->right_, *node);
                                }
                                else {
                                        /* replace existing node */
                                        p = new Node<KEY,VALUE>(k, v);
                                        p->left_  = (*node)->left_;
                                        p->right_ = (*node)->right_;
                                        p->parent_= (*node)->parent_;
                                        p->colour_= (*node)->colour_;
                                        if (parent->left_ == *node)
                                                parent->left_ = p;
                                        else
                                                parent->right_ = p;
                                        delete *node;
                                }
                        }

                        /* delete key internal: replace node with key k with proper node */
                        Node<KEY,VALUE> * deleteKeyInternal(const KEY & k) {
                                Node<KEY,VALUE> *node = searchKeyInternal(k, root_);
                                Node<KEY,VALUE> *parent, *child;

                                /* if key not present */
                                if (node == leaf_)
                                        return NULL;

                                /* node has a right child */
                                if (node->right_ != leaf_) {
                                        child = findMinKeyInternal(node->right_);
                                        *node = *child;
                                }
                                /* node has only left child */
                                else if (node->left_ != leaf_) {
                                        child = findMaxKeyInternal(node->left_);
                                        *node = *child;
                                }
                                /* node is a leaf node */
                                else {
                                         child = node;
                                }

                                return child;
                        }

                        /* search node internal used to implement recursion */
                        Node<KEY,VALUE> * searchKeyInternal(const KEY & k, Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * p = node;

                                if ((node == NULL) || (node == leaf_))
                                        return leaf_;

                                if (k < p->key_)
                                        return searchKeyInternal(k, p->left_);
                                else if (k > p->key_)
                                        return searchKeyInternal(k, p->right_);

                                return p;
                        }

                        /* find the maximum in this node's subtree */
                        Node<KEY,VALUE> * findMaxKeyInternal(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * max;

                                if ((node == NULL) || (node == leaf_))
                                        return NULL;

                                max = node;
                                while (max->right_ != leaf_)
                                        max = max->right_;

                                return max;
                        }

                        /* find the minumum in this node's subtree */
                        Node<KEY,VALUE> * findMinKeyInternal(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * min;

                                if ((node == NULL) || (node == leaf_))
                                        return NULL;

                                min = node;
                                while (min->left_ != leaf_)
                                        min = min->left_;

                                return min;
                        }

                        /* traverse tree internal */
                        void traverseTreeInternal(Node<KEY,VALUE> * node, void (*function)(Node<KEY,VALUE> *)) {
                                if ((node == NULL) || (node == leaf_))
                                        return;

                                /* apply function to left subtree first */
                                traverseTreeInternal(node->left_, function);
                                if (node->left_ != this->leaf_)
                                        function(node->left_);

                                /* apply function to right subtree second */
                                traverseTreeInternal(node->right_, function);
                                if (node->right_ != this->leaf_)
                                        function(node->right_);

                                /* finally apply function to root (e.g. delete) */
                                if (node == root_)
                                        function(node);
                         }

                        /* delete node */
                        static void deleteNode(Node<KEY,VALUE> * node) {
                                delete node;
                        }
        }; /* end of binary search tree */
} /* end of namespace */
#endif /* __BST_H__ */
