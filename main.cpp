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
            WS_POPUP,                       // Window style

            // Size and position
            0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),

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

        case WM_MOUSEWHEEL:
        {
            int xPos = rect.left;
            int yPos = rect.top;
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);

            // Check if Shift key is being held down
            if (GetKeyState(VK_SHIFT) & 0x8000)
            {
                // Shift+Scroll: Change position in the y-axis
                yPos -= delta;
            }
            else
            {
                // Scroll: Change size in the x-axis
                int width = rect.right - rect.left;
                width += delta;
                if (width < 0)
                    width = 0;
                xPos = rect.left + (rect.right - rect.left - width) / 2;
            }

            // Update the rectangle position and size
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