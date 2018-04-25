//
//  yyalloc.h
//  STLToys
//
//  Created by ntian on 2018/4/24.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YYALLOC_H
#define STLTOYS_YYALLOC_H

#include <new>
#include <cstddef>
#include <iostream>
/*
 * 此处只是 将 ::operator new 和 ::operator delete 作一层包装而已 效率并不佳
 * 只能有限度的搭配 PJ STL 因为 PJ STL 未遵循 STL 规格 它所提供的许多容器都需要一个非标准的空间配置器接口 allocator::_Charallocate()
 * 只能有限度搭配 RW STL 因为 RW STL 在很多容器内都使用了缓冲区
 * 完全无法用于 SGI STL 因为 SGI STL 在这个项目里完全没按照 STL 的标准 使用了专属的具备次层配置能力(sub-allocation)的 效率优越的特殊配置器 但是它仍然提供了一个 标准的配置器接口只不过做了一层隐藏
 */
namespace YY {

    template <typename T>
    inline T* _allocate(ptrdiff_t size, T*) {
        std::set_new_handler(0); // 卸载new_handler 一旦没有分配成功 就抛出异常
        T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
        if (tmp == 0) {
            std::cerr << "out of memmory" << std::endl;
            exit(1);
        }
        return tmp;
    }
    template <typename T>
    inline void _deallocate(T* buffer) {
        ::operator delete(buffer);
    }
    template <typename T1, typename T2>
    __inline void _construct(T1* p, const T2& value) {
        new(p) T1(value); // placement new
    };
    template <typename T>
    inline void _destory(T* p) {
        p->~T();
    }

    template <typename T>
    class allocator {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        // 成员模板 使其支持另一个类型赋给当前类型
        template <typename U>
        struct rebind {
            typedef allocator<U> other;
        };

        pointer allocate(size_type n, const void* hint = 0) {
            return _allocate((difference_type)n, (pointer)hint);
        }
        void deallocate(pointer p, size_type n) {
            _deallocate(p);
        }
        void construct(pointer p, const T& value) {
            _construct(p, value);
        }
        void destory(pointer p) {
            _destory(p);
        }
        pointer address(reference x) {
            return (pointer)&x;
        }
        const_pointer const_address(const_reference x) {
            return (const_pointer)&x;
        }
        size_type max_size() const {
            return size_type(UINT_MAX / sizeof(T));
        };
    };

}
#endif //STLTOYS_YYALLOC_H
