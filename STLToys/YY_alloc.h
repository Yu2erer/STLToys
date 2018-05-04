//
//  YY_alloc.h
//  STLToys
//
//  Created by ntian on 2018/4/25.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_ALLOC_H
#define STLTOYS_YY_ALLOC_H

#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)
#endif

namespace YY {
    // 这里 没有 template型别参数 而 inst 实际上根本没用到
    template <int inst>
    class __malloc_alloc_template {
    private:
        // oom = out of memory
        static void* oom_malloc(size_t);
        static void* oom_realloc(void *, size_t);
        static void (* __malloc_alloc_oom_handler)();
    public:
        static void* allocate(size_t n) {
            void *result = malloc(n);
            if (result == 0) { result = oom_malloc(n); }
            return result;
        }
        static void deallocate(void *p, size_t) {
            free(p);
        }
        static void* reallocate(void *p, size_t, size_t new_sz) {
            void *result = realloc(p, new_sz);
            if (result == 0) { result = oom_realloc(p, new_sz); }
            return result;
        }
        // 模仿 C++ set_new_handler
        static void (* set_malloc_handler(void(*f)()))() {
            void (* old)() = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return old;
        }
    };
    template <int inst>
    void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

    template <int inst>
    void * __malloc_alloc_template<inst>::oom_malloc(size_t n) {
        void (* my_malloc_handler)();
        void *result;
        // 重复释放 配置 释放直到配置成功
        while(true) {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
            result = malloc(n);
            if (result) { return result; }
        }
    }
    template <int inst>
    void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n) {
        void (* my_malloc_handler)();
        void *result;
        while(true) {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
            result = realloc(p, n);
            if (result) { return result; }
        }
    }
    typedef __malloc_alloc_template<0> malloc_alloc;

    enum { __ALIGN = 8 }; // 小型区块 下限
    enum { __MAX_BYTES = 128 }; // 小型区块 上限
    enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; // free-lists 个数

    template <bool threads, int inst>
    class __default_alloc_template {
    private:
        // ROUND_UP 将bytes 上调至 8的倍数
        static size_t ROUND_UP(size_t bytes) {
            return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
        }
    private:
        // union 共用一块内存 二者取其一 以节约内存
        union obj {
            union obj* free_list_link;
            // 如果没被free 则是指针指向的数据 如果被free free_list_link是下一个结点
            char client_data[1];
        };
    private:
        // 16个 freelists
        static obj* volatile free_list[__NFREELISTS];
        // 决定使用第几个 freelist n 从 1起
        static size_t FREELIST_INDEX(size_t bytes) {
            return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
        }

        static void *refill(size_t n);
        static char *chunk_alloc(size_t size, int &nobjs);

        // 内存池位置
        static char *start_free;
        static char *end_free;
        static size_t heap_size;
    public:
        static void * allocate(size_t n) {
            // 如果使用 obj **my_free_list; 会被优化到寄存器中, 使得 库代码无法lock住对他的读调用
            // 这里修饰的是my_free_list，是free_list数组中的一个元素，而不是数组指针，所以volatile放在两个中间
            obj * volatile *my_free_list;
            obj *result;
            if (n > (size_t)__MAX_BYTES) {
                return malloc_alloc::allocate(n);
            }
            my_free_list = free_list + FREELIST_INDEX(n);
            result = *my_free_list;
            if (result == 0) {
                void *r = refill(ROUND_UP(n));
                return r;
            }
            *my_free_list = result->free_list_link;
            return result;
        }
        static void deallocate(void *p, size_t n) {
            obj *q = (obj *)p;
            obj * volatile *my_free_list;
            if (n > (size_t)__MAX_BYTES) {
                return malloc_alloc::deallocate(p, n);
            }
            my_free_list = free_list + FREELIST_INDEX(n);
            q->free_list_link = *my_free_list;
            *my_free_list = q;
        }
        static void* reallocate(void *p, size_t old_sz, size_t new_sz);
    };

    template <bool threads, int inst>
    char *__default_alloc_template<threads, inst>::start_free = 0;

    template <bool threads, int inst>
    char *__default_alloc_template<threads, inst>::end_free = 0;

    template <bool threads, int inst>
    size_t __default_alloc_template<threads, inst>::heap_size = 0;

    template <bool threads, int inst>
    typename __default_alloc_template<threads, inst>::obj * volatile
            __default_alloc_template<threads, inst>::free_list[__NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // 返回一个大小为 n 的对象 并且会有时候会适当的为free list 添加节点
    // 假设n已被上调至8的倍数
    template <bool threads, int inst>
    void* __default_alloc_template<threads, inst>::refill(size_t n) {
        int nobjs = 20;
        char *chunk = chunk_alloc(n, nobjs);
        obj * volatile *my_free_list;
        obj *result;
        obj *current_obj, *next_obj;
        int i;
        // 如果只获得一个区块 这个区块就给调用者用 否则就添加到新节点
        if (nobjs == 1) { return chunk; }
        my_free_list = free_list + FREELIST_INDEX(n);
        result = (obj *)chunk;
        *my_free_list = next_obj = (obj *)(chunk + n);
        // 从1开始 因为第0个要返回给调用者 将节点窜起来
        for (i = 1; ; ++i) {
            current_obj = next_obj;
            next_obj = (obj *)((char *)next_obj + n);
            if (nobjs - 1 == i) {
                current_obj->free_list_link = 0;
                break;
            } else {
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }
    // 配置一大块空间 可容纳 nobjs 个大小为 size 的区块
    // 如果配置 nobjs 个区块有所不便 nobjs就会相应较少
    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int &nobjs) {
        char *result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;
        // 内存池剩余空间完全满足需求
        if (bytes_left >= total_bytes) {
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if (bytes_left >= size) {
            // 内存池空间只够一个 区块的量
            nobjs = bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {
            // 内存池一个区块都无法提供 向堆要需求量的两倍空间 + 一个随着配置次数越来越大的附加量
            size_t bytes_to_get =  2 * total_bytes + ROUND_UP(heap_size >> 4);
            // 榨干剩余价值 就将他放入free list
            if (bytes_left > 0) {
                obj * volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            // 实在没办法了 配置 heap 空间 来补充内存池
            start_free = (char *)malloc(bytes_to_get);
            if (start_free == 0) {
                // heap 空间也不足了 malloc() 失败
                // 遍历一次我们的 free list 看看还有没有未用区块且区块够大
                int i;
                obj * volatile *my_free_list, *p;
                for (i = size; i <= __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (p != 0) {
                        *my_free_list = p->free_list_link;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        // 找到了未用区块 递归自己 修正nobjs
                        return (chunk_alloc(size, nobjs));
                    }
                }
                // 山穷水尽之时 交给第一级配置器 看看 out-of-memory 能不能提供点帮助
                end_free = 0;
                start_free = (char *)malloc_alloc::allocate(bytes_to_get);
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            // 递归自己 修正nobjs
            return chunk_alloc(size, nobjs);
        }
    }

    // 将第二级配置器设置为默认配置器
    typedef __default_alloc_template<0, 0> alloc;

    template <typename T, typename Alloc>
    class simple_alloc {
    public:
        static T* allocate(size_t n) {
            return (n == 0) ? 0 : (T*)Alloc::allocate(n * sizeof(T));
        }
        static T* allocate() {
            return (T*)Alloc::allocate(sizeof(T));
        }
        static void deallocate(T *p, size_t n) {
            if (n != 0) {
                Alloc::deallocate(p, n * sizeof(T));
            }
        }
        static void deallocate(T *p) {
            Alloc::deallocate(p, sizeof(T));
        }
    };
}
#endif //STLTOYS_YY_ALLOC_H
