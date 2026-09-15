#include "App.h"
#include "AGWin32/Window.h"
#include "AGWin32/ListView.h"
#include "ListView.h"
#include <vector>
#include "AGWin32/Button.h"
#include "AGWin32/Edit.h"
#include "AGWin32/StatusBar.h"

/* Handle IDs */
#define FILEMENU_OPEN	1001
#define FILEMENU_SAVE	1002
#define FILEMENU_SAVEAS	1003
#define FILEMENU_CLOSE	1004
#define HELPMENU		1005

#define LISTVIEW_ID		2001

#define BTN_SHOWALL_ID	3001
#define BTN_HIDEALL_ID	3002
#define BTN_ADDLBL_ID	3003

#define EDIT_ADDLBL_ID	4001

#define STATUSBAR_ID	5001

/* Default Button Sizes */
#define DEF_BTN_WIDTH	100
#define DEF_BTN_HEIGHT	32
#define DEF_EDIT_HEIGHT	20

/* ListView Column Headers*/
#define LABEL		L"LABEL"
#define TYPE		L"TYPE"
#define VALUE		L"VALUE"
#define DIRECTION	L"DIRECTION"
#define DESCRIPTION	L"DESCRIPTION"

/* Status Bar Size */
#define SB_PARTS	3

namespace AGS_UI
{
	// Global Window Class...
	WNDCLASS wnd_class;

	// UI Elements...
	AGWin32::WINDOW ui_main_window;
	HMENU ui_main_menu;
	AGWin32::BUTTON ui_hide_all_btn;
	AGWin32::BUTTON ui_show_all_btn;
	AGWin32::BUTTON ui_add_lbl_btn;
	AGWin32::EDIT ui_add_lbl_edit;
	AGWin32::STATUSBAR ui_statusbar;


	// Main Callback...
	LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

	// Handler functions - All main logic is held here...
	void create_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void resize_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void close_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void notify_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void command_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
}

void AGS_UI::startApp(HINSTANCE hInst, LPWSTR lpCmdLine, int nCmdShow)
{
	// Calibrate main window class and register...
	wnd_class = {};
	wnd_class.hInstance = hInst;
	wnd_class.lpfnWndProc = WndProc;
	wnd_class.lpszClassName = L"MainWindow";
	wnd_class.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));

	if (!RegisterClass(&wnd_class))
	{
		MessageBox(NULL, L"Failed to register window!", L"Error", NULL);
		return;
	}

	// Create Main Window...
	if (!AGWin32::create_window(ui_main_window, NULL, wnd_class, LISTVIEW_ID, MAIN_WINDOW_TITLE, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT))
		return;
	
	ShowWindow(ui_main_window.handle, nCmdShow);


	/* MAIN MESSAGE LOOP */
	MSG msg = {};
	while (GetMessage(&msg, NULL, NULL, NULL) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK AGS_UI::WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	LRESULT retVal = 0;

	switch (message)
	{
	case WM_CLOSE:
		close_handler(hWnd, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		create_handler(hWnd, wParam, lParam);
		break;

	case WM_SIZE:
		resize_handler(hWnd, wParam, lParam);
		break;

	case WM_NOTIFY:
		notify_handler(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		command_handler(hWnd, wParam, lParam);
		break;

	default:
		retVal = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return retVal;
}

void AGS_UI::close_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);
}

/* Create the UI */
void AGS_UI::create_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/* Create Menus... */
	HMENU file_menu = CreateMenu();
	AppendMenuW(file_menu, MF_STRING, FILEMENU_OPEN, L"Open");
	AppendMenuW(file_menu, MF_STRING, FILEMENU_SAVE, L"Save");
	AppendMenuW(file_menu, MF_STRING, FILEMENU_SAVEAS, L"Save As");
	AppendMenuW(file_menu, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(file_menu, MF_STRING, FILEMENU_CLOSE, L"Close");

	ui_main_menu = CreateMenu();
	AppendMenuW(ui_main_menu, MF_POPUP, (UINT_PTR)file_menu, L"File");
	AppendMenuW(ui_main_menu, MF_STRING, HELPMENU, L"Help");
	SetMenu(hWnd, ui_main_menu);



	/* Create List view... */
	AGWin32::ListView::register_common_ctrls();
	std::vector<AGWin32::HEADER> headers;

	AGWin32::HEADER label = AGWin32::HEADER(LABEL, false, 200);
	headers.push_back(label);

	AGWin32::HEADER type = AGWin32::HEADER(TYPE, false, 96);
	headers.push_back(type);

	AGWin32::HEADER value = AGWin32::HEADER(VALUE, true, 96);
	headers.push_back(value);

	AGWin32::HEADER direction = AGWin32::HEADER(DIRECTION, false, 96);
	headers.push_back(direction);

	AGWin32::HEADER description = AGWin32::HEADER(DESCRIPTION, false, 400);
	headers.push_back(description);

	// ListView Layout...
	listview.x = 0;
	listview.y = 0;
	listview.width = MAIN_WINDOW_WIDTH;
	listview.height = MAIN_WINDOW_HEIGHT;

	if (!AGWin32::ListView::create(listview, hWnd, wnd_class.hInstance, LISTVIEW_ID, headers))
		return;



	/* Create Buttons... */
	AGWin32::Button::create(ui_show_all_btn, hWnd, wnd_class.hInstance, BTN_SHOWALL_ID, L"SHOW ALL");
	AGWin32::Button::create(ui_hide_all_btn, hWnd, wnd_class.hInstance, BTN_HIDEALL_ID, L"HIDE ALL");
	AGWin32::Button::create(ui_add_lbl_btn, hWnd, wnd_class.hInstance, BTN_ADDLBL_ID, L"SHOW LABEL");



	/* Create Add Label Edit... */
	AGWin32::Edit::create(ui_add_lbl_edit, hWnd, wnd_class.hInstance, EDIT_ADDLBL_ID, L"Add Label");



	/* Create Status Bar... */
	AGWin32::StatusBar::create(ui_statusbar, hWnd, wnd_class.hInstance, STATUSBAR_ID, SB_PARTS);
}

/* Layout UI and resize */
void AGS_UI::resize_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RECT wnd_rect;
	GetClientRect(hWnd, &wnd_rect);



	/* LIST VIEW LAYOUT... */
	int width_margin = (wnd_rect.right / 100) * 5;
	int height_margin = (wnd_rect.bottom / 100) * 30;

	listview.width = wnd_rect.right - (width_margin * 2);
	listview.height = wnd_rect.bottom - (height_margin + 10);
	listview.x = wnd_rect.left + width_margin;
	listview.y = wnd_rect.top + 10;

	SetWindowPos(listview.handle, NULL, listview.x, listview.y, listview.width, listview.height, SWP_NOZORDER);

	// Make last column stretch to the end of the list view...
	AGWin32::ListView::stretch_last_column(listview);



	/* BUTTON LAYOUT... */
	// Show All button...
	ui_show_all_btn.width = DEF_BTN_WIDTH;
	ui_show_all_btn.height = DEF_BTN_HEIGHT;
	ui_show_all_btn.x = listview.x;
	ui_show_all_btn.y = listview.y + listview.height + 10;
	SetWindowPos(ui_show_all_btn.handle, NULL, ui_show_all_btn.x, ui_show_all_btn.y, ui_show_all_btn.width, ui_show_all_btn.height, SWP_NOZORDER);

	// Hide All button...
	ui_hide_all_btn.width = DEF_BTN_WIDTH;
	ui_hide_all_btn.height = DEF_BTN_HEIGHT;
	ui_hide_all_btn.x = ui_show_all_btn.x + ui_show_all_btn.width + 20;
	ui_hide_all_btn.y = ui_show_all_btn.y;
	SetWindowPos(ui_hide_all_btn.handle, NULL, ui_hide_all_btn.x, ui_hide_all_btn.y, ui_hide_all_btn.width, ui_hide_all_btn.height, SWP_NOZORDER);

	// Add Label button...
	ui_add_lbl_btn.width = DEF_BTN_WIDTH;
	ui_add_lbl_btn.height = DEF_BTN_HEIGHT;
	ui_add_lbl_btn.x = (listview.width + listview.x) - ui_add_lbl_btn.width;
	ui_add_lbl_btn.y = ui_hide_all_btn.y;
	SetWindowPos(ui_add_lbl_btn.handle, NULL, ui_add_lbl_btn.x, ui_add_lbl_btn.y, ui_add_lbl_btn.width, ui_add_lbl_btn.height, SWP_NOZORDER);



	/* EDIT LAYOUT... */
	ui_add_lbl_edit.width = 200;
	ui_add_lbl_edit.height = DEF_EDIT_HEIGHT;
	ui_add_lbl_edit.x = ui_add_lbl_btn.x - (ui_add_lbl_edit.width + 20);
	ui_add_lbl_edit.y = ui_add_lbl_btn.y + ((ui_add_lbl_btn.height - ui_add_lbl_edit.height) / 2);
	SetWindowPos(ui_add_lbl_edit.handle, NULL, ui_add_lbl_edit.x, ui_add_lbl_edit.y, ui_add_lbl_edit.width, ui_add_lbl_edit.height, SWP_NOZORDER);



	/* STATUS BAR LAYOUT... */
	int part_widths[SB_PARTS];
	part_widths[0] = wnd_rect.right - 210;
	part_widths[1] = wnd_rect.right - 150;
	part_widths[2] = -1;

	SendMessage(ui_statusbar.handle, SB_SETPARTS, (WPARAM)ui_statusbar.parts, (LPARAM)part_widths);
	SendMessage(ui_statusbar.handle, WM_SIZE, 0, 0);
}

void AGS_UI::notify_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR hrd = (LPNMHDR)lParam;
}

void AGS_UI::command_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// Button commands...
	if (HIWORD(wParam) == BN_CLICKED)
	{
		switch (LOWORD(wParam))
		{
		case BTN_SHOWALL_ID:
			break;

		case BTN_HIDEALL_ID:
			break;

		case BTN_ADDLBL_ID:
			break;
		}
	}
}