#include<Windows.h>
#include<d3d9.h>
#include<time.h>
#include<iostream>

using namespace std;
#pragma comment(lib,"d3d9.lib")
//#pragma comment(lib,"d3dx9.lib")

const string APPTITLE = "Create Surface Program";

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0 )

#define SCREENW 1024
#define SCREENH 768

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPDIRECT3DSURFACE9 surface = NULL;

bool gameover = false;

bool Game_Init(HWND hWnd) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) {
		MessageBox(hWnd, "Error initializing Direct3D", "Error", MB_OK);
		return false;
	}
	
	//initializing d3dpresent_params;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREENW;
	d3dpp.BackBufferHeight = SCREENH;
	d3dpp.hDeviceWindow = hWnd;

	//create d3d device
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (!d3ddev) {
		MessageBox(hWnd, "Error creating Direct3D device", "Error", MB_OK);
		return false;
	}

	srand((unsigned int)time(NULL));

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	HRESULT result = d3ddev->CreateOffscreenPlainSurface(100, 100, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (result != D3D_OK) return false;
	return true;
}

void Game_Run(HWND hWnd) {
	if (!d3ddev) return;

	if (d3ddev->BeginScene()) {
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		d3ddev->ColorFill(surface, NULL, D3DCOLOR_XRGB(r, g, b));

		//copy the surface to backbuffer
		RECT rect;
		rect.left = rand() % SCREENW / 2;
		rect.right = rect.left + rand() % SCREENW / 2;
		rect.top = rand() % SCREENH / 2;
		rect.bottom = rect.top + rand() % SCREENH / 2;
		d3ddev->StretchRect(surface, NULL, backbuffer, &rect, D3DTEXF_NONE);
		//stop rendering 
		d3ddev->EndScene();
		//copy the backbuffer to the screen 
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
	//check for escape key
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hWnd, WM_DESTROY, 0, 0);
}

void Game_End(HWND hWnd){
	if (surface) surface->Release();
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MainClassName";
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);
	//create a new window
	HWND window = CreateWindow("MainClassName", APPTITLE.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREENW, SCREENH, NULL, NULL, hInstance, NULL);

	if (window == 0) return 0;
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	if (!Game_Init(window)) return 0;

	MSG message;
	while (!gameover) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		Game_Run(window);
	}
	return message.wParam;

}