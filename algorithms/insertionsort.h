#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include "../dataset.h"

// Type definition for comparator function
typedef int (*Comparator)(const Dataset1Row *, const Dataset1Row *);

// InsertionSort function prototype
void insertionSort(Dataset1Row arr[], size_t n, Comparator comparator);

#endif // INSERTIONSORT_H
