#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "dataset.h"

typedef int (*Comparator)(const Dataset1Row *, const Dataset1Row *);

int compareNumeric(const Dataset1Row* a, const Dataset1Row* b);
int compareString(const Dataset1Row* a, const Dataset1Row* b);
int compareComposite(const Dataset1Row* a, const Dataset1Row* b);

#endif // COMPARATORS_H
