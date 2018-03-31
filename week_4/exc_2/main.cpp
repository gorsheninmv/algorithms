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


class Queue {
private:
    static const int SZ = 1000000;
    int mDat[SZ];
    int * mB; // указывает на первый элемент, при пустой очереди указывает на -1 элемент
    int * mE; // указывает на пустую ячейку за последним элеменом
public:
    Queue() {
        mE = mDat;
        mB = mE - 1;
    }


    void enqueue(int _val, bool &_ok) {
        _ok = false;
        if (mE == mB) return; // очередь заполнена

        *(mE++) = _val;
        _ok = true;

        if (mB == mDat - 1) ++mB;
        if (mE == mDat + SZ) mE = mDat;
    }


    int dequeue(bool &_ok) {
        _ok = false;
        if (mB == mDat - 1) return 0; // очередь пуста

        auto ret = mB++;
        _ok = true;

        if (mB == mDat + SZ) mB = mDat;

        if (mB == mE) { // очередь пуста
            mB = mDat - 1;
            mE = mDat;
        }

        return *ret;
    }
};


int main() {
    edx_open();

    int cnt = edx_next_i32();
    const int BUF_SZ = 255;
    char buf[BUF_SZ];

    Queue q;

    for (int i = 0; i < cnt; ++i) {
        edx_next_bounded(buf, BUF_SZ);

        if (*buf == '+') {
            edx_next_bounded(buf, BUF_SZ);
            auto ok(false);
            q.enqueue(atoi(buf), ok);
        }
        else {
            auto ok(false);
            edx_print_i32(q.dequeue(ok));
            PRINT_NEW_LINE;
        }
    }

    edx_close();

    return 0;
}