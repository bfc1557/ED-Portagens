#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>

void merge_sort(void* arr[], int l, int r, int (*compare)(const void*, const void*));
int comparar_strings(const char* a, const char* b);

#endif