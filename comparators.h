#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "dataset.h"

int compareNumeric(const Dataset1Row* a, const Dataset1Row* b);
int compareString(const Dataset1Row* a, const Dataset1Row* b);
int compareComposite(const Dataset1Row* a, const Dataset1Row* b);

#endif // COMPARATORS_H
