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

}

#endif //STLTOYS_YY_ALGO_H
