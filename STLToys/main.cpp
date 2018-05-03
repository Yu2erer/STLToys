#include <iostream>
#include <vector>

#include "yyalloc.h"
//#include "YY_alloc.h"
#include "YY_vector.h"
using namespace std;

int main() {
    int ia[5] = {0, 1, 2, 3, 4};
    unsigned int i;
    vector<int, YY::allocator<int>> iv(ia, ia + 5);
    for (i = 0; i < iv.size(); ++i) {
        cout << iv[i] << " ";
    }
    YY::vector<int> ii;
    cout << ii.size();
//    cout << endl;
    return 0;
}