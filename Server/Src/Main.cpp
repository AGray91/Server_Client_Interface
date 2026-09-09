#include "UI/App.h"

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	AGS_UI::startApp(hInst, lpCmdLine, nCmdShow);
	return 0;
}