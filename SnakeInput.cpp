#include "SnakeInput.h"
#include "SnakeGraphics.h"
#include <iostream>

SnakeInput* SnakeInput::Instance = nullptr;

SnakeInput::SnakeInput(SnakeGraphics* InGraphics) : Graphics(InGraphics)
{
	WndProcFunction = std::bind(&SnakeInput::WndProc, this,
		std::placeholders::_1, std::placeholders::_2,
		std::placeholders::_3, std::placeholders::_4);

	Graphics->AddWndProc(WndProcFunction);
}

SnakeInput::~SnakeInput()
{
	Graphics->RemoveWndProc(WndProcFunction);
}

bool SnakeInput::Init(SnakeGraphics* InGraphics)
{
	if (Instance != nullptr)
	{
		std::cerr << "Input already initilized!" << std::endl;

		return false;
	}

	Instance = new SnakeInput(InGraphics);

	return true;
}

LRESULT SnakeInput::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
		for (auto it = Instance->OnKeyDownCallbacks.begin(); it != Instance->OnKeyDownCallbacks.end(); ++it)
		{
			(*it)((int)wParam);
		}
        break;
	case WM_KEYUP:
		for (auto it = Instance->OnKeyUpCallbacks.begin(); it != Instance->OnKeyUpCallbacks.end(); ++it)
		{
			(*it)((int)wParam);
		}
		break;
	}
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


void SnakeInput::AddKeyDownCallback(std::function<void(int)> KeyDownCallback)
{
	for (auto it = Instance->OnKeyDownCallbacks.begin(); it != Instance->OnKeyDownCallbacks.end(); ++it)
	{
		if (it->target<void(int)>() == KeyDownCallback.target<void(int)>())
		{
			return;
		}
	}
	Instance->OnKeyDownCallbacks.push_back(KeyDownCallback);
}

void SnakeInput::AddKeyUpCallback(std::function<void(int)> KeyUpCallback)
{
	for (auto it = Instance->OnKeyUpCallbacks.begin(); it != Instance->OnKeyUpCallbacks.end(); ++it)
	{
		if (it->target<void(int)>() == KeyUpCallback.target<void(int)>())
		{
			return;
		}
	}
	Instance->OnKeyUpCallbacks.push_back(KeyUpCallback);
}

void SnakeInput::RemoveKeyDownCallback(std::function<void(int)> KeyDownCallback)
{
	for (auto it = Instance->OnKeyDownCallbacks.begin(); it != Instance->OnKeyDownCallbacks.end(); ++it)
	{
		if (it->target<void(int)>() == KeyDownCallback.target<void(int)>())
		{
			Instance->OnKeyDownCallbacks.erase(it);
			break;
		}
	}
}

void SnakeInput::RemoveKeyUpCallback(std::function<void(int)> KeyUpCallback)
{
	for (auto it = Instance->OnKeyUpCallbacks.begin(); it != Instance->OnKeyUpCallbacks.end(); ++it)
	{
		if (it->target<void(int)>() == KeyUpCallback.target<void(int)>())
		{
			Instance->OnKeyUpCallbacks.erase(it);
			break;
		}
	}
}

void SnakeInput::CleanUp()
{
	if (Instance != nullptr)
	{
		delete Instance;

		Instance = nullptr;
	}
}