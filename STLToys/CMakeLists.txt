cmake_minimum_required(VERSION 3.10)
project(STLToys)

set(CMAKE_CXX_STANDARD 11)

add_executable(STLToys main.cpp yyalloc.h YY_construct.h YY_alloc.h YY_uninitialized.h YY_iterator.h YY_type_traits.h YY_vector.h YY_list.h YY_deque.h YY_stack.h YY_queue.h YY_heap.h YY_priority_queue.h YY_slist.h YY_RBTree.h YY_pair.h YY_function.h YY_set.h YY_map.h YY_multiset.h YY_multimap.h YY_hashtable.h YY_hash_fun.h)