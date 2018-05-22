//
//  YY_RBTree.h
//  STLToys
//
//  Created by ntian on 2018/5/20.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_RBTREE_H
#define STLTOYS_YY_RBTREE_H

#include "YY_alloc.h"
#include "YY_construct.h"
#include "YY_iterator.h"
#include "YY_pair.h"

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
        __rb_tree_iterator(link_type x) { node = x; }
        __rb_tree_iterator(const iterator& it) { node = it.node ; }

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
        bool operator==(const self& x) { return (link_type)node == (link_type)(x.node); }
        bool operator!=(const self& x) { return (link_type)node != (link_type)(x.node); }
    };

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
    class rb_tree {
    protected:
        typedef void* void_pointer;
        typedef __rb_tree_node_base* base_ptr;
        typedef __rb_tree_node<Value> rb_tree_node;
        typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
        typedef __rb_tree_color_type color_type;
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef rb_tree_node* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
    protected:
        link_type get_node() { return rb_tree_node_allocator::allocate(); }
        void put_node(link_type p) { return rb_tree_node_allocator::deallocate(p); }
        link_type create_node(const value_type& x) {
            link_type tmp = get_node();
            try {
                construct(&tmp->value_field, x);
            } catch(...) {
                put_node(tmp);
            }
            return tmp;
        }
        link_type clone_node(link_type x) {
            link_type tmp = create_node(x->value_field);
            tmp->color = x->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        }
        void destroy_node(link_type p) {
            destroy(&p->value_field);
            put_node(p);
        }
    protected:
        size_type node_count;
        link_type header;
        Compare key_compare;

        link_type& root() const { return (link_type&)header->parent; }
        link_type& leftmost() const { return (link_type&)header->left; }
        link_type& rightmost() const { return (link_type&)header->right; }

        static link_type& left(link_type x) { return (link_type&)(x->left); }
        static link_type& right(link_type x) { return (link_type&)(x->right); }
        static link_type& parent(link_type x) { return (link_type&)(x->parent); }
        static reference value(link_type x) { return x->value_field; }
        static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
        static color_type& color(link_type x) { return (color_type&)(x->color); }

        static link_type& left(base_ptr x) { return (link_type&)(x->left); }
        static link_type& right(base_ptr x) { return (link_type&)(x->right); }
        static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
        static reference value(base_ptr x) { return ((link_type)x)->value_field; }
        static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
        static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }

        static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
        static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }
    private:
        iterator __insert(base_ptr x_, base_ptr y_, const value_type& v);
        link_type __copy(link_type x, link_type p);
        void __erase(link_type x);
        void init() {
            header = get_node();
            color(header) = __rb_tree_red;
            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }
    public:
        rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) { init(); }
        ~rb_tree() {
            clear();
            put_node(header);
        }
        rb_tree& operator=(const rb_tree& x);

        Compare key_comp() const { return key_compare; }
        iterator begin() { return leftmost(); }
        iterator end() { return header; }
        bool empty() const { return node_count == 0; }
        size_type size() const { return node_count; }
        size_type max_size() const { return size_type(-1); }

        pair<iterator, bool> insert_unique(const value_type& v);
        iterator insert_equal(const value_type& v);
        iterator find(const Key& k);

        void clear() {
            if (node_count != 0) {
                __erase(root());
                leftmost() = header;
                root() = 0;
                rightmost() = header;
                node_count = 0;
            }
        }
    };

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v) {
        link_type y = header;
        link_type x = root();
        while  (x) {
            y = x;
            // 小就往左走 大就往右走
            x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, y, v);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v) {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x) {
            y = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        iterator j = iterator(y);
        if (comp) {
            if (j == begin()) {
                return pair<iterator, bool>(__insert(x, y, v), true);
            } else {
                --j;
            }
        }
        if (key_compare(key(j.node), KeyOfValue()(v))) {
            return pair<iterator, bool>(__insert(x, y, v), true);
        }
        return pair<iterator, bool>(j, false);
    }

    inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
        __rb_tree_node_base* y = x->right;
        x->right = y->left;
        if (y->left) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x == root) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }
    inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
        __rb_tree_node_base* y = x->left;
        x->left = y->right;
        if (y->right) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x == root) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
    }

    inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
        x->color = __rb_tree_red;
        while (x != root && x->parent->color == __rb_tree_red) {
            // 伯父节点
            if (x->parent == x->parent->parent->left) {
                __rb_tree_node_base* y = x->parent->parent->right;
                if (y && y->color == __rb_tree_red) {
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    y->color = __rb_tree_black;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    // 没有伯父 或伯父为黑
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }
            } else {
                __rb_tree_node_base* y = x->parent->parent->left;
                if (y && y->color == __rb_tree_red) {
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_red;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        root->color = __rb_tree_black;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const Value& v) {
        link_type x = (link_type)x_;
        link_type y = (link_type)y_;
        link_type z;
        if (y == header || x || key_compare(KeyOfValue()(v), key(y))) {
            z = create_node(v);
            left(y) = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            } else if (y == leftmost()) {
                leftmost() = z;
            }
        } else {
            z = create_node(v);
            right(y) = z;
            if (y == rightmost()) {
                rightmost() = z;
            }
        }
        parent(z) = y;
        left(z) = 0;
        right(z) = 0;
        __rb_tree_rebalance(z, header->parent);
        ++node_count;
        return iterator(z);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
        link_type y = header;
        link_type x = root();
        while (x) {
            if (!(key_compare(key(x), k))) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        iterator j = iterator(y);
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) {
        while (x) {
            __erase(right(x));
            link_type y = left(x);
            destroy_node(x);
            x = y;
        }
    }
};

#endif //STLTOYS_YY_RBTREE_H
