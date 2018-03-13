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


int main() {
    edx_open();

    auto sz = edx_next_i32();
    auto ar = new int[sz];

    for (auto pt = ar; pt != ar + sz; ++pt) {
        *pt = pt - ar + 1;
    }

    for (auto pt = ar + 2; pt < ar + sz; ++pt) {
        std::swap(*pt, *(ar + (pt - ar) / 2));
    }

    for (auto pt = ar; pt != ar + sz; ++pt) {
        edx_print_i32(*pt);
        PRINT_SPACE;
    }

    edx_close();
    return 0;
}