#include <iostream>
#include <unordered_map>
#include <queue>
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

    std::queue<int> q;
    int registers[26];
    memset(registers, 0, 4 * 26);

    std::unordered_map<std::string, int> labels;
    const int MAX_SZ = 100000;
    std::string * prog = new std::string[MAX_SZ];
    int sz = 0;
    const int BUF_SZ = 128;
    char buf[BUF_SZ];

    while (true) {
        if(!edx_next_bounded(buf, BUF_SZ)) break;

        prog[sz] = buf;

        if (*buf == ':') {
            std::string label(buf + 1);
            labels[label] = sz;
        }

        ++sz;
    }

    for (auto i = 0; i < sz; ++i) {
        switch (prog[i][0]) {
            case '+':
            {
                auto a = q.front(); q.pop();
                auto b = q.front(); q.pop();
                auto r = (a + b) & 65535;
                q.push(r);
                break;
            }
            case '-':
            {
                auto a = q.front(); q.pop();
                auto b = q.front(); q.pop();
                auto r = (a - b) & 65535;
                q.push(r);
                break;
            }
            case '*':
            {
                auto a = q.front(); q.pop();
                auto b = q.front(); q.pop();
                auto r = (a * b) & 65535;
                q.push(r);
                break;
            }
            case '/':
            {
                auto a = q.front(); q.pop();
                auto b = q.front(); q.pop();
                q.push(b ? (a / b) & 65535 : 0);
                break;
            }
            case '%':
            {
                auto a = q.front(); q.pop();
                auto b = q.front(); q.pop();
                q.push(b ? (a % b) & 65535 : 0);
                break;
            }
            case '>':
            {
                auto a = q.front(); q.pop();
                int reg_num = prog[i][1] - 97;
                registers[reg_num] = a;
                break;
            }
            case '<':
            {
                int reg_num = prog[i][1] - 97;
                auto a = registers[reg_num];
                q.push(a);
                break;
            }
            case 'P': {
                switch (prog[i].size())
                {
                    case 1: {
                        auto a = q.front(); q.pop();
                        edx_print_i32(a);
                        PRINT_NEW_LINE;
                        break;
                    }
                    case 2: {
                        int reg_num = prog[i][1] - 97;
                        auto a = registers[reg_num];
                        edx_print_i32(a);
                        PRINT_NEW_LINE;
                        break;
                    }
                }
                break;
            }
            case 'C': {
                switch (prog[i].size())
                {
                    case 1: {
                        auto a = q.front(); q.pop();
                        edx_print_char(a & 255);
                        break;
                    }
                    case 2: {
                        int reg_num = prog[i][1] - 97;
                        auto a = registers[reg_num];
                        edx_print_char(a & 255);
                        break;
                    }
                }
                break;
            }
            case 'J':
            {
                std::string lb(prog[i], 1);
                i = labels[lb];
                break;
            }
            case 'Z':
            {
                auto reg_num = prog[i][1] - 97;
                if (registers[reg_num] == 0){
                    std::string lb(prog[i], 2);
                    i = labels[lb];
                }
                break;
            }
            case 'E':
            {
                auto reg_num_a = prog[i][1] - 97;
                auto reg_num_b = prog[i][2] - 97;
                if (registers[reg_num_a] == registers[reg_num_b]){
                    std::string lb(prog[i], 3);
                    i = labels[lb];
                }
                break;
            }
            case 'G':
            {
                auto reg_num_a = prog[i][1] - 97;
                auto reg_num_b = prog[i][2] - 97;
                if (registers[reg_num_a] > registers[reg_num_b]){
                    std::string lb(prog[i], 3);
                    i = labels[lb];
                }
                break;
            }
            case 'Q':
            {
                i = sz + 1;
                break;
            }
            case ':':
            {
                break;
            }
            default:
            {
                if (prog[i].size() > 0) q.push(std::stoi(prog[i]));
                break;
            }
        }
    }

    delete[] prog;

    edx_close();

    return 0;
}


