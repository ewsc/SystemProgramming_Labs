#include <Windows.h>
#include <iostream>

// Function pointer type for the DLL function
typedef void (*SearchAndReplaceStringFunc)(DWORD, const char*, const char*);

int main()
{
    // Load the DLL
    HMODULE hDll = LoadLibrary(R"(X:\3Third_Year\SysP\CustomDLL.dll)");

    DWORD errorCode = GetLastError();
    LPSTR errorMessage = nullptr;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorMessage, 0, NULL);
    if (errorMessage != nullptr)
    {
        // Print or display the error message
        printf("Function failed with error: %s", errorMessage);

        // Free the error message buffer
        LocalFree(errorMessage);
    }

    if (hDll == NULL)
    {
        std::cout << "Failed to load the DLL." << std::endl;
        return 1;
    }


    // Get the function address
    SearchAndReplaceStringFunc searchAndReplaceFunc = reinterpret_cast<SearchAndReplaceStringFunc>(
            GetProcAddress(hDll, "SearchAndReplaceString"));
    if (searchAndReplaceFunc == NULL)
    {
        std::cout << "Failed to get the function address." << std::endl;
        FreeLibrary(hDll);
        return 1;
    }

    // Call the DLL function
    DWORD processId = GetCurrentProcessId();
    const char* searchString = "Hello";
    const char* replaceString = "Hi";
    searchAndReplaceFunc(processId, searchString, replaceString);

    // Unload the DLL
    FreeLibrary(hDll);

    return 0;
}