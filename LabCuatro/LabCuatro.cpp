#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

const int ArrayCount = 10;
const int NumbersPerArray = 1000;

struct Task {
    int id;
    std::string description;
};

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
    time_t start, end;
    time(&start);

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

    std::queue<Task> taskQueue;
    taskQueue.push({ 1, "Array 1" });
    taskQueue.push({ 2, "Array 2" });
    taskQueue.push({ 3, "Array 3" });
    taskQueue.push({ 4, "Array 4" });
    taskQueue.push({ 5, "Array 5" });
    taskQueue.push({ 6, "Array 6" });
    taskQueue.push({ 7, "Array 7" });
    taskQueue.push({ 8, "Array 8" });
    taskQueue.push({ 9, "Array 9" });
    taskQueue.push({ 10, "Array 10" });

    HANDLE threads[ArrayCount];

    int currentArrayIndex = 0;

    while (!taskQueue.empty()) {
        Task task = taskQueue.front();
        taskQueue.pop();
        threads[task.id] = CreateThread(nullptr, 0, SortArray, (LPVOID)(&arrays[currentArrayIndex]), 0, nullptr);
        if (threads[task.id] == nullptr) {
            std::cerr << "Failed to create thread " << task.id << std::endl;
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

    time(&end);
    auto time_taken = double(end - start);
    std::cout << "Time taken by program is : " << std::fixed
         << time_taken;
    std::cout << " sec " << std::endl;
    std::getchar();
    return 0;
}