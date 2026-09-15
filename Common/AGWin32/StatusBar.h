#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace AGWin32
{
	struct STATUSBAR
	{
		HWND handle;
		int x;
		int y;
		int width;
		int height;

		int parts;
	};

	namespace StatusBar
	{
		bool create(STATUSBAR& statusbar, HWND parent, HINSTANCE hInst, int id, int parts);
	}
}