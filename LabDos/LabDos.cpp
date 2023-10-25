#include <Windows.h>
#include <fstream>
#include <string>
#include <cmath>

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

            RECT rect;
            GetClientRect(hwnd, &rect); // Get the client area rectangle

            // Load and display the text from the text file
            char filePath[] = "Resources/example.txt";
            std::ifstream file(filePath);

            if (file.is_open())
            {
                std::wstring text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                int textLength = static_cast<int>(text.length());
                int numRows = static_cast<int>(sqrt(textLength)); // Number of rows (adjust as needed)
                int numCols = (textLength + numRows - 1) / numRows; // Number of columns

                int cellWidth = rect.right / numCols; // Width of each cell
                int cellHeight = rect.bottom / numRows; // Height of each cell

                for (int row = 0; row < numRows; ++row)
                {
                    for (int col = 0; col < numCols; ++col)
                    {
                        int index = row * numCols + col;
                        if (index < textLength)
                        {
                            std::wstring cellText = text.substr(index, 1);
                            RECT cellRect = {
                                    col * cellWidth, row * cellHeight,
                                    (col + 1) * cellWidth, (row + 1) * cellHeight
                            };

                            // Draw the cell text within the cell rectangle
                            DrawText(hdc, reinterpret_cast<LPCSTR>(cellText.c_str()), -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                        }
                    }
                }
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