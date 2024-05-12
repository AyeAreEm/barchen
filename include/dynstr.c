#include "dynstr.h"
#include <stdio.h>
#include <string.h>

string string_new() {
    string new;
    new.len = 0;
    new.cap = 50;
    new.data = malloc(new.cap); // * sizeof(char)
    if (new.data == NULL) {
        exit(1);
    }

    return new;
}

void resize_string(string *str, size_t modifier) {
    str->cap += (str->len + modifier) * 2;
    str->data = realloc(str->data, str->cap + 1); // * sizeof(char)
}

string string_from(char *str) {
    string new = string_new();
    size_t str_len = strlen(str);

    if (str_len > new.cap) {
        resize_string(&new, str_len);
    }

    new.len = str_len;
    strcpy_s(new.data, new.cap + 1, str);
    return new;
}

bool string_cmp(string x, string y) {
    if (x.len != y.len) {
        return false;
    }

    for (size_t i = 0; i < x.len; i++) {
        if (x.data[i] != y.data[i]) {
            return false;
        }
    }

    return true;
}

void string_push(string *str, char c) {
    if (str->len + 1 >= str->cap) {
        resize_string(str, 0);
    }

    str->data[str->len] = c;
    str->len++;
    // str->data[str->len] = '\0';
}

void string_pushstr(string *value, string *source) {
    if (value->len + source->len >= value->cap) {
        resize_string(value, source->len);
    }

    for (size_t i = 0; i < source->len; i++) {
        value->data[value->len] = source->data[i];
        value->len++;
        // value->data[value->len] = '\0';
    }
}

char string_pop(string *str) {
    char ch = str->data[str->len-1];
    str->len -= 1;
    return ch;
}

void string_clear(string *str) {
    free(str->data);

    str->len = 0;
    str->cap = 50;
    str->data = malloc(str->cap); // * sizeof(char)
    if (str->data == NULL) {
        exit(1);
    }
}

int string_to_int(string str) {
    long num = 0;

    for (size_t i = 0; i < str.len; i++) {
        num = (num * 10) + (str.data[i] - '0');
    }

    if (num >= INT_MIN && num <= INT_MAX) {
        return (int)num;
    } else {
        exit(1);
    }
}

void string_print(string str) {
    for (size_t i = 0; i < str.len; i++) {
        printf("%c", str.data[i]);
    }

    printf("\n");
}

char *string_to_cstr(string str) {
    if (str.len == 0) {
        char *cstr = malloc(1);
        if (cstr == NULL) {
            exit(1);
        }
        cstr[0] = '\0';
        return cstr;
    }

    char *cstr = malloc(str.len+1);
    if (cstr == NULL) {
        exit(1);
    }

    strncpy_s(cstr, str.len+1, str.data, str.len);
    cstr[str.len] = '\0';
    return cstr;
}
