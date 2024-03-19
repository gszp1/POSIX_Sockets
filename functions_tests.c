#include "utils.h"

int main() {
    printf ("%d\n", check_endianess());
    double num = 9;
    printf("%f\n", num);
    uint64_t num_b = get_double_bigendian(num);
    printf("%lu\n", num_b);
    num = get_double_little_endian(num_b);
    printf("%f\n", num);
    return 0;
}