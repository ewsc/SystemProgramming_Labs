#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>

int main()
{
    const char* dllPath = "Dll1.dll";

    // Получить идентификатор процесса целевого процесса
    DWORD processId = 0;
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot, &processEntry))
    {
        while (Process32Next(hSnapshot, &processEntry))
        {
            if (_wcsicmp(processEntry.szExeFile, L"jertva.exe") == 0)
            {
                processId = processEntry.th32ProcessID;
                break;
            }
        }
    }
    CloseHandle(hSnapshot);

    if (processId != 0)
    {
        // Открыть целевой процесс для записи
        HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
        if (hProcess)
        {
            // Выделить память внутри целевого процесса для пути к DLL
            LPVOID dllPathAddress = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (dllPathAddress)
            {
                // Записать путь к DLL в выделенную память
                if (WriteProcessMemory(hProcess, dllPathAddress, dllPath, strlen(dllPath) + 1, NULL))
                {
                    // Получить адрес функции LoadLibraryA внутри kernel32.dll
                    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
                    FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryA");

                    // Создать удаленный поток, вызывающий LoadLibraryA с путем к DLL в качестве аргумента
                    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryAddr), dllPathAddress, 0, NULL);
                    if (hRemoteThread)
                    {
                        // Ожидать завершения удаленного потока
                        WaitForSingleObject(hRemoteThread, INFINITE);

                        std::cout << "DLL injected successfully." << std::endl;

                        // Закрыть дескриптор удаленного потока
                        CloseHandle(hRemoteThread);
                    }
                    else
                    {
                        std::cout << "Can't create remote thread." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Failed to write the path to the DLL into process memory." << std::endl;
                }

                // Освободить выделенную память внутри целевого процесса
                VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
            }
            else
            {
                std::cout << "Failed to allocate memory within the process." << std::endl;
            }

            // Закрыть дескриптор процесса
            CloseHandle(hProcess);
        }
        else
        {
            std::cout << "Failed to open process." << std::endl;
        }
    }
    else
    {
        std::cout << "The target process was not found." << std::endl;
    }

    return 0;
}