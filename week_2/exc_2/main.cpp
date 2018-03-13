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

long long inverse_cnt;

template <typename T>
void merge(T * _l, T * _m, T * _r) {
    auto l_cnt = _m - _l + 1;
    auto r_cnt = _r - _m;
    auto T_sz = sizeof(T);

    auto l = new T[l_cnt];
    auto r = new T[r_cnt];

    std::memcpy(l, _l, T_sz * l_cnt);
    std::memcpy(r, _m + 1, T_sz * r_cnt);

    auto l_ptr = l;
    auto r_ptr = r;
    auto out_ptr = _l;

    while (l_ptr < l + l_cnt && r_ptr < r + r_cnt) {
        if (*l_ptr > *r_ptr) {
            inverse_cnt += l + l_cnt - l_ptr; // прибавить количество, равное: текущий + оставшиеся слева
        }

        *(out_ptr++) = *l_ptr <= *r_ptr ? *(l_ptr++) : *(r_ptr++);
    }

    if (l_ptr < l + l_cnt) {
        std::memcpy(out_ptr, l_ptr, T_sz * (l + l_cnt - l_ptr));
    }

    if (r_ptr < r + r_cnt) {
        std::memcpy(out_ptr, r_ptr, T_sz * (r + r_cnt - r_ptr));
    }
}


template <typename T>
void merge_sort(T* _arr, T * _l, T * _r) {
    auto c = _r - _l;

    if (c <= 0) return; // когда остается один элемент то выход

    if (c == 1) { // когда 2 элемента, то swap при необходимости
        if (*_l > *_r) {
            std::swap(*_l, *_r);
            ++inverse_cnt;
        }
        return;
    }

    // рекурсивный вызов
    auto m = (_r - _l) / 2 + _l;
    merge_sort(_arr, _l, m);
    merge_sort(_arr, m + 1, _r);

    merge(_l, m, _r);
}


int main() {
    edx_open();
    inverse_cnt = 0;
    auto sz = edx_next_i32();

    auto ar = new int[sz];

    for (auto pt = ar; pt != ar + sz; ++pt) {
        *pt = edx_next_i32();
    }

    merge_sort<int>(ar, ar, ar + sz - 1);

    edx_print_i64(inverse_cnt);

    edx_close();
    return 0;
}