//
//  YY_hashtable.h
//  STLToys
//
//  Created by ntian on 2018/5/24.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_HASHTABLE_H
#define STLTOYS_YY_HASHTABLE_H

#include "YY_alloc.h"
#include "YY_iterator.h"
#include "YY_vector.h"
#include "YY_pair.h"

namespace YY {

    template <typename Value>
    struct __hashtable_node {
        __hashtable_node* next;
        Value val;
    };

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey,
            typename EqualKey, typename Alloc>
    class hashtable;

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey,
            typename EqualKey, typename Alloc>
    struct __hashtable_const_iterator;

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey,
            typename EqualKey, typename Alloc>
    struct __hashtable_iterator {

        typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
        typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
        typedef __hashtable_node<Value> node;

        typedef forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef value_type& reference;
        typedef value_type* pointer;

        node *cur;
        hashtable* ht;

        __hashtable_iterator() {}
        __hashtable_iterator(node* n, hashtable* tab) : cur(n), ht(tab) {}

        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }

        iterator& operator++() {
            const node* old = cur;
            cur = cur->next;
            if (!cur) {
                size_type bucket = ht->bkt_num(old->val);
                while (!cur && ++bucket < ht->bucket_count()) {
                    cur = ht->buckets[bucket];
                }
            }
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        bool operator==(const iterator& it) const { return cur == it.cur; }
        bool operator!=(const iterator& it) const { return cur != it.cur; }
    };

    // 其实不管是什么数字都行 但是 作者认为 质数能减少碰撞的概率 成大概两倍增长
    static const int __stl_num_primes = 28;
    static const unsigned long __stl_prime_list[__stl_num_primes] = {
            53,         97,           193,         389,       769,
            1543,       3079,         6151,        12289,     24593,
            49157,      98317,        196613,      393241,    786433,
            1572869,    3145739,      6291469,     12582917,  25165843,
            50331653,   100663319,    201326611,   402653189, 805306457,
            1610612741, 3221225473ul, 4294967291ul
    };
    // 找出最接近n的质数
    inline unsigned long __stl_next_prime(unsigned long n) {
        const unsigned long* first = __stl_prime_list;
        const unsigned long* last = __stl_prime_list + __stl_num_primes;
        // FIXME: lower_bound 未实现
        const unsigned long* pos = std::lower_bound(first, last, n);
        return pos == last ? *(last - 1) : *pos;
    }

    template <typename Value, typename Key, typename HashFcn, typename ExtractKey,
            typename EqualKey, typename Alloc = alloc>
    class hashtable {
    public:
        typedef HashFcn hasher;
        typedef EqualKey key_equal;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Key key_type;
        typedef Value value_type;
        typedef Value& reference;
        typedef const Value& const_reference;
        typedef Value* pointer;
        typedef const Value* const_pointer;
        typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        friend struct
                __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
    private:
        hasher hash;
        key_equal equals;
        ExtractKey get_key;

        typedef __hashtable_node<Value> node;
        typedef simple_alloc<node, Alloc> node_allocator;
        vector<node*, Alloc> buckets;
        size_type num_elements;
    public:
        size_type bucket_count() { return buckets.size(); }
        size_type max_bucket_count() const { return __stl_prime_list[__stl_num_primes - 1]; }
        size_type size() const { return num_elements; }
        size_type max_size() const { return size_type(-1); }

        iterator begin() {
            for (size_type n = 0; n < buckets.size(); ++n) {
                if (buckets[n]) { return iterator(buckets[n], this); }
            }
            return end();
        }
        iterator end() { return iterator(0, this); }
        hasher hash_funct() const { return hash; }
        key_equal key_eq() const { return equals; }
        bool empty() const { return size() == 0; }
        void resize(size_type num_elements_hint);
    private:
        node* new_node(const value_type& obj) {
            node* n = node_allocator::allocate();
            n->next = 0;
            try {
                construct(&n->val, obj);
                return n;
            } catch(...) {
                node_allocator::deallocate(n);
            }
        }
        void delete_node(node *n) {
            destroy(&n->val);
            node_allocator::deallocate(n);
        }
        size_type next_size(size_type n) const { return __stl_next_prime(n); }
        void initialize_buckets(size_type n) {
            const size_type n_buckets = next_size(n);
            buckets.reserve(n);
            buckets.insert(buckets.end(), n_buckets, (node*)0);
            num_elements = 0;
        }

        size_type bkt_num(const value_type& obj, size_t n) { return bkt_num_key(get_key(obj), n); }
        size_type bkt_num(const value_type& obj) { return bkt_num_key(get_key(obj)); }
        size_type bkt_num_key(const key_type& key) { return bkt_num_key(key, buckets.size()); }
        size_type bkt_num_key(const key_type& key, size_t n) const { return hash(key) % n; }
    public:
        hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) : hash(hf),  equals(eql), get_key(ExtractKey()), num_elements(0) { initialize_buckets(n); }

        pair<iterator, bool> insert_unique(const value_type& obj) {
            resize(num_elements + 1);
            return insert_unique_noresize(obj);
        }
        iterator insert_equal(const value_type& obj) {
            resize(num_elements + 1);
            return insert_equal_noresize(obj);
        }
        pair<iterator, bool> insert_unique_noresize(const value_type& obj);
        iterator insert_equal_noresize(const value_type& obj);

        iterator find(const key_type& key) {
            size_type n = bkt_num_key(key);
            node* first;
            for (first = buckets[n];first && !equals(get_key(first->val), key);first = first->next) {}
            return iterator(first, this);
        }
        size_type count(const key_type& key) {
            size_type n = bkt_num_key(key);
            size_type result = 0;
            for (const node* cur =  buckets[n]; cur; cur = cur->next) {
                if (equals(get_key(cur->val), key)) { ++result; }
            }
            return result;
        }

        void clear();
        void copy_from(const hashtable& ht);
        reference find_or_insert(const value_type& obj);

        template <typename InputIterator>
        void insert_unique(InputIterator first, InputIterator last) {
            insert_unique(first, last, iterator_category(first));
        }
        template <typename InputIterator>
        void insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
            for(;first != last; ++first) { insert_unique(*first); }
        }
        template <typename ForwardIterator>
        void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
            size_type n = 0;
            distance(first, last, n);
            resize(num_elements + n);
            for (; n > 0; --n, ++first) { insert_unique_noresize(*first); }
        }
        template <typename InputIterator>
        void insert_equal(InputIterator first, InputIterator last) {
            insert_equal(first, last, iterator_category(first));
        }
        template <typename InputIterator>
        void insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
            for(;first != last; ++first) { insert_equal(*first); }
        }
        template <typename ForwardIterator>
        void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
            size_type n = 0;
            distance(first, last, n);
            resize(num_elements + n);
            for (; n > 0; --n, ++first) { insert_equal_noresize(*first); }
        }
    };

    template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
    void hashtable<V, K, HF, Ex, Eq, A>::resize(size_type num_elements_hint) {
        const size_type old_n = buckets.size();
        // 将 buckets list 与 buckets vector 作比较 只要 > 就重建表格
        if (num_elements_hint <= old_n) { return; }

        const size_type n = next_size(num_elements_hint);
        if (n <= old_n) { return; }

        vector<node*, A> tmp(n, (node*)0);
        for (size_type bucket = 0; bucket < old_n; ++bucket) {
            node* first = buckets[bucket];
            while (first) {
                // 先找节点落在哪一个新buckets
                size_type new_bucket = bkt_num(first->val, n);
                // 指向下一个 为了迭代
                buckets[bucket] = first->next;
                // 使其 插入到新buckets 并 成为对应链表的头部
                first->next = tmp[new_bucket];
                tmp[new_bucket] = first;
                // 迭代处理下一个节点
                first = buckets[bucket];
            }
        }
        buckets.swap(tmp);
    }

    template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
    pair<typename hashtable<V, K, HF, Ex, Eq, A>::iterator, bool> hashtable<V, K, HF, Ex, Eq, A>::insert_unique_noresize(const value_type &obj) {
        const size_type n = bkt_num(obj);
        node* first = buckets[n];
        for (node* cur = first; cur; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj))) {
                return pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return pair<iterator, bool>(iterator(tmp, this), true);
    }
    template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
    typename hashtable<V, K, HF, Ex, Eq, A>::iterator hashtable<V, K, HF, Ex, Eq, A>::insert_equal_noresize(const value_type &obj) {
        const size_type n = bkt_num(obj);
        node* first = buckets[n];
        for (node* cur = first; cur; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj))) {
                node* tmp = new_node(obj);
                tmp->next = cur->next;
                cur->next = tmp;
                ++num_elements;
                return iterator(tmp, this);
            }
        }
        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return iterator(tmp, this);
    }

    template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
    void hashtable<V, K, HF, Ex, Eq, A>::clear() {
        for (size_type i = 0; i < buckets.size(); ++i) {
            node* cur = buckets[i];
            while (cur) {
                node* next = cur->next;
                delete_node(cur);
                cur = next;
            }
            buckets[i] = 0;
        }
        num_elements = 0;
    }

    template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
    void hashtable<V, K, HF, Ex, Eq, A>::copy_from(const hashtable &ht) {
        buckets.clear();
        buckets.reserve(ht.buckets.size());
        buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
        try {
            for (size_type i = 0; i < ht.buckets.size(); ++i) {
                if (const node *cur = ht.buckets[i]) {
                    node *copy = new_node(cur);
                    buckets[i] = copy;
                    for (node *next = cur->next; next; next = cur->next) {
                        copy->next = new_node(next->val);
                        copy = copy->next;
                    }
                }
            }
            num_elements = ht.num_elements;
        } catch (...) {
            clear();
        }
    }

    template <typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
    typename hashtable<V, K, HF, Ex, Eq, A>::reference hashtable<V, K, HF, Ex, Eq, A>::find_or_insert(const value_type &obj) {
        resize(num_elements + 1);
        size_type n = bkt_num(obj);
        node* first = buckets[n];
        for (node* cur = first; cur; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj))) {
                return cur->val;
            }
        }
        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return tmp->val;
    }

}

#endif //STLTOYS_YY_HASHTABLE_H
