#include "Window.h"

bool AGWin32::create_window(AGWin32::WINDOW& window, HWND parent, WNDCLASS wndClass, int id, std::wstring title, int width, int height)
{
	bool retVal = true;

	window.width = width;
	window.height = height;

	HWND hnd = CreateWindow(
		wndClass.lpszClassName,
		title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL,
		NULL,
		wndClass.hInstance,
		NULL
	);

	if (hnd)
		window.handle = hnd;
	else
		retVal = false;

	return retVal;
}