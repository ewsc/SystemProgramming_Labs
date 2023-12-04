#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const int ArrayCount = 10;
const int NumbersPerArray = 1000;

std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right) {
    std::vector<int> merged;
    merged.reserve(left.size() + right.size());

    size_t i = 0;
    size_t j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            merged.push_back(left[i++]);
        } else {
            merged.push_back(right[j++]);
        }
    }

    while (i < left.size()) {
        merged.push_back(left[i++]);
    }

    while (j < right.size()) {
        merged.push_back(right[j++]);
    }

    return merged;
}

std::vector<int> mergeSort(const std::vector<int>& arr) {
    if (arr.size() <= 1) {
        return arr;
    }

    size_t mid = arr.size() / 2;

    std::vector<int> left(arr.begin(), arr.begin() + mid);
    std::vector<int> right(arr.begin() + mid, arr.end());

    left = mergeSort(left);
    right = mergeSort(right);

    return merge(left, right);
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

        std::sort(arrays[i].begin(), arrays[i].end());
    }

    inputFile.close();

    std::vector<int> mergedArray = arrays[0];

    for (int i = 1; i < ArrayCount; i++) {
        mergedArray = merge(mergedArray, arrays[i]);
    }

    mergedArray = mergeSort(mergedArray);

    std::cout << "Merged and Sorted Array:" << std::endl;
    for (const auto& num : mergedArray) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    time(&end);
    auto time_taken = double(end - start);
    std::cout << "Time taken by program is : " << std::fixed
              << time_taken;
    std::cout << " sec " << std::endl;
    std::getchar();
    return 0;
}