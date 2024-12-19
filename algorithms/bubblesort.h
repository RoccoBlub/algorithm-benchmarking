#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "../dataset.h"
#include "../comparators.h"

// Function to perform Bubble Sort on Dataset1Row array
void bubbleSort(Dataset1Row *arr, size_t n, Comparator comparator);

#endif // BUBBLESORT_H
