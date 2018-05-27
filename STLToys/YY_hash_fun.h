//
//  YY_hash_fun.h
//  STLToys
//
//  Created by ntian on 2018/5/27.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_HASH_FUN_H
#define STLTOYS_YY_HASH_FUN_H

#include <cstddef>

namespace YY {

    template <typename Key>
    struct Hash {};

    // 针对 const char* 和 char*  进行转换 其他均返回原值
    inline size_t __stl_hash_string(const char* s) {
        unsigned long h = 0;
        for (; *s; ++s) {
            h = 5 * h + *s;
        }
        return size_t(h);
    }

    template <>
    struct Hash<char*> {
        size_t operator()(const char* s) const { return __stl_hash_string(s); }
    };
    template <>
    struct Hash<const char*> {
        size_t operator()(const char* s) const { return __stl_hash_string(s); }
    };

    template <>
    struct Hash<char> {
        size_t operator()(char x) const { return x; }
    };
    template <>
    struct Hash<unsigned char> {
        size_t operator()(unsigned char x) const { return x; }
    };
    template <>
    struct Hash<signed char> {
        size_t operator()(unsigned char x) const { return x; }
    };
    template <>
    struct Hash<short> {
        size_t operator()(short x) const { return x; }
    };
    template <>
    struct Hash<unsigned short> {
        size_t operator()(unsigned short x) const { return x; }
    };
    template <>
    struct Hash<int> {
        size_t operator()(int x) const { return x; }
    };
    template <>
    struct Hash<unsigned int> {
        size_t operator()(unsigned int x) const { return x; }
    };
    template <>
    struct Hash<long> {
        size_t operator()(long x) const { return x; }
    };
    template <>
    struct Hash<unsigned long> {
        size_t operator()(unsigned long x) const { return x; }
    };
}

#endif //STLTOYS_YY_HASH_FUN_H
