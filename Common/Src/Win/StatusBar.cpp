#include "StatusBar.h"
#include <CommCtrl.h>

bool AGWin32::StatusBar::create(AGWin32::STATUSBAR& statusbar, HWND parent, HINSTANCE hInst, int id, int parts)
{
	bool retVal = false;

	HWND hnd = CreateWindow(
		STATUSCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		parent,
		(HMENU)id,
		hInst,
		NULL
	);

	if (hnd)
	{
		statusbar.parts = parts;
		statusbar.handle = hnd;
	}
	else
	{
		retVal = false;
	}

	return retVal;
}