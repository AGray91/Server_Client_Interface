#include "../Server/Src/App/App.h"

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	AGS_APP::startApp(hInst, lpCmdLine, nCmdShow);
	return 0;
}