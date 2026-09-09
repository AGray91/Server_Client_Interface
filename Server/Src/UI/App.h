#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// Constants...
#define MAIN_WINDOW_WIDTH	1000
#define MAIN_WINDOW_HEIGHT	600
#define MAIN_WINDOW_TITLE	L"AG SERVER"

namespace AGS_UI
{
	void startApp(HINSTANCE hInst, LPWSTR lpCmdLine, int nCmdShow);
}