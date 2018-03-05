#include <iostream>
#include <algorithm>
#include <vector>

#ifdef DEBUG
extern "C" {
#include <io.h>
}
#else
#include "edx-io.h"
#endif

struct Elem {
    int Idx;
    int Val;

    bool operator <(Elem &_elem) {
        return this->Val < _elem.Val;
    }
};


void print_arr(int * _b, int * _e, const char * _d) {
    using namespace std;

    cout << _d << ": ";

    for (auto pt = _b; pt != _e; ++pt) {
        cout << *pt << ' ';
    }

    cout << endl;
}


int main() {
    edx_open();

    int sz = edx_next_i32();

    std::vector<Elem> elems(sz);
    for (auto pt = elems.begin(); pt != elems.end(); ++pt) {
        pt->Idx = pt - elems.begin();
        pt->Val = edx_next_i32();
    }
    std::sort(elems.begin(), elems.end());

    int * temp = new int[sz];
    for (auto pt = temp; pt != temp + sz; ++pt)
        *pt = pt - temp;

    //print_arr(temp, temp + sz, "temp");

    int * nnew = new int[sz];

    for (auto pt = elems.begin(); pt != elems.end(); ++pt) {
        auto offset = pt->Idx;
        *(nnew + offset) = pt - elems.begin();
    }


    //print_arr(nnew, nnew + sz, "nnew");

        // pt - указатель на элемент в temp
        // pt-temp - сдвиг указателя в nnew
        // temp - имитация старых индексов
        // new - массив новых индексов в соотв-ии с массивом tmp,
        // т.е., например temp[0] - старый индекс элемента, nnew[0] - новый
        for (auto pt = temp; pt != temp + sz; ++pt) {
            if (*pt == nnew[pt - temp]) continue;

            auto tmp = *pt; // при смене мест элементов - значение, которое будет перемещено из той ячейки, куда встанет
                            // правильный элемент

            auto tmp_idx = nnew[pt - temp]; // индекс, на который будет перемещен промежуточный элемент tmp

            *pt = tmp_idx; // новое значение для текущей позиции

            while (*(temp + tmp_idx) != nnew[pt - temp]) {  // ищем место для элемента tmp, нужно поставить на место, откуда
                                                            // был взят элемент для pt. Однако, если это место уже занято
                                                            // "правильным" элементом, то ищем место, откуда был взят правильный
                                                            // элемент
                tmp_idx = *(temp + tmp_idx);
            }

            *(temp + tmp_idx) = tmp;

            auto x = *pt;
            auto y = tmp;

            auto xx = 0;
            auto yy = 0;

            if (x < y) {
                xx = x;
                yy = y;
            }
            else if (x > y) {
                xx = y;
                yy = x;
            }
            else continue;

            edx_print("Swap elements at indices ");
            edx_print_i32(xx + 1);
            edx_print(" and ");
            edx_print_i32(yy + 1);
            edx_print_char('.');
            edx_print_char('\n');
        }


    edx_print("No more swaps needed.\n");

    for (auto pt = elems.begin(); pt != elems.end(); ++pt) {
        edx_print_i32(pt->Val);
        edx_print_char(' ');
    }

    edx_close();
    return 0;
}