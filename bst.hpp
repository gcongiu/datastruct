#ifndef __BST_H__
#define __BST_H__

#include "node.hpp"

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
                        ~BST() { }

                        /* insert a new node with key k and value v */
                        void insertNode(const KEY & k, const VALUE & v) {
                                insertNodeInternal(k, v, &root_, root_);
                        }

                        /* delete the node with key k */
                        void deleteNode(const KEY & k) {
                                Node<KEY,VALUE> *node   = searchNodeInternal(k, root_);
                                Node<KEY,VALUE> *parent = node->parent_;
                                Node<KEY,VALUE> *right, *min;

                                /* node has both left and right child */
                                if (node->left_ != NULL && node->right_ != NULL) {
                                        right = node->right_;
                                        min = findSubMinimum(right);
                                        if (parent->left_ == node)
                                                parent->left_ = min;
                                        else
                                                parent->right_ = min;
                                        if (min->right_ != NULL)
                                                min->parent_->left_ = min->right_;
                                        min->right_ = node->right_;
                                        min->left_  = node->left_;
                                        min->parent_= parent;
                                }
                                /* node has only left child */
                                else if (node->left_ != NULL) {
                                        if (parent->left_ == node)
                                                parent->left_ = node->left_;
                                        else
                                                parent->right_ = node->left_;
                                }
                                /* node has only right child */
                                else if (node->right_ != NULL) {
                                        if (parent->left_ == node)
                                                parent->left_ = node->right_;
                                        else
                                                parent->right_ = node->right_;
                                }
                                /* node is a leaf node, just delete it */
                                else {
                                        if (parent->left_ == node)
                                                parent->left_ = NULL;
                                        else
                                                parent->right_ = NULL;
                                }
                                delete node;
                        }

                        /* return node with key k */
                        Node<KEY,VALUE> * searchNode(const KEY & k) {
                                return searchNodeInternal(k, root_);
                        }

                        /* find the maximum */
                        Node<KEY,VALUE> * findMaximum() {
                                Node<KEY,VALUE> * max;

                                if (root_ == NULL)
                                        return NULL;

                                max = root_;
                                while (max->right_ != NULL)
                                        max = max->right_;
                                return max;
                        }

                        /* find the minumum */
                        Node<KEY,VALUE> * findMinimum() {
                                Node<KEY,VALUE> * min;

                                if (root_ == NULL)
                                        return NULL;

                                min = root_;
                                while (min->left_ != NULL)
                                        min = min->left_;
                                return min;
                        }

                private:
                        /* insert node internal used to implement recursion */
                        void insertNodeInternal(const KEY & k, const VALUE & v, Node<KEY,VALUE> ** node, Node<KEY,VALUE> * parent) {
                                Node<KEY,VALUE> * p;

                                if (*node == NULL) {
                                        *node = new Node<KEY,VALUE>(k, v);
                                        (*node)->parent_ = parent;
                                        return;
                                }

                                if (k < (*node)->key_) {
                                        insertNodeInternal(k, v, &(*node)->left_, *node);
                                }
                                else if (k > (*node)->key_) {
                                        insertNodeInternal(k, v, &(*node)->right_, *node);
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
                        Node<KEY,VALUE> * searchNodeInternal(const KEY & k, Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * p = node;

                                if (k < p->key_)
                                        return searchNodeInternal(k, p->left_);
                                else if (k > p->key_)
                                        return searchNodeInternal(k, p->right_);

                                return p;
                        }

                        /* find the maximum in this node's subtree */
                        Node<KEY,VALUE> * findSubMaximum(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * max;

                                if (node == NULL)
                                        return NULL;

                                max = node;
                                while (max->right_ != NULL)
                                        max = max->right_;

                                return max;
                        }

                        /* find the minumum in this node's subtree */
                        Node<KEY,VALUE> * findSubMinimum(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * min;

                                if (node == NULL)
                                        return NULL;

                                min = node;
                                while (min->left_ != NULL)
                                        min = min->left_;

                                return min;
                        }
        }; /* end of binary search tree */
} /* end of namespace */
#endif /* __BST_H__ */
