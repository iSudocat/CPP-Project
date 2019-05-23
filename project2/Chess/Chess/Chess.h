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

const int Direction[8][2] = { { -1 , -1 } , { -1 , 0 } , { -1 , 1 } , { 0 , -1 } , { 0 , 1 } , { 1 , -1} , { 1 , 0 } , { 1 , 1 } }; //�˸�����

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
	int empty;	//����ĸ�������
public:
	Board() { };
	Board(int* board);
	Board(const Board& board);
	~Board();

	void PrintBoard();
	void PrintBoard(Point lastP);
	void ResetChess(Point w);
	bool SetChess(Point w, int player);
	int GetState(Point w, int direct, int dist);		//ȡ��ĳһ�����Ͼ൱ǰ��distλ�õ�״̬
	void Getline(int* line, Point p, int direct, int num);  	//ȡ������num�����ӵ�״̬
	int CalculateValue(int nPlayer, int nEmpty);		//���ز�ͬ���͵Ĺ���
	int Calculate(int* line, int player);					//����һ��ֱ�ߵĹ���
	int IsFinal();  						//�ж���Ϸ�Ƿ����
	int Evaluate();  						//���ۺ�����������ǰ���
	void clear();
};

class AI
{
private:
	Board virtualBoard;		//AI��ģ�����
public:
	AI(const Board board);
	AI(const AI& ai);
	~AI();

	void GenerateQueue(vector <Point>* decide); 	//���ɾ��߶���
	int AlphaBeta(int player, int depth, int MAXDEP, int alpha, int beta, Point& best); //����С���� player=1 ���� player=2 ����
};

class GAME
{
private:
	Board chessBoard;
public:
	void Run(int mode);
};

//�ǳ�Ա����
void findPoint(POINT& cursor);
int squareDist(Point P, Point Q);
int square(int x);