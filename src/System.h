#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Input.h"
#include "Graphics.h"

class System {
public:
	System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	Input* input;
	Graphics* graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = 0;