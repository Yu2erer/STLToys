//
//  YY_algobase.h
//  STLToys
//
//  Created by ntian on 2018/5/30.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_ALGOBASE_H
#define STLTOYS_YY_ALGOBASE_H

#include <cstring>
#include "YY_iterator.h"
#include "YY_pair.h"
#include "YY_type_traits.h"

namespace YY {

    template <typename InputIterator1, typename InputIterator2>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        for (;first1 != last1; ++first1, ++first2) {
            if (*first1 != *first2) {
                return false;
            }
        }
        return true;
    }
    template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
        for (;first1 != last1; ++first1, ++first2) {
            if (!binary_pred(*first1, *first2)) {
                return false;
            }
        }
        return true;
    }

    template <typename ForwardIterator, typename T>
    void fill(ForwardIterator first, ForwardIterator last, const T& value) {
        for (; first != last; ++first) {
            *first = value;
        }
    }

    template <typename OutputIterator, typename Size, typename T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
        for (; n > 0; --n, ++first) {
            *first = value;
        }
        return first;
    }

    template <typename ForwardIterator1, typename ForwardIterator2>
    inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
        typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
        *a = *b;
        *b = tmp;
    }

    template <typename T>
    inline const T& max(const T& a, const T& b) {
        return a < b ? b : a;
    }
    template <typename T>
    inline const T& min(const T& a, const T& b) {
        return a < b ? a : b;
    }

    // 以字典排列方式 对两个 序列进行比较
    template <typename InputIterator1, typename InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        for (;first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (*first1 < *first2) {
                return true;
            } else if (*first2 < *first1) {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }
    template <typename InputIterator1, typename InputIterator2, typename Compare>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
        for (;first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (comp(*first1, *first2)) {
                return true;
            } else if (comp(*first2, *first1)) {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }
    // 为了效率 SGI 针对const unsigned char* 和 const char* 特例化
    inline bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, min(len1, len2));
        return result != 0 ? result < 0 : len1 < len2;
    }

    // 返回两个序列之间的不同点
    template <typename InputIterator1, typename InputIterator2>
    pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        while (first1 != last1 && *first1 == *first2) {
            ++first1;
            ++first2;
        }
        return pair<InputIterator1, InputIterator2>(first1, first2);
    }
    template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
        while (first1 != last1 && binary_pred(*first1, *first2)) {
            ++first1;
            ++first2;
        }
        return pair<InputIterator1, InputIterator2>(first1, first2);
    }

    template <typename T>
    inline void swap(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
    inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
        for (Distance n = last - first; n > 0; --n, ++result, ++first) {
            *result = *first;
        }
        return result;
    }
    template <typename T>
    inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    template <typename T>
    inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
        return __copy_d(first, last, result, (ptrdiff_t*)0);
    }
    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
        for (; first != last; ++first) {
            *result = *first;
        }
        return result;
    }
    template <typename RandomAccessIterator, typename OutputIterator>
    inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
        return __copy_d(first, last, result, distance_type(first));
    }
    template <typename InputIterator, typename OutputIterator>
    struct __copy_dispatch {
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
            return __copy(first, last, result, iterator_category(first));
        }
    };
    template <typename T>
    struct __copy_dispatch<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            typedef typename __type_traits<T>::has_trivial_assignment_operator t;
            return __copy_t(first, last, result, t());
        }
    };
    template <typename T>
    struct __copy_dispatch<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            typedef typename __type_traits<T>::has_trivial_assignment_operator t;
            return __copy_t(first, last, result, t());
        }
    };
    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }
    // 针对 char* 和 wchar_t* 进行重载
    inline char* copy(const char* first, const char* last, char* result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }
    inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
        memmove(result, first, sizeof(wchar_t)  * (last - first));
        return result + (last - first);
    }

    template <typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        while (first != last) {
            *--result = *--last;
        }
        return result;
    }
    template <typename T>
    inline T* __copy_backward_t(const T* first, const T* last, T* result, __true_type) {
        const ptrdiff_t n = last - first;
        memmove(result - n, first, sizeof(T) * n);
        return result - n;
    }
    template <typename T>
    inline T* __copy_backward_t(const T* first, const T* last, T* result, __false_type) {
        return __copy_backward(first, last, result);
    }
    template <typename T>
    struct __copy_backward_dispatch<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            typedef typename __type_traits<T>::has_trivial_assignment_operator t;
            return __copy_backward_t(first, last, result, t());
        }
    };
    template <typename T>
    struct __copy_backward_dispatch<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            typedef typename __type_traits<T>::has_trivial_assignment_operator t;
            return __copy_backward_t(first, last, result, t());
        }
    };
    template <typename BidirectionalIterator1, typename BidirectionalIterator2>
    struct __copy_backward_dispatch {
        BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
            return __copy_backward(first, last, result);
        }
    };
    template <typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
    }

}

#endif //STLTOYS_YY_ALGOBASE_H
