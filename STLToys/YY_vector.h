//
//  YY_vector.h
//  STLToys
//
//  Created by ntian on 2018/5/3.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_VECTOR_H
#define STLTOYS_YY_VECTOR_H

#include "YY_alloc.h"
#include "YY_uninitialized.h"
#include "YY_algobase.h"

namespace YY {

    // 默认使用第二级配置器
    template <typename T, typename Alloc = alloc>
    class vector {
    public:
        typedef T           value_type;
        typedef value_type* pointer;
        typedef value_type* iterator;
        typedef value_type& reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

    protected:
        // 总是进入 iterator_traits<Iterator> 偏特化 所以只定义 typedef value_type* iterator
        // 使用经过包装的 simple_alloc 作为空间配置器
        typedef simple_alloc<value_type, Alloc> data_allocator;
        iterator start;
        iterator finish;
        iterator end_of_storage;
        void insert_aux(iterator position, const T& x);
        void deallocate() {
            if (start) {
                data_allocator::deallocate(start, end_of_storage - start);
            }
        }
        iterator allocate_and_fill(size_type n, const T& x) {
            iterator result = data_allocator::allocate(n);
            uninitialized_fill_n(result, n, x);
            return result;
        }
        void fill_initalize(size_type n, const T& x) {
            start = allocate_and_fill(n, x);
            finish = start + n;
            end_of_storage = finish;
        }
        template <typename InputIterator>
        void range_initialize(InputIterator first, InputIterator last, input_iterator_tag) {
            for (; first < last; ++first) {
                push_back(*first);
            }
        }
        template <typename ForwardIterator>
        iterator allocate_and_copy(size_type n, ForwardIterator first, ForwardIterator last) {
            iterator result = data_allocator::allocate(n);
            try {
                uninitialized_copy(first, last, result);
                return result;
            } catch (...) {
                data_allocator::deallocate(result, n);
            }
        }
    public:
        iterator begin() { return start; }
        iterator end() { return finish; }

        // 以下三个成员方法应该有 const 的 但是因为 begin() 和 end() 没有 const 版本 所以去掉了
        // 对比了自己手头上的 vector 里面还应该有一个 const_iterator
        size_type size() { return size_type(end() - begin()); }
        size_type capacity() { return size_type(end_of_storage - begin()); }
        bool empty() { return begin() == end(); }

        reference operator[](size_type n) { return *(begin() + n); }
        reference front() { return *begin(); }
        reference back() { return *(end() - 1); }

        vector() : start(0), finish(0), end_of_storage(0) {}
        vector(size_type n, const T& value) { fill_initalize(n, value); }
        vector(int n, const T& value) { fill_initalize(n, value); }
        vector(long n, const T& value) { fill_initalize(n, value); }
        explicit vector(size_type n) { fill_initalize(n, T()); }

        // 效率不高 没有针对 ForwardIterator 进行优化
        template <typename InputIterator>
        vector(InputIterator first, InputIterator last) : start(0), finish(0), end_of_storage(0) {
            range_initialize(first, last, iterator_category(first));
        }

        ~vector() {
            destroy(start, finish);
            deallocate();
        }
        void push_back(const T& x) {
            // 还有备用空间则直接构造 不够空间 就新建一个 更大的vector 然后将其拷贝过去
            if (finish != end_of_storage) {
                construct(finish, x);
                ++finish;
            } else {
                insert_aux(end(), x);
            }
        }
        void pop_back() {
            --finish;
            destroy(finish);
        }
        void insert(iterator position, size_type n, const T& x);
        iterator erase(iterator first, iterator last) {
            // 先暂时使用 std 里的函数
            // copy 函数 将 last-finish 挪到 first-(finish - first + 1)
            iterator i = copy(last, finish, first);
            destroy(i, finish);
            finish = finish - (last - first);
            return first;
        }
        iterator erase(iterator position) {
            if (position + 1 != end()) {
                copy(position + 1, finish, position);
            }
            --finish;
            destroy(finish);
            return position;
        }
        void resize(size_type new_size, const T& x) {
            if (new_size < size()) {
                erase(begin() + new_size, end());
            } else {
                insert(end(), new_size - size(), x);
            }
        }
        void resize(size_type new_size) { resize(new_size, T()); }
        void reserve(size_type n) {
            if (capacity() >= n) { return; }
            const size_type old_size = size();
            iterator tmp = allocate_and_copy(n, start, finish);
            destroy(start, finish);
            deallocate();
            start = tmp;
            finish = tmp + old_size;
            end_of_storage = start + n;
        }
        void swap(vector& x) {
            YY::swap(start, x.start);
            YY::swap(finish, x.finish);
            YY::swap(end_of_storage, x.end_of_storage);
        }
        void clear() { erase(begin(), end()); }
    };

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const T &x) {
        if (finish != end_of_storage) {
            construct(finish, *(finish - 1));
            ++finish;
            T x_copy = x;
            // 反向拷贝 eg: 0 2 3 4 5 要将1插入1的位置
            // 先 构造一个与当前元素值相同的元素 得到 0 2 3 4 5 5
            // 将 2 3 4 翻转 得到 4 3 2 插入到 5 的位置
            // 得到 0 * 2 3 4 5 此时 再给迭代器赋值即插入
            // FIXME: 以后将 std::copy_backward() 替换掉
            std::copy_backward(position, finish - 2, finish - 1);
            *position = x_copy;
        } else {
            // 没有备用空间了
            const size_type old_size = size();
            // 原空间为 0 时 配置1个元素大小的空间 否则 配置 之前空间的两倍
            // 其中前半段用来放置原数据 后半段用来放置新数据
            const size_type len = old_size != 0 ? 2 * old_size : 1;
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                // 将原vector的备用空间内容也拷贝过来 虽然并不知道有什么用?
                new_finish = uninitialized_copy(position, finish, new_finish);
            } catch (...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            // 析构并释放原vector
            destroy(begin(), end());
            deallocate();
            // 调整迭代器指向新vector
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    // 从 position 开始 插入 n 个元素
    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert(iterator position, size_type n, const T &x) {
        if (n == 0) { return; }
        // 备用空间充足
        if (size_type(end_of_storage - finish) >= n) {
            T x_copy = x;
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            if (elems_after > n) {
                // 插入点之后的已有元素 > 新增元素个数
                uninitialized_copy(finish - n, finish, finish);
                finish += n;
                // FIXME: 以后将 std::copy_backward()和 std::fill() 替换掉
                std::copy_backward(position, old_finish - n, old_finish);
                std::fill(position, position + n, x_copy);
            } else {
                // 插入点之后的已有元素 < 新增元素个数
                uninitialized_fill_n(finish, n - elems_after, x_copy);
                finish += n - elems_after;
                uninitialized_copy(finish - n, old_finish, finish);
                finish += elems_after;
                // FIXME: 以后将 std::fill() 替换掉
                std::fill(position, old_finish, x_copy);
            }
        } else {
            // 备用空间不足 需要额外配置空间
            // 新长度 旧长度两倍或者 旧+新增个数
            const size_type old_size = size();
            const size_type len = old_size + std::max(old_size, n);
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                // 拷贝 插入位置之前的元素
                new_finish = uninitialized_copy(start, position, new_start);
                // 填充要插入的元素
                new_finish = uninitialized_fill_n(new_finish, n, x);
                // 拷贝 插入位置之后的元素
                new_finish = uninitialized_copy(position, finish, new_finish);
            } catch (...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            destroy(start, finish);
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
}
#endif //STLTOYS_YY_VECTOR_H
