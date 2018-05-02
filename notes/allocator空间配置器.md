# STLToys
STLToys 是练习时实现的一个C++ STL标准库的玩具.

## allocator 空间配置器
STL 规定配置器(allocator) 定义在 memory 
memory 引入了 三个文件 

* `stl_construct.h`定义了construct() destroy() 负责对象的构建和析构
* `stl_alloc.h` 定义了一二级配置器 配置器名为 alloc
* `stl_uninitalized.h `定义了一些全局函数 用来 fill or copy 大块内存数据 un_initalized_copy() un_initalized_fill() un_initalized_fill_n()
### stl_construct.h
* construct(T1*p ,const T2& value) 泛化 new(p) T1(value);
* destroy(ForwardIterator first, ForwardIterator last) <ForwardIterator, ForwardIterator> 接收两个迭代器 泛化 __destroy 使用 trival destructor 判断数值型别 __false_type 循环析构
特化 __true_type no-op
特化 (T* p) p->~T();
其中特别的是对 迭代器 char* 和 wchar_t* 进行了特化
特化 (char*, char*) no-op
特化 (wchar_t*, wchar_t*) no-op

stl_construct.h 的 construct() destroy() 按照STL标准设计为了全局函数 但是还是要两个 construct() 和 destroy() 成员函数 但是 SGI STL 里的 std::alloc 配置器 并没有遵循这一规则

SGI 考虑内存碎片问题 设计了双层级配置器
SGI STL 第一级配置器 直接使用 malloc() free() 并且模拟了 C++ set_new_handler的处理方案 之所以不用new而改用malloc 可能是因为历史原因 其次 C++ 没有提供相应于 realloc() 的内存配置操作

SGI STL 第二级配置器 维护了 16个free lists. 当配置区块 > 128 bytes 则使用第一级配置器 < 128 bytes 采用 memory pool (由malloc()配置而成).
### stl_alloc.h
#### 第一级配置器 __malloc_alloc_template
`template <int inst>` 此处没有template型别参数 inst实际上也没有使用
定义两个 私有函数 与 一个函数指针
```
        static void* oom_malloc(size_t);
        static void* oom_realloc(void *, size_t);
        static void (* __malloc_alloc_oom_handler)();
```
使用 malloc() free() remalloc() 进行配置 当 malloc() 或者 remalloc() 配置失败时 调用 oom_malloc() 或者 oom_realloc() 循环释放配置 直到成功返回.
#### 第二级配置器 __default_alloc_template
`template <bool threads, int inst>` 此处不考虑多线程情况 同时 也没有template 型别参数 也没用上 inst
定义了 free-list 维护 16个free-lists 均为 8 的倍数 到 128 上限
```
union {
    union obj *free_list_link;
    char client_data[1];
}
```
因为 union 共用体的性质 他们共用一块内存 同时 一物两用 当其没被 free 时 client_data 是指针指向的数据 如果被 free 那么 free_list_link 为下一个结点 以此节约内存
allocate(size_t n) 分配内存 > 128 调用第一级配置器 否则 寻找 16个自由链表中合适的一个 如果没找到可用的自由链表 就 调用 refill() 进行填充 否则 调整 free-list 并返回
deallocate(void *p, size_t n) 也相同 注意回收
refill(size_t n) 通过chunk_alloc(size_t size, int &nobjs) 获取 区块并拼接到 free-list 上
chunk_alloc(size_t size, int &nobjs) 其中 size 假设为 8 的倍数 尽量返回区块 如果连一个区块都不够 榨干剩余内存 并加入 free-list 
实在不行 计算需要的水量 为需求量的两倍再加上一个随着配置次数越来越大的附加量 再使用 malloc() 配置 heap 空间 如果配置失败 就调用 第一级配置器 allocate() 进行配置 (其实本质都是 malloc() 但是第一级配置器里 模仿了 C++ 的 set_new_handler out_of_memory机制 想办法尽点力) 如果还是不行 就老老实实抛出异常了
#### 符合STL配置器标准接口 simple_alloc
SGI STL 里的 allocate配置器并不按照 STL标准来做 但是它提供了一个配置器标准接口 本质就是调用之前的配置器
```c++
    template <typename T, typename Alloc>
    class simple_alloc {
    public:
        static T* allocate(size_t n) { return (n == 0) ? 0 : (T*)Alloc::allocate(n * sizeof(T)); }
        static T* allocate() { return (T*)Alloc::allocate(sizeof(T)); }
        static void deallocate(T *p, size_t n) {
            if (n != 0) { Alloc::deallocate(p, n * sizeof(T)); }
        }
        static void deallocate(T *p) { Alloc::deallocate(p, sizeof(T));}
    };
```
### stl_uninitalized.h
* uninitialized_fill_n(ForwardIterator first, Size n, const T& x);
从 first 迭代器开始 到第 n 个为止 将 x 作为初值进行构造

```c++
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
        return __uninitialized_fill_n(first, n, x, value_type(x));
    }
```
通过 value_type 判断是不是 POD类型 = Plain Old Data POD就是 标量类型或传统的 C struct 类型. 它必然有 trivial ctor/copy/assignment 函数 因此针对 POD类型采用最有效率的初值填写, non-POD类型则采用 循环一个个进行操作.

* uninitalized_copy(InputIterator first, InputIterator last, ForwardIterator result);
从 first 迭代到 last 然后再 result上进行构造

```c++
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitilalized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        return __uninitialized_copy(first, last, result, value_type(result));
    }
```
同样是使用 value_type 判断是不是 POD 类型.
针对 char* 和 wchar_t* 两种类型 使用更有效率的 memmove(直接移动内存内容)来执行复制操作

* uninitalized_fill(ForwardIterator first, ForwardIterator last, const T& x)
从 first 迭代到 last 然后再用 x作为初值 赋值或构造
```c++
    template <typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
        __uninitialized_fill(first, last, x);
    }
```
POD 类型则赋值 non-POD 类型则构造

