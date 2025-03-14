#pragma once
#include <functional>
#include <vector>
#include <map>
#include "Windows.h"

class SnakeGraphics;

class SnakeInput
{
private:
	static SnakeInput* Instance;

	std::vector<std::function<void(int)>> OnKeyDownCallbacks;

	std::vector<std::function<void(int)>> OnKeyUpCallbacks;

	std::function<LRESULT(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)> WndProcFunction;

	SnakeGraphics* Graphics = nullptr;

	SnakeInput(SnakeGraphics* InGraphics);

	~SnakeInput();

	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static bool Init(SnakeGraphics* InGraphics);

	static void AddKeyDownCallback(std::function<void(int)> KeyDownCallback);

	static void AddKeyUpCallback(std::function<void(int)> KeyDownCallback);

	static void RemoveKeyDownCallback(std::function<void(int)> KeyDownCallback);

	static void RemoveKeyUpCallback(std::function<void(int)> KeyDownCallback);

	static void CleanUp();
};