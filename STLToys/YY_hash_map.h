//
//  YY_hash_map.h
//  STLToys
//
//  Created by ntian on 2018/5/28.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_HASH_MAP_H
#define STLTOYS_YY_HASH_MAP_H

#include "YY_alloc.h"
#include "YY_function.h"
#include "YY_hash_fun.h"
#include "YY_hashtable.h"

namespace YY {

    template <typename Key, typename T, typename HashFcn = Hash<Key>,
            typename EqualKey = equal_to<Key>, typename Alloc = alloc>
    class hash_map {
    private:
        typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
        ht rep;
    public:
        typedef typename ht::key_type key_type;
        typedef T data_type;
        typedef T mapped_type;
        typedef typename ht::value_type value_type;
        typedef typename ht::hasher hasher;
        typedef typename ht::key_equal key_equal;
        typedef typename ht::size_type size_type;
        typedef typename ht::difference_type difference_type;
        typedef typename ht::const_reference reference;
        typedef typename ht::const_reference const_reference;
        typedef typename ht::const_pointer pointer;
        typedef typename ht::const_pointer const_pointer;
        // set 的 iterator 都应该是 const_iterator 因为它的键值 就是 它的实值
        typedef typename ht::iterator iterator;
        typedef typename ht::iterator const_iterator;
        hasher hash_funct() const { return rep.hash_funct(); }
        key_equal key_eq() const { return rep.key_eq(); }
    public:
        hash_map() : rep(100, hasher(), key_equal()) {}
        explicit hash_map(size_type n) : rep(n, hasher(), key_equal()) {}
        hash_map(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
        hash_map(size_type n, const hasher& hf, const key_equal & eql) : rep(n, hf, eql) {}

        template <typename InputIterator>
        hash_map(InputIterator first, InputIterator last) : rep(100, hasher(), key_equal()) {
            rep.insert_unique(first, last);
        }
        template <typename InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n) : rep(n, hasher(), key_equal()) {
            rep.insert_unique(first, last);
        }
        template <typename InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n, const hasher& hf) : rep(n, hf, key_equal()) {
            rep.insert_unique(first, last);
        }
        template <typename InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) {
            rep.insert_unique(first, last);
        }
    public:
        size_type size() const { return rep.size(); }
        size_type max_size() const { return rep.max_size(); }
        bool empty() const { return rep.empty(); }
        iterator begin() { return rep.begin(); }
        iterator end() { return rep.end(); }
        iterator find(const key_type& key) { return rep.find(key); }
        size_type count(const key_type& key) { return rep.count(key); }
        void clear() { rep.clear(); }
        void resize(size_type hint) { rep.resize(hint); }
        size_type bucket_count() { return rep.bucket_count(); }
        size_type max_bucket_count() { return rep.max_bucket_count(); }
        pair<iterator, bool> insert(const value_type& obj) {
            return rep.insert_unique(obj);
        }
        template <typename InputIterator>
        void insert(InputIterator first, InputIterator last) { rep.insert_unique(first, last); }
        pair<iterator, bool> insert_noresize(const value_type& obj) {
            return rep.insert_unique_noresize(obj);
        }
        T& operator[](const key_type& key) {
            return rep.find_or_insert(value_type(key, T())).second;
        }
    };

}

#endif //STLTOYS_YY_HASH_MAP_H
