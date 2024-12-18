#include <string.h>
#include "dataset.h"

int compareNumeric(const Dataset1Row *a, const Dataset1Row *b) {
    return (a->value > b->value) - (a->value < b->value);
}

int compareString(const Dataset1Row *a, const Dataset1Row *b) {
    return strcmp(a->location, b->location);
}

int compareComposite(const Dataset1Row *a, const Dataset1Row *b) {
    int dateComparison = strcmp(a->date, b->date);
    if (dateComparison != 0) return dateComparison;
    return a->transaction_count - b->transaction_count;
}