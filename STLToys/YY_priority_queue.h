//
//  YY_priority_queue.h
//  STLToys
//
//  Created by ntian on 2018/5/16.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_PRIORITY_QUEUE_H
#define STLTOYS_YY_PRIORITY_QUEUE_H

#include "YY_vector.h"
#include "YY_heap.h"
#include "YY_function.h"

namespace YY {

    // 这里只实现了 最大堆的优先队列
    template <typename T, typename Sequence = vector<T> , typename Compare = less<typename Sequence::value_type>>
    class priority_queue {
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
    protected:
        Sequence c;
        Compare comp;
    public:
        priority_queue() : c() {}
        explicit priority_queue(const Compare& x) : c(), comp(x) {}

        template <typename InputIterator>
        priority_queue(InputIterator first, InputIterator last) : c(first, last) {
            make_heap(c.begin(), c.end(), comp);
        }
        template <typename InputIterator>
        priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x) {
            make_heap(c.begin(), c.end(), comp);
        }

        bool empty() { return c.empty(); }
        size_type size() { return c.size(); }
        reference top() { return c.front(); }
        void push(const value_type& x) {
            try {
                c.push_back(x);
                push_heap(c.begin(), c.end(), comp);
            } catch(...) {
                c.clear();
            }
        }
        void pop() {
            try {
                pop_heap(c.begin(), c.end(), comp);
                c.pop_back();
            } catch (...) {
                c.clear();
            }
        }
    };
}

#endif //STLTOYS_YY_PRIORITY_QUEUE_H
