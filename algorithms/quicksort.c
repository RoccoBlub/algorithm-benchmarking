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


size_t randomPartition(Dataset1Row *base, size_t low, size_t high, Comparator comparator) {
    size_t randomIndex = low + rand() % (high - low + 1);
    swap(&base[randomIndex], &base[high]); // Move random element to the end
    return partition(base, low, high, comparator);
}

size_t medianOfThreePartition(Dataset1Row *base, size_t low, size_t high, Comparator comparator) {
    size_t mid = low + (high - low) / 2;

    // Find the median of base[low], base[mid], base[high]
    if (comparator(&base[low], &base[mid]) > 0) swap(&base[low], &base[mid]);
    if (comparator(&base[low], &base[high]) > 0) swap(&base[low], &base[high]);
    if (comparator(&base[mid], &base[high]) > 0) swap(&base[mid], &base[high]);

    // Move the median to the end
    swap(&base[mid], &base[high]);
    return partition(base, low, high, comparator);
}


// Recursive quicksort function
void quickSortHelper(Dataset1Row *base, size_t low, size_t high, Comparator comparator) {
    if (low < high) {
        size_t pi = randomPartition(base, low, high, comparator);

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