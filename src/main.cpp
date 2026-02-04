#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>
#include <CommCtrl.h>
#include <string>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Constants
constexpr int ID_TRAY_ICON = 1;
constexpr int ID_TIMER = 2;
constexpr int WM_TRAYICON = WM_USER + 1;

constexpr int ID_INTERVAL_EDIT = 101;
constexpr int ID_MOVEMENT_EDIT = 102;
constexpr int ID_START_BTN = 103;
constexpr int ID_INTERVAL_LABEL = 104;
constexpr int ID_MOVEMENT_LABEL = 105;
constexpr int ID_MENU_HELP = 201;
constexpr int ID_MENU_ABOUT = 202;

// Global state
struct AppState {
    HWND hwnd = nullptr;
    HWND intervalEdit = nullptr;
    HWND movementEdit = nullptr;
    HWND startBtn = nullptr;
    HWND intervalLabel = nullptr;
    HWND movementLabel = nullptr;
    NOTIFYICONDATAW nid = {};
    HFONT hFont = nullptr;
    bool isRunning = false;
    int intervalMs = 1000;
    int movementPx = 10;
} g_app;

void SetControlFont(HWND hwnd) {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)g_app.hFont, TRUE);
}

void MoveMouse(int dx, int dy) {
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(INPUT));
}

void JiggleMouse(int movement) {
    MoveMouse(movement, 0);
    Sleep(50);
    MoveMouse(0, movement);
    Sleep(50);
    MoveMouse(-movement, 0);
    Sleep(50);
    MoveMouse(0, -movement);
}

void UpdateTrayTip() {
    if (g_app.isRunning) {
        wcscpy_s(g_app.nid.szTip, L"YAMJ - Running");
    } else {
        wcscpy_s(g_app.nid.szTip, L"YAMJ - Stopped");
    }
    Shell_NotifyIconW(NIM_MODIFY, &g_app.nid);
}

void StartJiggling() {
    // Read values from edit controls
    wchar_t buf[32];
    GetWindowTextW(g_app.intervalEdit, buf, 32);
    g_app.intervalMs = _wtoi(buf) * 1000;
    if (g_app.intervalMs < 100) g_app.intervalMs = 100;

    GetWindowTextW(g_app.movementEdit, buf, 32);
    g_app.movementPx = _wtoi(buf);
    if (g_app.movementPx < 1) g_app.movementPx = 1;

    SetTimer(g_app.hwnd, ID_TIMER, g_app.intervalMs, nullptr);
    g_app.isRunning = true;
    SetWindowTextW(g_app.startBtn, L"Stop");
    EnableWindow(g_app.intervalEdit, FALSE);
    EnableWindow(g_app.movementEdit, FALSE);
    UpdateTrayTip();
}

void StopJiggling() {
    KillTimer(g_app.hwnd, ID_TIMER);
    g_app.isRunning = false;
    SetWindowTextW(g_app.startBtn, L"Start");
    EnableWindow(g_app.intervalEdit, TRUE);
    EnableWindow(g_app.movementEdit, TRUE);
    UpdateTrayTip();
}

void AddTrayIcon(HWND hwnd) {
    g_app.nid.cbSize = sizeof(NOTIFYICONDATAW);
    g_app.nid.hWnd = hwnd;
    g_app.nid.uID = ID_TRAY_ICON;
    g_app.nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    g_app.nid.uCallbackMessage = WM_TRAYICON;
    g_app.nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcscpy_s(g_app.nid.szTip, L"YAMJ - Stopped");
    Shell_NotifyIconW(NIM_ADD, &g_app.nid);
}

void RemoveTrayIcon() {
    Shell_NotifyIconW(NIM_DELETE, &g_app.nid);
}

void MinimizeToTray() {
    ShowWindow(g_app.hwnd, SW_HIDE);
}

void RestoreFromTray() {
    ShowWindow(g_app.hwnd, SW_SHOW);
    SetForegroundWindow(g_app.hwnd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        g_app.hwnd = hwnd;

        // Create modern font (Segoe UI)
        g_app.hFont = CreateFontW(
            -14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

        // Create controls with better spacing
        g_app.intervalLabel = CreateWindowW(L"STATIC", L"Interval (seconds):",
            WS_VISIBLE | WS_CHILD | SS_LEFT,
            20, 22, 130, 20, hwnd, (HMENU)ID_INTERVAL_LABEL, nullptr, nullptr);

        g_app.intervalEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"1",
            WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_NUMBER | ES_CENTER,
            160, 18, 55, 26, hwnd, (HMENU)ID_INTERVAL_EDIT, nullptr, nullptr);

        g_app.movementLabel = CreateWindowW(L"STATIC", L"Move (pixels):",
            WS_VISIBLE | WS_CHILD | SS_LEFT,
            20, 58, 130, 20, hwnd, (HMENU)ID_MOVEMENT_LABEL, nullptr, nullptr);

        g_app.movementEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"10",
            WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_NUMBER | ES_CENTER,
            160, 54, 55, 26, hwnd, (HMENU)ID_MOVEMENT_EDIT, nullptr, nullptr);

        g_app.startBtn = CreateWindowW(L"BUTTON", L"Start",
            WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
            65, 100, 110, 32, hwnd, (HMENU)ID_START_BTN, nullptr, nullptr);

        // Apply font to all controls
        SetControlFont(g_app.intervalLabel);
        SetControlFont(g_app.intervalEdit);
        SetControlFont(g_app.movementLabel);
        SetControlFont(g_app.movementEdit);
        SetControlFont(g_app.startBtn);

        AddTrayIcon(hwnd);

        // Create menu bar
        HMENU hMenuBar = CreateMenu();
        HMENU hHelpMenu = CreatePopupMenu();
        AppendMenuW(hHelpMenu, MF_STRING, ID_MENU_HELP, L"&Help\tF1");
        AppendMenuW(hHelpMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(hHelpMenu, MF_STRING, ID_MENU_ABOUT, L"&About");
        AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hHelpMenu, L"&Help");
        SetMenu(hwnd, hMenuBar);

        return 0;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_START_BTN:
            if (g_app.isRunning) {
                StopJiggling();
            } else {
                StartJiggling();
            }
            break;
        case ID_MENU_HELP:
            MessageBoxW(hwnd,
                L"YAMJ - Yet Another Mouse Jiggler\n\n"
                L"This app moves your mouse cursor periodically to prevent\n"
                L"your computer from going idle, locking the screen, or\n"
                L"showing you as 'Away' in chat apps.\n\n"
                L"Settings:\n"
                L"• Interval: How often to jiggle (in seconds)\n"
                L"• Move: How far to move the cursor (in pixels)\n\n"
                L"Usage:\n"
                L"• Click Start to begin jiggling\n"
                L"• Minimize to send to system tray\n"
                L"• Double-click tray icon to restore\n"
                L"• Right-click tray icon to toggle Start/Stop",
                L"Help", MB_OK | MB_ICONINFORMATION);
            break;
        case ID_MENU_ABOUT:
            MessageBoxW(hwnd,
                L"YAMJ - Yet Another Mouse Jiggler\n\n"
                L"Version 1.0\n\n"
                L"A simple utility to keep your computer awake.\n\n"
                L"Copyright (c) 2026 Costantino Pipero\n\n"
                L"licensed under the GNU General Public License v3.0",
                L"About YAMJ", MB_OK | MB_ICONINFORMATION);
            break;
        }
        return 0;

    case WM_TIMER:
        if (wParam == ID_TIMER) {
            JiggleMouse(g_app.movementPx);
        }
        return 0;

    case WM_TRAYICON:
        if (lParam == WM_LBUTTONDBLCLK) {
            RestoreFromTray();
        } else if (lParam == WM_RBUTTONUP) {
            // Right-click on tray - toggle start/stop
            if (g_app.isRunning) {
                StopJiggling();
            } else {
                StartJiggling();
            }
        }
        return 0;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) {
            MinimizeToTray();
        }
        return 0;

    case WM_CLOSE:
        MinimizeToTray();
        return 0;

    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        SetBkMode(hdc, TRANSPARENT);
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
    }

    case WM_DESTROY:
        StopJiggling();
        RemoveTrayIcon();
        if (g_app.hFont) DeleteObject(g_app.hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    // Initialize common controls
    INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icex);

    // Register window class
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"YAMJWindow";
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassExW(&wc);

    // Create window (centered on screen)
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int winW = 260, winH = 210;
    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW, L"YAMJWindow", L"YAMJ",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        (screenW - winW) / 2, (screenH - winH) / 2, winW, winH,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
