#include <stdio.h>
void gy_print(const char *text, size_t length) {
    fwrite(text, 1, length, stdout);
    fflush(stdout);
}