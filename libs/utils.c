#include "utils.h"

#include <stdio.h>

void merge(void* arr[], int l, int m, int r, int (*compare)(const void*, const void*)) {
    int n1 = m - l + 1;
    int n2 = r - m;

    void* L[n1], * R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (compare(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(void* arr[], int l, int r, int (*compare)(const void*, const void*)) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m, compare);
        merge_sort(arr, m + 1, r, compare);
        merge(arr, l, m, r, compare);
    }
}

int comparar_strings(const char* a, const char* b) {
    return strcmp(a, b);
}

void paginacao(void **array, int size, int dataPage, void (*fshow)(void *data))
{
    int pag = 0;
    int max = (size + dataPage - 1) / dataPage; // total de páginas arredondado pra cima
    int escolha = 0;

    do {
        printf("\n\n\n");

        int start = pag * dataPage;
        int end = start + dataPage;
        if (end > size) end = size;

        printf("Página %d de %d\n", pag + 1, max);
        printf("=====================\n");

        for (int i = start; i < end; i++) {
            fshow(array[i]);
        }

        printf("\nEscolha a página (1 a %d), ou -1 para sair: ", max);
        scanf("%d", &escolha);

        if (escolha >= 1 && escolha <= max) {
            pag = escolha - 1;
        }

    } while (escolha != -1);
}