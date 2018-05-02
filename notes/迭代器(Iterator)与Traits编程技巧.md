# STLToys
STLToys 是练习时实现的一个C++ STL标准库的玩具.

## 迭代器(Iterator)与Traits编程技巧
### stl_iterator.h
#### 定义 iterator_tag
迭代器有五种类型
* InputIterator
* OutputIterator
* ForwardIterator
* BidrectionalIterator
* RandomAccessIterator

定义五个 classes 作为 迭代器的标记类型
#### struct iterator_traits
定义一个 `template <typename Iterator> struct iterator_traits` 类
来分别萃取 

* iterator_category 
* value_type
* difference_type
* pointer
* reference

其中要针对原生指针进行偏特化设计
此外定义一些全局辅助函数 来分别获取 萃取出来的内容
### type_traits.h
通过 知道对象的类型的特性 选择最有效率的操作
定义两个 类 来决定我们采取的最优策略

* __false_type
* __true_type

之所以不用 bool 是因为 编译器只有面对 class object 时 才会进行参数推导

#### struct __type_traits
定义 `template <typename type> struct __type_traits`
其中 因为有的编译器 有能力将 __type_traits 特化 所以要定义一个 typedef 通知编译器 我们这个 __type_traits 是特殊的 这样即使编译器也是使用一个名叫 __type_traits 的 template 也能正常运作
```c++
// 解释如上
typedef __true_type this_dummy_member_must_be_first;
// 此处 保守 的将 内嵌类型 都定义为 __false_type 然后再分别对标量类型设计 特化版本 再对 原生指针进行 偏特化
typedef __false_type has_trivial_default_constructor;
typedef __false_type has_trivial_copy_constructor;
typedef __false_type has_trivial_assignment_constructor;
typedef __false_type has_trivial_destructor;
typedef __false_type is_POD_type;
```
此外 如果是 SGI STL的用户 可以为自己的class 设计一个 __type_traits 特化版本
当一个 class 内含指针成员 并且会 对它进行动态内存配置 就需要实现出它的 non-trivial-xxx