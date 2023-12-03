#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>

// Function to be executed by the first thread
DWORD WINAPI ThreadOneFunction(LPVOID lpParam) {
    std::string threadName = reinterpret_cast<const char*>(lpParam);
    std::cout << "Thread Name: " << threadName << std::endl;

    // Count from 0 to 131337 with a 1-second delay
    for (int i = 0; i <= 131337; i++) {
        std::cout << "Thread One: " << i << std::endl;
        Sleep(1000);
    }

    return 0;
}

// Function to be executed by the second thread
DWORD WINAPI ThreadTwoFunction(LPVOID lpParam) {
    std::string threadName = reinterpret_cast<const char*>(lpParam);
    std::cout << "Thread Name: " << threadName << std::endl;

    // Add to 1 until reaching 34450 or more
    int count = 0;
    while (count < 34450) {
        count++;
        std::cout << "Thread Two: " << count << std::endl;
        Sleep(1000);
    }

    return 0;
}

// Function to be executed by the third thread
DWORD WINAPI ThreadThreeFunction(LPVOID lpParam) {
    std::string threadName = reinterpret_cast<const char*>(lpParam);
    std::cout << "Thread Name: " << threadName << std::endl;

    // Show current time
    while (true) {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::cout << "Thread Three: " << std::ctime(&currentTime);
        Sleep(1000);
    }

    return 0;
}

int main() {
    // Create three threads
    HANDLE threads[3];
    const char* threadNames[3] = { "Thread 1", "Thread 2", "Thread 3" };

    threads[0] = CreateThread(NULL, 0, ThreadOneFunction, (LPVOID)threadNames[0], 0, NULL);
    threads[1] = CreateThread(NULL, 0, ThreadTwoFunction, (LPVOID)threadNames[1], 0, NULL);
    threads[2] = CreateThread(NULL, 0, ThreadThreeFunction, (LPVOID)threadNames[2], 0, NULL);

    // Wait for all threads to finish
    WaitForMultipleObjects(3, threads, TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < 3; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}