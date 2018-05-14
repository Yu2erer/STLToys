//
//  YY_stack.h
//  STLToys
//
//  Created by ntian on 2018/5/14.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_STACK_H
#define STLTOYS_YY_STACK_H

#include "YY_deque.h"

namespace YY {

    // 此处如果不前置声明的话 就支持不了 模板友元全特化了
    template <class T, class Sequence>
    class stack;

    template <typename T, typename Sequence>
    bool operator==(const stack<T, Sequence> &x, const stack<T, Sequence> &y) {
        return x.c == y.c;
    }

    template <typename T, typename Sequence>
    bool operator<(const stack<T, Sequence> &x, const stack<T, Sequence> &y) {
        return x.c < y.c;
    }

    // 默认使用 deque 作为底层容器
    template <typename T, typename Sequence = deque<T>>
    class stack {
        // 对具体的T 指代全特化
        friend bool operator== <>(const stack&, const stack&);
        friend bool operator< <>(const stack&, const stack&);
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
    protected:
        Sequence c;
    public:
        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        reference top() { return c.back(); }
        void push(const value_type& x) { c.push_back(x); }
        void pop() { c.pop_back(); }
    };


}

#endif //STLTOYS_YY_STACK_H
