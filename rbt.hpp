#ifndef __RBT_H__
#define __RBT_H__

#include "bst.hpp"

namespace trees {

        /* red black tree class definition */
        template <typename KEY, typename VALUE> class RBT : public BST<KEY,VALUE> {

                private:
                        Node<KEY,VALUE> * root_;
                        Node<KEY,VALUE> * leaf_;

                public:
                        /* default constructor */
                        RBT() {
                                BST<KEY,VALUE>();
                                leaf_ = new BST<KEY,VALUE>();
                                leaf_->colour_ = BLACK;
                                root_->left_ = leaf_;
                                root_->right_= leaf_;
                        }

                        /* deconstructor */
                        ~RBT() { }

                        /* insert node */
                        void insertNode(const KEY & k, const VALUE & v) {
                                Node<KEY,VALUE> ** node = &root_;

                                /* insert the key value in the tree */
                                insertNodeInternal(k, v, node, *node);

                                node->left_ = leaf_;
                                node->right_= leaf_;

                                /* check Case 1 for tree rebalancing */
                                rebalanceInsertCase1(*node);
                        }

                        /* delete a node */
                        void deleteNode(const KEY & k) { }

                private:
                        /* Case 1: the root node is black */
                        void rebalanceInsertCase1(Node<KEY,VALUE> * node) {
                                if (node->parent_ == NULL)
                                        node->colour_ = BLACK;
                                else
                                        rebalanceInsertCase2(node);
                        }
                        /* Case 2: the parent node is black */
                        void rebalanceInsertCase2(Node<KEY,VALUE> * node) {
                                if (node->parent_ == BLACK)
                                        return;
                                else
                                        rebalanceInsertCase3(node);
                        }

                        /* Case 3: both parent and uncle are red */
                        void rebalanceInsertCase3(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * grandpa, * uncle = getUncle(node);

                                if ((uncle != NULL) && (uncle->colour_ == RED)) {
                                        node->parent_->colour_ = BLACK;
                                        uncle->colour_ = BLACK;
                                        grandpa = getGrandParent(node);
                                        grandpa->colour_ = RED;
                                        rebalanceInsertCase1(grandpa);
                                }
                                else
                                        rebalanceInsertCase4(node);
                        }

                        /* Case 4: parent is red and uncle is black */
                        void rebalanceInsertCase4(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * grandpa = getGrandParent(node);

                                if ((node == node->parent_->right_) && (node->parent_ == grandpa->left_)) {
                                        rotateLeft(node->parent_);
                                        node = node->left_;
                                }
                                else if ((node == node->parent_->left_) && (node->parent_ == grandpa->right_)) {
                                        rotateRight(node->parent_);
                                        node = node->right_;
                                }
                                rebalanceInsertCase5(node);
                        }

                        /* Case 5: */
                        void rebalanceInsertCase5(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * grandpa;

                                grandpa                = getGrandParent(node);
                                node->parent_->colour_ = BLACK;
                                grandpa->colour_       = RED;

                                if (node == node->parent_->left_)
                                        rotateRight(grandpa);
                                else
                                        rotateLeft(grandpa);
                        }

                        /* rotate left */
                        void rotateLeft(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * parent;
                                parent                = node->parent_;
                                parent->left_         = node->right_;
                                node->right_->parent_ = parent;
                                node->right_->left_   = node;
                        }

                        /* rotate right */
                        void rotateRight(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * parent;
                                parent               = node->parent_;
                                parent->right_       = node->left_;
                                node->left_->parent_ = parent;
                                node->left_->right_  = node;
                        }

                        /* get grand parent */
                        Node<KEY,VALUE> * getGrandParent(Node<KEY,VALUE> * node) {
                                if (node != NULL && node->parent_ != NULL)
                                        return node->parent_->parent_;
                                else
                                        return NULL;
                        }

                        /* get uncle */
                        Node<KEY,VALUE> * getUncle(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * grandpa = getGrandParent(node);
                                if (grandpa == NULL)
                                        return NULL;

                                if (grandpa->left_ == node->parent_)
                                        return grandpa->right_;
                                else
                                        return grandpa->left_;
                        }

                        /* get sibling */
                        Node<KEY,VALUE> * getSibling(Node<KEY,VALUE> * node) {
                                if ((node == NULL) || (node->parent_ == NULL))
                                        return NULL;

                                if (node->parent_->left_ == node)
                                        return node->parent_->right_;
                                else
                                        return node->parent_->left_;
                        }

                        /* is node a leaf node? */
                        bool isLeaf(Node<KEY,VALUE> * node) {
                                return (node != leaf_) ? false : true;
                        }
        }; /* end of red black tree */
} /* end of namespace */
#endif /* __RBT_H__ */
