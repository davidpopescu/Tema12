#include <iostream>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <chrono>
#include <algorithm>

const int MAX_THREADS = 8;

struct MergeSortArgs {
    long long* arr;
    int l;
    int r;
};

void merge(long long arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<long long> L(arr + l, arr + l + n1);
    std::vector<long long> R(arr + m + 1, arr + m + 1 + n2);

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(long long arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void* mergeSortHelper(void* args) {
    MergeSortArgs* ms_args = static_cast<MergeSortArgs*>(args);
    int l = ms_args->l;
    int r = ms_args->r;
    long long* arr = ms_args->arr;

    delete ms_args;

    mergeSort(arr, l, r);

    return nullptr;
}

int main() {
    int n = 1000000;
    int numThreads[MAX_THREADS] = {1, 2, 4, 8};

    long long* arr = new long long[n];

    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        inputFile >> arr[i];
    }

    inputFile.close();

    std::ofstream timeFile("time_pthreads.txt");
    if (!timeFile) {
        std::cerr << "Error opening time file." << std::endl;
        return 1;
    }

    for (int t = 0; t < 4; ++t) {
        auto start = std::chrono::high_resolution_clock::now();

        int chunk_size = n / numThreads[t];
        pthread_t threads[MAX_THREADS];
        MergeSortArgs args[MAX_THREADS];

        for (int i = 0; i < numThreads[t]; ++i) {
            args[i].arr = arr;
            args[i].l = i * chunk_size;
            args[i].r = (i == numThreads[t] - 1) ? (n - 1) : (args[i].l + chunk_size - 1);

            pthread_create(&threads[i], nullptr, mergeSortHelper, new MergeSortArgs(args[i]));
        }

        for (int i = 0; i < numThreads[t]; ++i) {
            pthread_join(threads[i], nullptr);
        }

        for (int i = 1; i < numThreads[t]; ++i) {
            merge(arr, 0, args[i - 1].r, args[i].r);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

        std::cout << "Sorting time with " << numThreads[t] << " threads: " << duration << " seconds.\n";
        
        timeFile << duration << std::endl;

        if (std::is_sorted(arr, arr + n))
            std::cout << "Sorted array is correct.\n";
        else
            std::cout << "Sorted array is not correct.\n";
    }

    timeFile.close();

    std::ofstream outputFile("output_pthreads.txt");
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        outputFile << arr[i] << " ";
    }

    outputFile << std::endl;
    outputFile.close();

    if (arr != nullptr) {
        delete[] arr;
        arr = nullptr;
    }

    return 0;
}
