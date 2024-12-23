#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_STRING_LENGTH 20

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

// Helper to write a row to the dataset file
void write_row(FILE* file, const char* date, const char* location, double value, int transaction_count) {
    fprintf(file, "%s,%s,%.2f,%d\n", date, location, value, transaction_count);
}

// Function to generate a dataset focused on numeric sorting
void generate_numeric_dataset(FILE* file, size_t num_rows, int dataset_type) {
    for (size_t i = 0; i < num_rows; i++) {
        char date[11];
        char location[MAX_STRING_LENGTH + 1] = "static_location"; // Same string for all rows
        generate_random_date(date);

        double value;
        if (dataset_type == 1) { // Fully sorted
            value = (double)i;
        } else if (dataset_type == 2) { // Reverse sorted
            value = (double)(num_rows - i);
        } else if (dataset_type == 3) { // Nearly sorted
            value = (i % 10 == 0) ? rand() % 10000 : (double)i;
        } else if (dataset_type == 4) { // Heavy duplication
            value = (i < num_rows * 0.8) ? 42.0 : rand() % 10000;
        } else { // Random
            value = rand() % 10000;
        }

        write_row(file, date, location, value, 1);
    }
}

// Function to generate a dataset focused on string sorting
void generate_string_dataset(FILE* file, size_t num_rows, int dataset_type) {
    for (size_t i = 0; i < num_rows; i++) {
        char date[11];
        char location[MAX_STRING_LENGTH + 1];
        generate_random_date(date);

        if (dataset_type == 1) { // Fully sorted
            sprintf(location, "location_%05zu", i);
        } else if (dataset_type == 2) { // Reverse sorted
            sprintf(location, "location_%05zu", num_rows - i);
        } else if (dataset_type == 3) { // Nearly sorted
            if (i % 10 == 0) {
                generate_random_string(location, MAX_STRING_LENGTH);
            } else {
                sprintf(location, "location_%05zu", i);
            }
        } else if (dataset_type == 4) { // Heavy duplication
            strcpy(location, (i < num_rows * 0.8) ? "common_prefix" : "unique_suffix");
        } else { // Random
            generate_random_string(location, rand() % MAX_STRING_LENGTH + 1);
        }

        double value = 42.0; // Fixed value for string-focused datasets
        write_row(file, date, location, value, 1);
    }
}

// Function to generate a dataset focused on composite sorting
void generate_composite_dataset(FILE* file, size_t num_rows, int dataset_type) {
    for (size_t i = 0; i < num_rows; i++) {
        char date[11];
        char location[MAX_STRING_LENGTH + 1];
        generate_random_date(date);
        generate_random_string(location, MAX_STRING_LENGTH);

        if (dataset_type == 1) { // Fully sorted
            sprintf(date, "2023-01-%02zu", i % 28 + 1);
        } else if (dataset_type == 2) { // Reverse sorted
            sprintf(date, "2023-01-%02zu", (num_rows - i) % 28 + 1);
        } else if (dataset_type == 3) { // Nearly sorted
            if (i % 10 == 0) {
                sprintf(date, "2023-01-01");
            } else {
                sprintf(date, "2023-01-%02zu", i % 28 + 1);
            }
        } else if (dataset_type == 4) { // Heavy duplication
            strcpy(date, (i < num_rows * 0.8) ? "2023-01-01" : "2023-01-15");
        } else { // Random
            generate_random_date(date);
        }

        double value = rand() % 10000;
        int transaction_count = (i % 2 == 0) ? i % 100 : rand() % 100;

        write_row(file, date, location, value, transaction_count);
    }
}

// Wrapper to generate datasets for different sorting focuses
void generate_dataset(const char* filename, size_t num_rows, int dataset_type, int focus_type) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "date,location,value,transaction_count\n");

    if (focus_type == 1) {
        generate_numeric_dataset(file, num_rows, dataset_type);
    } else if (focus_type == 2) {
        generate_string_dataset(file, num_rows, dataset_type);
    } else if (focus_type == 3) {
        generate_composite_dataset(file, num_rows, dataset_type);
    }

    fclose(file);
    printf("Generated %s\n", filename);
}

int main() {
    srand(time(NULL));

    printf("Generating datasets for benchmarks...\n");

    // Generate datasets with numeric focus
    generate_dataset("datasets/sorted.csv", 10000, 1, 1);
    generate_dataset("datasets/reverse_sorted.csv", 10000, 2, 1);
    generate_dataset("datasets/nearly_sorted.csv", 10000, 3, 1);
    generate_dataset("datasets/heavy_duplication.csv", 10000, 4, 1);

    printf("Datasets generated successfully!\n");
    return 0;
}
