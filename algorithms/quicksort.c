#include <stdlib.h>
#include <string.h>

#include "quicksort.h"

// Swap two elements in the array
void swap(Dataset1Row *a, Dataset1Row *b) {
    Dataset1Row temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for quicksort
size_t partition(Dataset1Row *base, size_t low, size_t high, Comparator comparator) {
    Dataset1Row pivot = base[high]; // Use the last element as the pivot
    size_t i = low - 1;

    for (size_t j = low; j < high; j++) {
        if (comparator(&base[j], &pivot) < 0) {
            i++;
            swap(&base[i], &base[j]);
        }
    }

    swap(&base[i + 1], &base[high]);
    return i + 1;
}

// Recursive quicksort function
void quickSortHelper(Dataset1Row *base, size_t low, size_t high, Comparator comparator) {
    if (low < high) {
        size_t pi = partition(base, low, high, comparator);

        if (pi > 0) { // Ensure no underflow for size_t
            quickSortHelper(base, low, pi - 1, comparator);
        }
        quickSortHelper(base, pi + 1, high, comparator);
    }
}

// Public interface for quicksort
void quickSort(Dataset1Row *base, size_t n, Comparator comparator) {
    if (n > 1) {
        quickSortHelper(base, 0, n - 1, comparator);
    }
}