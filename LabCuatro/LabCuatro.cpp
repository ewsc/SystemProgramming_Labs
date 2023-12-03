#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const int ArrayCount = 10;
const int NumbersPerArray = 100;

DWORD WINAPI SortArray(LPVOID lpParam) {
    auto* array = reinterpret_cast<std::vector<int>*>(lpParam);
    std::sort(array->begin(), array->end());
    return 0;
}

std::vector<int> MergeSortArrays(const std::vector<std::vector<int>>& arrays) {
    std::vector<int> mergedArray;

    for (const auto& array : arrays) {
        mergedArray.insert(mergedArray.end(), array.begin(), array.end());
    }

    std::sort(mergedArray.begin(), mergedArray.end());

    return mergedArray;
}

int main() {
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

    std::cout << "Unsorted Arrays:" << std::endl;
    for (int i = 0; i < ArrayCount; i++) {
        std::cout << "Array " << i + 1 << ": ";
        for (int j = 0; j < NumbersPerArray; j++) {
            std::cout << arrays[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    HANDLE threads[ArrayCount];

    int currentArrayIndex = 0;

    for (int i = 0; i < ArrayCount; i++) {
        threads[i] = CreateThread(nullptr, 0, SortArray, (LPVOID)(&arrays[currentArrayIndex]), 0, nullptr);
        if (threads[i] == nullptr) {
            std::cerr << "Failed to create thread " << i << std::endl;
            return 1;
        }
        currentArrayIndex++;
    }

    WaitForMultipleObjects(ArrayCount, threads, TRUE, INFINITE);

    std::cout << "Sorted Arrays:" << std::endl;
    for (int i = 0; i < ArrayCount; i++) {
        std::cout << "Array " << i + 1 << ": ";
        for (int j = 0; j < NumbersPerArray; j++) {
            std::cout << arrays[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::vector<int> mergedArray = MergeSortArrays(arrays);

    std::cout << "Merged and Sorted Array:" << std::endl;
    for (const auto& num : mergedArray) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    for (auto & thread : threads) {
        CloseHandle(thread);
    }

    return 0;
}