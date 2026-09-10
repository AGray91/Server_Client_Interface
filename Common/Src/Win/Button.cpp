#include "Button.h"
#include <CommCtrl.h>

bool AGWin32::Button::create(BUTTON& button, HWND parent, HINSTANCE hInst, int id, std::wstring btn_text)
{
	bool retVal = false;

	HWND hnd = CreateWindow(
		WC_BUTTON,
		(LPWSTR)btn_text.c_str(),
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		CW_DEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		parent,
		(HMENU)id,
		hInst,
		NULL
	);

	if (hnd)
		button.handle = hnd;
	else
		retVal = false;

	return retVal;
}
