#include <Windows.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  
    const char* CLASS_NAME = "MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

  
    HWND hwnd = CreateWindowEx(
            0,                            
            CLASS_NAME,                   
            "Transparent Image",          
            WS_POPUP,                     

          
            0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),

            nullptr,
            nullptr,
            hInstance,
            nullptr
    );

    if (hwnd ==
    nullptr)
    {
        return 0;
    }

  
    ShowWindow(hwnd, nCmdShow);

  
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmap = nullptr;
    static RECT rect = { 100, 100, 400, 400 }; 

    switch (uMsg)
    {
        case WM_CREATE:
        {
          
            hBitmap = (HBITMAP)LoadImage(nullptr, "todd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
            if (hBitmap == nullptr)
            {
                MessageBox(hwnd, "Failed to load the image.", "Error", MB_OK | MB_ICONERROR);
                return -1;
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

          
            HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
            DeleteObject(hBackgroundBrush);

          
            HDC hdcMem = CreateCompatibleDC(hdc);
            auto hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

            BITMAP bm;
            GetObject(hBitmap, sizeof(bm), &bm);
            int x = rect.left;
            int y = rect.top;

            COLORREF transparentColor = RGB(255, 0, 255); 

            TransparentBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, transparentColor);

            SelectObject(hdcMem, hBitmapOld);
            DeleteDC(hdcMem);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            int xPos = rect.left;
            int yPos = rect.top;
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (GetKeyState(VK_SHIFT) & 0x8000)
            {
                yPos -= delta;
            }
            else
            {
                int width = rect.right - rect.left;
                width += delta;
                if (width < 0)
                    width = 0;
                xPos = rect.left + (rect.right - rect.left - width) / 2;
            }
            SetRect(&rect, xPos, yPos, xPos + (rect.right - rect.left), yPos + (rect.bottom - rect.top));

          
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_DESTROY:
            if (hBitmap != nullptr)
            {
                DeleteObject(hBitmap);
            }
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}