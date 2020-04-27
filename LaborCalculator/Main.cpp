#include <windows.h>


#define IDC_MAIN_EDIT 101


//Global Entities
const char g_szClassName[] = "mainWindow";
const char g_WindowTitle[] = "Labor Calculator V0.0.0";
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
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, g_WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 600, NULL, NULL, hInstance, NULL);

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
	AppendMenu(hFileMenu, MF_STRING, NULL, "Add to Ignore List");
	AppendMenu(hFileMenu, MF_STRING, NULL, "Clear Ignore List");
	AppendMenu(hFileMenu, MF_STRING, NULL, "Reset Ignore List");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, NULL, "Exit");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hFileMenu, "File");
	//Settings Menu
	hSettingsMenu = CreatePopupMenu();
	AppendMenu(hSettingsMenu, MF_STRING, NULL, "Settings");
	AppendMenu(hSettingsMenu, MF_STRING, NULL, "AI Generator");
	AppendMenu(hSettingsMenu, MF_STRING, NULL, "Feed AI");
	AppendMenu(hSettingsMenu, MF_STRING, NULL, "Train AI");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSettingsMenu, "Generator");


	//Remaining Main Menu Items
	AppendMenu(hMenu, MF_STRING, NULL, "About");
	AppendMenu(hMenu, MF_STRING, NULL, "Help");

	SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd)
{


	hNote = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Overview | Xmin\nCalls | Ymin\nNotes | Zmin",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
		15, 210, 450, 200, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

	//Scrubber, Calculator, Copy to ClipBoard
	CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Clean, Calc, and Copy", WS_CHILD | WS_VISIBLE,
		25, 470, 200, 50, hwnd, NULL, GetModuleHandle(NULL), NULL);

	//Generate
	CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Generate", WS_CHILD | WS_VISIBLE,
		255, 470, 200, 50, hwnd, NULL, GetModuleHandle(NULL), NULL);
}