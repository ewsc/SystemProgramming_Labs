#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    while (true)
    {
        // Perform your desired operations here
        std::cout << "saying something" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for 1 second
    }

    return 0;
}