#include <Windows.h>
#include <fstream>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    const char *CLASS_NAME = "MyWindowClass";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles
            CLASS_NAME,                     // Window class name
            "Text File Viewer",            // Window title
            WS_OVERLAPPEDWINDOW,            // Window style

            // Position and size
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

            NULL,                           // Parent window
            NULL,                           // Menu
            hInstance,                      // Instance handle
            NULL                            // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Main message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Load and display the text from the text file
            const char *filePath = "Resources/example.txt";
            std::ifstream file(filePath);

            if (file.is_open())
            {
                std::wstring text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                TextOut(hdc, 10, 10, reinterpret_cast<LPCSTR>(text.c_str()), static_cast<int>(text.length()) * 3);
            }
            else
            {
                TextOut(hdc, 10, 10, "Failed to open the file.", 23);
            }

            EndPaint(hwnd, &ps);
        }
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}