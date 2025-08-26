#define ARENA_IMPLEMENTATION

#include "arena.h"
#include <stdio.h>
#include <string.h>

int main() {

    Arena *strings = arena_new(16);

    // "Some String"
    char *str1 = arena_alloc(strings, strlen("Some String") + 1);
    strcpy(str1, "Some String");

    puts(str1);

    printf("data_p: %p \n"
           "str_p: %p \n"
           "head: %zu \n"
           "cap: %zu \n"
           "next: %p \n\n",
           strings->data, str1, strings->head, strings->capacity, strings->next);

    char *str2 = arena_alloc(strings, strlen("Some String 2") + 1);
    strcpy(str2, "Some String 2");

    puts(str2);

    printf("data_p: %p \n"
           "str_p: %p \n"
           "head: %zu \n"
           "cap: %zu \n"
           "next: %p \n",
           strings->next->data, str2, strings->next->head, strings->next->capacity, strings->next->next);

    puts(str1);

    arena_free(strings);
}