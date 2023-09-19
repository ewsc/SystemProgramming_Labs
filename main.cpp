#include <Windows.h>

// Step 1: Declare the Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Step 2: Declare the entry point of the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Step 3: Register the window class
    const char* CLASS_NAME = "MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Step 4: Create the window
    HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles
            CLASS_NAME,                     // Window class name
            "Movable Rectangle",            // Window title
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,

            NULL,       // Parent window
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Step 5: Show the window
    ShowWindow(hwnd, nCmdShow);

    // Step 6: Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Step 7: Implement the Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static RECT rect = { 100, 100, 400, 400 }; // Initial rectangle position

    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Clear the window with a white background
            HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
            DeleteObject(hBackgroundBrush);

            // Draw the rectangle
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_KEYDOWN:
        {
            int xPos = rect.left;
            int yPos = rect.top;

            switch (wParam)
            {
                case VK_LEFT:
                    xPos -= 10; // Move left by 10 pixels
                    break;

                case VK_RIGHT:
                    xPos += 10; // Move right by 10 pixels
                    break;

                case VK_UP:
                    yPos -= 10; // Move up by 10 pixels
                    break;

                case VK_DOWN:
                    yPos += 10; // Move down by 10 pixels
                    break;
            }

            // Update the rectangle position
            SetRect(&rect, xPos, yPos, xPos + (rect.right - rect.left), yPos + (rect.bottom - rect.top));

            // Trigger a repaint to update the window
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}