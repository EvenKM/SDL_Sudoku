#ifndef C_UTILS
#define C_UTILS

#include "cutils.h"

void print_string(string_t string) {
    printf("Your string: %s\n", string_to_c_string(string));
}


string_array_t create_string_array(int len)
{
    string_array_t array = { 0 };
    array.strings = calloc(len, sizeof(string_t));
    array.length = len;
    return array;
}

void destroy_string_array(string_array_t array) {
    for (int i = 0; i < array.length; i++) {
        free(&array.strings[i]);
    }
}

//TODO: Does not work properly when the splitting character is first or last... Fix it! (or, for that matter, if two splitting characters are next to each other...)
string_array_t split_string(string_t string, char character) {
    int prev_pos = -1;
    int pos = 0;
    int count = 0;
    
    while (pos < string.length - 1) {
        if (string.data[pos] == character && pos-prev_pos > 1) {
            count++;
            prev_pos = pos;
        }
        pos++;
    }

    string_array_t array = create_string_array(count + 1);
    prev_pos = -1; //Dumb hack to make things easier.
    pos = 0;
    count = 0;
    while (pos < string.length - 1) {
        if (string.data[pos] == character && pos-prev_pos > 1) {
            array.strings[count] = slice_string_copy(string, prev_pos + 1, pos);
            count++;
            prev_pos = pos;
        }
        pos++;
    }
    array.strings[count] = slice_string_copy(string, prev_pos + 1, -1);
    return array;
}

string_t slice_string(string_t string, int from, int to)
{
    string_t slice = {0};
    int new_to;
    int new_from;
    if (to < 0) new_to = string.length + to;
    if (from < 0) new_from = string.length + from;
    
    if (new_from == new_to) {
        slice.data = string.data + new_from;
        slice.length = 1;
        return slice;
    }

    if (new_from > new_to) {
        slice.data = "";
        slice.length = 0;
        return slice;
    }
    
    slice.data = string.data + new_from;
    slice.length = new_to-new_from;
    return slice;
}

string_t slice_string_copy(string_t string, int from, int to) {
    string_t slice = { 0 };
    int new_to = to;
    int new_from = from;
    if (to < 0) new_to = string.length + to;
    if (from < 0) new_from = string.length + from;

    if (new_from == new_to) {
        slice.length = 1;
        slice.data = malloc(sizeof(char) * 1);
        memcpy(slice.data, string.data + new_from, slice.length);
        return slice;
    }

    if (new_from > new_to) {
        slice.data = NULL;
        slice.length = 0;
        return slice;
    }

    slice.length = new_to - new_from;
    slice.data = malloc(sizeof(char) * slice.length);
    memcpy(slice.data, string.data + new_from, slice.length);
    return slice;
}

char* string_to_c_string(string_t string) {
    char* c_string = calloc(string.length + 1, sizeof(char));
    if (c_string == NULL) return " ";
    memcpy(c_string, string.data, string.length);
    return c_string;
}

#endif