#pragma once
#include <windows.h>
#include "NoteParser.h"


#define IDC_MAIN_EDIT 101
#define ID_FILE_EXIT 9001
#define ID_ABOUT 9002
#define ID_HELP 9003
#define ID_CALC 9004
#define ID_OPENIGNORE 9005
#define ID_INPROGRESS 9020


//Global Entities
const char g_szClassName[] = "mainWindow";
const char g_WindowTitle[] = "Labor Calculator V0.0.1";
NoteParser g_Crafter;
HWND hNote, hHour, hMin;

//Global Funcs
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void AddMenu(HWND hwnd);
void AddControls(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(4, 66, 89));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = (HICON)LoadImage(hInstance, "Resources\\LC Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	wc.hIcon = (HICON)LoadImage(hInstance, "Resources\\LC Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, g_WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		AddMenu(hwnd);
		AddControls(hwnd);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_OPENIGNORE:
			ShellExecute(hwnd, "open", g_Crafter.GetIgnoreLoc().c_str(), NULL, NULL, SW_SHOW);
			g_Crafter.SetIgnoreOpened(true);
			break;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case ID_ABOUT:
			MessageBox(NULL, "I just figured I could save time with a quick application removing empty lines from my labor notes and running everything through a calculator.\nAlso, slightly faster than Excel.\n\n-Marius Ventus", "About", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_HELP:
			MessageBox(NULL, "No help, only Zuul.\nOr reaching me on Teams.\n\nOr the Readme:\nhttps://github.com/MariusVentus/LaborCalculator/blob/master/README.md ", "Halp", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_INPROGRESS:
			MessageBox(NULL, "Apologies, this feature is under construction.", "Under Construction", MB_OK | MB_ICONEXCLAMATION);
			break;
		case ID_CALC:
			//Init
			char rawNote[3000] = "";
			std::string stringNote = "";
			GetWindowText(hNote, rawNote, 3000);
			stringNote = rawNote;
			//Parse and Update
			stringNote = g_Crafter.ParseNCalc(stringNote);
			SetWindowText(hNote, stringNote.c_str());
			SetWindowText(hHour, g_Crafter.GetHours().c_str());
			SetWindowText(hMin, g_Crafter.GetMinutes().c_str());
			//Copy to Clipboard
			OpenClipboard(hwnd);
			EmptyClipboard();
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, stringNote.size() + 1);
			if (!hg) {
				CloseClipboard();
			}
			else {
				memcpy(GlobalLock(hg), stringNote.c_str(), stringNote.size() + 1);
				GlobalUnlock(hg);
				SetClipboardData(CF_TEXT, hg);
				CloseClipboard();
			}
			GlobalFree(hg);
			//End
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void AddMenu(HWND hwnd)
{
	HMENU hMenu, hFileMenu, hSettingsMenu;
	hMenu = CreateMenu();
	//File Menu
	hFileMenu = CreatePopupMenu();
	AppendMenu(hFileMenu, MF_STRING, ID_OPENIGNORE, "Open Ignore List");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, "Exit");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hFileMenu, "File");
	//Settings Menu
	hSettingsMenu = CreatePopupMenu();
	AppendMenu(hSettingsMenu, MF_STRING, ID_INPROGRESS, "Settings");
	AppendMenu(hSettingsMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hSettingsMenu, MF_STRING, ID_INPROGRESS, "AI Generator");
	AppendMenu(hSettingsMenu, MF_STRING, ID_INPROGRESS, "Feed AI");
	AppendMenu(hSettingsMenu, MF_STRING, ID_INPROGRESS, "Train AI");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSettingsMenu, "Generator");


	//Remaining Main Menu Items1e1e1e
	AppendMenu(hMenu, MF_STRING, ID_ABOUT, "About");
	AppendMenu(hMenu, MF_STRING, ID_HELP, "Help");

	SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd)
{

	//Notes
	CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", " Labor Notes ", WS_CHILD | WS_VISIBLE,
		15, 20, 380, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
	hNote = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Action A | X Minutes\r\nAction B | Y Minutes\r\nAction C | Z Minutes",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
		15, 45, 380, 300, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

	//Generate
	CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Generate", WS_CHILD | WS_VISIBLE,
		400, 20, 70, 50, hwnd, (HMENU)ID_INPROGRESS, GetModuleHandle(NULL), NULL);

	//Hours
	CreateWindowEx(WS_EX_CLIENTEDGE, "Static", " Hours ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		400, 75, 70, 25, hwnd, (HMENU)ID_INPROGRESS, GetModuleHandle(NULL), NULL);
	hHour = CreateWindowEx(WS_EX_CLIENTEDGE, "Static", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER,
		400, 100, 70, 50, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
	//Minutes
	CreateWindowEx(WS_EX_CLIENTEDGE, "Static", " Minutes ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		400, 155, 70, 25, hwnd, (HMENU)ID_INPROGRESS, GetModuleHandle(NULL), NULL);
	hMin = CreateWindowEx(WS_EX_CLIENTEDGE, "Static", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER,
		400, 180, 70, 50, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

	//Scrubber, Calculator, Copy to ClipBoard
	CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Clean, Calc, and Copy", WS_CHILD | WS_VISIBLE,
		15, 360, 440, 50, hwnd, (HMENU)ID_CALC, GetModuleHandle(NULL), NULL);
}
