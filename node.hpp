#ifndef __NODE_H__
#define __NODE_H__

#include <stdlib.h>

//extern template <typename KEY, typename VALUE> class BST;
//extern template <typename KEY, typename VALUE> class RBT;

namespace trees {

        enum {BLACK = 0, RED};

        /* Generic node class for following trees' implementations */
        template <typename KEY, typename VALUE> class Node {

                template <typename K, typename V> friend class BST;
                template <typename K, typename V> friend class RBT;

                private:
                        KEY key_;
                        VALUE value_;
                        int colour_;
                        Node<KEY,VALUE> *left_;
                        Node<KEY,VALUE> *right_;
                        Node<KEY,VALUE> *parent_;

                public:
                        /* default constructor */
                        Node() { }

                        /* custom constructor */
                        Node(KEY k, VALUE v) {
                                key_   = k;
                                value_ = v;
                                colour_= RED;
                                left_  = NULL;
                                right_ = NULL;
                                parent_= NULL;
                        }

                        /* copy constructor */
                        Node(const Node & node) {
                                key_   = node.key_;
                                value_ = node.value_;
                                colour_= node.colour_;
                                left_  = node.left_;
                                right_ = node.right_;
                                parent_= node.parent_;
                        }

                        /* assignment operator */
                        Node & operator=(const Node & node) {
                                key_   = node.key_;
                                value_ = node.value_;
                                colour_= node.colour_;
                                left_  = node.left_;
                                right_ = node.right_;
                                parent_= node.parent_;
                                return this;
                        }

                        /* destructor */
                        ~Node() {
                                left_  = NULL;
                                right_ = NULL;
                                parent_= NULL;
                        }

                        /* get key */
                        KEY & getKey() {
                                return key_;
                        }

                        /* get value */
                        VALUE & getValue() {
                                return value_;
                        }
        }; /* end of Node */

}
#endif
