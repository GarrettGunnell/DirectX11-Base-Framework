////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "System.h"


System::System() {
	input = 0;
	graphics = 0;
}


bool System::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	input = new Input();
	if(!input)
	{
		return false;
	}

	// Initialize the input object.
	input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	graphics = new Graphics();
	if(!graphics) {
		return false;
	}

	// Initialize the graphics object.
	result = graphics->Initialize(screenWidth, screenHeight, hwnd);
	if(!result) {
		return false;
	}
	
	return true;
}


void System::Shutdown() {
	if(graphics) {
		graphics->Shutdown();
		delete graphics;
		graphics = 0;
	}

	if(input) {
		delete input;
		input = 0;
	}

	ShutdownWindows();
}


void System::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));
	
	done = false;
	while(!done) {
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT) {
			done = true;
		}
		else {
			// Otherwise do the frame processing.
			result = Frame();
			if(!result) {
				done = true;
			}
		}
	}
}


bool System::Frame() {
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if(input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	// Do the frame processing for the graphics object.
	result = graphics->Frame();
	if(!result) {
		return false;
	}

	return true;
}


LRESULT CALLBACK System::MessageHandler(HWND _hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch(umsg) {
	case WM_KEYDOWN:
		input->KeyDown((unsigned int)wparam);
		return 0;
	case WM_KEYUP:
		input->KeyUp((unsigned int)wparam);
		return 0;
	default:
		return DefWindowProc(_hwnd, umsg, wparam, lparam);
	}
}


void System::InitializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN) {
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else {
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 1600;
		screenHeight = 900;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	hwnd = CreateWindowEx(NULL, applicationName, applicationName, 
						    WS_OVERLAPPEDWINDOW,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
}


void System::ShutdownWindows() {
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(hwnd);
	hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(applicationName, hinstance);
	hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch(umessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);		
		return 0;
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}