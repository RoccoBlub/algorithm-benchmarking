#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stddef.h>

#include "../dataset.h"

typedef int (*Comparator)(const Dataset1Row *, const Dataset1Row *);

// Quicksort function
void quickSort(Dataset1Row *base, size_t n, Comparator comparator);

#endif // QUICKSORT_H