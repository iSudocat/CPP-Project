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

const int nRows = 15;
const int mCols = 15;

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

	void PrintBoard();
	void PrintBoard(Point lastP);
	void ResetChess(Point w);
	bool SetChess(Point w, int player);
	int GetState(Point w, int direct, int dist);		//取出某一方向上距当前点dist位置的状态
	void Getline(int* line, Point p, int direct, int num);  	//取出连续num个棋子的状态
	int CalculateValue(int nPlayer, int nEmpty);		//返回不同棋型的估价
	int Calculate(int* line, int player);					//计算一条直线的估价
	int IsFinal();  						//判断游戏是否结束
	int Evaluate();  						//估价函数：评估当前棋局
	void clear();
};

class AI
{
private:
	Board virtualBoard;		//AI的模拟棋局
public:
	AI(const Board board);
	AI(const AI& ai);
	~AI();

	void GenerateQueue(vector <Point>* decide); 	//生成决策队列
	int AlphaBeta(int player, int depth, int MAXDEP, int alpha, int beta, Point& best); //极大极小搜索 player=1 先手 player=2 后手
};

class GAME
{
private:
	Board chessBoard;
public:
	void Run(int mode);
};

//非成员函数
void findPoint(POINT& cursor);
int squareDist(Point P, Point Q);
int square(int x);