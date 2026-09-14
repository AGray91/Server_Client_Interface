#pragma once
#include "Src/Win/ListView.h"

namespace AGS_UI
{
	extern AGWin32::LISTVIEW listview;

	void listview_item_right_clicked(LPNMITEMACTIVATE item);
}