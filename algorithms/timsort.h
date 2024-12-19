#ifndef TIMSORT_H
#define TIMSORT_H

#include "../dataset.h"
#include "../comparators.h"

// TimSort function prototype
void timSort(Dataset1Row *arr, size_t n, Comparator comparator);

#endif // TIMSORT_H