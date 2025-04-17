#ifndef C_UTILS_H
#define C_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct String
{
    char* data;
    int length;
} string_t;

typedef struct StringArray {
    string_t *strings;
    int length;
}string_array_t;

string_array_t create_string_array(int len);

void destroy_string_array(string_array_t array);

string_array_t split_string(string_t string, char character);

string_t slice_string(string_t string, int from, int to);

string_t slice_string_copy(string_t string, int from, int to);

char* string_to_c_string(string_t string);

void print_string(string_t string);



#endif
