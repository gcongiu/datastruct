#ifndef __RBT_H__
#define __RBT_H__

#include "bst.hpp"
#ifdef _ENABLE_DEBUG_
#include <iostream>
#endif

namespace trees {

        /* red black tree class definition */
        template <typename KEY, typename VALUE> class RBT : public BST<KEY,VALUE> {

                private:
                        Node<KEY,VALUE> * leaf_; /* place holder for deletion */

                public:
                        /* default constructor */
                        RBT() {
                                BST<KEY,VALUE>();
                                leaf_ = new Node<KEY,VALUE>();
                                leaf_->colour_ = BLACK;
                        }

                        /* deconstructor */
                        ~RBT() {
                                delete leaf_;
                        }

                        /* insert key */
                        void insertKey(const KEY & k, const VALUE & v) {
                                Node<KEY,VALUE> ** node = &this->root_;

                                /* insert the key value in the tree */
                                this->insertKeyInternal(k, v, node, *node);

                                /* check Case 1 for tree rebalancing */
                                rebalanceInsertCase1(this->searchKey(k));
#ifdef _ENABLE_DEBUG_
                                this->traverseTreeInternal(this->root_, &this->print_node);
                                std::cout << std::endl;
#endif
                        }

                        /* delete a key */
                        void deleteKey(const KEY & k) {
                                /*
                                 * find and replace node having
                                 * key k with either the node having
                                 * max k in the left branch or the
                                 * node having min k in the left.
                                 */
                                Node<KEY,VALUE> *node = this->deleteKeyInternal(k);
                                Node<KEY,VALUE> *parent, *child;

                                /* if key not present */
                                if (node == NULL)
                                        return;

                                child = (node->left_) ? node->left_ : node->right_;

                                /* node is a leaf, use leaf_ place holder as child */
                                if (child == NULL) {
                                        child = leaf_;
                                        child->parent_ = node->parent_;
                                }

                                /* replace node with child and keep node */
                                parent = node->parent_;
                                if (parent) {
                                        if (parent->left_ == node)
                                                parent->left_ = child;
                                        else
                                                parent->right_ = child;
                                        child->parent_ = parent;
                                }

                                if (node->colour_ == BLACK) {
                                        if (child->colour_ == RED)
                                                child->colour_ = BLACK;
                                        else
                                                /* double black case */
                                                rebalanceDeleteCase1(child);
                                }

                                /* eventually delete node */
                                this->deleteNode(node);

                                /* and replace leaf with null pointer */
                                if (child == leaf_) {
                                        if (child->parent_->left_ == child)
                                                child->parent_->left_ = NULL;
                                        else
                                                child->parent_->right_ = NULL;
                                }
                        }

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
                                if (node->parent_->colour_ == BLACK)
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

                                grandpa = getGrandParent(node);
                                node->parent_->colour_ = BLACK;
                                grandpa->colour_ = RED;

                                if (node == node->parent_->left_)
                                        rotateRight(grandpa);
                                else
                                        rotateLeft(grandpa);

                                /* if grand parent was root update root */
                                if (grandpa->parent_->parent_ == NULL)
                                        this->root_ = grandpa->parent_;
                        }

                        /* Case 1: */
                        void rebalanceDeleteCase1(Node<KEY,VALUE> * node) {
                                 if (node->parent_ != NULL)
                                        rebalanceDeleteCase2(node);
                        }

                        /* Case 2: */
                        void rebalanceDeleteCase2(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * sibling = getSibling(node);

                                if (sibling->colour_ == RED) {
                                        node->parent_->colour_ = RED;
                                        sibling->colour_ = BLACK;

                                        if (node->parent_->left_ == node)
                                                rotateLeft(node->parent_);
                                        else
                                                rotateRight(node->parent_);
                                }

                                rebalanceDeleteCase3(node);
                        }

                        /* Case 3: */
                        void rebalanceDeleteCase3(Node<KEY,VALUE> * node) {
                                 Node<KEY,VALUE> * sibling = getSibling(node);

                                 if ((node->parent_->colour_ == BLACK) &&
                                     (sibling->colour_ == BLACK) &&
                                     (sibling->left_->colour_ == BLACK) &&
                                     (sibling->right_->colour_ == BLACK)) {
                                         sibling->colour_ = RED;
                                         rebalanceDeleteCase1(node->parent_);
                                 }
                                 else
                                        rebalanceDeleteCase4(node);
                        }

                        /* Case 4: */
                        void rebalanceDeleteCase4(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * sibling = getSibling(node);
                                int sibling_left_colour, sibling_right_colour;

                                sibling_left_colour = (sibling->left_) ?
                                        sibling->left_->colour_ : BLACK;
                                sibling_right_colour = (sibling->right_) ?
                                        sibling->right_->colour_ : BLACK;

                                if ((node->parent_->colour_ == RED) &&
                                    (sibling->colour_ == BLACK) &&
                                    (sibling_left_colour == BLACK) &&
                                    (sibling_right_colour == BLACK)) {
                                        sibling->colour_ = RED;
                                        node->parent_->colour_ = BLACK;
                                } else
                                        rebalanceDeleteCase5(node);
                        }

                        /* Case 5: */
                        void rebalanceDeleteCase5(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * sibling = getSibling(node);

                                if (sibling->colour_ == BLACK) {
                                        if ((node->parent_->left_ == node) &&
                                            (sibling->right_->colour_ == BLACK) &&
                                            (sibling->left_->colour_ == RED)) {
                                                sibling->colour_ = RED;
                                                sibling->left_->colour_ = BLACK;
                                                rotateRight(sibling);
                                        }
                                        else if ((node->parent_->right_ == node) &&
                                                 (sibling->left_->colour_ == BLACK) &&
                                                 (sibling->right_->colour_ == RED)) {
                                                sibling->colour_ = RED;
                                                sibling->right_->colour_ = BLACK;
                                                rotateLeft(sibling);
                                        }
                                }
                                rebalanceDeleteCase6(node);
                        }

                        /* Case 6: */
                        void rebalanceDeleteCase6(Node<KEY,VALUE> * node) {
                                 Node<KEY,VALUE> * sibling = getSibling(node);

                                 sibling->colour_ = node->parent_->colour_;
                                 node->parent_->colour_ = BLACK;

                                 if (node->parent_->left_ == node) {
                                         sibling->right_->colour_ = BLACK;
                                         rotateLeft(node->parent_);
                                 }
                                 else {
                                         sibling->left_->colour_ = BLACK;
                                         rotateRight(node->parent_);
                                 }
                        }

                        /* rotate left */
                        void rotateLeft(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * parent, * right_child;

                                parent = node->parent_;
                                right_child = node->right_;

                                if (parent) {
                                        if (parent->left_ == node)
                                                parent->left_ = right_child;
                                        else
                                                parent->right_ = right_child;
                                }
                                right_child->parent_ = parent;
                                node->right_ = right_child->left_;
                                node->parent_ = right_child;
                                right_child->left_ = node;
                        }

                        /* rotate right */
                        void rotateRight(Node<KEY,VALUE> * node) {
                                Node<KEY,VALUE> * parent, * left_child;

                                parent = node->parent_;
                                left_child = node->left_;

                                if (parent) {
                                        if (parent->left_ == node)
                                                parent->left_ = left_child;
                                        else
                                                parent->right_ = left_child;
                                }
                                left_child->parent_ = parent;
                                node->left_ = left_child->right_;
                                node->parent_ = left_child;
                                left_child->right_ = node;
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
                                return (node != NULL) ? false : true;
                        }
        }; /* end of red black tree */
} /* end of namespace */
#endif /* __RBT_H__ */
