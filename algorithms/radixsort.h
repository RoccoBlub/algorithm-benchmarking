#ifndef RADIXSORT_H
#define RADIXSORT_H

#include "../dataset.h"
#include <stddef.h>

typedef int (*Comparator)(const Dataset1Row *, const Dataset1Row *);

// RadixSort function prototypes
void radixSort(Dataset1Row arr[], size_t n, Comparator comparator);             // For integers
void radixSortStrings(Dataset1Row arr[], size_t n);      // For strings
void radixSortComposite(Dataset1Row arr[], size_t n);    // For composite types

#endif // RADIXSORT_H