//
//  YY_uninitialized.h
//  STLToys
//
//  Created by ntian on 2018/4/26.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_UNINITIALIZED_H
#define STLTOYS_YY_UNINITIALIZED_H

#include "YY_construct.h"

namespace YY {

    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
        // FIXME: 应该使用高阶函数 fill_n() 来完成
        ForwardIterator cur = first;
        for (; n > 0; --n, ++cur) {
            construct(&*cur, x);
        }
        return cur;
    };
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
        ForwardIterator cur = first;
        for (; n > 0; --n, ++cur) {
            construct(&*cur, x);
        }
        return cur;
    };

    // POD = Plain old data 标量型别 必然拥有trival ctor/dtor/copy/assignment函数 对POD型别做更有效率的初值填写手法 而 non-POD 型别用保险安全的做法
    template <typename ForwardIterator, typename Size, typename T, typename T1>
    inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
        typedef typename __type_traits<T1>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, x, is_POD());
    }

    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
        return __uninitialized_fill_n(first, n, x, value_type(x));
    }

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
        // FIXME: 应该使用 copy()
        ForwardIterator cur = result;
        for (; first != last; ++first, ++cur) {
            construct(&*cur, *first);
        }
        return cur;
    }

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
        ForwardIterator cur = result;
        for (; first != last; ++first, ++cur) {
            construct(&*cur, *first);
        }
        return cur;
    }
    template <typename InputIterator, typename ForwardIterator, typename T, typename T1>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
        typedef typename __type_traits<T1>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, is_POD());
    }

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitilalized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        return __uninitialized_copy(first, last, result, value_type(result));
    }

    template <typename ForwardIterator, typename T, typename T1>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
        // FIXME: 应该使用 fill()
        ForwardIterator cur = first;
        for (; cur != last; ++cur) {
            construct(&*cur, x);
        }
    }
    template <typename ForwardIterator, typename T, typename T1>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
        ForwardIterator cur = first;
        for (; cur != last; ++cur) {
            construct(&*cur, x);
        }
    }

    template <typename ForwardIterator, typename T, typename T1>
    inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
        typedef typename __type_tratis<T1>::is_POD_type is_POD;
        __uninitialized_fill_aux(first, last, x, is_POD());
    }

    template <typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
        __uninitialized_fill(first, last, x);
    }
}
#endif //STLTOYS_YY_UNINITIALIZED_H
