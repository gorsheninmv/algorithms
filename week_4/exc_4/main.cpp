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
    static const int SZ = 1000000;
    T* mDat;
    T * mTop;
public:
    Stack() {
        mDat = new T[SZ];
        mTop = mDat;
    }


    ~Stack() {
        delete[] mDat;
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
        if (mTop == mDat) return *mDat;

        _ok = true;
        return *(--mTop);
    }


    void clear() {
        mTop = mDat;
    }


    bool is_empty() {
        return mTop == mDat;
    };


    const T& top(bool &_ok) {
        _ok = mTop != mDat;
        return *(mTop - 1);
    }
};


template <typename T>
class QueueMin {
private:
    /*
     * first - value
     * second - min
     */
    Stack<std::pair<T,T>> in;
    Stack<std::pair<T,T>> out;
public:
    void enqueue(T _val, bool &_ok) {
        _ok = true;
        T min = in.is_empty() ? _val : std::min(_val, in.top(_ok).second);
        if (!_ok) return;

        in.push({_val, min}, _ok);
    }


    T dequeue(bool &_ok) {
        if (!out.is_empty()) return out.pop(_ok).first;

        while (!in.is_empty()) {
            auto replaced = in.pop(_ok);
            if (!_ok) return replaced.first;

            _ok = true;
            T min = out.is_empty() ? replaced.first : std::min(replaced.first, out.top(_ok).second);
            if (!_ok) return replaced.first;

            out.push({replaced.first, min}, _ok);
        }

        if (out.is_empty()) {
            _ok = false;
            T nul;
            return nul;
        }
        else return out.pop(_ok).first;
    }


    T min(bool &_ok) {
        if (in.is_empty() && out.is_empty()) {
            _ok = false;
            T nul;
            return nul;
        }
        else if (!in.is_empty() && !out.is_empty()) {
            return std::min(in.top(_ok).second, out.top(_ok).second);
        }
        else if (!in.is_empty()) {
            return in.top(_ok).second;
        }
        else {
            return out.top(_ok).second;
        }
    }

    void clear() {
        in.clear();
        out.clear();
    }
};


int main() {

    edx_open();
    int cnt = edx_next_i32();
    const int BUF_SZ = 128;
    char buf[BUF_SZ];

    QueueMin<int> qmin;
    auto ok(false);

    for (int i = 0; i < cnt; ++i) {
        edx_next_bounded(buf, BUF_SZ);

        switch (*buf) {
            case '+':
                edx_next_bounded(buf, BUF_SZ);
                qmin.enqueue(atoi(buf), ok);
                break;
            case '-':
                qmin.dequeue(ok);
//                edx_print_i32(qmin.dequeue(ok));
//                PRINT_NEW_LINE;
                break;
            case '?':
                edx_print_i32(qmin.min(ok));
                PRINT_NEW_LINE;
                break;
        }
    }

    edx_close();

    return 0;
}