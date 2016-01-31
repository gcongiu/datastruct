#ifndef __BST_H__
#define __BST_H__

#include "node.hpp"
#ifdef _ENABLE_DEBUG_
#include <iostream>
#endif

namespace trees {

        /* rooted binary search tree class definition */
        template <typename KEY, typename VALUE> class BST {

                private:
                        Node<KEY,VALUE> *root_;

                public:
                        /* default constructor */
                        BST() {
                                root_ = NULL;
                        }

                        /* destructor */
                        ~BST() {
                                traverseTreeInternal(root_, &this->deleteNode);
                        }

                        /* insert a new node with key k and value v */
                        void insertKey(const KEY & k, const VALUE & v) {
                                insertKeyInternal(k, v, &root_, root_);
                        }

                        /* delete the node with key k */
                        void deleteKey(const KEY & k) {
                                Node<KEY,VALUE> *node   = searchKeyInternal(k, root_);
                                Node<KEY,VALUE> *parent = node->parent_;
                                Node<KEY,VALUE> *min, *max;

                                /* if key not present */
                                if (node == NULL)
                                        return;

                                /* node has both left and right child */
                                if (node->left_ != NULL && node->right_ != NULL) {
                                        min = findMinKeyInternal(node->right_);
                                        replaceNode(node, min);
                                        node = min->parent_;
                                        if (node->left_ == min) {
                                                deleteNode(node->left_);
                                                node->left_ = NULL;
                                        }
                                        else {
                                                deleteNode(node->right_);
                                                node->right_ = NULL;
                                        }
                                }
                                /* node has only left child */
                                else if (node->left_ != NULL) {
                                        max = findMaxKeyInternal(node->left_);
                                        replaceNode(node, max);
                                        node = max->parent_;
                                        if (node->left_ == max) {
                                                deleteNode(node->left_);
                                                node->left_ = NULL;
                                        }
                                        else {
                                                deleteNode(node->right_);
                                                node->right_ = NULL;
                                        }
                                }
                                /* node has only right child */
                                else if (node->right_ != NULL) {
                                        min = findMinKeyInternal(node->right_);
                                        replaceNode(node, max);
                                        node = max->parent_;
                                        if (node->left_ == max) {
                                                deleteNode(node->left_);
                                                node->left_ = NULL;
                                        }
                                        else {
                                                deleteNode(node->right_);
                                                node->right_ = NULL;
                                        }
                                }
                                /* node is a leaf node, just delete it */
                                else if (parent != NULL) {
                                           if (parent->left_ == node) {
                                                deleteNode(parent->left_);
                                                parent->left_ = NULL;
                                        }
                                        else if (parent->right_ == node){
                                                deleteNode(parent->right_);
                                                parent->right_ = NULL;
                                        }
                                }
                                else {
                                        deleteNode(node);
                                }
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
                                while (max->right_ != NULL)
                                        max = max->right_;
                                return max;
                        }

                        /* find the minumum */
                        Node<KEY,VALUE> * findMinKey() {
                                Node<KEY,VALUE> * min;

                                if (root_ == NULL)
                                        return NULL;

                                min = root_;
                                while (min->left_ != NULL)
                                        min = min->left_;
                                return min;
                        }

                        /* traverse tree */
                        void traverseTree(void (*function)(Node<KEY,VALUE> *)) {
                                traverseTreeInternal(root_, function);
                        }

                private:
                        /* insert node internal used to implement recursion */
                        void insertKeyInternal(const KEY & k, const VALUE & v, Node<KEY,VALUE> ** node, Node<KEY,VALUE> * parent) {
                                Node<KEY,VALUE> * p;

                                if (*node == NULL) {
                                        *node = new Node<KEY,VALUE>(k, v);
                                        (*node)->parent_ = parent;
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
                                        if (parent->left_ == *node)
                                                parent->left_ = p;
                                        else
                                                parent->right_ = p;
                                        delete *node;
                                }
                        }

                        /* search node internal used to implement recursion */
                        Node<KEY,VALUE> * searchKeyInternal(const KEY & k, Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * p = node;

                                if (k < p->key_)
                                        return searchKeyInternal(k, p->left_);
                                else if (k > p->key_)
                                        return searchKeyInternal(k, p->right_);

                                return p;
                        }

                        /* find the maximum in this node's subtree */
                        Node<KEY,VALUE> * findMaxKeyInternal(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * max;

                                if (node == NULL)
                                        return NULL;

                                max = node;
                                while (max->right_ != NULL)
                                        max = max->right_;

                                return max;
                        }

                        /* find the minumum in this node's subtree */
                        Node<KEY,VALUE> * findMinKeyInternal(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * min;

                                if (node == NULL)
                                        return NULL;

                                min = node;
                                while (min->left_ != NULL)
                                        min = min->left_;

                                return min;
                        }

                        /* replace node with child */
                        void replaceNode(Node<KEY,VALUE> * node, Node<KEY,VALUE> * child) {
                                /* invoke assignment operator */
                                *node = *child;
#ifdef _ENABLE_DEBUG_
                                std::cout << "child key: " <<
                                        child->key_ <<
                                        ", node key: " <<
                                        node->key_ << std::endl;
                                std::cout << "child value: " <<
                                        child->value_ <<
                                        ", node value: " <<
                                        node->value_ << std::endl;
                                std::cout << "child->left: " <<
                                        child->left_ <<
                                        ", node->left: " <<
                                        node->left_ << std::endl;
                                std::cout << "child->right: " <<
                                        child->right_ <<
                                        ", node->right: " <<
                                        node->right_ << std::endl;
#endif
                        }

                        /* traverse tree internal */
                        void traverseTreeInternal(Node<KEY,VALUE> * node, void (*function)(Node<KEY,VALUE> *)) {
                                if (node == NULL)
                                        return;

                                /* apply function to left subtree first */
                                traverseTreeInternal(node->left_, function);
                                if (node->left_)
                                        function(node->left_);

                                /* apply function to right subtree second */
                                traverseTreeInternal(node->right_, function);
                                if (node->right_)
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
