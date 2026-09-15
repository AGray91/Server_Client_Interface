#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string>

namespace AGWin32
{
	struct BUTTON
	{
		HWND handle;
		int x;
		int y;
		int width;
		int height;
	};

	namespace Button
	{
		bool create(BUTTON& button, HWND parent, HINSTANCE hInst, int id, std::wstring btn_text);
	}
}