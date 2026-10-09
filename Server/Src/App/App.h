#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace AGS_APP
{
	void startApp(HINSTANCE hInst, LPWSTR lpCmdLine, int nCmdShow);
}