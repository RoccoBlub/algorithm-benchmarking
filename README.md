# Algorithm Benchmarking Framework

This project provides a framework for benchmarking various sorting algorithms. It allows you to measure the performance of different algorithms across multiple datasets with various characteristics. The project is written in C and is a work in progress, so some issues may arise, especially with large datasets.

## Features

### Sorting Algorithms

The project includes the following sorting algorithms:

- Implemented by me:
  - HeapSort (Working)
  - QuickSort (Working)
  - MergeSort (Working)
  - CountingSort (Working)
  - RadixSort (Working)
  - TimSort (Working)
  - BubbleSort (WIP)
  - HybridQuickSort (WIP)
  - InsertionSort (WIP)
  - ShellSort (WIP)
- Built-in library function:
  - qsort (Working)

### Components

1. Dataset Generator
   - Creates datasets for benchmarking. Datasets currently supported include:
     - Sorted
     - Reverse Sorted
     - Nearly Sorted
     - Heavy Duplication
2. Benchmarking Program
   - Runs the benchmarks and measures the performance of each algorithm on various datasets.

## Usage

### Dataset generator

The dataset generator creates datasets for benchmarking. You can adjust the dataset size and type by modifying the `generate_dataset` calls in `datasetgenerator.c`:
```c++
generate_dataset("datasets/sorted.csv", 10000, 1, 1);
```
**Parameters:**
- **Row Count:** The first number specifies the number of rows in the dataset.
- **Dataset Type:** The first `1` indicates the type of dataset:
  - `1`: Fully sorted
  - `2`: Reverse sorted
  - `3`: Nearly sorted
  - `4`: Heavy duplication
- **Focus Type:** The second `1` specifies the dataset's focus:
  - `1`: Numeric dataset
  - `2`: String dataset
  - `3`: Composite dataset (e.g., a mix of numeric and string data)

**For example:**
```c++
generate_dataset("datasets/reverse_sorted.csv", 5000, 2, 1);
```
This generates a numeric dataset with 5,000 rows, where the data is reverse sorted.

After editing, compile and run the dataset generator:
```shell
gcc -o dataset_generator datasetgenerator.c dataset.c logging.c
./dataset_generator
```

### Benchmarking Progam
Specify the datasets and algorithms to benchmark in `main.c`:

**Datasets:**
```c++
const char *numericDatasets[] = {
    "datasets/sorted.csv",
    "datasets/reverse_sorted.csv",
    "datasets/nearly_sorted.csv",
    "datasets/heavy_duplication.csv"
};
```
**Sorting Algorithms:**

```c++
const char *sortTypes[] = {"HeapSort", "Qsort", "MergeSort", "RadixSort", "TimSort", "CountingSort", "QuickSort"};
```

Compile the benchmarking program:

```shell
gcc -o program main.c benchmark.c dataset.c comparators.c logging.c algorithms/*.c -lm
./program
```

## Known Issues
1. **Memory Issues with Large Datasets**
   - The program may encounter memory allocation issues when handling large datasets. This is a known limitation and will be addressed in future updates.
2. Work in Progress
   - Some algorithms are incomplete or may contain bugs:
      - BubbleSort
      - HybridQuickSort 
      - InsertionSort 
      - ShellSort
         
    
## To-Do

- Fix memory management issues for large datasets.
- Complete and debug the work-in-progress algorithms.
- Optimize build process and provide a Makefile for convenience.
- Add support for additional dataset types and custom dataset generation parameters.

## Disclaimer
This project is still under development, and there may be bugs or incomplete features. Use at your own risk.