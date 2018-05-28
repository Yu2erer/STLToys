#include <iostream>
#include <vector>

#include "yyalloc.h"
#include "YY_alloc.h"
#include "YY_vector.h"
#include "YY_list.h"
#include "YY_deque.h"
#include "YY_stack.h"
#include "YY_queue.h"
#include "YY_heap.h"
#include "YY_priority_queue.h"
#include "YY_slist.h"
#include "YY_RBTree.h"
#include "YY_function.h"
#include "YY_set.h"
#include "YY_map.h"
#include "YY_multiset.h"
#include "YY_multimap.h"
#include "YY_hashtable.h"
#include "YY_hash_fun.h"
#include "YY_hash_set.h"
#include "YY_hash_multiset.h"
#include "YY_hash_map.h"

int main() {
    int ia[5] = {0, 1, 2, 3, 4};
    unsigned int i;
    std::vector<int, YY::allocator<int>> iv(ia, ia + 5);
    for (i = 0; i < iv.size(); ++i) {
        std::cout << iv[i] << " ";
    }
    YY::vector<int> vec(2, 9);
    vec.push_back(2);
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    YY::list<int> list;
    list.push_back(3);
    list.push_front(2);
    std::cout << *list.begin() << std::endl;
    std::cout << std::endl;

    YY::deque<int> ideq(20, 9);
    for (auto i : ideq) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    ideq.push_back(1);
    ideq.push_front(2);
    for (int i = 0; i < ideq.size(); ++i) {
        std::cout << ideq[i] << std::endl;
    }
    for (auto i : ideq) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    YY::stack<int> stack;
    stack.push(2);
    stack.push(10);
    std::cout << stack.top() << std::endl;

    YY::queue<int> queue;
    queue.push(3);
    queue.push(4);
    std::cout << queue.back() << std::endl;
    std::vector<int> ve {0, 1, 2, 3 ,4 , 8, 9, 3 , 5};
    YY::make_heap(ve.begin(), ve.end());
    for (int i = 0; i < ve.size(); ++i) {
        std::cout << ve[i] << " ";
    }
    std::cout << std::endl;
    YY::pop_heap(ve.begin(), ve.end());
    std::cout << ve.back() << std::endl;

    std::cout << "priority_queue" << std::endl;
    int it[9] = {0 , 1 , 2, 3, 4, 8,9,3,5};
    YY::priority_queue<int, YY::vector<int>, YY::greater<int>> ipq(it, it + 9);
    while (!ipq.empty()) {
        std::cout << ipq.top() << " ";
        ipq.pop();
    }
    std::cout << std::endl;

    std::cout << "slist" << std::endl;
    YY::slist<int> slist;
    std::cout << "slist size: " <<slist.size() << std::endl;
    slist.push_front(1);
    slist.push_front(2);
    slist.push_front(3);
    std::cout << "slist size: " <<slist.size() << std::endl;
    YY::slist<int>::iterator ite = slist.begin();
    for (;ite != slist.end(); ++ite) {
        std::cout << *ite << std::endl;
    }

    std::cout << "PB-Tree" << std::endl;
    YY::rb_tree<int, int, YY::identity<int>, YY::less<int>> itree;
    std::cout << "PB-Tree size: " << itree.size() << std::endl;
    itree.insert_unique(10);
    itree.insert_unique(7);
    itree.insert_unique(8);
    itree.insert_unique(15);
    itree.insert_unique(5);
    itree.insert_unique(6);
    itree.insert_unique(11);
    itree.insert_unique(13);
    itree.insert_unique(12);
    std::cout << "PB-Tree size: " << itree.size() << std::endl;
    YY::rb_tree<int, int, YY::identity<int>, YY::less<int>>::iterator itbegin = itree.begin();
    while (itbegin != itree.end()) {
        std::cout << *itbegin << " ";
        ++itbegin;
    }
    std::cout << std::endl;

    std::cout << "set" << std::endl;
    int test[5] = {0, 1, 2, 3, 4};
    YY::set<int> iset(test, test + 5);
    std::cout << "set size: " << iset.size() << std::endl;
    iset.insert(3);
    iset.insert(5);
    YY::set<int>::iterator itebegin = iset.begin();
    for (; itebegin != iset.end(); ++itebegin) {
        std::cout << *itebegin << " ";
    }
    std::cout << std::endl;
    itebegin = iset.find(3);
    std::cout << "find 3: " << *itebegin << std::endl;

    std::cout << "map" << std::endl;
    YY::map<int, int> simap;
    YY::pair<const int, int> pa(2, 1);
    simap.insert(pa);
    simap.insert(pa);
    simap[2] = 2;
    YY::map<int, int>::iterator simap_iter = simap.begin();
    for (; simap_iter != simap.end(); ++simap_iter) {
        std::cout << simap_iter->first <<  " " << simap_iter->second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "multiset" << std::endl;
    YY::multiset<int> imultiset(test, test + 5);
    std::cout << "multiset size: " << imultiset.size() << std::endl;
    imultiset.insert(3);
    imultiset.insert(5);
    YY::multiset<int>::iterator imultibegin = imultiset.begin();
    for (; imultibegin != imultiset.end(); ++imultibegin) {
        std::cout << *imultibegin << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "multimap" << std::endl;
    YY::multimap<int, int> simultimap;
    simultimap[2] = 1;
    simultimap[2] = 2;
    YY::multimap<int, int>::iterator simultimap_iter = simultimap.begin();
    for (; simultimap_iter != simultimap.end(); ++simultimap_iter) {
        std::cout << simultimap_iter->first <<  " " << simultimap_iter->second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "hashtable" << std::endl;
    YY::hashtable<int, int, YY::Hash<int>, YY::identity<int>, YY::equal_to<int>, YY::alloc>
            iht(50, YY::Hash<int>(), YY::equal_to<int>());
    std::cout << "hashtable size: " << iht.size() << std::endl;
    std::cout << "bucket counts: " << iht.bucket_count() << std::endl;
    iht.insert_unique(59);
    iht.insert_unique(63);
    iht.insert_unique(108);
    iht.insert_unique(2);
    iht.insert_unique(55);
    iht.insert_unique(53);
    iht.insert_unique(55);
    std::cout << "hashtable size: " << iht.size() << std::endl;
    YY::hashtable<int, int, YY::Hash<int>, YY::identity<int>, YY::equal_to<int>, YY::alloc>::iterator
        ihtbegin = iht.begin();
    for (int i = 0; i < iht.size(); ++i, ++ihtbegin) {
        std::cout << *ihtbegin << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i <= 47; ++i) {
        iht.insert_equal(i);
    }
    std::cout << "hashtable size: " << iht.size() << std::endl;
    std::cout << "bucket counts: " << iht.bucket_count() << std::endl;

    std::cout << "hash_set" << std::endl;
    YY::hash_set<int> hash_set;
    hash_set.insert(59);
    hash_set.insert(63);
    hash_set.insert(108);
    hash_set.insert(2);
    hash_set.insert(55);
    hash_set.insert(53);
    hash_set.insert(55);
    YY::hash_set<int>::iterator hash_setbegin = hash_set.begin();
    for (;  hash_setbegin != hash_set.end(); ++hash_setbegin) {
        std::cout << *hash_setbegin << " ";
    }
    std::cout << std::endl;

    std::cout << "hash_multiset" << std::endl;
    YY::hash_multiset<int> hash_multiset;
    hash_multiset.insert(59);
    hash_multiset.insert(63);
    hash_multiset.insert(108);
    hash_multiset.insert(2);
    hash_multiset.insert(55);
    hash_multiset.insert(53);
    hash_multiset.insert(55);
    YY::hash_multiset<int>::iterator hash_multisetbegin = hash_multiset.begin();
    for (;  hash_multisetbegin != hash_multiset.end(); ++hash_multisetbegin) {
        std::cout << *hash_multisetbegin << " ";
    }
    std::cout << std::endl;

    std::cout << "hash_map" << std::endl;
    YY::hash_map<int, int> hash_map;
    hash_map[1] = 2;
    hash_map[2] = 3;
    hash_map[1] = 0;
    YY::hash_map<int, int>::iterator hash_mapbegin = hash_map.begin();
    for (; hash_mapbegin != hash_map.end(); ++hash_mapbegin) {
        std::cout << (*hash_mapbegin).first << " " << (*hash_mapbegin).second << std::endl;
    }

    
    return 0;
}