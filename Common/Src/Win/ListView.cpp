#include "ListView.h"

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace AGWin32
{
	namespace ListView
	{
		bool find_column(LISTVIEW& listview, const wchar_t* column, LVCOLUMN& col);
		bool find_row(LISTVIEW& listview, const wchar_t* row, LVITEM& item);
		int get_row_count(LISTVIEW& listview);
	}
}

void AGWin32::ListView::register_common_ctrls()
{
	INITCOMMONCONTROLSEX icex = {};
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
}

bool AGWin32::ListView::create_listview(LISTVIEW& listview, HWND parent, HINSTANCE hInst, int id, std::vector<HEADER> columns)
{
	bool retVal = true;

	HWND hnd = CreateWindow(
		WC_LISTVIEW,
		NULL,
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | WS_BORDER,
		0, 0, 256, 256,
		parent,
		(HMENU)id,
		hInst,
		NULL
	);

	if (hnd)
	{
		ListView_SetExtendedListViewStyle(hnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		ListView_SetItemCount(hnd, 0);
		listview.handle = hnd;

		for (size_t i = 0; i < columns.size(); i++)
		{
			LVCOLUMN lv_col = { 0 };
			lv_col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
			lv_col.pszText = (LPWSTR)columns[i].text.c_str();
			lv_col.cx = columns[i].width;

			listview.columns.push_back(columns[i]);

			ListView_InsertColumn(listview.handle, i, &lv_col);
		}
	}
	else
	{
		retVal = false;
	}

	return retVal;
}

void AGWin32::ListView::add_row(LISTVIEW& listview, std::vector<std::wstring> row)
{
	LVITEM lv_item = { 0 };
	lv_item.mask = LVIF_TEXT;
	lv_item.iItem = get_row_count(listview);
	lv_item.pszText = (LPWSTR)row[0].c_str();

	ListView_InsertItem(listview.handle, &lv_item);

	for (size_t i = 1; i < listview.columns.size(); i++)
	{
		lv_item.iSubItem = i;
		lv_item.pszText = (LPWSTR)row[i].c_str();

		ListView_SetItem(listview.handle, &lv_item);
	}
}

void AGWin32::ListView::add_row_item(LISTVIEW& listview, const wchar_t* row)
{
	wchar_t row_text[MAX_CELL_TEXT_LENGTH];
	lstrcpyW(row_text, row);

	LVITEM lv_item = { 0 };
	lv_item.mask = LVIF_TEXT;
	lv_item.iItem = get_row_count(listview);
	lv_item.pszText = row_text;

	ListView_InsertItem(listview.handle, &lv_item);
}

void AGWin32::ListView::add_row_sub_item(LISTVIEW& listview, const wchar_t* row, const wchar_t* col, const wchar_t* value)
{
	wchar_t row_text[MAX_CELL_TEXT_LENGTH];
	lstrcpyW(row_text, row);

	wchar_t col_text[MAX_CELL_TEXT_LENGTH];
	lstrcpyW(col_text, col);

	wchar_t val_text[32];
	lstrcpyW(val_text, value);

	LVCOLUMN lv_col = { 0 };
	if (!find_column(listview, col, lv_col))
		return;

	LVITEM lv_row = { 0 };
	if (!find_row(listview, row, lv_row))
		return;

	LVITEM sub_item = { 0 };
	sub_item.mask = LVIF_TEXT;
	sub_item.iItem = lv_row.iItem;
	sub_item.iSubItem = lv_col.iSubItem;
	sub_item.pszText = val_text;

	ListView_SetItem(listview.handle, &sub_item);
}

void AGWin32::ListView::set_column_width(LISTVIEW& listview, const wchar_t* col, int value)
{
	LVCOLUMN lv_col = { 0 };
	if (!find_column(listview, col, lv_col))
		return;

	ListView_SetColumnWidth(listview.handle, lv_col.iSubItem, value);
}

bool AGWin32::ListView::find_column(LISTVIEW& listview, const wchar_t* column, LVCOLUMN& col)
{
	bool retVal = false;

	WCHAR buffer[MAX_CELL_TEXT_LENGTH];

	col.mask = LVCF_TEXT;
	col.pszText = buffer;
	col.cchTextMax = MAX_CELL_TEXT_LENGTH;

	for (size_t i = 0; i < listview.columns.size(); i++)
	{
		if (lstrcmpW(listview.columns[i].text.data(), column) == 0)
		{
			retVal = true;
			col.iSubItem = i;
			break;
		}
	}

	return retVal;
}

bool AGWin32::ListView::find_row(LISTVIEW& listview, const wchar_t* row, LVITEM& item)
{
	bool retVal = false;

	WCHAR buffer[MAX_CELL_TEXT_LENGTH];
	item.mask = LVIF_TEXT;
	item.pszText = buffer;
	item.cchTextMax = MAX_CELL_TEXT_LENGTH;

	for (int i = 0; i < get_row_count(listview); i++)
	{
		item.iItem = i;

		ListView_GetItem(listview.handle, &item);
		if (lstrcmpW(item.pszText, row) == 0)
		{
			retVal = true;
			break;
		}
	}

	return retVal;
}

int AGWin32::ListView::get_row_count(LISTVIEW& listview)
{
	int retVal = 0;

	retVal = ListView_GetItemCount(listview.handle);

	return retVal;
}

// Use this to stretch the last column to the end of the ListView
void AGWin32::ListView::stretch_last_column(LISTVIEW& listview)
{
	// Get the combined widths of every column but the last one and then subtract this from the whole width of the ListView and set this to the width of the last column...
	int total_column_widths = 0;
	LVCOLUMN col = { 0 };
	RECT h_rect = { 0 };
	RECT lv_rect = { 0 };

	HWND header = ListView_GetHeader(listview.handle);

	Header_GetItemRect(header, Header_GetItemCount(header) - 2, &h_rect);
	total_column_widths = h_rect.right;

	GetWindowRect(listview.handle, &lv_rect);
	int last_column_width = lv_rect.right - total_column_widths;
	ListView_SetColumnWidth(listview.handle, Header_GetItemCount(header) - 1, last_column_width - lv_rect.left);
}

void AGWin32::ListView::edit_sub_item_begin(LISTVIEW& listview, LPNMITEMACTIVATE item)
{
	// Check for valid item...
	if (item->iItem < 0 || item->iSubItem < 0)
		return;

	// Check if this column is editable...
	if (!listview.columns[item->iSubItem].is_editable)
		return;

	// Get the size of the ListView item...
	RECT rect;
	ListView_GetSubItemRect(listview.handle, item->iItem, item->iSubItem, LVIR_BOUNDS, &rect);

	// Get the text that already exists in the ListView item...
	WCHAR buffer[MAX_CELL_TEXT_LENGTH];
	ListView_GetItemText(listview.handle, item->iItem, item->iSubItem, buffer, sizeof(buffer));

	// Get the position of the ListView item in relation to the screen rather than the App window...
	POINT pt = { rect.left, rect.top };
	ClientToScreen(listview.handle, &pt);
	ScreenToClient(GetParent(listview.handle), &pt);

	listview.item_edit = CreateWindow(
		WC_EDIT,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top,
		GetParent(listview.handle),
		NULL,
		NULL,
		NULL
	);

	if (listview.item_edit)
	{
		SetFocus(listview.item_edit);

		listview.edit_row = item->iItem;
		listview.edit_col = item->iSubItem;
	}
}

void AGWin32::ListView::edit_sub_item_end(LISTVIEW& listview, bool write)
{
	if (!listview.item_edit)
		return;

	if (write)
	{
		WCHAR buffer[MAX_CELL_TEXT_LENGTH];
		GetWindowText(listview.item_edit, buffer, MAX_CELL_TEXT_LENGTH);

		ListView_SetItemText(listview.handle, listview.edit_row, listview.edit_col, buffer);
	}

	DestroyWindow(listview.item_edit);
	listview.item_edit = NULL;
}

int AGWin32::ListView::find_item(LISTVIEW& listview, std::wstring item)
{
	int retVal;

	LVFINDINFO item_info = { 0 };
	item_info.flags = LVFI_STRING;
	item_info.psz = item.c_str();

	retVal = ListView_FindItem(listview.handle, -1, &item_info);

	return retVal;
}