#ifndef TIMSORT_H
#define TIMSORT_H

#include "../dataset.h"

// Type definition for comparator function
typedef int (*Comparator)(const Dataset1Row *, const Dataset1Row *);

// TimSort function prototype
void timSort(Dataset1Row arr[], size_t n, Comparator comparator);

#endif // TIMSORT_H