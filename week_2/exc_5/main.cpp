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
void merge_sort(T * _l, T * _r) {
    auto c = _r - _l;

    if (c <= 0) return; // когда остается один элемент то выход

    if (c == 1) { // когда 2 элемента, то swap при необходимости
        if (*_l > *_r) std::swap(*_l, *_r);
        return;
    }

    // рекурсивный вызов
    auto m = (_r - _l) / 2 + _l;
    merge_sort(_l, m);
    merge_sort(m + 1, _r);

    merge(_l, m, _r);
}


void print_bucket(long long * _b, long long * _e) {
    for (auto ptr = _b; ptr != _e; ++ptr) {
        edx_print_i64(*ptr);
        PRINT_SPACE;
    }
    PRINT_NEW_LINE;
}


int main() {
    edx_open();

    auto sz = edx_next_i32();
    auto step = edx_next_i32();

    if (step == 1) {
        edx_print("YES");
        edx_close();
        return EXIT_SUCCESS;
    }

    // размер каждой из корзин
    auto bucket_sz = sz / step;
    if (sz % step != 0) ++bucket_sz;

    auto data = new long long[bucket_sz * step]; // массив с матрешками, единый для всех корзин
    auto ends = new long long *[step]; // массив указателей на концы "корзин" с матрешками

    // заполнение массива указателей на конец корзин - изначально все указывают на нулевой элемент каждой корзины
    for (auto ptr = ends; ptr != ends + step; ++ptr) {
        size_t bucket_offset = (ptr - ends) * bucket_sz;
        long long * bucket_begin = data + bucket_offset;
        *ptr = bucket_begin;
    }

    // заполнение корзин
    // после заполнения ends будут указывать на след. за последним элементом
    auto bucket_offset = 0;
    for (auto cnt = 0; cnt < sz; ++cnt) {
        if (bucket_offset == step) bucket_offset = 0;
        **(ends + bucket_offset) = edx_next_i64();
        ++(*(ends + bucket_offset)); // сдвиг указателя на конец корзины для корзины номер bucket_offset
        ++bucket_offset;
    }

    // сортировка матрешек в корзинах
    for (bucket_offset = 0; bucket_offset < step; ++bucket_offset) {
        merge_sort(data + (bucket_offset * bucket_sz), *(ends + bucket_offset) - 1);
    }

//    for (bucket_offset = 0; bucket_offset < step; ++bucket_offset) {
//        print_bucket(data + (bucket_offset * bucket_sz), *(ends + bucket_offset));
//    }

    long long * last = nullptr; // последний существующий элемент который был проверен
    // проход по длине одной корзины
    for (auto i = 0; i < bucket_sz; ++i) {

        //проверим, что последний элемент в предыдущем i меньше первого элемента текущего i
        if (last && *last > *(data + i)) {
            edx_print("NO");
            edx_close();
            return EXIT_SUCCESS;
        }

        // изначально pt на первый эл-т второй корзины, затем на 1 эл-т третьей корзины, итерируемся по i-ым элементам всех корзин и.т.д
        for (auto pt = data + bucket_sz + i; pt < data + bucket_sz * step; pt += bucket_sz) {
            /*  если адрес элемента предыдущей корзины и адрес элемента текущей корзины больше либо равны конца этих корзин,
                то дальше не проверять эту итерацию
                pt - bucket_sz - указатель на i-ый элемент предыдущей корзины
                ends + (pt - bucket_sz - data) / bucket_sz - указатель на конец предыдущей корзины
                pt - указатель на i-ый элемент текущей корзины
                ends + (pt - data) / bucket_sz - указатель на конец текущей корзины
            */
            auto prev_bucket_pt = pt - bucket_sz;
            if (prev_bucket_pt < *(ends + (pt - bucket_sz - data) / bucket_sz)) {
                last = prev_bucket_pt;
                if (pt < *(ends + (pt - data) / bucket_sz)) {
                    last = pt;
                    if (*prev_bucket_pt > *pt) {
                        edx_print("NO");
                        edx_close();
                        return EXIT_SUCCESS;
                    }
                }
            }
        }
    }

    edx_print("YES");

    edx_close();
    return 0;
}