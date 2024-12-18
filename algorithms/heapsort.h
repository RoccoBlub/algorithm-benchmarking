#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stddef.h>
#include "../dataset.h"

typedef int (*Comparator)(const Dataset1Row *, const Dataset1Row *);

void heapify(Dataset1Row arr[], size_t n, size_t i, Comparator comparator);

void heapSort(Dataset1Row arr[], size_t n, Comparator comparator);

#endif
