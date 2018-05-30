//
//  YY_numeric.h
//  STLToys
//
//  Created by ntian on 2018/5/29.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_NUMERIC_H
#define STLTOYS_YY_NUMERIC_H

#include "YY_iterator.h"
#include "YY_function.h"

namespace YY {

    // 计算区间的和
    template <typename InputIterator, typename T>
    T accmulate(InputIterator first, InputIterator last, T init) {
        for (; first != last; ++first) {
            init = init + *first;
        }
        return init;
    }
    template <typename InputIterator, typename T, typename BinaryOperation>
    T accmulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
        for (; first != last; ++first) {
            init = binary_op(init, *first);
        }
        return init;
    }

    // 第一个为其本身 后面的计算n - n-1 的差值
    template <typename InputIterator, typename OutputIterator>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result){
        if (first == last) { return result; }
        *result = *first;
        typename iterator_traits<InputIterator>::value_type value = *first;
        while (++first != last) {
            iterator_traits<InputIterator>::value_type tmp = *first;
            *++result = tmp - value;
            value = tmp;
        }
        return ++result;
    }
    template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
    OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
        if (first == last) { return result; }
        *result = *first;
        typename iterator_traits<InputIterator>::value_type value = *first;
        while (++first != last) {
            iterator_traits<InputIterator>::value_type tmp = *first;
            *++result = binary_op(tmp, value);
            value = tmp;
        }
        return ++result;
    }

    // 两个序列内积
    template <typename InputIterator1, typename InputIterator2, typename T>
    T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
        for (; first1 != last1; ++first1, ++first2) {
            init = init + (*first1 * *first2);
        }
        return init;
    }
    template <typename InputIterator1, typename InputIterator2, typename T,
            typename BinaryOperation1, typename BinaryOperation2>
    T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
        for (; first1 != last1; ++first1, ++first2) {
            init = binary_op1(init, (binary_op2(*first1, *first2)));
        }
        return init;
    }

    // 前n个元素的和
    template <typename InputIterator, typename OutputIterator>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
        if (first == last) { return result; }
        *result = *first;
        typename iterator_traits<InputIterator>::value_type value = *first;
        while (++first != last) {
            value = value + *first;
            *++result = value;
        }
        return ++result;
    }
    template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
    OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
        if (first == last) { return result; }
        *result = *first;
        typename iterator_traits<InputIterator>::value_type value = *first;
        while (++first != last) {
            value = binary_op(value, *first);
            *++result = value;
        }
        return ++result;
    }

    // 以下算法为 SGI 专属
    // 默认为乘幂
    template <typename T, typename Integer>
    inline T power(T x, Integer n) {
        return power(x, n, multiplies<T>());
    }
    // 自己对自己进行某种操作
    template <typename T, typename Integer, typename MonoidOperation>
    T power(T x, Integer n, MonoidOperation op) {
        if (n == 0) { return identity_element(op); }
        while ((n & 1) == 0) {
            n >>= 1;
            x = op(x, x);
        }
        T result = x;
        n >>= 1;
        while (n != 0) {
            x = op(x, x);
            if ((n & 1) != 0) {
                result = op(result, x);
            }
            n >>= 1;
        }
        return result;
    }

    // 在 [first, last) 填入 value, value + 1 , value + 2
    template <typename ForwardIterator, typename T>
    void iota(ForwardIterator first, ForwardIterator last, T value) {
        while (first != last) { *first++ = value++; }
    }
}

#endif //STLTOYS_YY_NUMERIC_H
