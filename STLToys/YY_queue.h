//
//  YY_queue.h
//  STLToys
//
//  Created by ntian on 2018/5/14.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_QUEUE_H
#define STLTOYS_YY_QUEUE_H

#include "YY_deque.h"

namespace YY {

    template <typename T, typename Sequence>
    class queue;

    template <typename T, typename Sequence>
    bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
        return x.c == y.c;;
    }

    template <typename T, typename Sequence>
    bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
        return x.c < y.c;
    }

    template <typename T, typename Sequence = deque<T>>
    class queue {
        friend bool operator== <>(const queue&, const queue&);
        friend bool operator< <>(const queue&, const queue&);
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
    protected:
        Sequence c;
    public:
        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        reference front() { return c.front(); }
        reference back() { return c.back(); }
        void push(const value_type& x) { c.push_back(x); }
        void pop() { c.pop_front(); }
    };
}

#endif //STLTOYS_YY_QUEUE_H
