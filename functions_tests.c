#include "utils.h"

int main() {
    printf ("%d\n", check_endianess());
    double num = 9;
    printf("%f\n", num);
    uint64_t num_b = htond(num);
    printf("%lu\n", num_b);
    num = ntohd(num_b);
    printf("%f\n", num);

    time_t t;
    time(&t);
    char* date = ctime(&t);
    *(date + strlen(date) - 1) = '\0';
    printf("%s\n", date);
    printf("%lu", strlen(date));
    return 0;
}