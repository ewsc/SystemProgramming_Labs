#include <iostream>
#include <fstream>
#include <vector>

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = 0;

    std::vector<int> temp(right - left + 1);

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (int p = 0; p < k; p++) {
        arr[left + p] = temp[p];
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int main() {
    time_t start, end;
    time(&start);

    std::ifstream inputFile("Resources/input.txt");
    if (!inputFile) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    std::vector<int> numbers;
    int number;

    while (inputFile >> number) {
        numbers.push_back(number);
    }

    inputFile.close();

    mergeSort(numbers, 0, numbers.size() - 1);

    // Display the sorted numbers
    std::cout << "Sorted Numbers:" << std::endl;
    for (const auto& num : numbers) {
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