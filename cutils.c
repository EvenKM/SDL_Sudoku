#ifndef C_UTILS
#define C_UTILS

#include <stdio.h>
#include "cutils.h"

void printString(string_t string) {
    printf("Your string: %s\n", string.data);
}

#endif