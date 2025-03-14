#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <functional>

struct Color {
	int r, g, b;

	Color(int red = 0, int green = 0, int blue = 0) : r(red), g(green), b(blue) {}
};

class SnakeGraphics
{
public:
	enum Alignment
	{
		Left, 
		Center, 
		Right
	};
private:
	struct TileData
	{
		TileData(Color InBackgroundColor = {0, 0, 0}, Color InForegroundColor = { 0, 0, 0 }, wchar_t InCharacter = 0, int InOrderId = 0, int InFrameId = 0) : BackgroundColor(InBackgroundColor), ForegroundColor(InForegroundColor), Character(InCharacter), OrderId(InOrderId), FrameId(InFrameId) {}

		Color BackgroundColor;
		
		Color ForegroundColor;
		
		wchar_t Character;
		
		int OrderId;
		
		int FrameId;

		bool operator==(const TileData& Other) const
		{
			return BackgroundColor.r == Other.BackgroundColor.r && BackgroundColor.g == Other.BackgroundColor.g && BackgroundColor.b == Other.BackgroundColor.b && ForegroundColor.r == Other.ForegroundColor.r && ForegroundColor.g == Other.ForegroundColor.g && ForegroundColor.b == Other.ForegroundColor.b && Character == Other.Character;
		}
	};

	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	int FrameId = 0;

	std::map<unsigned int, HBRUSH> Brushes;

	std::vector<std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>> WndProcs;

	HWND Hwnd = nullptr;

	std::vector<TileData> NewTileGrid;

	std::vector<TileData> TileGrid;

	int TileWidth = 0;

	int TileHeight = 0;

	int NumColumns = 0;

	int NumRows = 0;

	int Width = 0;

	int Height = 0;

	bool WindowClosed = false;

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HBRUSH GetBrushWithColor(const Color InColor);

	bool IsTileWithinBounds(const int InX, const int InY) const;

	int GetTileIndex(const int InX, const int InY) const;

	void ClearBrushes();

public:
	SnakeGraphics(const int InWidth, const int InHeight, const int InNumColumns, const int InNumRows);

	~SnakeGraphics();

	bool Init();

	void PlotTile(const int InX, const int InY, const int InOrderId, const Color InBackgroundColor, const Color InForegroundColor, const wchar_t InCharacter);

	void PlotText(const int InX, const int InY, const int InOrderId, const Color InBackgroundColor, const wchar_t* InText, const Color InForegroundColor, Alignment InAlignment);

	void Render();

	int GetNumColumns() const;

	int GetNumRows() const;

	bool UpdateWindowMessages();

	void AddWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WndProc);

	void RemoveWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WndProc);
};