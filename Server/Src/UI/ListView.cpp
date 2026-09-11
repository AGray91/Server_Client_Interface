#include "ListView.h"

namespace AGS_UI
{
	AGWin32::LISTVIEW listview;

	void show_context_menu(AGWin32::LISTVIEW& listview, POINT pt, int item_index);
}

void AGS_UI::listview_item_right_clicked(LPNMITEMACTIVATE item)
{
	POINT pt;
	GetCursorPos(&pt);

	POINT pt_client = pt;
	ScreenToClient(listview.handle, &pt_client);

	LVHITTESTINFO hit_test = {};
	hit_test.pt = pt_client;

	int index = ListView_HitTest(listview.handle, &hit_test);

	if (index != -1)
		show_context_menu(listview, pt, index);
}

void AGS_UI::show_context_menu(AGWin32::LISTVIEW& listview, POINT pt, int item_index)
{
	HMENU hmenu = CreatePopupMenu();
	AppendMenu(hmenu, MF_STRING, 1, L"Delete Label");

	SetWindowLongPtr(listview.handle, GWLP_USERDATA, item_index);

	TrackPopupMenu(
		hmenu,
		TPM_RIGHTBUTTON,
		pt.x,
		pt.y,
		0,
		GetParent(listview.handle),
		NULL
	);

	DestroyMenu(hmenu);
}