#ifndef TZFE_H_
#define TZFE_H_
#include<iostream>
#include<iomanip>
#include<ctime>
#include<cstdlib>
#include<conio.h>
//����windowsƽ̨����
#include<windows.h>
#include<io.h>
#include<direct.h>
#include<ShlObj.h>
#include "resource.h"
//����Ϊ����ʶ��AI����
//#include"pch.h"

static  char configPath[4096] = { 0 };//�ļ�·������
struct Board
{
	int x;//������
	int y;//������
	int number;//���������
};
void DisplayMessageBox(HWND hWnd, const TCHAR* szText, const TCHAR* szTitle, void (*OKFunction)(),void(*can)());

class TZFE {
public:
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	int readKeyboard();

	int score;
	int bestScore;
	bool ifNeedRandomNumber;//�Ƿ���Ҫ������������true,�����false
	bool ifGameOver;//��Ϸ�Ƿ�����������true�������false
	bool ifEndGame;//�Ƿ��˳���Ϸ�������true,�����false


	void randomNumber();
	void ifLoseGame();
	int nullNumber();//�ж��ٸ��ո�
	void clear();//����
	void refreshShow();//ˢ������



	void newGame();//��ʼ����Ϸ
	void loopGame();//ѭ����Ϸ
	void endGame(int);
	void resetGame();
	void newBoard();
	Board board[4][4];
};
#endif


