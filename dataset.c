#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dataset.h"

int fileExists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

int parseCSV(const char *filename, LineProcessor processor, Dataset1Row **data, size_t *capacity, size_t elemSize) {
    if (!fileExists(filename)) {
        fprintf(stderr, "Error: File %s does not exist.\n", filename);
        return -1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char line[1024];
    int rowIndex = 0;
    long bytesRead = 0;

    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error reading header from %s\n", filename);
        fclose(file);
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        bytesRead += strlen(line);

        if (rowIndex >= *capacity) {
            *capacity *= 2;
            void *newData = realloc(*data, *capacity * elemSize);
            if (!newData) {
                perror("Memory reallocation failed");
                fclose(file);
                return -1;
            }
            *data = newData;
        }

        processor(line, *data, rowIndex);
        rowIndex++;

        if (rowIndex % 1000 == 0) {
            printf("\rProcessing %s: %.2f%% complete", filename, (double)bytesRead / fileSize * 100);
            fflush(stdout);
        }
    }

    printf("\rProcessing %s: 100.00%% complete\n", filename);
    fclose(file);
    return rowIndex;
}

void processLineDataset1(const char *line, Dataset1Row *data, int rowIndex) {
    sscanf(line, "%19[^,],%99[^,],%lf,%d",
           data[rowIndex].date,
           data[rowIndex].location,
           &data[rowIndex].value,
           &data[rowIndex].transaction_count);
}
