#include <iostream>
#include <algorithm>
#include <cstring>

#ifdef DEBUG
extern "C" {
#include <io.h>
}
#else
#include "edx-io.h"
#endif

#define PRINT_SPACE edx_print_char(' ');
#define PRINT_NEW_LINE edx_print_char('\n');

void find_interval(int * _l, int * _r, int * _int_l, int * _int_r) {

    if (_l >= _r) return;

    auto m = *(_l + (_r - _l) / 2);
    auto l = _l;
    auto r = _r;

    while (l <= r) {
        while (l <= _r && *l < m) ++l;
        while (r >= _l && *r > m) --r;

        if (l < r){
            std::swap(*(l++), *(r--));
            continue;
        }

        if (l == r) {
            ++l;
            --r;
        }
    }

    if (_int_l <= r) find_interval(_l, r, _int_l, _int_r);
    if (_int_r >= l) find_interval(l, _r, _int_l, _int_r);
}


int main() {
    edx_open();

    auto sz = edx_next_i32();
    auto int_l = edx_next_i32();
    auto int_r = edx_next_i32();

    auto a = edx_next_i32();
    auto b = edx_next_i32();
    auto c = edx_next_i32();

    auto ar = new int[sz];
    *ar = edx_next_i32();
    *(ar + 1) = edx_next_i32();

    for (auto pt = ar + 2; pt != ar + sz; ++pt) {
        *pt = a * *(pt - 2) + b * *(pt - 1) + c;
    }

    find_interval(ar, ar + sz - 1, ar + int_l - 1, ar + int_r - 1);

    for (auto pt = ar + int_l - 1; pt != ar + int_r; ++pt) {
        edx_print_i32(*pt);
        PRINT_SPACE;
    }

    edx_close();
    return 0;
}