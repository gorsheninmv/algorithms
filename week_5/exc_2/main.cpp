#include <iostream>
#include <stack>
#include <cstring>
#include <string>
#include <memory>
#include <vector>


#ifdef DEBUG
extern "C" {
#include <io.h>
}
#else
#include "edx-io.h"
#endif

#define PRINT_SPACE edx_print_char(' ');
#define PRINT_NEW_LINE edx_print_char('\n');

class PriorityQueue {
    typedef int Result;
    typedef bool Status;

private:
    struct Node {
        int v;  // value
        int si; // string index

        bool operator >(const Node & _nd) {
            return this->v > _nd.v;
        }

        bool operator <(const Node & _nd) {
            return this->v < _nd.v;
        }

        bool operator >=(const Node & _nd) {
            return this->v >= _nd.v;
        }

        bool operator <=(const Node & _nd) {
            return this->v <= _nd.v;
        }
    };

    const int SZ = 1000000;
    std::vector<Node> m_dat; // массив данных
    std::vector<int> m_si_to_di; // массив-преобразователь индекса строки в индекс с данными
    int m_top; // индекс элемента, следующего за последним

    void min_heapify(int _idx) {

        while (true) {

            auto l_ch_i = 2 * _idx + 1;
            auto r_ch_i = 2 * _idx + 2;
            if (l_ch_i >= m_top) return;

            auto min_i = m_dat[_idx] <= m_dat[l_ch_i] ? _idx : l_ch_i;

            if (r_ch_i < m_top)
                min_i = m_dat[min_i] <= m_dat[r_ch_i] ? min_i : r_ch_i;

            if (min_i != _idx) {
                std::swap(m_si_to_di[m_dat[_idx].si], m_si_to_di[m_dat[min_i].si]);
                std::swap(m_dat[_idx], m_dat[min_i]);
                _idx = min_i;
            }
            else return;
        }
    }
public:
    PriorityQueue() : m_dat(SZ), m_si_to_di(SZ), m_top(0){}


    void add(int _v, int _si) {
        auto cur = m_top++;
        m_dat[cur] = {_v, _si};
        m_si_to_di[_si] = cur;

        while (cur != 0) { // пока не окажемся в корне
            auto parent = (cur - 1) / 2;
            if (m_dat[parent] <= m_dat[cur]) break;

            std::swap(m_si_to_di[m_dat[parent].si], m_si_to_di[m_dat[cur].si]);
            std::swap(m_dat[parent], m_dat[cur]);
            cur = parent;
        }
    }


    std::pair<Result, Status> extract_min() {
        if (m_top == 0) return std::make_pair<Result, Status>(0, false);

        std::swap(m_si_to_di[m_dat[0].si], m_si_to_di[m_dat[m_top - 1].si]);
        std::swap(m_dat[0], m_dat[m_top - 1]);

        --m_top;
        min_heapify(0);
        return std::pair<Result, Status>(m_dat[m_top].v, true);
    }


    void decrease_key(int _v, int _sn){
        auto parent = m_si_to_di[_sn];
        m_dat[parent].v = _v;if (parent == 0) return;

        while (parent >= 0) {
            min_heapify(parent);
            if (parent == 0) return;
            parent = (parent - 1) / 2;
        }
    }
};


int main() {

    edx_open();

    const int BUF_SZ = 128;
    char buf[BUF_SZ];

    auto cnt(edx_next_i32());

    PriorityQueue pq;

    for(auto i = 0; i < cnt; ++i){

        edx_next_bounded(buf, BUF_SZ);

        switch (*buf) {
            case 'A':{
                edx_next_bounded(buf, BUF_SZ);
                auto v = atoi(buf);
                pq.add(v, i);
                break;
            }
            case 'D':{
                edx_next_bounded(buf, BUF_SZ);
                auto si = atoi(buf) - 1;
                edx_next_bounded(buf, BUF_SZ);
                auto v = atoi(buf);

                pq.decrease_key(v, si);
                break;

            }
            default:{
                auto r = pq.extract_min();

                if (r.second) edx_print_i32(r.first);
                else edx_print_char('*');

                PRINT_NEW_LINE;
            }
        }
    }

    edx_close();

    return 0;
}


