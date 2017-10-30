#pragma once
#include "stdafx.h"
#include "Window.h"

#if defined VK_USE_PLATFORM_WIN32_KHR

namespace ThroneEngine
{
    // Microsoft Windows specific versions of window functions
    LRESULT CALLBACK WindowsEventHandler(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
    {
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

        switch (uMsg)
        {
            case WM_CLOSE:
                window->close();
                return 0;
            default:
                break;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    uint64_t Window::win32ClassIdCounter = 0;


    bool Window::initOSWindow()
    {
        WNDCLASSEX winClass;
        assert(surfaceSizeX > 0);
        assert(surfaceSizeY > 0);

        win32Instance = GetModuleHandle(nullptr);
        win32ClassName = windowName + "_" + std::to_string(win32ClassIdCounter);
        win32ClassIdCounter++;

        std::wstring win32ClassNameWideStr = std::wstring(win32ClassName.begin(), win32ClassName.end());
        LPCWSTR win32ClassNameWideStr_2 = win32ClassNameWideStr.c_str();

        // Initialize the window class structure:
        winClass.cbSize = sizeof(WNDCLASSEX);
        winClass.style = CS_HREDRAW | CS_VREDRAW;
        winClass.lpfnWndProc = WindowsEventHandler;
        winClass.cbClsExtra = 0;
        winClass.cbWndExtra = 0;
        winClass.hInstance = win32Instance; // hInstance
        winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        winClass.lpszMenuName = NULL;
        winClass.lpszClassName = win32ClassNameWideStr_2;
        winClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

        // Register window class:
        if (!RegisterClassEx(&winClass))
        {
            return false;
        }

        DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;


        std::wstring windowNameWideStr = std::wstring(windowName.begin(), windowName.end());
        LPCWSTR windowNameWideStr_2 = windowNameWideStr.c_str();

        // Create window with the registered class:
        RECT wr = { 0, 0, LONG(surfaceSizeX), LONG(surfaceSizeY) };
        AdjustWindowRectEx(&wr, style, FALSE, ex_style);
        win32Window = CreateWindowEx(0,
            win32ClassNameWideStr_2,		// class name
            windowNameWideStr_2,			// app name
            style,							// window style
            CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
            wr.right - wr.left,				// width
            wr.bottom - wr.top,				// height
            NULL,							// handle to parent
            NULL,							// handle to menu
            win32Instance,				    // hInstance
            NULL);							// no extra parameters
        if (!win32Window)
        {
            return false;
        }
        SetWindowLongPtr(win32Window, GWLP_USERDATA, (LONG_PTR)this);

        ShowWindow(win32Window, SW_SHOW);
        SetForegroundWindow(win32Window);
        SetFocus(win32Window);
    }

    void Window::deInitOSWindow()
    {
        std::wstring win32ClassNameWideStr = std::wstring(win32ClassName.begin(), win32ClassName.end());
        LPCWSTR win32ClassNameWideStr_2 = win32ClassNameWideStr.c_str();

        DestroyWindow(win32Window);
        UnregisterClass(win32ClassNameWideStr_2, win32Instance);
    }

    void Window::updateOSWindow()
    {
        MSG msg;
        if (PeekMessage(&msg, win32Window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Window::initOSSurface(VkInstance instance)
    {
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.hinstance = win32Instance;
        createInfo.hwnd = win32Window;
        vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
    }
}
#endif //VK_USE_PLATFORM_WIN32_KHR