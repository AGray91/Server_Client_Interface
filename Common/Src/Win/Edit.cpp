#include "Edit.h"
#include <CommCtrl.h>

bool AGWin32::Edit::create(EDIT& edit, HWND parent, HINSTANCE hInst, int id, std::wstring placeholder_text)
{
	bool retVal = true;

	HWND hnd = CreateWindow(
		WC_EDIT,
		placeholder_text.c_str(),
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		parent,
		(HMENU)id,
		hInst,
		NULL
	);

	if (hnd)
		edit.handle = hnd;
	else
		retVal = false;

	return retVal;
}