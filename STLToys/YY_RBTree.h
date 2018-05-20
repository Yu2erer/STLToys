//
//  YY_RBTree.h
//  STLToys
//
//  Created by ntian on 2018/5/20.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_RBTREE_H
#define STLTOYS_YY_RBTREE_H

#include "YY_iterator.h"

namespace YY {

    typedef bool __rb_tree_color_type;
    const __rb_tree_color_type __rb_tree_red = false;
    const __rb_tree_color_type __rb_tree_black = true;

    struct __rb_tree_node_base {
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node_base* base_ptr;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr minimum(base_ptr x) {
            while (x->left) { x = x->left; }
            return x;
        }
        static base_ptr maximum(base_ptr x) {
            while (x->right) { x = x->right; }
            return x;
        }
    };

    template <typename Value>
    struct __rb_tree_node : public __rb_tree_node_base {
        typedef __rb_tree_node<Value>* link_type;
        Value value_field;
    };

    struct __rb_tree_iterator_base {
        typedef __rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;

        base_ptr node;

        void increment() {
            if (node->right) {
                node = node->right;
                while (node->left) {
                    node = node->left;
                }
            } else {
                base_ptr parent = node->parent;
                while (node == parent->right) {
                    node = parent;
                    parent = parent->parent;
                }
                // 是为了应付 寻找根节点的下一节点 但是根节点的右子节点不存在时的情况
                if (node->right != parent) {
                    node = parent;
                }
            }
        }
        void decrement() {
            if (node->color == __rb_tree_red && node->parent->parent == node) {
                node = node->right;
            } else if (node->left) {
                base_ptr node_left = node->left;
                while (node_left->right) {
                    node_left = node_left->right;
                }
                node = node_left;
            } else {
                base_ptr parent = node->parent;
                while (node == parent->left) {
                    node = parent;
                    parent = parent->parent;
                }
                node = parent;
            }
        }
    };

    template <typename Value, typename Ref, typename Ptr>
    struct __rb_tree_iterator : public __rb_tree_iterator_base {
        typedef Value value_type;
        typedef Ref reference;
        typedef Ptr pointer;

        typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
        typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
        typedef __rb_tree_iterator<Value, Ref, Ptr> self;

        typedef __rb_tree_node<Value>* link_type;

        __rb_tree_iterator() {}
        __rb_tree_iterator(link_type x) : node(x) {}
        __rb_tree_iterator(const iterator& it) : node(it.node) {}

        reference operator*() const { return link_type(node)->value_field; }
        pointer operator->() const { return &(operator*()); }

        self& operator++() {
            increment();
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            increment();
            return tmp;
        }
        self& operator--() {
            decrement();
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            decrement();
            return tmp;
        }
    };
}

#endif //STLTOYS_YY_RBTREE_H
