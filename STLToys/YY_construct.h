//
//  YY_construct.h
//  STLToys
//
//  Created by ntian on 2018/4/25.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_CONSTRUCT_H
#define STLTOYS_YY_CONSTRUCT_H

#include <new>
#include "YY_iterator.h"
#include "YY_type_traits.h"

namespace YY {

    template <typename T1, typename T2>
    inline void construct(T1 *p, const T2 &value) {
        new(p) T1(value);
    };

    template <typename T>
    inline void destroy(T* p) {
        p->~T();
    }

    // 判断 元素 value_type 类型型别 是否有 trivial destructor
    template <typename ForwardIterator, typename T>
    inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
        typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    };

    // 元素型别 有 non-trivial destructor
    template <typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
        for (; first < last; ++first) {
            destroy(&*first);
        }
    }

    // 元素型别 有 trivial destructor
    template <typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

    // 接受两个迭代器 设法找出元素的类型型别 进而利用 __type_traits<> 找最佳方案
    template <typename ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        __destroy(first, last, value_type(first));
    }

    inline void destroy(char*, char*) {}
    inline void destroy(wchar_t*, wchar_t*) {}

}
#endif //STLTOYS_YY_CONSTRUCT_H
