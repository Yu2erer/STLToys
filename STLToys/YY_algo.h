//
//  YY_algo.h
//  STLToys
//
//  Created by ntian on 2018/5/30.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_ALGO_H
#define STLTOYS_YY_ALGO_H

#include "YY_algobase.h"

namespace YY {

    // 这里是 对 两个区间取 并集 但是这个并集不是普通的并集 它允许元素的重复
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                *result = *first1;
                ++first1;
            } else if (*first2 < * first1) {
                *result = *first2;
                ++first2;
            } else {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        YY::copy(first2, last2, YY::copy(first1, last1, result));
    }

    // 取交集
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                ++first1;
            } else if (*first2 < *first1) {
                ++first2;
            } else {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    // 取差集
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                *result = *first1;
                ++first1;
                ++result;
            } else if (*first2 < *first1) {
                ++first2;
            } else {
                ++first1;
                ++first2;
            }
        }
        return YY::copy(first1, last1, result);
    }

    // 对称取差集
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                *result = *first1;
                ++first1;
                ++result;
            } else if (*first2 < * first1) {
                *result = *first2;
                ++first2;
                ++result;
            } else {
                ++first1;
                ++first2;
            }
        }
        return YY::copy(first2, last2, YY::copy(first1, last1, result));
    }

    template <typename ForwardIterator, typename T, typename Distance>
    ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
        Distance len = 0;
        distance(first, last, len);
        Distance half;
        ForwardIterator middle;
        while (len > 0) {
            half = len >> 1;
            middle = first;
            advance(middle, half);
            if (*middle < value) {
                first = middle;
                ++first;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        return first;
    }

    template <typename RandomAccessIterator, typename T, typename Distance>
    RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
        Distance len = last - first;
        Distance half;
        RandomAccessIterator middle;
        while (len > 0) {
            half = len >> 1;
            middle = first + half;
            if (*middle < value) {
                first = middle;
                ++first;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        return first;
    }

    // 返回 可以插入 value 的第一个位置
    // 版本2 传入 comp 就不写出了
    template <typename ForwardIterator, typename T>
    inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
        return __lower_bound(first, last, value, distance_type(first), iterator_category(first));
    }


    template <typename ForwardIterator, typename T, typename Distance>
    inline ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
        Distance len = 0;
        distance(first, last, len);
        Distance half;
        ForwardIterator middle;
        while (len > 0) {
            half = len >> 1;
            middle = first;
            advance(middle, half);
            if (value < *middle) {
                len = half;
            } else {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }
        return first;
    }

    template <typename RandomAccessIterator, typename T, typename Distance>
    RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
        Distance len = last - first;
        Distance half;
        RandomAccessIterator middle;
        while (len > 0) {
            half = len >> 1;
            middle = first + half;
            if (value < *middle) {
                len = half;
            } else {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }
        return first;
    }
    // 返回 可以插入 value 的 最后一个位置
    template <typename ForwardIterator, typename T>
    inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
        return __upper_bound(first, last, value, distance_type(first), iterator_category(first));
    }
}

#endif //STLTOYS_YY_ALGO_H
