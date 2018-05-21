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
    YY::priority_queue<int> ipq(it, it + 9);
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
    YY::slist<int>::iterator ite2 = slist.end();
    for (;ite != ite2; ++ite) {
        std::cout << *ite << std::endl;
    }

    std::cout << "PB-Tree" << std::endl;
    std::__identity<int>;
    YY::rb_tree<int, int, std::__identity<int>> itree;
    std::cout << "PB-Tree size: " << itree.size() << std::endl;
//    itree.insert_unique(10);
//    itree.insert_unique(7);
//    itree.insert_unique(8);
//    std::cout << "PB-Tree size: " << itree.size() << std::endl;
//    itree.insert_unique(8);
//    std::cout << "PB-Tree size: " << itree.size() << std::endl;
//    YY::rb_tree<int, int, std::__identity<int>>::iterator itbegin = itree.begin();
//    YY::rb_tree<int, int, std::__identity<int>>::iterator itend = itree.end();
//    while (itbegin != itend) {
//        std::cout << *itbegin << " ";
//        ++itbegin;
//    }
//    std::cout << std::endl;
    return 0;
}