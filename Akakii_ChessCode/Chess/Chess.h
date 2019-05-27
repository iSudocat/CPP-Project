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

const int Direction[8][2] = { { -1 , -1 } , { -1 , 0 } , { -1 , 1 } , { 0 , -1 } , { 0 , 1 } , { 1 , -1} , { 1 , 0 } , { 1 , 1 } }; //�˸�����

const int nRows = 15;		//���̵�����
const int mCols = 15;		//���̵�����

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
	//���Ӽ�¼
	SYSTEMTIME localTime;	//����ʱ��
	Point chessCoordinate;		//����λ��
	int player;				//�������(1)/����(2)
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

	void clear();
	bool SetChess(Point w, int player);
	void ResetChess(Point w);
	void RePaintBoard(HWND hWnd, Point nowP);					//���´��ڲ���ʾ��һ������λ��
	int GetState(Point w, int direct, int dist);				//ȡ��ĳһ�����Ͼ൱ǰ��distλ�õ�״̬
	void Getline(int* line, Point p, int direct, int num);  	//ȡ������num�����ӵ�״̬
	int CalculateValue(int nPlayer, int nEmpty);				//���ز�ͬ���͵Ĺ���
	int Calculate(int* line, int player);						//����һ��ֱ�ߵĹ���
	int IsFinal();  											//�ж���Ϸ�Ƿ����
	int Evaluate();  											//���ۺ�����������ǰ���
};

class AI
{
private:
	Board virtualBoard;		//AI��ģ�����
public:
	AI(const Board board);
	AI(const AI& ai);
	~AI();

	void GenerateQueue(vector <Point>* decide); 										//���ɾ��߶���
	int AlphaBeta(int player, int depth, int MAXDEP, int alpha, int beta, Point& best); //����С���� player=1 ���� player=2 ����
};

//�ǳ�Ա����
void findPoint(POINT& cursor);			//�����ͻ�������ת������������
int squareDist(Point P, Point Q);		//��������ƽ��
int square(int x);						//����ƽ��

void DisplayMessageBox(HWND hWnd, const TCHAR* szText, const TCHAR* szTitle, void (*OKFunction)() = NULL, void (*CancelFunction)() = NULL);				//��ʾ��Ϣ��
void ChangeDifficulty(HMENU hMenu, UINT checked, UINT unchecked_one, UINT unchecked_two);			//�ı��Ѷȸ�ѡ��
void TryAgain();							//���¿�ʼ��Ϸ
void SaveGame();							//������Ϸ
void ReadGame();							//��ȡ��Ϸ
void ExitGame();							//�˳���Ϸ
void NewRecord(Point w, int player);		//����һ����¼
void ComputersTurn();						//���Լ�������λ��
void ReadGame_Mulitithread();