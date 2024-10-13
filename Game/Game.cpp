// Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>

#include <core/Engine.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nShowCmd)
{
#ifdef __EDITOR__
	renegade::core::ENGINE.Initialize(3, hInstance, 1920, 1080, L"Renegade Engine");
#else
	renegade::core::ENGINE.Initialize(3, hInstance, 1920, 1080, L"CoopScoop");
#endif
	return 0;
}