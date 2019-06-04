#ifndef TZFE_H_
#define TZFE_H_
#include<iostream>
#include<iomanip>
#include<ctime>
#include<cstdlib>
#include<conio.h>
//以下windows平台函数
#include<windows.h>
#include<io.h>
#include<direct.h>
#include<ShlObj.h>
#include "resource.h"
//以下为语音识别AI所需
//#include"pch.h"

static  char configPath[4096] = { 0 };//文件路径配置
struct Board
{
	int x;//横坐标
	int y;//纵坐标
	int number;//这里的数字
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
	bool ifNeedRandomNumber;//是否需要随机数，是输出true,否输出false
	bool ifGameOver;//游戏是否结束，是输出true，否输出false
	bool ifEndGame;//是否退出游戏，是输出true,否输出false


	void randomNumber();
	void ifLoseGame();
	int nullNumber();//有多少个空格
	void clear();//清屏
	void refreshShow();//刷新数据



	void newGame();//初始化游戏
	void loopGame();//循环游戏
	void endGame(int);
	void resetGame();
	void newBoard();
	Board board[4][4];
};
#endif


