#include <iostream>
#include <stack>
#include <cstring>
#include <string>

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

    std::stack<int> s;

    const int BUF_SZ = 32;
    char buf[BUF_SZ];

    auto sz = edx_next_i32();

    for (auto i = 0; i < sz; ++i) {
        edx_next_bounded(buf, BUF_SZ);

        switch (*buf) {
            case '+':
            case '-':
            case '*': {
                auto b = s.top(); s.pop();
                auto a = s.top(); s.pop();

                switch (*buf) {
                    case '+': s.push(a + b); break;
                    case '-': s.push(a - b); break;
                    case '*': s.push(a * b); break;
                }
                break;
            }
            default: s.push(atoi(buf)); break;
        }
    }

    edx_print_i32(s.top());

    edx_close();

    return 0;
}


