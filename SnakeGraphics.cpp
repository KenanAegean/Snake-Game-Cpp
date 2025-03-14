#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include "SnakeGraphics.h"

HBRUSH SnakeGraphics::GetBrushWithColor(const Color InColor)
{
    // Compute brush ID
    int BrushId = InColor.r * 65536 + InColor.g * 256 + InColor.b;

    auto it = Brushes.find(BrushId);

    if (it != Brushes.end()) 
    {
        return it->second;
    }
    
    HBRUSH Brush = CreateSolidBrush(RGB(InColor.r, InColor.g, InColor.b));
        
    Brushes[BrushId] = Brush;
        
    return Brush;
}

LRESULT CALLBACK SnakeGraphics::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBackBuffer = nullptr;
    static HDC hBackBufferDC = nullptr;
    static HFONT hFont = nullptr;
    static int width = 0, height = 0;


    switch (msg)
    {
    case WM_CLOSE:
        // Close the window when the user clicks the close button
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        WindowClosed = true;

        if (hBackBuffer) 
        {
            DeleteObject(hBackBuffer);
            DeleteDC(hBackBufferDC);
        }

        if (hFont)
        {
            DeleteObject(hFont);
        }

        PostQuitMessage(0);

        return 0;

    case WM_SIZE:
        {
        // Resize the backbuffer when the window is resized
        if (hBackBuffer) {
            DeleteObject(hBackBuffer);
            DeleteDC(hBackBufferDC);
        }
        width = LOWORD(lParam);
        height = HIWORD(lParam);

        HDC hdc = GetDC(hwnd);
        hBackBufferDC = CreateCompatibleDC(hdc);
        hBackBuffer = CreateCompatibleBitmap(hdc, Width, Height);
        SelectObject(hBackBufferDC, hBackBuffer);

        // Create a font with desired size
        hFont = CreateFont(
            -(TileHeight - 4),     // Height of font in logical units (negative for character height)
            0,      // Width of font (0 = proportional width)
            0,                     // Escapement angle
            0,                     // Orientation angle
            FW_BOLD,             // Font weight
            FALSE,                 // Italic
            FALSE,                 // Underline
            FALSE,                 // Strikeout
            ANSI_CHARSET,          // Character set
            OUT_DEFAULT_PRECIS,    // Output precision
            CLIP_DEFAULT_PRECIS,   // Clipping precision
            DEFAULT_QUALITY,       // Output quality
            DEFAULT_PITCH | FF_SWISS, // Pitch and family
            L"Arial");             // Font name
                
        return 0;
        }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Clear the backbuffer
        RECT clearRect = { 0, 0, width, height };
        FillRect(hBackBufferDC, &clearRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        // Render everything on the backbuffer
        for (int y = 0; y < NumRows; y++) 
        {
            for (int x = 0; x < NumColumns; x++) 
            {
                TileData Data = TileGrid[x + y * NumColumns];

                RECT rect = { x * TileWidth, y * TileHeight, (x + 1) * TileWidth, (y + 1) * TileHeight };

                HBRUSH Brush = GetBrushWithColor(Data.BackgroundColor);

                FillRect(hBackBufferDC, &rect, Brush);

                // Draw text
                if (Data.Character != ' ' && Data.Character != 0) 
                {
                    HFONT hOldFont = (HFONT)SelectObject(hBackBufferDC, hFont);

                    SetTextColor(hBackBufferDC, RGB(Data.ForegroundColor.r, Data.ForegroundColor.g, Data.ForegroundColor.b));

                    SetBkColor(hBackBufferDC, RGB(Data.BackgroundColor.r, Data.BackgroundColor.g, Data.BackgroundColor.b));

                    wchar_t Text[2] = { Data.Character, 0 }; // Convert to null-terminated string

					SIZE Dimensions = { 0, 0 };

                    if (GetTextExtentPoint32(hBackBufferDC, Text, (int)wcslen(Text), &Dimensions)) 
                    {    
                        TextOut(hBackBufferDC, x * TileWidth + (TileWidth - Dimensions.cx) / 2, y * TileHeight + (TileHeight - Dimensions.cy) / 2, Text, 1);
                    }


                    SelectObject(hBackBufferDC, hOldFont);
                }
            }
        }

		// Draw grid
        {
			HBRUSH Brush = GetBrushWithColor({ 0, 0, 0 });

            for (int x = 0; x < NumColumns; x += 2)
            {
                RECT rect = { x * TileWidth, -1, (x + 1) * TileWidth, Height + 1 };

				FrameRect(hBackBufferDC, &rect, Brush);
            }

            for (int y = 0; y < NumRows; y++)
            {
                RECT rect = { -1, y * TileHeight, Width + 1, y * (Height + 1) };

                FrameRect(hBackBufferDC, &rect, Brush);
            }
        }

        // Copy the backbuffer to the screen
        BitBlt(hdc, 0, 0, width, height, hBackBufferDC, 0, 0, SRCCOPY);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_ERASEBKGND:
        return 1; // Prevent default background erasure

    default:
        for (auto it = WndProcs.begin(); it != WndProcs.end(); ++it)
        {
            LRESULT Result = (*it)(hwnd, msg, wParam, lParam);
            if (Result != 0)
            {
                return Result;
            }
        }
        // Default handling for unprocessed messages
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK SnakeGraphics::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    SnakeGraphics* Graphics = reinterpret_cast<SnakeGraphics*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (!Graphics)
	{
        return DefWindowProc(hwnd, msg, wParam, lParam);
	}

    return Graphics->WndProc(hwnd, msg, wParam, lParam);
}


SnakeGraphics::SnakeGraphics(const int InWidth, const int InHeight, const int InNumColumns, const int InNumRows)
{
    TileGrid.resize(InNumColumns * InNumRows);

    NewTileGrid.resize(InNumColumns * InNumRows);

    NumColumns = InNumColumns;

    NumRows = InNumRows;

    TileWidth = InWidth / InNumColumns;

    TileHeight = InHeight / InNumRows;

    Width = TileWidth * InNumColumns;

    Height = TileHeight * InNumRows;
}

SnakeGraphics::~SnakeGraphics()
{
	if (Hwnd)
	{
		DestroyWindow(Hwnd);
	}

    ClearBrushes();
}

bool SnakeGraphics::Init()
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    if (!hInstance) 
    {
        std::cerr << "Failed to get HINSTANCE!" << std::endl;
    }

    // Define and register the window class
    WNDCLASS wc = {};

    wc.lpfnWndProc = SnakeGraphics::StaticWndProc;           // Set the WndProc function
    wc.hInstance = hInstance;          // Handle to the application instance
    wc.lpszClassName = L"SnakeGraphics";     // Class name
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // Default arrow cursor
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // White background

    if (!RegisterClass(&wc)) {
        MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_OK | MB_ICONERROR);

        return false;
    }

    // Set the desired window style
    DWORD style = WS_CAPTION | WS_BORDER | WS_SYSMENU; // Top border and title bar
    DWORD exStyle = WS_EX_APPWINDOW;                  // Application window

    // Adjust the window size to account for borders
    RECT rect = { 0, 0, Width, Height };
    AdjustWindowRectEx(&rect, style, FALSE, exStyle);

    // Create the window
    Hwnd = CreateWindowEx(
        exStyle,                        // Optional window styles
        L"SnakeGraphics",               // Window class name
        L"FGSnake",                     // Window title
        style,                          // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Window position
        rect.right - rect.left,         // Adjusted width
        rect.bottom - rect.top,         // Adjusted height
        nullptr,                        // Parent window
        nullptr,                        // Menu
        hInstance,                      // Instance handle
        nullptr                         // Additional application data
    );

    if (!Hwnd)
    {
        MessageBox(nullptr, L"Failed to create window!", L"Error", MB_OK | MB_ICONERROR);

        return false;
    }

	// Store the SnakeGraphics instance in the window user data
	SetWindowLongPtrA(Hwnd, GWLP_USERDATA, (LONG_PTR)this);

    // Show and update the window
    ShowWindow(Hwnd, 1);

    UpdateWindow(Hwnd);

    return true;
}

bool SnakeGraphics::UpdateWindowMessages()
{
    MSG msg;
	if (PeekMessage(&msg, Hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }

    return !WindowClosed;
}

bool SnakeGraphics::IsTileWithinBounds(const int InX, const int InY) const
{
	return InX >= 0 && InY >= 0 && InX < NumColumns && InY < NumRows;
}

int SnakeGraphics::GetTileIndex(int InX, int InY) const
{
	return InX + InY * NumColumns;
}

void SnakeGraphics::PlotText(const int InX, const int InY, const int InOrderId, const Color InBackgroundColor, const wchar_t* InText, const Color InForegroundColor, const Alignment InAlignment)
{
	int Length = (int)wcslen(InText);

	int StartX = InX;

	switch (InAlignment)
	{
	case Alignment::Center:
		StartX -= Length / 2;
		break;

	case Alignment::Right:
		StartX -= Length;
		break;
	}

    if (IsTileWithinBounds(StartX, InY))
    {
        for (int i = 0; i < Length; i++)
		{
			PlotTile(StartX + i, InY, InOrderId, InBackgroundColor, InForegroundColor, InText[i]);
		}
	}

}

void SnakeGraphics::PlotTile(const int InX, const int InY, const int InOrderId, const Color InBackgroundColor, const Color InForegroundColor, const wchar_t InCharacter)
{
    if (IsTileWithinBounds(InX, InY))
    {
        int Index = GetTileIndex(InX, InY);

		if (NewTileGrid[Index].FrameId == FrameId && NewTileGrid[Index].OrderId >= InOrderId)
		{
			return;
		}

		NewTileGrid[Index] = TileData(InBackgroundColor, InForegroundColor, InCharacter, InOrderId, FrameId);
    }
}

void SnakeGraphics::Render()
{
	for (int y = 0; y < NumRows; y++)
	{
		for (int x = 0; x < NumColumns; x++)
		{
			int Index = GetTileIndex(x, y);

			if (NewTileGrid[Index].FrameId != FrameId || NewTileGrid[Index] == TileGrid[Index])
			{
				continue;
			}

			TileGrid[Index] = NewTileGrid[Index];

			RECT Rect = { x * TileWidth, y * TileHeight, (x + 1) * TileWidth - 1, (y + 1) * TileHeight - 1 };
		}
	}

    InvalidateRect(Hwnd, nullptr, TRUE);

    ClearBrushes();

    FrameId++;
}

void SnakeGraphics::ClearBrushes()
{
    for (std::pair<unsigned int, HBRUSH> Pair : Brushes)
    {
        DeleteObject(Pair.second);
    }

    Brushes.clear();
}

int SnakeGraphics::GetNumColumns() const
{
    return NumColumns;
}

int SnakeGraphics::GetNumRows() const
{
    return NumRows;
}


void SnakeGraphics::AddWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WndProc)
{
    for (auto it = WndProcs.begin(); it != WndProcs.end(); ++it)
    {
        if (it->target_type() == WndProc.target_type() &&
            it->target<LRESULT(HWND, UINT, WPARAM, LPARAM)>() == WndProc.target<LRESULT(HWND, UINT, WPARAM, LPARAM)>())
        {
            return;
        }
    }

    WndProcs.push_back(WndProc);
}

void SnakeGraphics::RemoveWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WndProc)
{
	for (auto it = WndProcs.begin(); it != WndProcs.end(); ++it)
	{
        if (it->target_type() == WndProc.target_type() &&
            it->target<LRESULT(HWND, UINT, WPARAM, LPARAM)>() == WndProc.target<LRESULT(HWND, UINT, WPARAM, LPARAM)>())
        {
            WndProcs.erase(it);
			break;
		}
	}
}