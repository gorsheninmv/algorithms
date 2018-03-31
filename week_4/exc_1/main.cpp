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


class Stack {
private:
    static const int SZ = 1000000;
    int mDat[SZ];
    int * mTop;
public:
    Stack() {
        mTop = mDat;
    }


    void push(int _val, bool &_ok) {
        _ok = false;
        if (mTop == mDat + SZ) return;

        _ok = true;
        *(mTop++) = _val;
        return;
    }


    int pop(bool &_ok) {
        _ok = false;
        if (mTop == mDat) return 0;

        _ok = true;
        return *(--mTop);
    }
};


int main() {
    edx_open();

    int cnt = edx_next_i32();
    const int BUF_SZ = 255;
    char buf[BUF_SZ];

    Stack stck;

    for (int i = 0; i < cnt; ++i) {
        edx_next_bounded(buf, BUF_SZ);

        if (*buf == '+') {
            edx_next_bounded(buf, BUF_SZ);
            auto ok(false);
            stck.push(atoi(buf), ok);
        }
        else {
            auto ok(false);
            edx_print_i32(stck.pop(ok));
            PRINT_NEW_LINE;
        }
    }

    edx_close();

    return 0;
}