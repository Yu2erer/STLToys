//
//  YY_slist.h
//  STLToys
//
//  Created by ntian on 2018/5/17.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_SLIST_H
#define STLTOYS_YY_SLIST_H

#include "YY_alloc.h"
#include "YY_construct.h"

namespace YY {

    struct __slist_node_base {
        __slist_node_base* next;
    };

    template <typename T>
    struct __slist_node : public __slist_node_base {
        T data;
    };

    // 已知某个节点 插入新节点之后
    inline __slist_node_base* __slist_make_link(__slist_node_base* prev_node, __slist_node_base* new_node) {
        new_node->next = prev_node->next;
        prev_node->next = new_node;
        return new_node;
    }

    inline size_t __slist_size(__slist_node_base* node) {
        size_t result = 0;
        for (; node != 0; node = node->next) {
            ++result;
        }
        return result;
    }

    inline __slist_node_base* __slist_previous(__slist_node_base* head, const __slist_node_base* node) {
        while (head && head->next != node) {
            head = head->next;
        }
        return head;
    }

    struct __slist_iterator_base {
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef forward_iterator_tag iterator_category;

        __slist_node_base* node;

        __slist_iterator_base(__slist_node_base* x) : node(x) {}

        void incr() { node = node->next; }

        bool operator==(const __slist_iterator_base& x) const { return node == x.node; }
        bool operator!=(const __slist_iterator_base& x) const { return node != x.node; }
    };

    template <typename T, typename Ref, typename Ptr>
    struct __slist_iterator : public __slist_iterator_base {
        typedef __slist_iterator<T, T&, T*> iterator;
        typedef __slist_iterator<T, const T&, const T*> const_iterator;
        typedef __slist_iterator<T, Ref, Ptr> self;

        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef __slist_node<T> list_node;

        __slist_iterator() : __slist_iterator_base(0) {}
        __slist_iterator(list_node* x) : __slist_iterator_base(x) {}
        __slist_iterator(const iterator& x) : __slist_iterator_base(x.node) {}

        reference operator*() const { return ((list_node *)node)->data; }
        pointer operator->() const { return &(operator*()); }

        self& operator++() {
            incr();
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
    };

    template <typename T, typename Alloc = alloc>
    class slist {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef __slist_iterator<T, T&, T*> iterator;
        typedef __slist_iterator<T, const T&, const T*> const_iterator;
    private:
        typedef __slist_node<T> list_node;
        typedef __slist_node_base list_node_base;
        typedef __slist_iterator_base iterator_base;
        typedef simple_alloc<list_node, Alloc> list_node_allocator;

        static list_node* create_node(const value_type& x) {
            list_node* node = list_node_allocator::allocate();
            try {
                construct(&node->data, x);
                node->next = 0;
            } catch (...) {
                list_node_allocator::deallocate(node);
            }
            return node;
        }

        static void destroy_node(list_node* node) {
            destroy(&node->data);
            list_node_allocator::deallocate(node);
        }
    private:
        list_node_base head;
    public:
        slist() { head.next = 0; }
        ~slist() { clear(); }

        iterator begin() { return iterator((list_node *)head.next); }
        iterator end() { return iterator(0); }
        size_type size() const { return __slist_size(head.next); }
        bool empty() const { return head.next == 0; }
        void clear() { erase_after(&head, 0); }
        void swap(slist& L) {
            list_node_base* tmp = head.next;
            head.next = L.head.next;
            L.head.next = tmp;
        }

        reference front() { return ((list_node *)head.next)->data; }
        void push_front(const value_type& x) {
            __slist_make_link(&head, create_node(x));
        }
        void pop_front() {
            list_node* node = (list_node *)head.next;
            head.next = node->next;
            destroy_node(node);
        }

        iterator erase(iterator position) {
            return iterator((list_node *)erase_after(__slist_previous(&head, position.node)));
        }
        iterator erase(iterator first, iterator last) {
            return iterator((list_node *)erase_after(__slist_previous(&head, first.node), last.node));
        }

    private:
        list_node_base* erase_after(list_node_base* pos) {
            list_node* next = (list_node *)pos->next;
            list_node_base* next_next = next->next;
            pos->next = next_next;
            destroy_node(next);
            return next_next;
        }
        list_node_base* erase_after(list_node_base* before_first, list_node_base* last_node) {
            list_node* cur = (list_node *)before_first->next;
            while (cur != last_node) {
                list_node* tmp = cur;
                cur = (list_node *)cur->next;
                destroy_node(tmp);
            }
            before_first->next = last_node;
            return last_node;
        }
    };
}

#endif //STLTOYS_YY_SLIST_H
