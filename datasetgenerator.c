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
void generate_challenging_dataset(const char* filename, size_t num_rows, int partially_sorted, int dataset_type) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "date,location,value,transaction_count\n");

    char date[11];
    char random_string[MAX_STRING_LENGTH + 1];
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

        // Primary Key: Date
        if (i % 10 == 0) {
            strcpy(date, "2024-01-01"); // Identical primary key
        } else {
            generate_random_date(date);
        }

        // Secondary Key: Random String
        if (rand() % 2 == 0) {
            sprintf(random_string, "prefix_%d", rand() % 1000); // Common prefix
        } else {
            generate_random_string(random_string, rand() % MAX_STRING_LENGTH + 1);
        }

        // Value Distribution Based on Dataset Type
        double value;
        if (dataset_type == 1) { // Fully sorted
            value = i;
        } else if (dataset_type == 2) { // Reverse sorted
            value = num_rows - i;
        } else if (dataset_type == 3) { // Nearly sorted with noise
            value = (i % 10 == 0) ? ((double)rand() / RAND_MAX) * 10000.0 : i;
        } else if (dataset_type == 4) { // Heavy duplication
            value = (i < num_rows * 0.8) ? 42.0 : ((double)rand() / RAND_MAX) * 10000.0;
        } else if (dataset_type == 5) { // Clustered values
            value = (i < num_rows * 0.9) ? rand() % 100 : 10000 + rand() % 100;
        } else {
            value = ((double)rand() / RAND_MAX) * 10000.0; // Default random values
        }

        // Transaction Count
        int transaction_count = (rand() % 2 == 0) ? i % 100 : rand() % 100;

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

        for (size_t i = 0; i < num_rows; i++) {
            generate_random_date(date);
            generate_random_string(random_string, MAX_STRING_LENGTH);
            int transaction_count = rand() % 100 + 1;  // Generate transaction count
            fprintf(file, "%s,%s,%.2f,%d\n", date, random_string, values[i % half], transaction_count);
        }

        free(values);
    }

    fclose(file);
    printf("Finished writing dataset to %s\n", filename);
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    printf("Generating fully sorted dataset with 1 million rows...\n");
    generate_challenging_dataset("datasets/dataset_1m_sorted.csv", 1000000, 0, 1);

    printf("Generating reverse sorted dataset with 1 million rows...\n");
    generate_challenging_dataset("datasets/dataset_1m_reverse_sorted.csv", 1000000, 0, 2);

    printf("Generating nearly sorted dataset with 1 million rows...\n");
    generate_challenging_dataset("datasets/dataset_1m_nearly_sorted.csv", 1000000, 0, 3);

    printf("Generating dataset with heavy duplication and 1 million rows...\n");
    generate_challenging_dataset("datasets/dataset_1m_duplicated.csv", 1000000, 0, 4);

    printf("Generating clustered dataset with 1 million rows...\n");
    generate_challenging_dataset("datasets/dataset_1m_clustered.csv", 1000000, 0, 5);

    printf("Datasets generated successfully.\n");
    return 0;
}
