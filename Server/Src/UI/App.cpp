#include "App.h"
#include "Src/Win/Window.h"
#include "Src/Win/ListView.h"
#include <vector>

/* Handle IDs */
#define FILEMENU

/* ListView Column Headers*/
#define LABEL L"LABEL"
#define TYPE L"TYPE"
#define VALUE L"VALUE"
#define DIRECTION L"DIRECTION"
#define DESCRIPTION L"DESCRIPTION"

namespace AGS_UI
{
	// Global Window Class...
	WNDCLASS wnd_class;

	// UI Elements...
	AGWin32::WINDOW ui_main_window;

	// Main Callback...
	LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

	// Handler functions - All main logic is held here...
	void create_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void resize_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void close_handler(HWND hWnd, WPARAM wParam, LPARAM lParam);
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
	if (!AGWin32::create_window(ui_main_window, NULL, wnd_class, NULL, MAIN_WINDOW_TITLE, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT))
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

	default:
		retVal = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return retVal;
}

/* Create the UI */
void AGS_UI::create_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	AGWin32::ListView::register_common_ctrls();
	std::vector<AGWin32::HEADER> headers;

	AGWin32::HEADER label = AGWin32::HEADER(LABEL, false, 200);
	headers.push_back(label);

	AGWin32::HEADER type = AGWin32::HEADER(TYPE, false, 96);
	headers.push_back(label);

	AGWin32::HEADER value = AGWin32::HEADER(VALUE, true, 96);
	headers.push_back(value);

	AGWin32::HEADER direction = AGWin32::HEADER(DIRECTION, false, 96);
	headers.push_back(direction);

	AGWin32::HEADER description = AGWin32::HEADER(DESCRIPTION, false, 400);
	headers.push_back(description);

}

/* Layout UI and resize */
void AGS_UI::resize_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RECT wnd_rect;
	GetClientRect(hWnd, &wnd_rect);


}

void AGS_UI::close_handler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);
}