//
//  YY_set.h
//  STLToys
//
//  Created by ntian on 2018/5/23.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_SET_H
#define STLTOYS_YY_SET_H

#include "YY_alloc.h"
#include "YY_function.h"
#include "YY_RBTree.h"

namespace YY {

    template <typename Key, typename Compare = less<Key>, typename Alloc = alloc>
    class set {
    public:
        typedef Key key_type;
        typedef Key value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
    private:
        typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
        rep_type t;
    public:
        typedef typename rep_type::const_pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::const_reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::const_iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;

        set() : t(Compare()) {}
        explicit set(const Compare& comp) : t(comp) {}

        template <typename InputIterator>
        set(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first, last); }

        template <typename InputIterator>
        set(InputIterator first, InputIterator last, const Compare& comp) : t(comp) {
            t.insert_unique(first, last);
        }

        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return t.key_comp(); }
        iterator begin() const { return t.begin(); }
        iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }
        size_type max_size() const { return t.max_size(); }

        pair<iterator, bool> insert(const value_type& x) {
            pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
            return pair<iterator, bool>(p.first, p.second);
        }

        template <typename InputIterator>
        void insert(InputIterator first, InputIterator last) { t.insert_unique(first, last); }

        iterator find(const key_type& x) { return t.find(x); }

        void clear() { t.clear(); }
    };
}

#endif //STLTOYS_YY_SET_H
