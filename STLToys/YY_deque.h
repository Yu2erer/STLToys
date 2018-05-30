//
//  YY_deque.h
//  STLToys
//
//  Created by ntian on 2018/5/7.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_DEQUE_H
#define STLTOYS_YY_DEQUE_H

#include "YY_alloc.h"
#include "YY_iterator.h"
#include "YY_uninitialized.h"
#include "YY_algobase.h"

namespace YY {

    // 如果 n 不为 0 则返回由用户自定义的BufSiz 否则 如果 元素大小 小于 512 则 返回 512 / 元素大小
    // 大于 则 返回 1
    inline size_t __deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }

    template <typename T, typename Ref, typename Ptr, size_t BufSiz>
    struct __deque_iterator {
        typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
        typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
        typedef __deque_iterator<T, Ref, Ptr, BufSiz> self;

        static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T** map_pointer;

        __deque_iterator() : cur(0), first(0), last(0), node(0) {}
        __deque_iterator(value_type* x, map_pointer y) : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
        __deque_iterator(const iterator& x) : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

        T* cur;
        T* first;
        T* last;
        map_pointer node;

        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

        reference operator*() const { return *cur; }
        pointer operator->() const { return &(operator*()); }
        difference_type operator-(const self& x) const {
            return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
        }
        self& operator++() {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--() {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
        self& operator+=(difference_type n) {
            difference_type offset = n + (cur - first);
            // 在同一个缓冲区内
            if (offset >= 0 && offset < difference_type(buffer_size())) {
                cur += n;
            } else {
                    difference_type node_offset = offset > 0 ?
                                                  offset / difference_type(buffer_size()) :
                                                  -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * difference_type(buffer_size()));
            }
            return *this;
        }
        self& operator-=(difference_type n) { return *this += -n; }
        self operator+(difference_type n) const {
            self tmp = *this;
            return tmp += n;
        }
        self operator-(difference_type n) const {
            self tmp = *this;
            return tmp -= n;
        }
        reference operator[](difference_type n) const { return *(*this + n); }
        bool operator==(const self& x) const { return cur == x.cur; }
        bool operator!=(const self& x) const { return !(*this == x); }
        bool operator<(const self& x) const {
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }
    };

    // BufSiz 默认值为0 是为了避开编译器处理常数算式的bug
    template <typename T, typename Alloc = alloc, size_t BufSiz = 0>
    class deque {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
    protected:
        typedef simple_alloc<value_type, Alloc> data_allocator;
        typedef simple_alloc<pointer, Alloc> map_allocator;

        typedef pointer* map_pointer;
        iterator start;
        iterator finish;
        map_pointer map;
        size_type map_size;
        static size_type buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }
        static size_type initial_map_size() { return 8; }
    public:
        deque() : start(), finish(), map(0), map_size(0) { create_map_and_nodes(0); }
        deque(int n, const value_type& value) : start(), finish(), map(0), map_size(0) {
            fill_initialize(n, value);
        }
        ~deque() {
            destroy(start, finish);
            destroy_map_and_nodes();
        }

        iterator begin() { return start; }
        iterator end() { return finish; }

        reference operator[](size_type n) { return start[difference_type(n)]; }
        reference front() { return *start; }
        reference back() { return *(finish - 1); }
        size_type size() const { return size_type(finish - start); }
        // size_type 是 unsigned 取 -1为最大
        size_type max_size() const { return size_type(-1); }
        bool empty() const { return finish == start; }

        void push_back(const value_type& value) {
            if (finish.cur != finish.last - 1) {
                // 还有一个以上空间时
                construct(finish.cur, value);
                ++finish.cur;
            } else {
                // 只剩一个或者没空间了
                push_back_aux(value);
            }
        }
        void push_front(const value_type& value) {
            if (start.cur != start.first) {
                construct(start.cur - 1, value);
                --start.cur;
            } else {
                // 没有备用空间了
                push_front_aux(value);
            }
        }
        // finish 的 cur 和 一般的节点的 cur 不一样 它指向下一个可能被构造的节点
        void pop_back() {
            if (finish.cur != finish.first) {
                --finish.cur;
                destroy(finish.cur);
            } else {
                pop_back_aux();
            }
        }
        void pop_front() {
            if (start.cur != start.last - 1) {
                // 意味着 这一个缓冲区有一个或多个元素 可以随便删除 就要释放掉 节点内存
                destroy(start.cur);
                ++start.cur;
            } else {
                pop_front_aux();
            }
        }

        iterator erase(iterator pos) {
            iterator next = pos;
            ++next;
            difference_type index = pos - start;
            // 如果 pos之前的元素小于 size() 的一半 就移动前面的 元素 否则 移动后面的元素
            if (index < (size() >> 1)) {
                copy_backward(start, pos, next);
                pop_front();
            } else {
                copy(next, finish, pos);
                pop_back();
            }
            return start + index;
        }

        iterator insert(iterator position, const value_type& x) {
            if (position.cur == start.cur) {
                push_front(x);
                return start;
            } else if (position.cur == finish.cur) {
                push_back(x);
                return --finish;
            } else {
                return insert_aux(position, x);
            }
        }
        iterator erase(iterator first, iterator last);

        void clear();
    protected:
        void create_map_and_nodes(size_type num_elements);
        void destroy_map_and_nodes(){
            for (map_pointer cur = start.node; cur <= finish.node; ++cur){
                deallocate_node(*cur);
            }
            map_allocator::deallocate(map, map_size);
        }

        void fill_initialize(size_type n, const value_type& value);
        pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
        void deallocate_node(pointer n) { data_allocator::deallocate(n, buffer_size()); }

        void push_back_aux(const value_type& value);
        void push_front_aux(const value_type& value);
        void pop_back_aux();
        void pop_front_aux();

        iterator insert_aux(iterator pos, const value_type& x);

        void reserve_map_at_back(size_type nodes_to_add = 1) {
            if (nodes_to_add + 1 > map_size - (finish.node - map)) {
                // 尾端 节点备用空间不足 才配置一个更大的map 然后拷贝过去
                reallocate_map(nodes_to_add, false);
            }
        }
        void reserve_map_at_front(size_type nodes_to_add = 1) {
            if (nodes_to_add > start.node - map) {
                reallocate_map(nodes_to_add, true);
            }
        }
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
    };

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::create_map_and_nodes(size_type num_elements) {
        // 如果刚好整除 会多出一个 节点
        size_type num_nodes = num_elements / buffer_size() + 1;
        // 一个map 至少管理 8 个 节点 最多为 节点数 + 2 前后各多留一个
        map_size = std::max(initial_map_size(), num_nodes + 2);
        map = map_allocator::allocate(map_size);

        // 取中间 使得前后扩展能力一样大
        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;
        map_pointer cur;

        for (cur = nstart; cur <= nfinish; ++cur) {
            *cur = allocate_node();
        }
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        // 刚好整除时 前面会多配 一个节点 此时 finish 就指向多的那个节点的 first 的位置
        finish.cur = finish.first + num_elements % buffer_size();
    }

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type &value) {
        // 先将 deque 结构构建好
        create_map_and_nodes(n);
        map_pointer cur;
        for (cur = start.node; cur < finish.node; ++cur) {
            uninitialized_fill(*cur, *cur + buffer_size(), value);
        }
        // 单独为最后一个 节点构造 因为最后一个 有可能有备用空间 不需要初始值
        uninitialized_fill(finish.first, finish.cur, value);
    }

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::push_back_aux(const value_type &value) {
        value_type value_copy = value;
        reserve_map_at_back();
        *(finish.node + 1) = allocate_node();
        try {
            construct(finish.cur, value_copy);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        } catch (...) {
            deallocate_node(*(finish.node + 1));
        }
    }

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::push_front_aux(const value_type &value) {
        value_type value_copy = value;
        reserve_map_at_front();
        *(start.node - 1) = allocate_node();
        try {
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, value_copy);
        } catch (...) {
            start.set_node(start.node + 1);
            start.cur = start.first;
            deallocate_node(*(start.node - 1));
            throw;
        }
    }

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_nstart;
        // 本来的空间就大过2倍新的所需要的空间 只需要移动下位置 接着用就是了
        if (map_size > 2 * new_num_nodes) {
            new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            if (new_nstart < start.node) {
                copy(start.node, finish.node + 1, new_nstart);
            } else {
                copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
            }
        } else {
            // 不够了 配置新map
            size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2 + (add_at_front ? nodes_to_add : 0);
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            // 将原 map 拷贝过来
            copy(start.node, finish.node + 1, new_nstart);
            // 释放掉原来的 map
            map_allocator::deallocate(map, map_size);
            // 重新设置 map
            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num_nodes - 1);
    }

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::pop_back_aux() {
        deallocate_node(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destroy(finish.cur);
    }

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::pop_front_aux() {
        destroy(start.cur);
        deallocate_node(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    template <typename T, typename Alloc, size_t BufSiz>
    void deque<T, Alloc, BufSiz>::clear() {
        // 对除了头尾的节点进行析构和释放
        for (map_pointer node = start.node + 1; node < finish.node; ++node) {
            destroy(*node, *node + buffer_size());
            data_allocator::deallocate(*node, buffer_size());
        }
        // 至少有头和尾两个节点
        if (start.node != finish.node) {
            destroy(start.cur, start.last);
            destroy(finish.first, finish.cur);
            // 释放尾缓存区 保留头缓存区
            data_allocator::deallocate(finish.first, buffer_size());
        } else {
            destroy(start.cur, finish.cur);
            // 保留唯一的缓存区
        }
        finish = start;
    }

    template <typename T, typename Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::erase(iterator first, iterator last) {
        // 相当于是从最前面到最后面
        if (first == start && last == finish) {
            clear();
            return finish;
        } else {
            difference_type n = last - first;
            difference_type elems_before = first - start;
            if (elems_before < (size() - n) / 2) {
                copy_backward(start, first, last);
                iterator new_start = start + n;
                destroy(start, new_start);
                for (map_pointer cur = start.node; cur < new_start.node; ++cur) {
                    data_allocator::deallocate(*cur, buffer_size());
                }
                start = new_start;
            } else {
                copy(last, finish, first);
                iterator new_finish = finish - n;
                destroy(new_finish, finish);
                for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur) {
                    data_allocator::deallocate(*cur, buffer_size());
                }
                finish = new_finish;
            }
            return start + elems_before;
        }
    }

    template <typename T, typename Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert_aux(iterator pos, const value_type &x) {
        difference_type index = pos - start;
        value_type x_copy = x;
        if (index < size() / 2) {
            push_front(front());
            iterator front1 = start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            pos = start + index;
            iterator pos1 = pos;
            ++pos1;
            copy(front2, pos1, front1);
        } else {
            push_back(back());
            iterator back1 = finish;
            --back1;
            iterator back2 = back1;
            --back2;
            pos = start + index;
            copy_backward(pos, back2, back1);
        }
        *pos = x_copy;
        return pos;
    }

};
#endif //STLTOYS_YY_DEQUE_H
