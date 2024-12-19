#ifndef DATASET_H
#define DATASET_H

#include <stddef.h>

typedef struct {
    char date[20];
    char location[100];
    double value;
    int transaction_count;
} Dataset1Row;

// Callback for processing CSV lines
typedef void (*LineProcessor)(const char *line, Dataset1Row *data, int rowIndex);

// Function declarations
int fileExists(const char *path);
int parseCSV(const char *filename, LineProcessor processor, Dataset1Row **data, size_t *capacity, size_t elemSize);
void processLineDataset1(const char *line, Dataset1Row *data, int rowIndex);

#endif