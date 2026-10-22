#include <iostream>
#include <algorithm>
#include <fstream>
#include "DxLib.h"
#include "tchar.h"
#include <vector>
#include <string>


//
//ここで変数を用意
//

constexpr int MAP_WIDTH = 10;
constexpr int MAP_HEIGHT = 10;
constexpr int MAP_LIST_X = 3;
constexpr int MAP_LIST_Y = 3;
constexpr int MAP_LIST_SIZE = 2;
constexpr int MAP_WIDTH_X = 150;
constexpr double PI = 3.14;

std::vector<std::string> logs;
bool win_sound_played = false;

void Log(const std::string& text)
{
    logs.push_back(text);

    // 最大10行
    if (logs.size() > 10)
    {
        logs.erase(logs.begin());
    }
}

void printMap(int map[MAP_LIST_SIZE][MAP_LIST_Y][MAP_LIST_X][MAP_HEIGHT][MAP_WIDTH],
	int map_list_size,
	bool key_3,
	bool key_5,
	int map_player_x,
	int map_player_y,
	bool player_win,
	int player_x,
	int player_y,
	int pix_0,
	int pix_1,
	int pix_2,
	int pix_3,
	int pix_4,
	int pix_5,
	int pix_6,
	int pix_7,
	int pix_p,
	int Rad,
	int pix_win,
	int se[3]) {

	//
	//ここに毎フレーム呼ぶ処理を書く
	//

	if (player_win == true)
	{
		DrawGraph(32,32, pix_win, false);
		
		if (win_sound_played == false)
		{
			PlaySoundMem(se[2], DX_PLAYTYPE_BACK);
			win_sound_played = true;
		}
	}
	else
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				if (x == player_x && y == player_y)
				{
					DrawRotaGraph(x * 64 + MAP_WIDTH_X + 32, y * 64 + 32, 1.0f, PI / 180 * Rad, pix_p, false, false);
				}
				else
				{
					if (map[map_list_size][map_player_y][map_player_x][y][x] == 1)
					{
						DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_1, false);
					}
					if (map[map_list_size][map_player_y][map_player_x][y][x] == 2)
					{
						DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_2, false);
					}
					if (map[map_list_size][map_player_y][map_player_x][y][x] == 3)
					{
						if (key_3 == true)
						{
							DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_0, false);
						}
						else 
						{
							DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_3, false);
						}
					}
					if (map[map_list_size][map_player_y][map_player_x][y][x] == 4)
					{
						DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_4, false);
					}
					if (map[map_list_size][map_player_y][map_player_x][y][x] == 5)
					{
						if (key_5 == true)
						{
							DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_0, false);
						}
						else 
						{
							DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_5, false);
						}
					}
					if (map[map_list_size][map_player_y][map_player_x][y][x] == 6)
					{
						DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_6, false);
					}
					if (map[map_list_size][map_player_y][map_player_x][y][x] == 7)
					{
						DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_7, false);
					}
					else if (map[map_list_size][map_player_y][map_player_x][y][x] == 0)
					{
						DrawGraph(x * 64 + MAP_WIDTH_X, y * 64, pix_0, false);
					}
				}
				
			}
			
		}

		
		DrawFormatString(10, 10, GetColor(0, 0, 0), _T("map(%d)"), map_list_size + 1);
		for (int i = 0; i < logs.size(); i++)
		{
			DrawString(
				10,
				40 + i * 30,
				logs[i].c_str(),
				GetColor(0, 0, 0)
			);
			
		}
	}
}

int map[MAP_LIST_SIZE][MAP_LIST_Y][MAP_LIST_X][MAP_HEIGHT][MAP_WIDTH];

int pix_0;
int pix_1;
int pix_2;
int pix_3;
int pix_4;
int pix_5;
int pix_6;
int pix_7;
int pix_p;
int pix_win;

int Rad;

int bgm[MAP_LIST_SIZE];
int se[3];

int player_x = 0;
int player_y = 1;
int map_player_x = 0;
int map_player_y = 0;
bool player_win = false;

int map_list_size = 0;

bool key_3 = false;
bool key_5 = false;

bool door_4 = false;
bool door_6 = false;

int old_w;
int old_s;
int old_a;
int old_d;
int old_q;

int w;
int s;
int a;
int d;
int q;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定
	SetBackgroundColor(255, 255, 255);
	SetFontSize(24);
	ChangeWindowMode(TRUE);//非全画面にセット
	SetGraphMode(790, 640, 32);//画面サイズ指定
	SetWindowSizeExtendRate(1.0);
	if (DxLib_Init() == 1){ return -1; }//初期化に失敗時にエラーを吐かせて終了


	std::ifstream map_date("map_gui.map");

	if (!map_date)
	{
		std::cout << "Not map.map file.\n";
		return 1;
	}

	for (int size = 0; size < MAP_LIST_SIZE; size++) {
		for (int y = 0; y < MAP_LIST_Y; y++) {
			for (int x = 0; x < MAP_LIST_X; x++) {
				for (int height = 0; height < MAP_HEIGHT; height++) {
					for (int width = 0; width < MAP_WIDTH; width++) {
						map_date >> map[size][y][x][height][width];
					}
				}
			}
		}
	}

	//
	//ここで画像・音を読み込み
	//

    pix_0 = LoadGraph(_T("jpg\\0.jpg"));
	pix_1 = LoadGraph(_T("jpg\\1.jpg"));
	pix_2 = LoadGraph(_T("jpg\\2.jpg"));
	pix_3 = LoadGraph(_T("jpg\\3.jpg"));
	pix_4 = LoadGraph(_T("jpg\\4.jpg"));
	pix_5 = LoadGraph(_T("jpg\\5.jpg"));
	pix_6 = LoadGraph(_T("jpg\\6.jpg"));
	pix_7 = LoadGraph(_T("jpg\\7.jpg"));
	pix_p = LoadGraph(_T("jpg\\p.jpg"));
	pix_win = LoadGraph(_T("jpg\\win.jpg"));

	Rad = 0;

	bgm[0] = LoadSoundMem(_T("bgm\\bgm1.mp3"));
	bgm[1] = LoadSoundMem(_T("bgm\\bgm2.mp3"));

	se[0] = LoadSoundMem(_T("bgm\\key_get.wav"));
	se[1] = LoadSoundMem(_T("bgm\\erabu.wav"));
	se[2] = LoadSoundMem(_T("bgm\\win.mp3"));

	PlaySoundMem(bgm[0], DX_PLAYTYPE_LOOP);

	while (ProcessMessage() == 0) {
		ClearDrawScreen();//裏画面消す
		SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に

		if (key_3 == true)
		{
			door_4 = true;
		}
		if (key_5 == true)
		{
			door_6 = true;
		}

		w = CheckHitKey(KEY_INPUT_W);
		s = CheckHitKey(KEY_INPUT_S);
		a = CheckHitKey(KEY_INPUT_A);
		d = CheckHitKey(KEY_INPUT_D);
		q = CheckHitKey(KEY_INPUT_Q);

		if (w && !old_w)
		{
			Rad = 0;
			int next_x = player_x;
			int next_y = player_y - 1;

			int next_map_player_x = map_player_x;
			int next_map_player_y = map_player_y;

			if (next_y < 0) {

				if (next_map_player_y > 0) {
					next_map_player_y--;
					next_y = MAP_HEIGHT - 1;
					PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
				}
			}

			int tile = map[map_list_size][next_map_player_y][next_map_player_x][next_y][next_x];

			bool canMove = false;

			if (tile == 0) {
				canMove = true;
			}
			else if (tile == 2) {
				canMove = true;
			}
			else if (tile == 3) {
				canMove = true;
			}
			else if (tile == 5) {
				canMove = true;
			}else if (tile == 7) {
				canMove = true;
			}
			else if (tile == 7) {
				canMove = true;
			}
			else if (tile == 4 && door_4 == true) {
				canMove = true;
			}
			else if (tile == 6 && door_6 == true) {
				canMove = true;
			}

			if (canMove) {
				player_x = next_x;
				player_y = next_y;

				map_player_x = next_map_player_x;
				map_player_y = next_map_player_y;
			}

			if (map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 2) {
				player_win = true;
				DeleteSoundMem(bgm[map_list_size]);
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 3) {
				if (key_3 == false)
				{
					key_3 = true;
					Log("Key_3 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 5) {
				if (key_5 == false)
				{
					key_5 = true;
					Log("Key_5 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
				
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 7) {
				StopSoundMem(bgm[map_list_size]);

				map_list_size += 1;

				PlaySoundMem(bgm[map_list_size], DX_PLAYTYPE_LOOP);
				player_x = 0;
				player_y = 1;
				map_player_x = 0;
				map_player_y = 0;

				key_3 = false;
				key_5 = false;

				door_4 = false;
				door_6 = false;

				logs.clear();
			}
		}
		else if (s && !old_s)
		{
			Rad = 180;
			int next_x = player_x;
			int next_y = player_y + 1;

			int next_map_player_x = map_player_x;
			int next_map_player_y = map_player_y;

			if (next_y >= MAP_HEIGHT) {

					if (next_map_player_y < MAP_LIST_Y - 1) {
						next_map_player_y++;
						next_y = 0;
						PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
					}
			}

			int tile = map[map_list_size][next_map_player_y][next_map_player_x][next_y][next_x];

			bool canMove = false;

			if (tile == 0) {
				canMove = true;
			}
			else if (tile == 2) {
				canMove = true;
			}
			else if (tile == 3) {
				canMove = true;
			}
			else if (tile == 5) {
				canMove = true;
			}else if (tile == 7) {
				canMove = true;
			}
			else if (tile == 4 && door_4 == true) {
				canMove = true;
			}
			else if (tile == 6 && door_6 == true) {
				canMove = true;
			}

			if (canMove) {
				player_x = next_x;
				player_y = next_y;

				map_player_x = next_map_player_x;
				map_player_y = next_map_player_y;
			}

			if (map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 2) {
				player_win = true;
				DeleteSoundMem(bgm[map_list_size]);
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 3) {
				if (key_3 == false)
				{
					key_3 = true;
					Log("Key_3 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 5) {
				if (key_5 == false)
				{
					key_5 = true;
					Log("Key_5 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
				
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 7) {
				StopSoundMem(bgm[map_list_size]);

				map_list_size += 1;

				PlaySoundMem(bgm[map_list_size], DX_PLAYTYPE_LOOP);
				player_x = 0;
				player_y = 1;
				map_player_x = 0;
				map_player_y = 0;

				key_3 = false;
				key_5 = false;

				door_4 = false;
				door_6 = false;

				logs.clear();
			}
		}
		else if (a && !old_a)
		{
			Rad = -90;
			int next_x = player_x - 1;
			int next_y = player_y;

			int next_map_player_x = map_player_x;
			int next_map_player_y = map_player_y;

			if (next_x < 0) {

				if (next_map_player_x > 0) {
					next_map_player_x--;
					next_x = MAP_WIDTH - 1;
					PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
				}
			}

			int tile = map[map_list_size][next_map_player_y][next_map_player_x][next_y][next_x];

			bool canMove = false;

			if (tile == 0) {
				canMove = true;
			}
			else if (tile == 2) {
				canMove = true;
			}
			else if (tile == 3) {
				canMove = true;
			}
			else if (tile == 5) {
				canMove = true;
			}else if (tile == 7) {
				canMove = true;
			}
			else if (tile == 4 && door_4 == true) {
				canMove = true;
			}
			else if (tile == 6 && door_6 == true) {
				canMove = true;
			}

			if (canMove) {
				player_x = next_x;
				player_y = next_y;

				map_player_x = next_map_player_x;
				map_player_y = next_map_player_y;
			}

			if (map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 2) {
				player_win = true;
				DeleteSoundMem(bgm[map_list_size]);
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 3) {
				if (key_3 == false)
				{
					key_3 = true;
					Log("Key_3 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 5) {
				if (key_5 == false)
				{
					key_5 = true;
					Log("Key_5 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
				
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 7) {
				StopSoundMem(bgm[map_list_size]);

				map_list_size += 1;

				PlaySoundMem(bgm[map_list_size], DX_PLAYTYPE_LOOP);
				player_x = 0;
				player_y = 1;
				map_player_x = 0;
				map_player_y = 0;

				key_3 = false;
				key_5 = false;

				door_4 = false;
				door_6 = false;

				logs.clear();
			}
		}
		else if (d && !old_d)
		{
			Rad = 90;
			int next_x = player_x + 1;
			int next_y = player_y;

			int next_map_player_x = map_player_x;
			int next_map_player_y = map_player_y;

			if (next_x >= MAP_WIDTH) {

				if (next_map_player_x < MAP_LIST_X - 1) {
					next_map_player_x++;
					next_x = 0;
					PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
				}
			}

			int tile = map[map_list_size][next_map_player_y][next_map_player_x][next_y][next_x];

			bool canMove = false;

			if (tile == 0) {
				canMove = true;
			}
			else if (tile == 2) {
				canMove = true;
			}
			else if (tile == 3) {
				canMove = true;
			}
			else if (tile == 5) {
				canMove = true;
			}else if (tile == 7) {
				canMove = true;
			}
			else if (tile == 4 && door_4 == true) {
				canMove = true;
			}
			else if (tile == 6 && door_6 == true) {
				canMove = true;
			}

			if (canMove) {
				player_x = next_x;
				player_y = next_y;

				map_player_x = next_map_player_x;
				map_player_y = next_map_player_y;
			}
			
			if (map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 2) {
				player_win = true;
				DeleteSoundMem(bgm[map_list_size]);
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 3) {
				if (key_3 == false)
				{
					key_3 = true;
					Log("Key_3 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 5) {
				if (key_5 == false)
				{
					key_5 = true;
					Log("Key_5 Get");
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				}
				
			}else if(map[map_list_size][map_player_y][map_player_x][player_y][player_x] == 7) {
				StopSoundMem(bgm[map_list_size]);

				map_list_size += 1;

				PlaySoundMem(bgm[map_list_size], DX_PLAYTYPE_LOOP);
				player_x = 0;
				player_y = 1;
				map_player_x = 0;
				map_player_y = 0;

				key_3 = false;
				key_5 = false;

				door_4 = false;
				door_6 = false;

				logs.clear();
			}
		}
		else if (q && !old_q)
		{
			break;
		}

		old_w = w;
		old_s = s;
		old_a = a;
		old_d = d;
		old_q = q;

		player_x = std::clamp(player_x, 0, MAP_WIDTH - 1);
		player_y = std::clamp(player_y, 0, MAP_HEIGHT - 1);
		map_player_x = std::clamp(map_player_x, 0, MAP_LIST_X - 1);
		map_player_y = std::clamp(map_player_y, 0, MAP_LIST_Y - 1);

		printMap(map, map_list_size, key_3, key_5, map_player_x, map_player_y, player_win, player_x, player_y, pix_0, pix_1, pix_2, pix_3, pix_4, pix_5, pix_6, pix_7, pix_p, Rad, pix_win, se);

		ScreenFlip();//裏画面を表画面にコピー
	}

	DxLib_End();
	return 0;
}
