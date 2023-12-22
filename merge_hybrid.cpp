#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include <mpi.h>
#include <algorithm>

void merge(long long arr[], long long temp[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    for (int i = 0; i < n1; i++)
        temp[i] = arr[l + i];

    int i = 0;
    int j = m + 1;
    int k = l;

    while (i < n1 && j <= r) {
        if (temp[i] <= arr[j]) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = temp[i];
        i++;
        k++;
    }
}

void mergeSort(long long arr[], long long temp[], int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (depth > 0) {
#pragma omp task firstprivate(arr, temp, l, m, depth)
            mergeSort(arr, temp, l, m, depth - 1);

#pragma omp task firstprivate(arr, temp, m, r, depth)
            mergeSort(arr, temp, m + 1, r, depth - 1);

#pragma omp taskwait
        } else {
            mergeSort(arr, temp, l, m, depth);
            mergeSort(arr, temp, m + 1, r, depth);
        }

        merge(arr, temp, l, m, r);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 1000000;
    const int chunk_size = n / size;
    long long *arr = new long long[n];
    long long *temp = new long long[n];

    if (rank == 0) {
        std::ifstream inFile("input.txt");
        for (int i = 0; i < n; i++) {
            inFile >> arr[i];
        }
        inFile.close();
    }

    MPI_Bcast(arr, n, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    std::ofstream timeFile("time_hybrid.txt", std::ios::app);

    for (int numThreads : {1, 2, 4, 8}) {
        omp_set_num_threads(numThreads);

        double start_time = MPI_Wtime();

#pragma omp parallel
#pragma omp single
        mergeSort(arr, temp, 0, n - 1, 2);

        double end_time = MPI_Wtime();
        double duration = end_time - start_time;

        if (rank == 0) {
            timeFile << duration << std::endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

#pragma omp parallel
#pragma omp single
    mergeSort(arr, temp, 0, n - 1, 0);

    if (rank == 0) {
        std::ofstream outFile("output_hybrid.txt");
        for (int i = 0; i < n; i++) {
            outFile << arr[i] << " ";
        }
        outFile << std::endl;

        if (std::is_sorted(arr, arr + n)) {
            std::cout << "Sorted array is correct.\n";
        } else {
            std::cout << "Sorted array is not correct.\n";
        }

        outFile.close();
    }

    MPI_Finalize();

    delete[] arr;
    delete[] temp;
    return 0;
}
