#pragma once

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string>

namespace AGWin32
{
		struct WINDOW
		{
			HWND handle;
			int x;
			int y;
			int width;
			int height;
		};

		bool create_window(WINDOW& window, HWND parent, WNDCLASS wndClass, int id, std::wstring title, int width, int height);
}