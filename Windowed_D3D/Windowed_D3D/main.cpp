#include<Windows.h>
#include<d3d9.h>
#include<iostream>
#include<time.h>
using namespace std;
#pragma comment(lib,"d3d9.lib")

const string APPTITLE = "Windowed D3D";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
bool gameover = false;

#define KEY_DOWN(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000 ) ? 1 : 0 )


bool Game_Init(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		MessageBox(hWnd, "Error initializing Direct3D", "Error", MB_OK);
		return FALSE;
	}
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE dm;
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = dm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = dm.Width;
	d3dpp.BackBufferHeight = dm.Height;
	d3dpp.hDeviceWindow = hWnd;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	if (d3ddev == NULL)
	{
		MessageBox(hWnd, "Error creating Direct3D device", "Error", MB_OK);
		return FALSE;
	}
	return TRUE;
}


void Game_Run(HWND hWnd) {
	if (!d3ddev) return;
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (d3ddev->BeginScene()) {
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL,NULL);
	}
	if (KEY_DOWN(VK_ESCAPE)) {
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
}

void Game_End(HWND hWnd)
{
	if (d3ddev)
	{
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d)
	{
		d3d->Release();
		d3d = NULL;
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;// CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = 0;//(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MainWindowClass";
	wc.hIconSm = NULL;
	if (!RegisterClassEx(&wc))
		return FALSE;
	MSG msg;

	HWND hWnd = CreateWindow(
		"MainWindowClass",
		APPTITLE.c_str(),
		WS_EX_TOPMOST|WS_POPUP,0,0,
		SCREENW, SCREENH,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);
	if (hWnd == 0) return 0;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd)) return 0;

	while (!gameover)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Run(hWnd);
		//_sleep(500);
	}
	Game_End(hWnd);
	return msg.wParam;
}