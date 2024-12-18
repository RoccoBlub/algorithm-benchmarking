#ifndef MERGESORT_H
#define MERGESORT_H

#include "../dataset.h"

typedef int (*Comparator)(const Dataset1Row*, const Dataset1Row*);

void mergeSort(Dataset1Row arr[], size_t n, Comparator comparator);

#endif // MERGESORT_H
