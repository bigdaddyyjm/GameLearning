#include"MyDirectX.h"
const string APPTITLE = "BombCatcher";
const int SCREENW = 1024;
const int SCREENH = 768;
LPDIRECT3DSURFACE9 bomb_surf = NULL;
LPDIRECT3DSURFACE9 bucket_surf = NULL;
struct BOMB {
	float x, y;
	void reset() {
		x = (float)(rand() % (SCREENW - 128));
		y = 0;
	}
};
BOMB bomb;

struct BUCKET {
	float x, y;
};
BUCKET bucket;

int score = 0;
int vibrating = 0;
bool Game_Init(HWND window) {
	Direct3D_Init(window, SCREENW, SCREENH, false);
	DirectInput_Init(window);
	bomb_surf = LoadSurface("bomb.bmp");
	if (!bomb_surf) {
		MessageBox(window, "Error Loading Bomb", "Error", 0);
		return false;
	}
	bucket_surf = LoadSurface("bucket.bmp");
	if (!bucket_surf) {
		MessageBox(window, "Error Loading Bucket", "Error", 0);
		return false;
	}
	//get the back buffer surface
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	//position the bomb
	srand((unsigned int)time(NULL));
	bomb.reset();
	//position the bucket
	bucket.x = 500;
	bucket.y = 630;
	return true;
}

void Game_Run(HWND window) {
	//make sure the d3d device id valid
	if (!d3ddev) return;
	//update input devices
	DirectInput_Update();
	//move the bomb down the screen 
	bomb.y += 2.0f;
	//see if bomb hit the floor
	if (bomb.y > SCREENH) {
		MessageBox(window, "Oh,no,the bomb exploded", "YOU SUCKED", 0);
		gameover = true;
	}
	//move the bucket with mouse
	int mx = Mouse_X();
	if (mx < 0)
		bucket.x -= 6.0f;
	else if (mx > 0)
		bucket.y -= 6.0f;

	//move the bucket with the keyboard
	if (Key_Down(DIK_LEFT))
		bucket.x -= 6.0f;
	else if (Key_Down(DIK_RIGHT))
		bucket.y += 6.0f;

	//move the bucket with the controller
}