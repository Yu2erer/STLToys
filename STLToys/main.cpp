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
using namespace std;

int main() {
    int ia[5] = {0, 1, 2, 3, 4};
    unsigned int i;
    vector<int, YY::allocator<int>> iv(ia, ia + 5);
    for (i = 0; i < iv.size(); ++i) {
        cout << iv[i] << " ";
    }
    YY::vector<int> vec(2, 9);
    vec.push_back(2);
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;

    YY::list<int> list;
    list.push_back(3);
    list.push_front(2);
    cout << *list.begin() << endl;
    cout << endl;

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

    vector<int> ve {0, 1, 2, 3 ,4 , 8, 9, 3 , 5};
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
    return 0;
}