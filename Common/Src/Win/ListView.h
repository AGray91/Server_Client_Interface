#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <commctrl.h>

#define MAX_CELL_TEXT_LENGTH 32		// Maxium character length for rows and columns...

namespace AGWin32
{
	struct HEADER
	{
		std::wstring text;
		bool is_editable;
		int width;

		HEADER(std::wstring txt, bool editable = false, int cx = 200)
		{
			text = txt;
			is_editable = editable;
			width = cx;
		}
	};

	struct LISTVIEW
	{
		HWND handle;
		int x;
		int y;
		int width;
		int height;
		std::vector<HEADER> columns;
		HWND item_edit;
		int edit_row;
		int edit_col;
	};

	namespace ListView
	{
		void register_common_ctrls();
		bool create_listview(LISTVIEW& listview, HWND parent, HINSTANCE hInst, int id, std::vector<HEADER> columns);
		void add_row(LISTVIEW& listview, std::vector<std::wstring> row);
		void add_row_item(LISTVIEW& listview, const wchar_t* row);
		void add_row_sub_item(LISTVIEW& listview, const wchar_t* row, const wchar_t* col, const wchar_t* value);
		void set_column_width(LISTVIEW& listview, const wchar_t* col, int value);
		void stretch_last_column(LISTVIEW& listview);	// Use this to stretch the last column to the end of the ListView
		void edit_sub_item_begin(LISTVIEW& listview, LPNMITEMACTIVATE item);
		void edit_sub_item_end(LISTVIEW& listview, bool write);
		int find_item(LISTVIEW& listview, std::wstring item);
	}
}