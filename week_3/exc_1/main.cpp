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


void calc_cnts(unsigned int * _b, unsigned int * _e, int * &_cnt_b, int * &_cnt_e) {

    auto data_e = reinterpret_cast<unsigned char *>(_e);
    const auto CNT_SZ = 1024;
    auto cnt = new int[CNT_SZ];
    _cnt_b = cnt;
    _cnt_e = cnt + CNT_SZ;

    std::memset(cnt, 0, CNT_SZ * sizeof(int));

    auto pt = reinterpret_cast<unsigned char *>(_b);

    while (pt != data_e) {
        for (auto i = 0; i < 4; ++i) {
            auto c_pt = cnt + 256 * i + *(pt++);
            ++(*c_pt);
        }
    }

    // суммирование элементов каждого массива счетчиков + сдвиг
    for (auto i = 0; i < 4; ++i) {
        auto carry_cnt = 0;
        auto b_el_pt = _cnt_b + i * 256; // первый элемент i-го масиива счетчиков
        auto e_el_pt = b_el_pt + 256; // след. за последним элементом i-го массива счетчиков
        for (auto pt = b_el_pt; pt != e_el_pt; ++pt) {
            auto cur_carry_cnt = carry_cnt;
            carry_cnt += *pt;
            *pt = cur_carry_cnt;
        }
    }
}


void digit_sort(int _dig_offset, unsigned int * _b_in, unsigned int * _b_out, int _size, int * _cnt_b) {
    for (auto src_pt = _b_in; src_pt != _b_in + _size; ++src_pt) {
        auto key_pt = reinterpret_cast<unsigned char *>(src_pt) + _dig_offset;
        auto key_val = *key_pt;
        auto out_idx_pt = _cnt_b + key_val;
        auto out_idx = *out_idx_pt;
        auto out_pt = _b_out + out_idx;
        *out_pt = *src_pt;
        ++(*out_idx_pt);
    }
}


void radix_sort(unsigned int * &_b, unsigned int * &_e) {
    int * cnt_b; // начало общего массива счетчиков
    int * cnt_e; // след. за последним элементов общего массива счетчиков
    calc_cnts(_b, _e, cnt_b, cnt_e);

    auto sz = _e - _b;
    auto out = new unsigned int[sz];

    for(auto i = 0; i < 4; ++i) {
        auto shifted_cnt_b = cnt_b + 256 * i;
        if (*shifted_cnt_b == _e - _b) continue;
        digit_sort(i, _b, out, sz, shifted_cnt_b);
        std::swap(_b, out);
    }

    delete[] out;
}


int main() {
    edx_open();

    auto n_sz = edx_next_i32();
    auto m_sz = edx_next_i32();

    // исходные данные
    auto arr = new int[n_sz + m_sz];

    for (auto pt = arr; pt != arr + n_sz + m_sz; ++pt) {
        *pt = edx_next_i32();
    }

    // заполнение нового массива
    auto big_arr_sz = n_sz * m_sz;
    auto big_arr = new unsigned int[big_arr_sz];

    auto big_pt = big_arr;
    for (auto n_pt = arr; n_pt != arr + n_sz; ++n_pt) {
        for (auto m_pt = arr + n_sz; m_pt != arr + n_sz + m_sz; ++m_pt) {
            *(big_pt++) = static_cast<unsigned int>(*n_pt) * static_cast<unsigned int>(*m_pt);
        }
    }

    unsigned int * e = big_arr + big_arr_sz;
    radix_sort(big_arr, e);

    long long sum = 0;
    for (auto pt = big_arr; pt < e; pt += 10) {
        sum += *pt;
    }

    edx_print_i64(sum);

    edx_close();

    return 0;
}