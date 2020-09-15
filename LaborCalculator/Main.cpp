#pragma once
#include <windows.h>
#include "Generator.h"
#include "NoteParser.h"


#define IDC_MAIN_EDIT 101
#define ID_FILE_EXIT 9001
#define ID_ABOUT 9002
#define ID_HELP 9003
#define ID_CALC 9004
#define ID_OPEN_IGNORE_LIST 9005
#define ID_GENERATE 9006
#define ID_OPENGENSET 9007
#define ID_GENSETHELP 9008
#define ID_INPROGRESS 9020


//Global Entities
const char g_szClassName[] = "mainWindow";
const char g_WindowTitle[] = "Labor Calculator V0.0.46";
NoteParser g_Crafter;
Generator g_Generator;
HWND hMainWindow, hBanner, hGenWindow, hNote, hHour, hLocalHour, hMin;
RECT rcMain;
HBITMAP hBannerImage;



//Global Funcs
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void AddMenu(HWND hwnd);
void AddControls(HWND hwnd);
void RegisterGeneratorWindow(HINSTANCE hInst);
void OpenGeneratorWindow(HWND hWnd);
LRESULT CALLBACK GenWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
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

	RegisterGeneratorWindow(hInstance);


	hMainWindow = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, g_WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 520, NULL, NULL, hInstance, NULL);

	if (hMainWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

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
		case ID_OPEN_IGNORE_LIST:
			ShellExecute(hwnd, "open", g_Crafter.GetIgnoreLoc().c_str(), NULL, NULL, SW_SHOW);
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
		case ID_GENERATE:
			OpenGeneratorWindow(hwnd);
			SetFocus(hLocalHour);
			break;
		case ID_OPENGENSET:
			if (MessageBox(hwnd, "No UI is available for Generator Settings. This will directly open the Generator.txt configuration file.\nThis file is only used while the AI generator is inactive.\n\nAre you sure your want to enter?", "Wait", 
				MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK) {
				ShellExecute(hwnd, "open", g_Generator.GetGenSettingsLoc().c_str(), NULL, NULL, SW_SHOW);
			}
			break;
		case ID_GENSETHELP:
			MessageBox(hwnd, "Tips for altering the generator's configuration:\n\n>Everything after \";;\" is ignored for commenting purposes.\n\n>Each labor \"entry\" does not need to be followed by all six numbers, but must be followed by one. For the Generator to create a bank entry, simply enter 0.\n\n>The first two numbers are the base labor amount and the random range to be added.\n\n>Next is an hourly random increase.\n\n>The last three are a bonus hour mark, a base bonus addition, and a random bonus addition.", "Manual",
				MB_OK);
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
	AppendMenu(hFileMenu, MF_STRING, ID_OPEN_IGNORE_LIST, "Open Ignore List");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, "Exit");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hFileMenu, "File ");
	//Settings Menu
	hSettingsMenu = CreatePopupMenu();
	AppendMenu(hSettingsMenu, MF_STRING, ID_INPROGRESS, "Main Settings");
	AppendMenu(hSettingsMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hSettingsMenu, MF_STRING, ID_OPENGENSET, "Generator");
	AppendMenu(hSettingsMenu, MF_STRING, ID_GENSETHELP, "Configuration Help");
	AppendMenu(hSettingsMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hSettingsMenu, MF_STRING, ID_INPROGRESS, "AI Generator");

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSettingsMenu, "Settings");


	//Remaining Main Menu Items1e1e1e
	AppendMenu(hMenu, MF_STRING, ID_ABOUT, "About");
	AppendMenu(hMenu, MF_STRING, ID_HELP, "Help");

	SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd)
{

	//Logo and Title --------------------------------------------------
	hBannerImage = (HBITMAP)LoadImage(NULL, "Resources\\LC Banner.bmp", IMAGE_BITMAP, 480, 50, LR_LOADFROMFILE);
	if (hBannerImage == NULL) { MessageBox(hwnd, "Could not load Logo!", "Error", MB_OK | MB_ICONEXCLAMATION); }
	hBanner = CreateWindowEx(NULL, "static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 0, 0, 480, 50, hwnd, NULL, NULL, NULL);
	if (hBanner == NULL) { MessageBox(hwnd, "Logo window creation failed!", "Error", MB_OK | MB_ICONEXCLAMATION); }
	SendMessage(hBanner, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBannerImage);

	//Main Controls 

	//Notes
	CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", " Labor Notes ", WS_CHILD | WS_VISIBLE,
		15, 45, 380, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
	hNote = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Action A | X Minutes\r\nAction B | Y Minutes\r\nAction C | Z Minutes",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
		15, 70, 380, 300, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

	//Generate
	CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Generate", WS_CHILD | WS_VISIBLE,
		400, 45, 70, 50, hwnd, (HMENU)ID_GENERATE, GetModuleHandle(NULL), NULL);

	//Hours
	CreateWindowEx(WS_EX_CLIENTEDGE, "Static", " Hours ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		400, 100, 70, 25, hwnd, (HMENU)ID_INPROGRESS, GetModuleHandle(NULL), NULL);
	hHour = CreateWindowEx(WS_EX_CLIENTEDGE, "Static", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER,
		400, 125, 70, 50, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
	//Minutes
	CreateWindowEx(WS_EX_CLIENTEDGE, "Static", " Minutes ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		400, 180, 70, 25, hwnd, (HMENU)ID_INPROGRESS, GetModuleHandle(NULL), NULL);
	hMin = CreateWindowEx(WS_EX_CLIENTEDGE, "Static", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER,
		400, 205, 70, 50, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

	//Scrubber, Calculator, Copy to ClipBoard
	CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Clean, Calc, and Copy", WS_CHILD | WS_VISIBLE,
		15, 385, 440, 50, hwnd, (HMENU)ID_CALC, GetModuleHandle(NULL), NULL);
}

void RegisterGeneratorWindow(HINSTANCE hInst){
	WNDCLASSEX genWin = { 0 };

	genWin.cbSize = sizeof(WNDCLASSEX);
	genWin.style = 0;
	genWin.lpfnWndProc = GenWinProc;
	genWin.cbClsExtra = 0;
	genWin.cbWndExtra = 0;
	genWin.hInstance = hInst;
	genWin.hCursor = LoadCursor(NULL, IDC_ARROW);
	genWin.hbrBackground = (HBRUSH)COLOR_WINDOW;
	genWin.lpszMenuName = NULL;
	genWin.lpszClassName = "myGeneratorWindow";
	genWin.hIconSm = (HICON)LoadImage(hInst, "Resources\\LC Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

	RegisterClassEx(&genWin);
}

void OpenGeneratorWindow(HWND hWnd){

	GetWindowRect(hMainWindow, &rcMain);

	hGenWindow = CreateWindowEx(WS_EX_CLIENTEDGE, "myGeneratorWindow", "Hours", WS_VISIBLE | WS_OVERLAPPEDWINDOW, rcMain.left+140, rcMain.top, 200, 200, hWnd, NULL, NULL, NULL);
	if (hGenWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	CreateWindowEx(NULL, "Static", " Hours Owned", WS_CHILD | WS_VISIBLE | SS_CENTER,
		40, 15, 100, 25, hGenWindow, (HMENU)ID_INPROGRESS, GetModuleHandle(NULL), NULL);
	hLocalHour = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER,
		65, 40, 50, 50, hGenWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Ok", WS_VISIBLE | WS_CHILD, 15, 100, 75, 50, hGenWindow, (HMENU)IDOK, NULL, NULL);
	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Cancel", WS_VISIBLE | WS_CHILD, 100, 100, 75, 50, hGenWindow, (HMENU)IDCANCEL, NULL, NULL);

	//Disable the main window, turning a Modless dialogue box into a modal dialogue
	EnableWindow(hWnd, false);
}

LRESULT CALLBACK GenWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		switch (wp)
		{
		case IDCANCEL:
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		case IDOK:
			char lHourC[10] = "";
			GetWindowText(hLocalHour, lHourC, 10);
			std::string lHours = lHourC;
			//All non-numbers to whitespace (Excluding Demcimals, which Stoul ignores anyway)
			for (unsigned i = 0; i < lHours.size(); i++) {
				if ((lHours[i] < 48 || lHours[i] > 57) && lHours[i] != 46) {
					lHours[i] = 32;
				}
			}
			//Remove Whitespace
			while (lHours.find(" ") != std::string::npos) {
				lHours.erase(lHours.find(" "), 1);
			}
			//Remove Double Decimals
			while (lHours.find("..") != std::string::npos) {
				lHours.erase(lHours.find(".."), 1);
			}
			unsigned setHours = 0;
			if (!lHours.empty()) {
				//Handle first decimal
				if (lHours.find(".") != 0) {
					setHours = std::stoul(lHours);
				}
				//Handle after Decimal
				if (lHours.find(".") != std::string::npos && lHours.find(".") != lHours.size() - 1) {
					if (lHours[lHours.find(".") + 1] >= 53) {
						setHours++;
					}
				}
			}
			SetWindowText(hNote, g_Generator.GenerateLabor(setHours).c_str());
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_CLOSE:
		EnableWindow(hMainWindow, true);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}