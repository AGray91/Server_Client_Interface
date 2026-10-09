#include "App.h"
#include "../UI/UI.h"

void AGS_APP::startApp(HINSTANCE hInst, LPWSTR lpCmdLine, int nCmdShow)
{
	AGS_UI::start_ui(hInst, lpCmdLine, nCmdShow);
}