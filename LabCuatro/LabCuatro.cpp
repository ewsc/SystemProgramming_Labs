#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const int ArrayCount = 10;
const int NumbersPerArray = 100;

// Function to be executed by the sorting threads
DWORD WINAPI SortArray(LPVOID lpParam) {
    std::vector<int>* array = reinterpret_cast<std::vector<int>*>(lpParam);
    std::sort(array->begin(), array->end());
    return 0;
}

int main() {
    // Read integers from file and distribute them into arrays
    std::ifstream inputFile("Resources/input.txt");
    if (!inputFile) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> arrays(ArrayCount, std::vector<int>(NumbersPerArray));

    for (int i = 0; i < ArrayCount; i++) {
        for (int j = 0; j < NumbersPerArray; j++) {
            inputFile >> arrays[i][j];
        }
    }

    inputFile.close();

    // Display the unsorted arrays
    std::cout << "Unsorted Arrays:" << std::endl;
    for (int i = 0; i < ArrayCount; i++) {
        std::cout << "Array " << i + 1 << ": ";
        for (int j = 0; j < NumbersPerArray; j++) {
            std::cout << arrays[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Create two threads to sort the arrays
    HANDLE threads[ArrayCount];

    int currentArrayIndex = 0;

    for (int i = 0; i < ArrayCount; i++) {
        threads[i] = CreateThread(NULL, 0, SortArray, (LPVOID)(&arrays[currentArrayIndex]), 0, NULL);
        if (threads[i] == NULL) {
            std::cerr << "Failed to create thread " << i << std::endl;
            return 1;
        }
        currentArrayIndex++;
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(ArrayCount, threads, TRUE, INFINITE);

    // Display the sorted arrays
    std::cout << "Sorted Arrays:" << std::endl;
    for (int i = 0; i < ArrayCount; i++) {
        std::cout << "Array " << i + 1 << ": ";
        for (int j = 0; j < NumbersPerArray; j++) {
            std::cout << arrays[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Close thread handles
    for (int i = 0; i < ArrayCount; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}