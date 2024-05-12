#ifndef _DYNAMIC_STRING_
#define _DYNAMIC_STRING_

#include <stdlib.h>
#include <stdbool.h>
typedef struct string {
    char *data;
    size_t len;
    size_t cap;

} string;

string string_new();
string string_from(char *str);
bool string_cmp(string str1, string str2);
void string_push(string *str, char ch);
void string_pushstr(string *dest, string *src);
char string_pop(string *str);
void string_clear(string *str);
int string_to_int(string str);
char *string_to_cstr(string str);
void string_print(string str);

#endif
