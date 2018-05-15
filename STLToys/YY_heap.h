//
//  YY_heap.h
//  STLToys
//
//  Created by ntian on 2018/5/15.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_HEAP_H
#define STLTOYS_YY_HEAP_H

#include "YY_iterator.h"

namespace YY {


    template <typename RandomAccessIterator, typename Distance, typename T>
    void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
        Distance parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && *(first + parent) < value) {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
        __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
    }

    // STL 中并不讲heap单独做成一个容器 而是作为一个幕后英雄 为 优先队列 priority queue 的助手
    // 此组算法前提是 新元素已经被插入到 底部容器的最尾端
    template <typename RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __push_heap_aux(first, last, distance_type(first), value_type(first));
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
        Distance topIndex = holeIndex;
        Distance secondChild = 2 * holeIndex + 2;
        while (secondChild < len) {
            if (*(first + secondChild) < *(first + (secondChild - 1))) {
                secondChild--;
            }
            *(first + holeIndex) = *(first + secondChild);
            holeIndex = secondChild;
            secondChild = 2 * (holeIndex + 1);
            // 右孩子 不存在 就搞左孩子
            if (secondChild == len) {
                *(first + holeIndex) = *(first + (secondChild - 1));
                holeIndex = secondChild - 1;
            }
        }
        __push_heap(first, holeIndex, topIndex, value);
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Distance*, T value) {
        *result = *first;
        __adjust_heap(first, Distance(0), Distance(last - first), value);
    }

    template <typename RandomAccessIterator, typename T>
    inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
        __pop_heap(first, last - 1, last - 1, distance_type(first), T(*(last - 1)));
    }

    template <typename RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __pop_heap_aux(first, last, value_type(first));
    }

    template <typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
        while (last - first > 1) {
            pop_heap(first, last--);
        }
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
        if (last - first < 2) { return; }
        Distance len = last - first;
        Distance parent = (len - 2) / 2;
        while (true) {
            __adjust_heap(first, parent, len, T(*(first + parent)));
            if (parent == 0) { return; }
            parent--;
        }
    }
    template <typename RandomAccessIterator>
    inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __make_heap(first, last, distance_type(first), value_type(first));
    }
}

#endif //STLTOYS_YY_HEAP_H
