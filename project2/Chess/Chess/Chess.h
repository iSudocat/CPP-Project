#pragma once

#include "resource.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
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
	void PrintBoard();
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

class GAME
{
	//控制台应用下的GAME类
private:
	Board chessBoard;
public:
	void Run(int mode);
};

//非成员函数
void findPoint(POINT& cursor);			//将鼠标客户端坐标转换成棋盘坐标
int squareDist(Point P, Point Q);		//计算距离的平方
int square(int x);						//计算平方