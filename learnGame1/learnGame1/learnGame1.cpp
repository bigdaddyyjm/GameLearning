#include<Windows.h>
#include<iostream>
#include<time.h>
using namespace std;
const string APPTITLE = "Game Loop";

HWND window;
HDC device;
bool gameover = false;

void DrawBitmap(char *filename, int x, int y) 
{
	HBITMAP image = (HBITMAP)LoadImage(0, "c.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	BITMAP bm;
	GetObject(image, sizeof(BITMAP), &bm);
	HDC hdcImage = CreateCompatibleDC(device);
	SelectObject(hdcImage, image);
	BitBlt(
		device,
		x, y,
		bm.bmWidth, bm.bmHeight,
		hdcImage,
		0, 0,
		SRCCOPY
	);
	DeleteDC(hdcImage);
	DeleteObject((HBITMAP)image);
}

bool Game_Init()
{
	srand(time(NULL));
	return 1;
}

void Game_Run()
{
	if (gameover == true) return;
	RECT rect;
	GetClientRect(window, &rect);
	int x = rand() % (rect.right - rect.left);
	int y = rand() % (rect.bottom - rect.top);
	BitBlt(
		device,
		0, 0,
		rect.right,rect.bottom,
		NULL,
		0, 0,
		BLACKNESS
	);
	DrawBitmap("c.bmp", x, y);
}

void Game_End()
{
	ReleaseDC(window, device);
}


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) 
	{
		case WM_DESTROY:
			gameover = true;
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE.c_str();
	wc.hIconSm = NULL;
	return RegisterClassEx(&wc);
}

bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	window = CreateWindow(
		APPTITLE.c_str(),
		APPTITLE.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (window == 0) return 0;
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);
	device = GetDC(window);
	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;

	cout << window << endl;

	if (!Game_Init()) return 0;

	while (!gameover)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run();
		_sleep(500);
	}
	Game_End();
	return msg.wParam;
}