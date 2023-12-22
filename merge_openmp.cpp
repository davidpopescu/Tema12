#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <omp.h>

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

void mergeSort(long long arr[], int l, int r, int numThreads) {
    if (l < r) {
        int m = l + (r - l) / 2;
        omp_set_num_threads(8);
            #pragma omp task firstprivate (arr, l, m)
            mergeSort(arr, l, m, numThreads);

            #pragma omp task firstprivate (arr, m, r)
            mergeSort(arr, m + 1, r, numThreads);
        
        #pragma omp taskwait
        merge(arr, l, m, r);
    }
}

void printArray(long long A[], int size) {
    for (int i = 0; i < size; i++)
        std::cout << A[i] << " ";
    std::cout << std::endl;
}

bool arraySortedOrNot(long long arr[], long long n) {
    if (n == 1 || n == 0)
        return true;

    for (long long i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) {
            return false; 
        }
    }

    return true; 
}

void readArrayFromFile(long long arr[], int n, const std::string& filename) {
    std::ifstream inFile(filename);
    for (int i = 0; i < n; i++) {
        inFile >> arr[i];
    }
    inFile.close();
}

int main() {
    int n = 1000000;
    long long* arr = new long long[n];

    readArrayFromFile(arr, n, "input.txt");

    std::ofstream outFile("output_openmp.txt");

    std::ofstream timeFile("time_openmp.txt");
    for (int numThreads : {1, 2, 4, 8}) {
        auto start = std::chrono::high_resolution_clock::now();

        mergeSort(arr, 0, n - 1, numThreads);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

        timeFile << duration << std::endl;

        std::cout << "Sorting time with " << numThreads << " threads: " << duration << " seconds.\n";

        for (int i = 0; i < n; i++) {
            outFile << arr[i] << " ";
        }
        outFile << std::endl;

        if (arraySortedOrNot(arr, n))
            std::cout << "Sorted array is correct.\n";
        else
            std::cout << "Sorted array is not correct.\n";
    }

    outFile.close();
    timeFile.close();

    delete[] arr;

    return 0;
}
