// tie-tac-toe-win32.cpp : Defines the entry point for the application.
//


#include "framework.h"
#include "tie-tac-toe-win32.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int grid[3][3] = { 0 }; // 0 for empty, 1 for X, 2 for O
bool isXturn = true;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TIETACTOEWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIETACTOEWIN32));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIETACTOEWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TIETACTOEWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      CW_USEDEFAULT, 0, 320, 361, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

POINT winningLineStart = { -1, -1 };
POINT winningLineEnd = { -1, -1 };

void ResetBoard() {
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            grid[row][col] = 0;

    isXturn = 1;
    winningLineStart = { -1, -1 };
    winningLineEnd = { -1, -1 };
}

int CheckWin() {
    for (int i = 0; i < 3; ++i) {
        if (grid[i][0] != 0 &&
            grid[i][0] == grid[i][1] &&
            grid[i][1] == grid[i][2]) {
            winningLineStart = { 0, i };
            winningLineEnd = { 2, i };
            return grid[i][0];
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (grid[0][i] != 0 &&
            grid[0][i] == grid[1][i] &&
            grid[1][i] == grid[2][i]) {
            winningLineStart = { i, 0 };
            winningLineEnd = { i, 2 };
            return grid[0][i];
        }
    }

    if (grid[0][0] != 0 &&
    grid[0][0] == grid[1][1] &&
        grid[1][1] == grid[2][2]) {
            winningLineStart = { 0, 0 };
            winningLineEnd = { 2, 2 };
            return grid[0][0];
    }

    if (grid[0][2] != 0 &&
        grid[0][2] == grid[1][1] &&
        grid[1][1] == grid[2][0]) {
        winningLineStart = { 2, 0 };
        winningLineEnd = { 0, 2 };
        return grid[0][2];
    }

    bool isTie = true;
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            if (grid[row][col] == 0)
                isTie = false;

    if (isTie) return 3;

    return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        {
		    int xPos = LOWORD(lParam);
		    int yPos = HIWORD(lParam);
		    int cellSize = 100;
		    int row = yPos / cellSize;
		    int col = xPos / cellSize;
		    if (row >= 0 && row < 3 && col >= 0 && col < 3 && grid[row][col] == 0) {
			    grid[row][col] = isXturn ? 1 : 2;
			    isXturn = !isXturn;
			    InvalidateRect(hWnd, NULL, TRUE);
		    }

            int result = CheckWin();
            if (result == 1) {
                MessageBox(hWnd, L"Player X wins!", L"Game Over", MB_OK);
                ResetBoard();
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (result == 2) {
                MessageBox(hWnd, L"Player O wins!", L"Game Over", MB_OK);
                ResetBoard();
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (result == 3) {
                MessageBox(hWnd, L"It's a tie!", L"Game Over", MB_OK);
                ResetBoard();
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            int cellSize = 100;
            int gridSize = 3 * cellSize;
            for (int i = 0; i <= 2; ++i) {
				MoveToEx(hdc, i * cellSize, 0, nullptr);
				LineTo(hdc, i * cellSize, 3 * cellSize);
				MoveToEx(hdc, 0, i * cellSize, nullptr);
				LineTo(hdc, 3 * cellSize, i * cellSize);
            }
            MoveToEx(hdc, gridSize, 0, NULL);
            LineTo(hdc, gridSize, gridSize);
            MoveToEx(hdc, 0, gridSize, NULL);
            LineTo(hdc, gridSize, gridSize);
            HFONT hFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            SelectObject(hdc, hFont);
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    wchar_t symbol[2] = L" ";
                    if (grid[row][col] == 1) {
                        symbol[0] = L'X';
                    }
                    else if (grid[row][col] == 2) {
                        symbol[0] = L'O';
                    }

                    TextOutW(hdc, col * 100 + 35, row * 100 + 20, symbol, 1);
                }
            }
            if (winningLineStart.x != -1 && winningLineEnd.x != -1) {
                HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); // Red line
                HPEN oldPen = (HPEN)SelectObject(hdc, pen);

                int cellSize = 100;
                int x1 = winningLineStart.x * cellSize + cellSize / 2;
                int y1 = winningLineStart.y * cellSize + cellSize / 2;
                int x2 = winningLineEnd.x * cellSize + cellSize / 2;
                int y2 = winningLineEnd.y * cellSize + cellSize / 2;

                MoveToEx(hdc, x1, y1, NULL);
                LineTo(hdc, x2, y2);

                SelectObject(hdc, oldPen);
                DeleteObject(pen);
            }
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
