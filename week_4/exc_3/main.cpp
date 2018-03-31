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


template <typename T>
class Stack {
private:
    static const int SZ = 10000;
    T mDat[SZ];
    T * mTop;
public:
    Stack() {
        mTop = mDat;
    }


    void push(T _val, bool &_ok) {
        _ok = false;
        if (mTop == mDat + SZ) return;

        _ok = true;
        *(mTop++) = _val;
        return;
    }


    T pop(bool &_ok) {
        _ok = false;
        if (mTop == mDat) return 0;

        _ok = true;
        return *(--mTop);
    }


    void clear() {
        mTop = mDat;
    }


    bool is_empty() {
        return mTop == mDat;
    };
};


int main() {
    edx_open();

    int cnt = edx_next_i32();
    const int BUF_SZ = 10001;
    char buf[BUF_SZ];

    Stack<char> stck;
    auto ok(false);

    for (int i = 0; i < cnt; ++i) {
        edx_next_bounded(buf, BUF_SZ);

        auto err(false);
        for (char * cur = buf; *cur != '\0'; ++cur) {
            switch (*cur) {
                case '[':
                case '(':
                    stck.push(*cur, ok);
                    break;
                case ']':
                    if (stck.pop(ok) != '[') err = true;
                    break;
                case ')':
                    if (stck.pop(ok) != '(') err = true;
                    break;
            }

            if (err) break;
        }

        auto ret = stck.is_empty() && !err ? "YES" : "NO";
        edx_print(ret);
        PRINT_NEW_LINE;
        stck.clear();
    }


    edx_close();

    return 0;
}