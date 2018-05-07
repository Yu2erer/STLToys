#include <iostream>
#include <vector>

#include "yyalloc.h"
//#include "YY_alloc.h"
#include "YY_vector.h"
#include "YY_list.h"
using namespace std;

int main() {
//    int ia[5] = {0, 1, 2, 3, 4};
//    unsigned int i;
//    vector<int, YY::allocator<int>> iv(ia, ia + 5);
//    for (i = 0; i < iv.size(); ++i) {
//        cout << iv[i] << " ";
//    }
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
    return 0;
}