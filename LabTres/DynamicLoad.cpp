#include <Windows.h>
#include <iostream>

// Function pointer type
typedef void (*SearchAndReplaceString)();

int main()
{
    // Load the DLL
    HMODULE hModule = LoadLibrary(L"Dll1.dll");
    if (hModule)
    {
        std::cout << "DLL loaded." << std::endl;
        // Get the function address
        SearchAndReplaceString searchAndReplaceString = reinterpret_cast<SearchAndReplaceString>(GetProcAddress(hModule, "SearchAndReplaceString"));
        
        if (searchAndReplaceString)
        {
            // Call the function
            searchAndReplaceString();

            std::cout << "String replacement completed." << std::endl;
        }

        // Unload the DLL
        FreeLibrary(hModule);
    }
    else
    {
        std::cout << "Failed to load the DLL." << std::endl;
    }

    return 0;
}