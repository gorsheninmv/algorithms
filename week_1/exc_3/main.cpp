#include <iostream>

#ifdef DEBUG
extern "C" {
#include <io.h>
}
#else
#include "edx-io.h"
#endif

void insert_sort(int * _begin, int * _end) {

    if (_end - _begin == 1) return;

    auto begin = _begin;
    auto last = _end - 1;

    while (begin != last) {

        auto sort_end = begin + 1;

        while (true) {

            auto pre_sort_end = sort_end - 1;

            if (*sort_end < *pre_sort_end) {
                std::swap(*sort_end, *pre_sort_end);
                --sort_end;

                if (_begin == sort_end) {
                    edx_print_i32(sort_end - _begin + 1);
                    edx_print_char(' ');
                    break;
                }
            }
            else {
                edx_print_i32(sort_end - _begin + 1);
                edx_print_char(' ');
                break;
            }
        }

        ++begin;
    }
}

int main() {
    edx_open();

    auto sz = edx_next_i32();
    auto arr = new int[sz];

    for (auto pt = arr; pt != arr + sz; ++pt) {
        *pt = edx_next_i32();
    }

    edx_print_i32(1);
    edx_print_char(' ');
    insert_sort(arr, arr + sz);
    edx_print_char('\n');

    for (auto pt = arr; pt != arr + sz; ++pt) {
        edx_print_i32(*pt);
        edx_print_char(' ');
    }

    edx_close();

    return 0;
}