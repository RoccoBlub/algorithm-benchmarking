#include <math.h>
#include <stdlib.h>   // For size_t
#include "hybridquicksort.h"
#include "insertionsort.h"
#include "heapsort.h" // Fallback to HeapSort

#define INSERTION_SORT_THRESHOLD 16

// Partition the array and return the pivot index
static size_t partition(Dataset1Row *arr, size_t low, size_t high, Comparator comparator) {
    Dataset1Row pivot = arr[high];
    size_t i = low;

    for (size_t j = low; j < high; j++) {
        if (comparator(&arr[j], &pivot) <= 0) {
            Dataset1Row temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
        }
    }

    Dataset1Row temp = arr[i];
    arr[i] = arr[high];
    arr[high] = temp;

    return i;
}

// Recursive Hybrid QuickSort
static void hybridQuickSortRecursive(Dataset1Row *arr, size_t low, size_t high, size_t maxDepth, Comparator comparator) {
    while (low < high) {
        if (high - low + 1 < INSERTION_SORT_THRESHOLD) {
            // Use Insertion Sort for small arrays
            insertionSort(arr + low, high - low + 1, comparator);
            break;
        }

        if (maxDepth == 0) {
            // Switch to HeapSort if recursion depth limit is reached
            heapSort(arr + low, high - low + 1, comparator);
            break;
        }

        // Perform QuickSort partitioning
        size_t pivotIndex = partition(arr, low, high, comparator);

        // Tail call optimization: sort smaller partition first
        if (pivotIndex - low < high - pivotIndex) {
            hybridQuickSortRecursive(arr, low, pivotIndex - 1, maxDepth - 1, comparator);
            low = pivotIndex + 1;
        } else {
            hybridQuickSortRecursive(arr, pivotIndex + 1, high, maxDepth - 1, comparator);
            high = pivotIndex - 1;
        }
    }
}

// Hybrid QuickSort entry point
void hybridQuickSort(Dataset1Row *arr, size_t n, Comparator comparator) {
    size_t maxDepth = 2 * (log(n) / log(2)); // Calculate maximum depth
    hybridQuickSortRecursive(arr, 0, n - 1, maxDepth, comparator);
}
