#include "include.h"
#include "Application.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpczCmd, int nShowCmd)
{
	Application app;
	return app.runProgramm();
}