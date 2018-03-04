#include <iostream>

#ifdef DEBUG
extern "C" {
#include <io.h>
}
#else
#include "edx-io.h"
#endif

typedef double Income;

struct Man {
    int Id;
    Income Inc;
};

void sort_avg(Man * _l,  Man * _r, Man * _avg) {

    if (_l >= _r) return;

    auto m = ((_r - _l) / 2 + _l)->Inc;
    auto r = _r;
    auto l = _l;

    while (l <= r) {
        while (l <= _r && l->Inc < m) ++l;

        while (r >= _l && r->Inc > m) --r;

        if (l <= r) std::swap(*(l++), *(r--));
    }

    if (_avg <= r) sort_avg(_l, r, _avg);
    if (_avg >= l) sort_avg(l, _r, _avg);
}


int main() {
    edx_open();

    auto first = true;
    Man min, max;
    int sz = edx_next_i32();

    Man * incomes = new Man[sz];

    for (auto pt = incomes; pt != incomes + sz; ++pt) {
        auto val = edx_next_double();

        pt->Inc = val;
        pt->Id = pt - incomes + 1;

        if (first) {
            min = max = *pt;
            first = false;
        }
        else {
            if (val < min.Inc) min = *pt;
            if (val > max.Inc) max = *pt;
        }
    }

    auto avg = (incomes + sz - 1 - incomes) / 2 + incomes;

    sort_avg(incomes, incomes + sz - 1, avg);

    edx_print_i32(min.Id);
    edx_print_char(' ');
    edx_print_i32(avg->Id);
    edx_print_char(' ');
    edx_print_i32(max.Id);

    edx_close();

    return 0;
}