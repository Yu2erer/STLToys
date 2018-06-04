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

    // 算术类仿函数共 6 个 这里只写两个 加 减 乘 除 模取 否定
    template <typename T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x + y; }
    };
    template <typename T>
    struct multiplies : public binary_function<T, T, T> {
        T operator()(const T& x, const T& y) const { return x * y; }
    };

    // 证同元素 作为内部使用 返回加法类型的证同元素 0 因为 加0等于本身
    template <typename T>
    inline T identity_element(plus<T>) { return T(0); }
    template <typename T>
    inline T identity_element(multiplies<T>) { return T(1); }

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

    // not1 not2
    template <typename Predicate>
    class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
    protected:
        Predicate pred;
    public:
        explicit unary_negate(const Predicate& x) : pred(x) {}
        bool operator()(const typename Predicate::argument_type& x) const {
            return !pred(x);
        }
    };
    // 辅助函数 方便使用
    template <typename Predicate>
    inline unary_negate<Predicate> not1(const Predicate& pred) {
        return unary_negate<Predicate>(pred);
    }
    template <typename Predicate>
    class binary_negate : public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool> {
    protected:
        Predicate pred;
    public:
        explicit binary_negate(const Predicate& x) : pred(x) {}
        bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const {
            return !pred(x, y);
        }
    };
    template <typename Predicate>
    inline binary_negate<Predicate> not2(const Predicate& pred) {
        return binary_negate<Predicate>(pred);
    }

    // bind1st bind2nd
    template <typename Operation>
    class binder1st : public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {
    protected:
        Operation op;
        typename Operation::first_argument_type value;
    public:
        binder1st(const Operation& x, const typename Operation::first_argument_type& y) : op(x), value(y) {}
        typename Operation::result_type operator()(const typename Operation::second_argument& x) const {
            return op(value, x);
        }
    };
    template <typename Operation, typename T>
    inline binder1st<Operation> bind1st(const Operation& op, const T& x) {
        typedef typename Operation::first_argument_type arg1_type;
        return binder1st<Operation>(op, arg1_type(x));
    }
    template <typename Operation>
    class binder2nd : public unary_function<typename Operation::first_argument_type, typename Operation::result_type> {
    protected:
        Operation op;
        typename Operation::second_argument_type value;
    public:
        binder2nd(const Operation& x, const typename Operation::second_argument_type& y) : op(x), value(y) {}
        typename Operation::result_type operator()(const typename Operation::first_argument& x) const {
            return op(x, value);
        }
    };
    template <typename Operation, typename T>
    inline binder2nd<Operation> bind2st(const Operation& op, const T& x) {
        typedef typename Operation::second_argument_type arg2_type;
        return binder2nd<Operation>(op, arg2_type(x));
    }

    // compose1 compose2 STL 规范里没有这两个 是 SGI STL 特有的
    // h(x) = F(g(x))
    template <typename Operation1, typename Operation2>
    class unary_compose : public unary_function<Operation2::argument_type, Operation1::result_type> {
    protected:
        Operation1 op1;
        Operation2 op2;
    public:
        unary_compose(const Operation1& x, const Operation2& y) : op1(x), op2(y) {}
        typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
            return op1(op2(x));
        }
    };
    template <typename Operation1, typename Operation2>
    inline unary_compose<Operation1, Operation2> compose1(const Operation1& op1, const Operation2& op2) {
        return unary_compose<Operation1, Operation2>(op1, op2);
    }
    // h(x) = F(g1(x), g2(x))
    template <typename Operation1, typename Operation2, typename Operation3>
    class binary_compose : public unary_function<Operation2::argument_type, Operation1::result_type> {
    protected:
        Operation1 op1;
        Operation2 op2;
        Operation3 op3;
    public:
        binary_compose(const Operation1& x,const Operation2& y, const Operation3& z) : op1(x), op2(y), op3(z) {}
        typename Operation1::result_type operator()(const Operation2::argument_type& x) const {
            return op1(op2(x), op3(x));
        }
    };
    template <typename Operation1, typename Operation2, typename Operation3>
    inline binary_compose<Operation1, Operation2, Operation3> compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
        return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
    };

    template <typename Arg, typename Result>
    class pointer_to_unary_function : public unary_function<Arg, Result> {
    protected:
        Result (*ptr)(Arg);
    public:
        pointer_to_unary_function() {}
        explicit pointer_to_unary_function(Result (*x)(Arg)) : ptr(x) {}
        Result operator()(Arg x) const { return ptr(x); }
    };
    template <typename Arg, typename Result>
    inline pointer_to_unary_function<Arg, Result> ptr_fun(Result (*x)(Arg)) {
        return pointer_to_unary_function<Arg, Result>(x);
    }

    template <typename Arg1, typename Arg2, typename Result>
    class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result> {
    protected:
        Result (*ptr)(Arg1, Arg2);
    public:
        pointer_to_binary_function() {}
        explicit pointer_to_binary_function(Result (*x)(Arg1, Arg2)) : ptr(x) {}
        Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
    };
    template <typename Arg1, typename Arg2, typename Result>
    inline pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result (*x)(Arg1, Arg2)) {
        return pointer_to_binary_function<Arg1, Arg2, Result>(x);
    }

    template <typename S, typename T>
    class mem_fun_t : public unary_function<T*, S> {
    public:
        explicit mem_fun_t(S (T::*pf)()) : f(pf) {}
        S operator()(T* p) const { return (p->*f)(); }
    private:
        S (T::*f)();
    };
    template <typename S, typename T>
    class const_mem_fun_t : public unary_function<const T*, S> {
    public:
        explicit const_mem_fun_t(S (T::*pf)() const) : f(pf) {}
        S operator()(const T* p) const { return (p->*f)(); }
    private:
        S (T::*f)() const;
    };
    template <typename S, typename T>
    class mem_fun_ref_t : public unary_function<T, S> {
    public:
        explicit mem_fun_ref_t(S (T::*pf)()) : f(pf) {}
        S operator()(T& r) const { return (r.*f)(); }
    private:
        S (T::*f)();
    };
    template <typename S, typename T>
    class const_mem_fun_ref_t : public unary_function<const T, S> {
    public:
        explicit const_mem_fun_ref_t(S (T::*pf)() const) : f(pf) {}
        S operator()(const T& r) const { return (r.*f)(); }
    private:
        S (T::*f)() const;
    };
    template <typename S, typename T, typename A>
    class mem_fun1_t : public binary_function<T*, A, S> {
    public:
        explicit mem_fun1_t(S (T::*pf)(A)) : f(pf) {}
        S operator()(T* p, A x) const { return (p->*f)(x); }
    private:
        S (T::*f)(A);
    };
    template <typename S, typename T, typename A>
    class const_mem_fun1_t : public binary_function<const T*, A, S> {
    public:
        explicit const_mem_fun1_t(S (T::*pf)(A) const) : f(pf) {}
        S operator()(const T* p, A x) const { return (p->*f)(x); }
    private:
        S (T::*f)(A) const;
    };
    template <typename S, typename T, typename A>
    class mem_fun1_ref_t : public binary_function<T, A, S> {
    public:
        explicit mem_fun1_ref_t(S (T::*pf)(A)) : f(pf) {}
        S operator()(T& r, A x) const { return (r.*f)(x); }
    private:
        S (T::*f)(A);
    };
    template <typename S, typename T, typename A>
    class const_mem_fun1_ref_t : public binary_function<const T, A, S> {
    public:
        explicit const_mem_fun1_ref_t(S (T::*pf)(A)) : f(pf) {}
        S operator()(const T& r, A x) const { return (r.*f)(x); }
    private:
        S (T::*f)(A) const;
    };
    template <typename S, typename T>
    inline mem_fun_t<S, T> mem_fun(S (T::*f)()) {
        return mem_fun_t<S, T>(f);
    }
    template <typename S, typename T>
    inline const_mem_fun_t<S, T> mem_fun(S (T::*f)() const) {
        return const_mem_fun_t<S, T>(f);
    }
    template <typename S, typename T>
    inline mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)()) {
        return mem_fun_ref_t<S, T>(f);
    }
    template <typename S, typename T>
    inline const_mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)() const) {
        return const_mem_fun_ref_t<S, T>(f);
    }
    template <typename S, typename T, typename A>
    inline mem_fun1_t<S, T, A> mem_fun(S (T::*f)(A)) {
        return mem_fun1_t<S, T, A>(f);
    }
    template <typename S, typename T, typename A>
    inline const_mem_fun1_t<S, T, A> mem_fun(S (T::*f)(A) const) {
        return const_mem_fun1_t<S, T, A>(f);
    }
    template <typename S, typename T, typename A>
    inline mem_fun1_ref_t<S, T, A> mem_fun_ref(S (T::*f)(A)) {
        return mem_fun1_ref_t<S, T, A>(f);
    };
    template <typename S, typename T, typename A>
    inline const_mem_fun1_ref_t<S, T, A> mem_fun_ref(S (T::*f)(A)) {
        return const_mem_fun1_ref_t<S, T, A>(f);
    }
}

#endif //STLTOYS_YY_FUNCTION_H
