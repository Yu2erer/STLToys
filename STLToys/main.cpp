#include <iostream>
#include <vector>

#include "yyalloc.h"
//#include "YY_alloc.h"
#include "YY_vector.h"
using namespace std;

int main() {
//    int ia[5] = {0, 1, 2, 3, 4};
//    unsigned int i;
//    vector<int, YY::allocator<int>> iv(ia, ia + 5);
//    for (i = 0; i < iv.size(); ++i) {
//        cout << iv[i] << " ";
//    }
    YY::vector<int> ii(2, 9);
    ii.push_back(2);
    for (int i = 0; i < ii.size(); ++i) {
        cout << ii[i] << " ";
    }
    cout << endl;
    return 0;
}