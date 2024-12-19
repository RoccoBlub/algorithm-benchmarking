#ifndef COUNTINGSORT_H
#define COUNTINGSORT_H

#include "../dataset.h"
#include "../comparators.h"

// Function to perform Counting Sort on Dataset1Row array
void countingSort(Dataset1Row *arr, size_t n, Comparator comparator);

#endif // COUNTINGSORT_H
