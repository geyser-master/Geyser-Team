#include <stdio.h>
#include <stdlib.h>
const char *gy_toString_int(int value) {
    char *buffer = malloc(12);
    if (buffer != nullptr) {
        snprintf(buffer, 12, "%d", value);
    }
    return buffer;
}