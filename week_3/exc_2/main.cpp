#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>

#ifdef DEBUG
extern "C" {
#include <io.h>
}
#else
#include "edx-io.h"
#endif

#define PRINT_SPACE edx_print_char(' ');
#define PRINT_NEW_LINE edx_print_char('\n');

inline unsigned char calc_ascii_offset(unsigned char _symb) {
    return _symb - 97;
}


void count_sort(char * _dat, int * _in_idx, int * _out_idx, int _in_out_sz, long long * _cnt_b, long long * _cnt_e) {
    std::memset(_cnt_b, 0, sizeof(*_cnt_b) * (_cnt_e - _cnt_b)); // обнуление счетчика

    // инициализация счетчика
    for (auto pt = _dat; pt != _dat + _in_out_sz; ++pt) {
        auto cnt_pt = _cnt_b + calc_ascii_offset(*pt); // указатель на элемент-счетчик для значения *pt
        ++*cnt_pt;
    }

    long long sum = 0;
    for (auto pt = _cnt_b; pt != _cnt_e; ++pt) {
        sum += *pt;
        *pt = sum;
    }
    // ^^инициализация счетчика^^

    // сортировка индексов
    for (auto pt = _in_idx + _in_out_sz - 1; pt >= _in_idx; --pt) {
        auto dat_pt = _dat + *pt; // указатель на значение в массиве данных
        auto cnt_pt = _cnt_b + calc_ascii_offset(*dat_pt); // указатель на значение в массиве счетчиков
        auto out_pt = _out_idx + (--*cnt_pt); // указатель на значение в массиве выходных индексов
        *out_pt = *pt;
    }
}


int * radix_sort(char * _dat_b, char * _dat_e, int _row_cnt, int _k) {
    auto row_sz = (_dat_e - _dat_b) / _row_cnt; // количество эл-тов в одной строке
    auto idx_in = new int[row_sz];  // массив индексов, который будет подаваться на вход
    auto idx_out = new int[row_sz]; // массив индексов, который будет заполняться

    // инициализация массива индексов
    for (auto pt = idx_in; pt != idx_in + row_sz; ++pt) {
        *pt = pt - idx_in;
    }

    // инициализация счетчика
    const int CNT_SZ = 26;
    auto cnt = new long long[CNT_SZ];

    // запуск цифровой сортировки
    for (auto i = 0; i < _k; ++i) {
        count_sort(_dat_b + (_row_cnt - i - 1) * row_sz, idx_in, idx_out, row_sz, cnt, cnt + CNT_SZ);
        std::swap(idx_in, idx_out);
    }

    delete[] idx_out;
    return idx_in;
}


int main() {
    edx_open();

    auto n = edx_next_i32(); // число строк
    auto m = edx_next_i32(); // длина строк
    auto k = edx_next_i32(); // число фаз сортировки

    auto dat_len = n * m; // длина массива с данными
    auto dat = new char[dat_len]; // массив с данными

    // заполнение массива данными
    auto buff = new char[n + 1];
    char * dat_pt = dat;
    for (auto row = 0; row < m; ++row) {
        edx_next_bounded(buff, n + 1);

        for (auto pt = buff; pt != buff + n; ++pt) {
            *(dat_pt++) = *pt;
        }
    }

    auto r = radix_sort(dat, dat + dat_len, m, k);

    for (auto pt = r; pt != r + n; ++pt) {
        edx_print_i32(*pt + 1);
        PRINT_SPACE;
    }

    edx_close();

    return 0;
}