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
	struct EDIT
	{
		HWND handle;
		int x;
		int y;
		int width;
		int height;
	};

	namespace Edit
	{
		bool create(EDIT& edit, HWND parent, HINSTANCE hInst, int id, std::wstring placeholder_text);
	}
}