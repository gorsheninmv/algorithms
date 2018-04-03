#include <iostream>
#include <stack>
#include <cstring>
#include <string>
#include <memory>


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

    auto cnt(edx_next_i32());
    auto arr(std::unique_ptr<int[]>(new int[cnt]));
    auto ok(true);

    for (auto i = 0; i < cnt; ++i) {
        arr[i] = edx_next_i32();
    }

    for (auto i = 0; i < cnt; ++i) {
        if (2 * i + 1 >= cnt) break;

        if (arr[i] > arr[2 * i + 1]) {
            ok = false;
            break;
        }

        if (2 * i + 2 >= cnt) break;

        if (arr[i] > arr[2 * i + 2]) {
            ok = false;
            break;
        }
    }

    edx_print(ok ? "YES" : "NO");

    edx_close();

    return 0;
}


