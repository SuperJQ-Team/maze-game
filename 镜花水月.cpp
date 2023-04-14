/*
*	 By Arnock、Bromine
*
*	 2023/04/09
*/

#include <iostream>
#include <stdio.h>
#include <vector>
#include <stack>
#include <string>
#include <windows.h>
#include <conio.h>
using namespace std;

#define KEYDOWN(vk_code) ((GetKeyState(vk_code) & 0x8000) ? 1 : 0)
#define RAND(min,max) ((rand()%((min)-(max)+1))+(min))
#define MAY(x) (RAND(0,10000)<=(x)*100)
#define elif else if

#define down 1
#define right 2
#define left 4
#define up 8
#define WALL -1
#define NOTHING 2
#define FINDED 3
#define FINDBACK 5
#define MONSTER 6
//#define Starting_point 6
#define Destination 7
#define ESC 27
#define ENTER 13
#define SPACE 32
#define up_key 57416
#define down_key 57424
#define left_key 57419
#define right_key 57421
#define maxViewLen 10
#define maxViewWidth 100
#define MAXHEIGHT 47
#define MAXWIDTH 93
#define screenWidth (MAXWIDTH + 3)
#define screenHafeHeight  (MAXHEIGHT / 2)

const double Pi = 3.1415926535897;
struct block
{
	int row, column, direction;
	block(int _row, int _column, int _direction)
	{
		row = _row;
		column = _column;
		direction = _direction;
	}
};
struct point
{
	int x;
	int y;
	point()
	{
		x = y = 0;
	}
	point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
}player[2], last_player[2], camera, monster[102];

bool playerlive;
int playernum;
int monster_num = 0;
vector<block> myblock;
int x_num = 1, y_num = 1;//矿工位置
int G[1002][1002];
int n = 0, m = 0;//地图大小
point model;	//模式、人数
int font = 48;	//字号大小
int dir[2] = { down, down };	//朝向
const int step[8] = { -1,0,1,0,0,-1,0,1 };
const int stepdir[4] = { up,left,down,right };
const int difficulty[5][2] = {
	{7,7},
	{11,11},
	{21,21},
	{31,41},
	{MAXHEIGHT,MAXWIDTH},
};

const int monster_size = 50;
const bool monster_image[monster_size + 5][monster_size + 5]
= { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
};

void gotoxy(int x = 0, int y = 0)
{//移动光标
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void HideCursor(int n = 0)
{//隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = { 1,n };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void WindowMax()
{//窗体最大化
	HANDLE HOUT = GetStdHandle(STD_OUTPUT_HANDLE);    //获得控制台句柄
	COORD NewSize = GetLargestConsoleWindowSize(HOUT);//获得控制台最大坐标，坐标以字符数为单位
	NewSize.X -= 1;
	NewSize.Y -= 1;    //可以不加，不加时显示滚动条
	SetConsoleScreenBufferSize(HOUT, NewSize); //设置控制台缓冲区大小

	SMALL_RECT DisplayArea = { 0,0,0,0 };
	DisplayArea.Right = NewSize.X;
	DisplayArea.Bottom = NewSize.Y;
	SetConsoleWindowInfo(HOUT, TRUE, &DisplayArea);    //设置控制台大小

	//控制台已经最大化，但是初始位置不在屏幕左上角，添加如下代码
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_MAXIMIZE);    //窗体最大化
}

void rgb_init()
{//设置RGB初始化
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);		//输入句柄
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);		//输出句柄
	DWORD dwInMode, dwOutMode;
	GetConsoleMode(hIn, &dwInMode);						//获取控制台输入模式
	GetConsoleMode(hOut, &dwOutMode);					//获取控制台输出模式
	dwInMode |= 0x0200;									//更改
	dwOutMode |= 0x0004;
	SetConsoleMode(hIn, dwInMode);						//设置控制台输入模式
	SetConsoleMode(hOut, dwOutMode);					//设置控制台输出模式
}

void inline rgb_set(int wr, int wg, int wb, int br, int bg, int bb)
{//设置RGB
	printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm", wr, wg, wb, br, bg, bb);	//\033[38表示前景，\033[48表示背景，三个%d表示混合的数
}

int SleepAndClear(long long i)
{
	int key = -1;
	long long t = GetTickCount64();
	while (GetTickCount64() - t < i)
	{
		key = _getch();
		if (key == 0 || key == 0xE0) key = key << 8 | _getch();
	}
	return key;
}

void inline outputWithType(int type)
{//根据内容输出
	if (type == NOTHING || type == FINDBACK)
	{
		//rgb_set(255, 255, 255, 0, 0, 0);
		printf("  ");
	}
	else if (type == Destination)
	{
		rgb_set(0, 125, 0, 0, 0, 0);
		printf("■");
		rgb_set(204, 204, 204, 0, 0, 0);
	}
	else if (type == FINDED)
	{
		//rgb_set(255, 255, 0, 0, 0, 0);
		printf("**");
	}
	else if (type == MONSTER)
	{
		rgb_set(204, 0, 0, 0, 0, 0);
		printf("@@");
		rgb_set(204, 204, 204, 0, 0, 0);
	}
	else
	{
		//rgb_set(255, 255, 255, 255, 255, 255);
		printf("■");
		//rgb_set(255, 255, 255, 0, 0, 0);
	}
}

void inline output(int i, int j)
{//输出G[i][j]
	if (model.y == 1 && i == player[0].x && j == player[0].y)
	{
		if (playerlive)
			rgb_set(255, 255, 0, 0, 0, 0);
		else
			rgb_set(255, 0, 0, 0, 0, 0);
		if (dir[0] == up)printf("AA");
		else if (dir[0] == down)printf("VV");
		else if (dir[0] == left)printf("<<");
		else if (dir[0] == right)printf(">>");
		rgb_set(204, 204, 204, 0, 0, 0);
	}
	else if (model.y == 2 && i == player[0].x && j == player[0].y && i == player[1].x && j == player[1].y)
	{
		for (int i = 0; i < model.y; i++)
		{
			if (i == 0)rgb_set(255, 255, 0, 0, 0, 0);
			else rgb_set(255, 0, 0, 0, 0, 0);
			if (dir[i] == up) printf("A");
			else if (dir[i] == down) printf("V");
			else if (dir[i] == left) printf("<");
			else if (dir[i] == right) printf(">");
		}
		rgb_set(204, 204, 204, 0, 0, 0);
	}
	else if (model.y == 2 && i == player[0].x && j == player[0].y)
	{
		rgb_set(255, 255, 0, 0, 0, 0);
		if (dir[0] == up)printf("A ");
		else if (dir[0] == down)printf("V ");
		else if (dir[0] == left)printf("< ");
		else if (dir[0] == right)printf("> ");
		rgb_set(204, 204, 204, 0, 0, 0);
	}
	else if (model.y == 2 && i == player[1].x && j == player[1].y)
	{
		rgb_set(255, 0, 0, 0, 0, 0);
		if (dir[1] == up)printf(" A");
		else if (dir[1] == down)printf(" V");
		else if (dir[1] == left)printf(" <");
		else if (dir[1] == right)printf(" >");
		rgb_set(204, 204, 204, 0, 0, 0);
	}
	else outputWithType(G[i][j]);
}

void SetFont(int size = 16)
{//设置字体大小
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = size;  //设置字体大小
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL; //字体粗细 FW_BOLD
	wcscpy_s(cfi.FaceName, L"黑体");  //设置字体，必须是控制台已有的
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

bool isAnyKey()
{
	for (int i = 0; i < 0xFF; ++i)
	{
		if (KEYDOWN(i)) return true;
	}
	return false;
}

inline double get3Dblockwidth(double len)
{
	return 0.5 / (len * tan(maxViewWidth / 2.0 / 180.0 * Pi)) * screenWidth;
}

struct Ray
{
	double k;
	double x0, y0;

	Ray() = default;

	Ray(double x, double y, double k)
	{
		this->k = k;
		x0 = x; y0 = y;
	}
	Ray(int x, int y, double k) :Ray(x + 0.5, y + 0.5, k) {}
	double operator()(double x) const
	{
		double b = y0 - k * x0;
		return k * x + b;
	}
	double Y(double y)const
	{
		double b = y0 - k * x0;
		return (y - b) / k;
	}
};

double touchBlock(int x, int y, Ray ray)
{
	double x0 = ray.x0, y0 = ray.y0;
	//if (G[x][y] == NOTHING)return -1;
	double b1 = (x < x0 ? 1 : 0), b2 = (y < y0 ? 1 : 0);
	double a1 = ray(x + b1), a2 = ray.Y(y + b2);
	if (a1 >= y && a1 <= y + 1)
	{
		return sqrt((a1 - y0) * (a1 - y0) + (x + b1 - x0) * (x + b1 - x0));
	}
	else if (a2 >= x && a2 <= x + 1)
	{
		return sqrt((a2 - x0) * (a2 - x0) + (y + b2 - y0) * (y + b2 - y0));
	}
	return -1;
}

pair<double, int> getrayline(int dir, Ray ray)
{
	for (double i = 0; i < maxViewLen; i += 0.01)
	{
		bool bk = false;
		double dx, dy;
		switch (dir)
		{
		case up:
			dx = ray.x0 - i;
			dy = ray(dx);
			break;
		case down:
			dx = ray.x0 + i;
			dy = ray(dx);
			break;
		case left:
			dy = ray.y0 - i;
			dx = ray.Y(dy);
			break;
		case right:
			dy = ray.y0 + i;
			dx = ray.Y(dy);
			break;
		default:
			bk = true;
		}

		if (bk) break;
		if ((int)dx > m + 1 || (int)dy > n + 1 || dy < 0 || dx < 0) break;

		if (G[(int)dx][(int)dy] != NOTHING
			&& G[(int)dx][(int)dy] != FINDED
			&& G[(int)dx][(int)dy] != FINDBACK
			&& G[(int)dx][(int)dy] != MONSTER)
		{
			return pair<double, int>(touchBlock((int)dx, (int)dy, ray), G[(int)dx][(int)dy]);
		}
	}

	return pair<double, char>(-1, NOTHING);
}

void getList(double* list, int* clist, int size, int x, int y, int dir, double view)
{

	double begin;
	switch (dir)
	{
	case up:
		begin = Pi;
		break;
	case down:
		begin = 0;
		break;
	case left:
		begin = Pi * 3 / 2;
		break;
	case right:
		begin = Pi / 2;
		break;
	default:
		return;
	}
	view = view / 180 * Pi;
	for (int i = 0; i < size; ++i)
	{
		double k = tan(begin + view / 2 - view / size * i);
		Ray ray(x, y, k);
		auto a = getrayline(dir, ray);
		list[i] = 1 / a.first;
		clist[i] = a.second;
	}
}

void showMoster(double len)
{
	if (len < 0)return;
	if (len == 0)
	{
		return;
	}

	int x, y;
	double size = get3Dblockwidth(len);
	double scale = monster_size / size;
	bool breakflg;
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
		{
			x = screenWidth / 2 - size / 2 + i;
			y = screenHafeHeight - size / 2 + j;

			breakflg = false;
			for (int ii = i * scale; ii < (i + 1) * scale && !breakflg; ++ii)
				for (int jj = j * scale; jj < (j + 1) * scale; ++jj)
				{
					if (monster_image[jj][ii])
					{
						breakflg = true;
						break;
					}
				}
			if (breakflg)
			{
				gotoxy(x * 2, y);
				outputWithType(MONSTER);
			}
		}
}

void show3D()
{//刷新(3D)
	gotoxy(0, 0);
	double* dl = new double[screenWidth];
	int* cl = new int[screenWidth];
	getList(dl, cl, screenWidth, player[0].x, player[0].y, dir[0], maxViewWidth);
	for (int h = 0; h < screenHafeHeight; ++h)
	{
		for (int i = 0; i < screenWidth; ++i)
		{
			if (screenHafeHeight * dl[i] > screenHafeHeight - h)
				outputWithType(cl[i]);
			else
				outputWithType(NOTHING);
		}
		printf("\n");
	}
	for (int h = screenHafeHeight; h > 0; --h)
	{
		for (int i = 0; i < screenWidth; ++i)
		{
			if (screenHafeHeight * dl[i] > screenHafeHeight - h)
				outputWithType(cl[i]);
			else
				outputWithType(NOTHING);
		}
		printf("\n");
	}
	delete[]dl;
	delete[]cl;

	for (int j = 0; j < 4; ++j)
	{
		if (stepdir[j] != dir[0])continue;
		for (int i = 0; i < maxViewLen; ++i)
		{
			if (G[i * step[j] + player[0].x][i * step[j + 4] + player[0].y] == WALL) break;
			if (G[i * step[j] + player[0].x][i * step[j + 4] + player[0].y] == MONSTER)
			{
				showMoster(i);
				break;
			}
		}
	}

}

void init()
{//地图初始化
	memset(G, WALL, sizeof(G));//将地图全部置为墙
	x_num = 1, y_num = 1;//矿工位置
	//定义起始点
	//G[1][1] = Starting_point;
	G[1][1] = NOTHING;
	for (int i = 0; i < 2; i++)
	{//设置玩家位置和朝向
		player[i] = { 1,1 };
		dir[i] = down;
	}
	camera = { MAXHEIGHT / 2, MAXWIDTH / 2 };
	playerlive = true;
}

void FindBlock()
{//找出与当前位置相邻的墙
	if (x_num + 1 <= m && G[x_num + 1][y_num] == WALL)
	{//down
		myblock.push_back(block(x_num + 1, y_num, down));
	}
	if (y_num + 1 <= n && G[x_num][y_num + 1] == WALL)
	{//right
		myblock.push_back(block(x_num, y_num + 1, right));
	}
	if (x_num - 1 >= 1 && G[x_num - 1][y_num] == WALL)
	{//up
		myblock.push_back(block(x_num - 1, y_num, up));
	}
	if (y_num - 1 >= 1 && G[x_num][y_num - 1] == WALL)
	{//left
		myblock.push_back(block(x_num, y_num - 1, left));
	}
}

void initMonster()
{
	for (int i = 0; i < monster_num; ++i)
	{
		int randx = rand() % (m - 6) + 3;
		int randy = rand() % (n - 6) + 3;
		if (G[randx][randy] == NOTHING)
		{
			G[randx][randy] = MONSTER;
			monster[i].x = randx;
			monster[i].y = randy;
		}
		else
		{
			bool flg = false;
			for (int t = 0; t < 4; ++t)
			{
				if (G[randx + step[t]][randy + step[t + 4]] == NOTHING
					&& randx + step[t] < m && randy + step[t + 4] < n)
				{
					G[randx + step[t]][randy + step[t + 4]] = MONSTER;
					monster[i].x = randx + step[t];
					monster[i].y = randy + step[t + 4];
					flg = true;
					break;
				}
			}
			if (!flg)
			{
				--i;
				--monster_num;
			}
		}
	}
}

void MapGeneration()
{//地图生成
	init();
	srand((unsigned)time(NULL));//随机数种子
	FindBlock();
	//第一步压入两堵墙（起点右边和起点下面）进入循环
	while (myblock.size())
	{
		int BlockSize = myblock.size();
		//随机选择一堵墙（生成0 ~ BlockSize-1之间的随机数，同时也是vector里墙的下标）
		int randnum = rand() % BlockSize;
		block SelectBlock = myblock[randnum];
		x_num = SelectBlock.row;//矿工来到我们“选择的墙”这里
		y_num = SelectBlock.column;
		//根据当前选择的墙的方向进行后续操作
		//此时，起始点 选择的墙 目标块 三块区域在同一直线上
		//我们让矿工从“选择的墙”继续前进到“目标块”
		//矿工有穿墙能力 ：)
		switch (SelectBlock.direction)
		{
		case down:
		{
			x_num++;
			break;
		}
		case right:
		{
			y_num++;
			break;
		}
		case left:
		{
			y_num--;
			break;
		}
		case up:
		{
			x_num--;
			break;
		}
		}
		//目标块如果是墙
		if (G[x_num][y_num] == WALL)
		{
			//打通墙和目标块
			G[SelectBlock.row][SelectBlock.column] = G[x_num][y_num] = NOTHING;
			//再次找出与矿工当前位置相邻的墙
			FindBlock();
		}
		else
		{//如果不是呢？说明我们的矿工挖到了一个空旷的通路上面 休息一下就好了
	  //relax
		}
		//删除这堵墙(把用不了的墙删了，对于那些已经施工过了不必再施工了，同时也是确保我们能跳出循环)
		myblock.erase(myblock.begin() + randnum);
	}
	return;
}

void inline qshow(point pos, point pos2 = point(0, 0))
{//快速刷新
	point p1 = point((pos.y + MAXWIDTH / 2 - camera.y) * 2, (pos.x + MAXHEIGHT / 2 - camera.x)),
		p2 = point((pos2.y + MAXWIDTH / 2 - camera.y) * 2, (pos2.x + MAXHEIGHT / 2 - camera.x));
	if (p1.x >= 0 && p1.x <= MAXWIDTH * 2 && p1.y >= 0 && p1.y <= MAXHEIGHT)
	{
		gotoxy(p1.x, p1.y);
		output(pos.x, pos.y);
	}
	if (p2.x >= 0 && p2.x <= MAXWIDTH * 2 && p2.y >= 0 && p2.y <= MAXHEIGHT)
	{
		gotoxy(p2.x, p2.y);
		output(pos2.x, pos2.y);
	}
	Sleep(1);
}

void inline show(bool if3D = false)
{//刷新
	bool camera_move = false;
	for (int i = 0; i < model.y; ++i)
		if (m > MAXHEIGHT || n > MAXWIDTH)
		{
			if (player[i].x < camera.x - MAXHEIGHT / 3 && camera.x > MAXHEIGHT / 2) { camera.x--; camera_move = true; }
			else if (player[i].x > camera.x + MAXHEIGHT / 3 && camera.x < m - MAXHEIGHT / 2 - 1) { camera.x++; camera_move = true; }
			else if (player[i].y < camera.y - MAXWIDTH / 3 && camera.y > MAXWIDTH / 2) { camera.y--; camera_move = true; }
			else if (player[i].y > camera.y + MAXWIDTH / 3 && camera.y < n - MAXWIDTH / 2 - 1) { camera.y++; camera_move = true; }
			break;
		}
	if (if3D) show3D();
	else
	{
		if (!camera_move && (m > MAXHEIGHT || n > MAXWIDTH))
		{
			for (int i = 0; i < model.y; i++) qshow(player[i], last_player[i]);
			return;
		}
		gotoxy(0, 0);
		for (int i = max(0, camera.x - MAXHEIGHT / 2); i < min(m + 2, camera.x + MAXHEIGHT / 2 + 3); i++)
		{
			for (int j = max(0, camera.y - MAXWIDTH / 2); j < min(n + 2, camera.y + MAXWIDTH / 2 + 3); j++)
			{
				output(i, j);
			}
			printf("\n");
		}
	}
}

int AToBFace(point a, point b)
{//B在A的哪个方向
	if (b.x < a.x) return up;
	if (b.x > a.x)return down;
	if (b.y < a.y) return left;
	if (b.y > a.y) return right;
	return -1;
}

void moveMonster(int i)
{
	if (i > monster_num)return;
	auto& mst = monster[i];
	int mov = rand() % 4;
	if (G[mst.x + step[mov]][mst.y + step[mov + 4]] == NOTHING)
	{
		point s = mst;
		G[s.x][s.y] = NOTHING;
		mst.x += step[mov];
		mst.y += step[mov + 4];
		G[mst.x][mst.y] = MONSTER;
		if (model.x != 3) qshow(mst, s);
	}
}

void moveMosnters()
{
	for (int i = 0; i < monster_num; ++i)
	{
		moveMonster(i);
	}
}

void autofind(bool if3D = false)
{//自动寻路(我焯，挂)
	stack<point>* s = new stack<point>[model.y]();
	bool book = true;
	bool temp = true;
	for (int num = 0; num < model.y; num++)
	{
		for (int i = 0; i < 4; i++)
			if (G[player[num].x + step[i]][player[num].y + step[i + 4]] != WALL
				&& G[player[num].x + step[i]][player[num].y + step[i + 4]] != FINDED)
			{
				s[num].push(point(player[num].x + step[i], player[num].y + step[i + 4]));
			}
		G[player[num].x][player[num].y] = FINDED;
	}
	while (1)
	{
		book = true;
		for (int num = 0; num < model.y; num++)
		{
			if (s[num].size())
			{
				book = false;
				dir[num] = AToBFace(player[num], s[num].top());
				last_player[num] = player[num];
				player[num] = s[num].top();
				if (G[player[num].x][player[num].y] == Destination)
				{
					if (if3D) { show(if3D); show(); }
					else if (m > MAXHEIGHT || n > MAXWIDTH) show();
					else qshow(player[num], last_player[num]);
					delete[]s;
					return;
				}
				G[player[num].x][player[num].y] = FINDED;
				temp = true;
				for (int i = 0; i < 4; i++)
					if (G[player[num].x + step[i]][player[num].y + step[i + 4]] != WALL
						&& G[player[num].x + step[i]][player[num].y + step[i + 4]] != FINDED
						&& G[player[num].x + step[i]][player[num].y + step[i + 4]] != FINDBACK)
					{
						s[num].push(point(player[num].x + step[i], player[num].y + step[i + 4]));
						temp = false;
					}
				if (temp)
				{
					G[player[num].x][player[num].y] = FINDBACK;
					s[num].pop();
				}
			}
			if (if3D || m > MAXHEIGHT || n > MAXWIDTH) show(if3D);
			else qshow(player[num], last_player[num]);
		}
		if (book) { delete[]s; return; }
		if (KEYDOWN(VK_ESCAPE))
		{
			int temp = SleepAndClear(100);
			if (temp == ESC)
			{
				for (int i = 0; i <= m + 1; i++)
					for (int j = 0; j <= n + 1; j++)
						if (G[i][j] == FINDED || G[i][j] == FINDBACK) G[i][j] = NOTHING;
				gotoxy(0, 0);
				for (int i = max(0, camera.x - MAXHEIGHT / 2); i < min(m + 2, camera.x + MAXHEIGHT / 2 + 3); i++)
				{
					for (int j = max(0, camera.y - MAXWIDTH / 2); j < min(n + 2, camera.y + MAXWIDTH / 2 + 3); j++)
					{
						output(i, j);
					}
					printf("\n");
				}
				delete[]s;
				return;
			}
		}
	}
}

void inline move()
{//移动
	int i = model.y - 1;
	for (int j = 0; j <= i; j++) last_player[j] = player[j];
	if (KEYDOWN(VK_UP) && G[player[i].x - 1][player[i].y] != WALL)
	{
		player[i].x--;
		dir[i] = up;
	}
	else if (KEYDOWN(VK_DOWN) && G[player[i].x + 1][player[i].y] != WALL)
	{
		player[i].x++;
		dir[i] = down;
	}
	else if (KEYDOWN(VK_LEFT) && G[player[i].x][player[i].y - 1] != WALL)
	{
		player[i].y--;
		dir[i] = left;
	}
	else if (KEYDOWN(VK_RIGHT) && G[player[i].x][player[i].y + 1] != WALL)
	{
		player[i].y++;
		dir[i] = right;
	}
	if (KEYDOWN('W') && G[player[0].x - 1][player[0].y] != WALL)
	{
		player[0].x--;
		dir[0] = up;
	}
	else if (KEYDOWN('S') && G[player[0].x + 1][player[0].y] != WALL)
	{
		player[0].x++;
		dir[0] = down;
	}
	else if (KEYDOWN('A') && G[player[0].x][player[0].y - 1] != WALL)
	{
		player[0].y--;
		dir[0] = left;
	}
	else if (KEYDOWN('D') && G[player[0].x][player[0].y + 1] != WALL)
	{
		player[0].y++;
		dir[0] = right;
	}
	else if (KEYDOWN('R'))
	{
		for (int i = 0; i < 2; i++)
		{
			player[i] = { 1,1 };
			dir[i] = down;
		}
		G[m][n] = Destination;
	}
	else if (KEYDOWN('P'))
	{
		autofind(false);
		int temp = _getch();
		if (temp == 0 || temp == 0xE0) _getch();
	}
	moveMosnters();
}

void dirmove(int d)
{//视角转动
	if (d == left)
	{
		switch (dir[0])
		{
		case up:
			dir[0] = left;
			break;
		case down:
			dir[0] = right;
			break;
		case left:
			dir[0] = down;
			break;
		case right:
			dir[0] = up;
			break;
		default:
			break;
		}
	}
	else if (d == right)
	{
		switch (dir[0])
		{
		case up:
			dir[0] = right;
			break;
		case down:
			dir[0] = left;
			break;
		case left:
			dir[0] = up;
			break;
		case right:
			dir[0] = down;
			break;
		default:
			break;
		}
	}
}

void move3D()
{//移动(3D)
	if (KEYDOWN(VK_UP) || KEYDOWN('W'))
	{
		switch (dir[0])
		{
		case up:
			if (G[player[0].x - 1][player[0].y] != WALL) player[0].x--;
			break;
		case down:
			if (G[player[0].x + 1][player[0].y] != WALL) player[0].x++;
			break;
		case left:
			if (G[player[0].x][player[0].y - 1] != WALL) player[0].y--;
			break;
		case right:
			if (G[player[0].x][player[0].y + 1] != WALL) player[0].y++;
			break;
		default:
			break;
		}
		moveMosnters();
	}
	else if (KEYDOWN(VK_DOWN) || KEYDOWN('S'))
	{
		switch (dir[0])
		{
		case up:
			if (G[player[0].x + 1][player[0].y] != WALL) player[0].x++;
			break;
		case down:
			if (G[player[0].x - 1][player[0].y] != WALL) player[0].x--;
			break;
		case left:
			if (G[player[0].x][player[0].y + 1] != WALL) player[0].y++;
			break;
		case right:
			if (G[player[0].x][player[0].y - 1] != WALL) player[0].y--;
			break;
		default:
			break;
		}
		moveMosnters();
	}
	else if (KEYDOWN(VK_LEFT) || KEYDOWN('A')) dirmove(left);
	else if (KEYDOWN(VK_RIGHT) || KEYDOWN('D')) dirmove(right);
	else if (KEYDOWN('R'))
	{
		player[0] = { 1,1 };
		dir[0] = down;
		G[m][n] = Destination;
	}
	else if (KEYDOWN('P'))
	{
		autofind(true);
		int temp = _getch();
		if (temp == 0 || temp == 0xE0)  _getch();
	}
	else if (KEYDOWN('M'))
	{
		gotoxy(0, 0);
		for (int i = max(0, camera.x - MAXHEIGHT / 2); i < min(m + 2, camera.x + MAXHEIGHT / 2); i++)
		{
			for (int j = max(0, camera.y - MAXWIDTH / 2); j < min(n + 2, camera.y + MAXWIDTH / 2 + 3); j++)
			{
				output(i, j);
			}
			printf("\n");
		}
		SleepAndClear(300);
	}
}

void middle(const char str[], int y)
{//居中输出
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hOutput, &bInfo);
	int dwSizeX = bInfo.dwSize.X * (16.0 / font),
		dwSizey = bInfo.dwSize.Y * (16.0 / font);//获取控制台屏幕缓冲区大小
	int len = strlen(str);//获取要输出的字符串的长度
	int x = (dwSizeX / 2 - len / 2);
	gotoxy(x, y);//移动光标
	cout << str;
}

void exitgame()
{//退出游戏
	system("cls");
	middle("感谢您的游玩", 5);
	middle("再次按Esc键退出", 9);
	middle("其他任意键返回", 12);
	Sleep(200);
	if (_getch() == ESC) exit(0);
	return;
}

point menu()
{//菜单
	point choose = { 1,1 };
	int key;
	system("cls");
	SetFont(font);
	middle("欢迎来到 镜花水月", 1);
	middle("请选择游戏模式", 4);
	middle("『闯关模式』", 6);
	middle("  自由模式  ", 8);
	middle("  娱乐模式  ", 10);
	middle("  无尽模式  ", 12);
	middle("  按键说明  ", 14);
	bool first = true;
	while (1)
	{
		if (first)
		{
			key = SleepAndClear(300);
			first = false;
		}
		key = _getch();
		if (key == 0 || key == 0xE0) key = key << 8 | _getch();
		if ((key == 'w' || key == 'W' || key == up_key) && choose.x > 1) choose.x--;
		else if ((key == 's' || key == 'S' || key == down_key) && choose.x < 5) choose.x++;
		else if (key == ENTER || key == SPACE)
		{
			system("cls");
			if (choose.x == 3 || choose.x == 5) return choose;
			middle("『单人游玩』", 5);
			middle("  双人游玩  ", 9);
			while (1)
			{
				key = _getch();
				if (key == 0 || key == 0xE0) key = key << 8 | _getch();
				if ((key == 'w' || key == 'W' || key == up_key)) choose.y = 1;
				else if ((key == 's' || key == 'S' || key == down_key)) choose.y = 2;
				else if (key == ENTER || key == SPACE)
				{
					system("cls");
					return choose;
				}
				else if (key == ESC)
				{
					system("cls");
					middle("欢迎来到 镜花水月", 1);
					middle("请选择游戏模式", 4);
					middle("『闯关模式』", 6);
					middle("  自由模式  ", 8);
					middle("  娱乐模式  ", 10);
					middle("  无尽模式  ", 12);
					middle("  按键说明  ", 14);
					break;
				}
				if (choose.y == 1)
				{
					middle("『单人游玩』", 5);
					middle("  双人游玩  ", 9);
				}
				else if (choose.y == 2)
				{
					middle("  单人游玩  ", 5);
					middle("『双人游玩』", 9);
				}
			}
		}
		else if (key == ESC)
		{
			exitgame();
			system("cls");
			middle("欢迎来到 镜花水月", 1);
			middle("请选择游戏模式", 4);
			middle("『闯关模式』", 6);
			middle("  自由模式  ", 8);
			middle("  娱乐模式  ", 10);
			middle("  无尽模式  ", 12);
			middle("  按键说明  ", 14);
		}
		if (choose.x == 1)
		{
			middle("『闯关模式』", 6);
			middle("  自由模式  ", 8);
		}
		else if (choose.x == 2)
		{
			middle("  闯关模式  ", 6);
			middle("『自由模式』", 8);
			middle("  娱乐模式  ", 10);
		}
		else if (choose.x == 3)
		{
			middle("  自由模式  ", 8);
			middle("『娱乐模式』", 10);
			middle("  无尽模式  ", 12);
		}
		else if (choose.x == 4)
		{
			middle("  娱乐模式  ", 10);
			middle("『无尽模式』", 12);
			middle("  按键说明  ", 14);
		}
		else if (choose.x == 5)
		{
			middle("  无尽模式  ", 12);
			middle("『按键说明』", 14);
		}
	}
}

void InstructionBook()
{//说明书
	middle("  按键说明  ", 2);
	middle("移动 —— WSAD/↑↓←→", 5);
	middle("重新开始(回到起点) —— R", 7);
	middle("地图(3D模式下) —— M", 9);
	middle("自动寻路 —— P", 11);
	middle("跳关/退出游戏 —— Esc", 13);
}

void start()
{//程序开始时处理
	printf("初始化中，请稍等……");
	system("title 镜花水月");
	WindowMax();
	HideCursor();
	rgb_init();
	rgb_set(204, 204, 204, 0, 0, 0);
	system("cls");
}

bool isGameKey()
{
	static int keys[] = { 'W','S','A','D',VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,VK_ESCAPE,'R','P','M' };
	for (auto k : keys)
	{
		if (KEYDOWN(k))return true;
	}
	return false;
}

int main()
{
	start();
	int checknum = 1;//关卡数量
	int  temp;
	//int key;
	while (1)
	{
		system("title 镜花水月");
		monster_num = 0;
		model = menu();
		n = m = 0;
		switch (model.x)
		{
		case 1:
			middle("闯关模式共 5 关 ", 5);
			middle("按任意键开始挑战", 10);
			system("title 镜花水月 闯关模式");
			checknum = 5;
			temp = _getch();
			if (temp == 0 || temp == 0xE0) _getch();
			break;
		case 2:
			cout << "请输入迷宫的行数、列数和敌人数量：\n";
			system("title 镜花水月 自由模式");
			checknum = 1;
			cin >> m >> n >> monster_num;
			if (m <= 0 || n <= 0 || monster_num < 0 || m > 1000 || n > 1000 || monster_num > 100)
			{
				cout << "请输入正确的迷宫大小和敌人数量\n";
				temp = _getch();
				if (temp == 0 || temp == 0xE0) _getch();
				cin.clear();
				while ((temp = getchar()) != '\n');
				continue;
			}
			if (m % 2 == 0) m++;
			if (n % 2 == 0) n++;
			break;
		case 3:
			cout << "请输入迷宫的行数、列数和敌人数量：\n";
			system("title 镜花水月 娱乐模式");
			checknum = 1;
			cin >> m >> n >> monster_num;
			if (m <= 0 || n <= 0 || monster_num < 0 || m > 1000 || n > 1000 || monster_num > 100)
			{
				cout << "请输入正确的迷宫大小和敌人数量\n";
				temp = _getch();
				if (temp == 0 || temp == 0xE0) _getch();
				cin.clear();
				while ((temp = getchar()) != '\n');
				continue;
			}
			if (m % 2 == 0) m++;
			if (n % 2 == 0) n++;
			break;
		case 4:
			cout << "请输入迷宫的行数、列数和敌人数量：\n";
			system("title 镜花水月 无尽模式");
			checknum = 1;
			cin >> m >> n >> monster_num;
			if (m <= 0 || n <= 0 || monster_num < 0 || m > 1000 || n > 1000 || monster_num > 100)
			{
				cout << "请输入正确的迷宫大小和敌人数量\n";
				temp = _getch();
				if (temp == 0 || temp == 0xE0) _getch();
				cin.clear();
				while ((temp = getchar()) != '\n');
				continue;
			}
			if (m % 2 == 0) m++;
			if (n % 2 == 0) n++;
			break;
		case 5:
			InstructionBook();
			temp = _getch();
			if (temp == 0 || temp == 0xE0) _getch();
			continue;
			break;
		default:
			break;
		}
		playernum = model.y;
		system("cls");
		SetFont();
		for (int i = 0; i < checknum; i++)
		{
			if (model.x == 1)
			{
				string tit = "title 镜花水月 闯关模式-第" + to_string(i + 1) + "关";
				system(tit.c_str());
				m = difficulty[i][0];
				n = difficulty[i][1];
			}
			MapGeneration();		//生成地图
			G[m][n] = Destination;	//设定终点
			initMonster();
			bool des = false;
			bool die = false;
			bool outbreak = true;
			if (model.x == 3) show3D();	//显示地图
			else
			{
				gotoxy(0, 0);
				for (int i = max(0, camera.x - MAXHEIGHT / 2); i < min(m + 2, camera.x + MAXHEIGHT / 2 + 3); i++)
				{
					for (int j = max(0, camera.y - MAXWIDTH / 2); j < min(n + 2, camera.y + MAXWIDTH / 2 + 3); j++)
					{
						output(i, j);
					}
					printf("\n");
				}
			}
			while (1)
			{
				for (int i = 0; i < model.y; i++)
					if (G[player[i].x][player[i].y] == Destination) des = true;
				for (int i = 0; i < model.y; i++)
				{
					if (G[player[i].x][player[i].y] == MONSTER)
					{
						--model.y;
						if (i == 0)
						{
							player[0] = player[1];
							playerlive = false;
						}
						if (model.x != 3) show();
					}
				}
				if (des || model.y == 0)
				{
					if (model.x == 3)
					{
						show();
						temp = _getch();
						if (temp == 0 || temp == 0xE0) _getch();
					}
					if (i < checknum - 1 && des)
					{
						gotoxy(0, m + 2);
						cout << "恭喜通第 " << i + 1 << " 关\n按任意键继续\n按R键重置本关\n";
					}
					else if (des)
					{
						system("cls");
						SetFont(font);
						middle("恭喜通关！", 5);
						middle("按任意键继续", 9);
						middle("按R键重置本关", 11);
					}
					else
					{
						system("cls");
						SetFont(font);
						rgb_set(255, 0, 0, 0, 0, 0);
						middle("GAME OVER", 5);
						rgb_set(204, 204, 204, 0, 0, 0);
						middle("按任意键继续", 9);
						middle("按R键重置本关", 11);
					}
					SleepAndClear(1000);
					outbreak = true;
					while (1)
					{
						if (KEYDOWN('R'))
						{
							model.y = playernum;
							system("cls");
							SetFont();
							for (int i = 0; i < 2; i++)
							{
								player[i] = { 1,1 };
								dir[i] = down;
							}
							for (int i = 0; i <= m + 1; i++)
								for (int j = 0; j <= n + 1; j++)
									if (G[i][j] == FINDED || G[i][j] == FINDBACK) G[i][j] = NOTHING;
							G[m][n] = Destination;
							des = false;
							camera = { MAXHEIGHT / 2, MAXWIDTH / 2 };
							show(model.x == 3);
							outbreak = false;
							break;
						}
						else if (isAnyKey())
						{
							break;
						}
					}
					if (outbreak)
						break;
					else
						continue;
				}
				if (!isGameKey()) continue;
				if (KEYDOWN(VK_ESCAPE))
				{
					while (_getch() != ESC);
					break;
				}
				if (model.x == 3) move3D();
				else move();
				if (model.x == 4) {
					for (int num = 0; num < model.y; num++) {
						for (int x = max(0, player[num].x - 3); x < min(m + 1, player[num].x + 3); x++)
							for (int y = max(0, player[num].y - 3); y < min(n + 1, player[num].y + 3); y++)
								if (G[x][y] == Destination) {
									G[x][y] = NOTHING;
									int newx = rand() & 1 ? 1 : m;
									int newy = rand() & 1 ? 1 : n;
									G[newx][newy] = Destination;
									qshow(point(x, y), point(newx, newy));
								}
					}
				}
				if (model.x == 3) show(true);
				else if (m > MAXHEIGHT || n > MAXWIDTH) show();
				else for (int i = 0; i < model.y; i++) qshow(player[i], last_player[i]);
				Sleep(100);
			}
		}
	}
	return 0;
}