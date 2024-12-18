#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_STRING_LENGTH 10

// Function to generate a random string
void generate_random_string(char* buffer, size_t length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (size_t i = 0; i < length; i++) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    buffer[length] = '\0'; // Null-terminate the string
}

// Function to generate a random date
void generate_random_date(char* buffer) {
    int year = rand() % 10 + 2015;  // Random year between 2015 and 2024
    int month = rand() % 12 + 1;    // Random month between 1 and 12
    int day = rand() % 28 + 1;      // Random day between 1 and 28 (to simplify)
    sprintf(buffer, "%04d-%02d-%02d", year, month, day);
}

// Quick sort comparator function
int compare(const void* a, const void* b) {
    double arg1 = *(const double*)a;
    double arg2 = *(const double*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

// Function to generate the dataset
void generate_dataset(const char* filename, size_t num_rows, int partially_sorted) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Write CSV header
    fprintf(file, "date,location,value,transaction_count\n");

    char date[11];         // Buffer to hold generated date
    char random_string[MAX_STRING_LENGTH + 1]; // Buffer for random string
    double* values = NULL;

    if (partially_sorted) {
        values = (double*)malloc(num_rows * sizeof(double));
        if (!values) {
            perror("Memory allocation failed");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < num_rows; i++) {
        if (i % (num_rows / 10) == 0) {
            printf("Progress: %.0f%%\n", (double)i / num_rows * 100);
        }

        generate_random_date(date);
        generate_random_string(random_string, MAX_STRING_LENGTH);
        double value = ((double)rand() / RAND_MAX) * 10000.0;  // Random double value between 0 and 10000
        int transaction_count = rand() % 100 + 1;              // Random integer between 1 and 100

        if (partially_sorted) {
            values[i] = value;
        } else {
            fprintf(file, "%s,%s,%.2f,%d\n", date, random_string, value, transaction_count);
        }
    }


    if (partially_sorted) {
        printf("Sorting partially sorted dataset...\n");
        size_t half = num_rows / 2;
        qsort(values, half, sizeof(double), compare);

        // Write the partially sorted dataset to the file
        for (size_t i = 0; i < num_rows; i++) {
            generate_random_date(date);
            generate_random_string(random_string, MAX_STRING_LENGTH);
            int transaction_count = rand() % 100 + 1;  // Random integer between 1 and 100
            fprintf(file, "%s,%s,%.2f,%d\n", date, random_string, values[i], transaction_count);
        }


        free(values);
    }

    fclose(file);
    printf("Finished writing dataset to %s\n", filename);
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    printf("Generating dataset with 10k rows...\n");
    generate_dataset("datasets/dataset_10k.csv", 10000, 0);

    printf("Generating dataset with 100k rows...\n");
    generate_dataset("datasets/dataset_100k.csv", 100000, 0);

    printf("Generating dataset with 500k rows...\n");
    generate_dataset("datasets/dataset_500k.csv", 500000, 0);

    // Generate a dataset with 1 million rows
    printf("Generating dataset with 1 million rows...\n");
    generate_dataset("datasets/dataset_1m.csv", 1000000, 0);

    // Generate a dataset with 10 million rows
    printf("Generating dataset with 10 million rows...\n");
    generate_dataset("datasets/dataset_10m.csv", 10000000, 0);

    // Generate a partially sorted dataset with 1 million rows
    printf("Generating partially sorted dataset with 1 million rows...\n");
    generate_dataset("datasets/dataset_1m_sorted.csv", 1000000, 1);

    // Generate a partially sorted dataset with 10 million rows
    printf("Generating partially sorted dataset with 10 million rows...\n");
    generate_dataset("datasets/dataset_10m_sorted.csv", 10000000, 1);

    printf("Datasets generated successfully.\n");
    return 0;
}