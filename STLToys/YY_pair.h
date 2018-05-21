//
//  YY_pair.h
//  STLToys
//
//  Created by ntian on 2018/5/21.
//  Copyright © 2018 ntian. All rights reserved.
//

#ifndef STLTOYS_YY_PAIR_H
#define STLTOYS_YY_PAIR_H

namespace YY {

    template <typename T1, typename T2>
    struct pair {
        typedef T1 first_type;
        typedef T2 second_type;
        T1 first;
        T2 second;

        pair() : first(T1()), second(T2()) {}
        pair(const T1& _first, const T2& _second) : first(_first), second(_second) {}
    };
}

#endif //STLTOYS_YY_PAIR_H
