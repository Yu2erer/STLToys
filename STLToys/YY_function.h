//
//  YY_function.h
//  STLToys
//
//  Created by ntian on 2018/5/22.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_FUNCTION_H
#define STLTOYS_YY_FUNCTION_H

namespace YY {

    // STL 规定 每个 Adaptable unary function 都要继承他
    template <typename Arg, typename Result>
    struct unary_function {
        typedef Arg argument_type;
        typedef Result result_type;
    };

    template <typename Arg1, typename Arg2, typename Result>
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    // 算术类仿函数共 6 个 这里只写一个 加 减 乘 除 模取 否定
    template <typename T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x + y; }
    };

    // 证同元素 作为内部使用 返回加法类型的证同元素 0 因为 加0等于本身
    template <typename T>
    inline T identity_element(plus<T>) { return T(0); }

    // 关系运算符 这里只写三个 = != < <= >= >
    template <typename T>
    struct greater : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const { return x > y; }
    };
    template <typename T>
    struct less : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const { return x < y; }
    };
    template <typename T>
    struct equal_to : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const { return x == y; }
    };

    // 逻辑运算符 只写一个 and or not
    template <typename T>
    struct logical_and : public binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const { return x && y; }
    };

    // identity 证同 返回自身 在 set 中使用 因为set键值就是实值
    template <typename T>
    struct identity : public unary_function<T, T> {
        const T& operator()(const T& x) const { return x; }
    };

    // 选择函数
    template <typename Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type> {
        const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
    };
    template <typename Pair>
    struct select2nd : public unary_function<Pair, typename Pair::second_type> {
        const typename Pair::second_type& operator()(const Pair& x) const { return x.second; }
    };

    // 投射函数 SGI STL 并没有使用过 这里只写一个
    template <typename Arg1, typename Arg2>
    struct project1st : public binary_function<Arg1, Arg2, Arg1> {
        Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
    };
}

#endif //STLTOYS_YY_FUNCTION_H
