//
//  YY_list.h
//  STLToys
//
//  Created by ntian on 2018/5/5.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_LIST_H
#define STLTOYS_YY_LIST_H

#include "YY_alloc.h"
#include "YY_construct.h"
#include "YY_iterator.h"

namespace YY {

    template <typename T>
    struct __list_node {
        __list_node<T>* prev;
        __list_node<T>* next;
        T data;
    };

    template <typename T, typename Ref, typename Ptr>
    struct __list_iterator {

        // 为了 不维护两份代码 同时 为了方便后面的定义
        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, Ref, Ptr> self;

        typedef bidirectional_iterator_tag iterator_Category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef ptrdiff_t difference_type;
        typedef __list_node<T>* link_type;
        typedef size_t size_type;

        link_type node;

        __list_iterator() {}
        __list_iterator(link_type x) : node(x) {}
        __list_iterator(const iterator& x) : node(x.node) {}

        bool operator==(const self& x) const { return node == x.node; }
        bool operator!=(const self& x) const { return node != x.node; }

        reference operator*() const { return (*node).data; }
        pointer operator->() const { return &(operator*()); }

        self& operator++() {
            node = (*node).next;
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--() {
            node = (*node).prev;
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };

    template <typename T, typename Alloc = alloc>
    class list {
    protected:
        typedef __list_node<T> list_node;
        typedef simple_alloc<list_node, Alloc> list_node_allocator;
    public:
        typedef list_node* link_type;
        typedef __list_iterator<T, T&, T*> iterator;
        typedef size_t size_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
    protected:
        link_type node;

        // 配置一个节点
        link_type get_node() { return list_node_allocator::allocate(); }
        // 释放一个节点
        void put_node(link_type p) { list_node_allocator::deallocate(p); }

        link_type create_node(const T& x) {
            link_type p = get_node();
            construct(&p->data, x);
            return p;
        }
        void destroy_node(link_type p) {
            destroy(&p->data);
            put_node(p);
        }

        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }
        void fill_initialize(size_type n, const T& x) {
            empty_initialize();
            try {
                insert(begin(), n, x);
            } catch(...) {
                clear();
                put_node(node);
            }
        }
    public:
        list() { empty_initialize(); }
        list(size_type n, const T& value) { fill_initialize(n, value); }
        list(int n, const T& value) { fill_initialize(n, value); }
        list(long n, const T& value) { fill_initialize(n, value); }
        explicit list(size_type n) { fill_initialize(n, T()); }

        ~list() {
            clear();
            put_node(node);
        }

        void insert(iterator position, size_type n, const T& value);
        template <typename Inputiterator>
        void insert(iterator position, Inputiterator first, Inputiterator last);
        iterator insert(iterator position, const T& x) {
            link_type tmp = create_node(x);
            tmp->next = position.node;
            tmp->prev = position.node->prev;
            position.node->prev->next = tmp;
            position.node->prev = tmp;
            return tmp;
        }

        void push_front(const T& x) { insert(begin(), x); }
        void push_back(const T& x) { insert(end(), x); }
        iterator erase(iterator position) {
            link_type next_node = position.node->next;
            link_type prev_node = position.node->prev;
            prev_node->next = next_node;
            next_node->prev = prev_node;
            destroy_node(position.node);
            return next_node;
        }

        void pop_front() { erase(begin()); }
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }
        // 此处是假设 node 在尾后迭代器的位置
        // 发生了隐式转换
        iterator begin() { return (*node).next; }
        iterator end() { return node; }

        bool empty() const { return node->next == node; }

        size_type size() {
            size_type result = 0;
            distance(begin(), end(), result);
            return result;
        }

        reference front() { return *begin(); }
        reference back() { return *(--end()); }

        void clear();

        void remove(const T& value);

        void unique();

        void merge(list& x);

        void reverse();

        void sort();

        void swap(list& x) {
            link_type tmp = x.node;
            x.node = node;
            node = tmp;
        }

        // 将 x 接在 position 之前 x 必须不同于 *this
        void splice(iterator position, list& x) {
            if (x.empty()) { return; }
            transfer(position, x.begin(), x.end());
        }
        // 将 i 所指的元素接合在 position 之前 position 和 i 可以指向同一个list
        void splice(iterator position, list&, iterator i) {
            iterator j = i;
            ++j;
            if (position == i || position == j) { return; }
            transfer(position, i, j);
        }
        // 将 [first, last) 接合在 position 之前 可以指向同一个list 但是 pos 不能在 [first, last) 之间
        void splice(iterator position, list&, iterator first, iterator last) {
            if (first == last) { return; }
            transfer(position, first, last);
        }
    protected:
        // 将 [first, last) 移动到 position 之前 暂未搞懂
        void transfer(iterator position, iterator first, iterator last) {
            if (position == last) { return; }
            (*(*last.node).prev).next = position.node;
            (*(*first.node).prev).next = last.node;
            (*(*position.node).prev).next = first.node;
            link_type tmp = (*position.node).prev;
            (*position.node).prev = (*last.node).prev;
            (*last.node).prev = (*first.node).prev;
            (*first.node).prev = tmp;
        }
    };

    template <typename T, typename Alloc>
    void list<T, Alloc>::insert(iterator position, size_type n, const T &value) {
        for (; n > 0; --n) {
            insert(position, value);
        }
    }

    template <typename T, typename Alloc>
    template <typename Inputiterator>
    void list<T, Alloc>::insert(iterator position, Inputiterator first, Inputiterator last) {
        for (; first != last; ++first) {
            insert(position, *first);
        }
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::clear() {
        // begin
        link_type cur = node->next;
        while (cur != node) {
            link_type tmp = cur;
            cur = cur->next;
            destroy_node(tmp);
        }
        // 恢复成初始状态
        node->next = node;
        node->prev = node;
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::remove(const T &value) {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            iterator next = first;
            ++next;
            if (*first == value) { erase(first); }
            first = next;
        }
    }

    // 移除连续且相同的元素 只保留一个
    template <typename T, typename Alloc>
    void list<T, Alloc>::unique() {
        iterator first = begin();
        iterator last = end();
        // 空链 啥都不做
        if (first == last) { return; }
        iterator next = first;
        while (++next != last) {
            if (*first == *next) {
                erase(next);
            } else {
                first = next;
            }
            // 修正迭代器
            next = first;
        }
    }

    // 将 x 合并到 *this 身上 前提是两个 lists 都递增排序好的
    template <typename T, typename Alloc>
    void list<T, Alloc>::merge(list<T, Alloc>& x) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();
        // 前提是都排序过的
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }
        if (first2 != last2) {
            transfer(last1, first2, last2);
        }
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::reverse() {
        // 如果是空链表 或者只有一个元素 就不操作 不用 size() 是因为 size() 效率不高
        if (node->next == node || node->next->next == node) { return; }
        iterator first = begin();
        ++first;
        while (first != end()) {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }

    // list 不能使用 stl 的 sort 因为 stl sort() 使用的是 RandomAccessIterator
    // 此处 书上说使用的 quicksort 但我个人觉得 应该是 mergesort
    template <typename T, typename Alloc>
    void list<T, Alloc>::sort() {
        // 如果是空链表 或者只有一个元素 就不操作 不用 size() 是因为 size() 效率不高
        if (node->next == node || node->next->next == node) { return; }
        // carry和counter 就是中转站
        list<T, Alloc> carry;
        list<T, Alloc> counter[64];
        // 总而言之 就是 counter[i] 能存放2的(i + 1)次方 每次存放采用 merge 进行排序存放
        // 当满了以后 就 将它转移到 counter[i+1] 里面去 此时里面已经是排好序的了
        // 依次循环 直到 *this 为空 然后 再依次归并 最后 将结果交换回来
        int fill = 0;
        while (!empty()) {
            // 将 begin() 放入 carry.begin() 并删除掉 begin()所在的元素
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) { ++fill; }
        }
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i - 1]);
        }
        swap(counter[fill - 1]);
    }
}

#endif //STLTOYS_YY_LIST_H
