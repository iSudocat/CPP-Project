#pragma once

#include "resource.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
using namespace std;

const int N = 20;
const int M = 20;

const int Direction[8][2] = { { -1 , -1 } , { -1 , 0 } , { -1 , 1 } , { 0 , -1 } , { 0 , 1 } , { 1 , -1} , { 1 , 0 } , { 1 , 1 } }; //八个方向

const int nRows = 15;		//棋盘的行数
const int mCols = 15;		//棋盘的列数

struct Point
{
	int x, y;
	Point(int X = 0, int Y = 0) { x = X, y = Y; }
	bool Check()
	{
		if (x < 0 || y < 0 || x > nRows || y > mCols) return 0;
		return 1;
	}
};

struct Record
{
	//落子记录
	SYSTEMTIME localTime;	//落子时间
	Point chessCoordinate;		//落子位置
	int player;				//落子玩家(1)/电脑(2)
};

class Board
{
private:
	int state[N][M];
	int empty;	//空余的格子数量
public:
	Board() { };
	Board(int* board);
	Board(const Board& board);
	~Board();

	void clear();
	bool SetChess(Point w, int player);
	void ResetChess(Point w);
	void RePaintBoard(HWND hWnd, Point nowP);					//更新窗口并显示上一次落子位置
	int GetState(Point w, int direct, int dist);				//取出某一方向上距当前点dist位置的状态
	void Getline(int* line, Point p, int direct, int num);  	//取出连续num个棋子的状态
	int CalculateValue(int nPlayer, int nEmpty);				//返回不同棋型的估价
	int Calculate(int* line, int player);						//计算一条直线的估价
	int IsFinal();  											//判断游戏是否结束
	int Evaluate();  											//估价函数：评估当前棋局
};

class AI
{
private:
	Board virtualBoard;		//AI的模拟棋局
public:
	AI(const Board board);
	AI(const AI& ai);
	~AI();

	void GenerateQueue(vector <Point>* decide); 										//生成决策队列
	int AlphaBeta(int player, int depth, int MAXDEP, int alpha, int beta, Point& best); //极大极小搜索 player=1 先手 player=2 后手
};

//非成员函数
void findPoint(POINT& cursor);			//将鼠标客户端坐标转换成棋盘坐标
int squareDist(Point P, Point Q);		//计算距离的平方
int square(int x);						//计算平方

void DisplayMessageBox(HWND hWnd, const TCHAR* szText, const TCHAR* szTitle, void (*OKFunction)() = NULL, void (*CancelFunction)() = NULL);				//显示消息框
void ChangeDifficulty(HMENU hMenu, UINT checked, UINT unchecked_one, UINT unchecked_two);			//改变难度复选框
void TryAgain();							//重新开始游戏
void SaveGame();							//保存游戏
void ReadGame();							//读取游戏
void ExitGame();							//退出游戏
void NewRecord(Point w, int player);		//新增一条记录
void ComputersTurn();						//电脑计算落子位置
void ReadGame_Mulitithread();