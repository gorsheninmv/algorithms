#ifdef DEBUG
extern "C" {
#include <io.h>
}
#else
#include "edx-io.h"
#endif

int main() {
    edx_open();

    auto a = edx_next_i32();
    auto b = static_cast<long long int>(edx_next_i32());
    auto r = a + b*b;

    edx_print_i64(r);

    edx_close();

    return 0;
}