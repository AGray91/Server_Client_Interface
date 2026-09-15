#pragma once
#include "AGWin32/ListView.h"

namespace AGS_UI
{
	extern AGWin32::LISTVIEW listview;

	void listview_item_right_clicked(LPNMITEMACTIVATE item);
}