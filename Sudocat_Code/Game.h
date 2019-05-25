#pragma once
#include <Windows.h>
using namespace std;

#define CELL 20
#define ROWS 20		//行数
#define COLS 15		//列数

#define SCORE_LEVEL_INC 50	//升级所需分值


class Game
{
public:
	void DrawPanel(HDC hdc);  //绘制表格
	void RefreshPanel(HDC hdc);  //刷新面板
	void DoDownShift(HDC hdc);  //下移
	void DoLeftShift(HDC hdc);  //左移
	void DoRightShift(HDC hdc);  //右移
	void DoAccelerate(HDC hdc);  //加速
	void DoRedirection(HDC hdc); //改变方向
	void ClearRow(HDC hdc);   //消行
	bool ExportBlock();  //输出方块
	bool IsTouchBottom(HDC hdc);   //触底检测
	void color(int a);	//控制台文字颜色
	void DeleteBlock(int** block);	//释放Block动态分配的内存
	int score_get() { return score; }
	int level_get() { return level; }
	int interval_unit_get() { return interval_unit; }
	int interval_base_get() { return interval_base; }
	int old_interval_get() { return old_interval; }
private:
	int score = 0;    //分数
	int level = 0;    //级数
	int interval_unit = 30;  //随级数递增的时间间隔增量
	int interval_base = 500;  //时间间隔基量
	int old_interval;   //保存当前的时间间隔，用于加速操作

};

class Block		//当前下落中的方块类
{
private:
	int m_cur_top, m_cur_left;
	int m_Red, m_Green, m_Blue;
	int m_width, m_height;
public:
	void SetBlock(int cur_top, int cur_left, int width, int height, int Red, int Green, int Blue)
	{
		m_cur_top = cur_top;
		m_cur_left = cur_left;
		m_Red = Red;
		m_width = width;
		m_height = height;
		m_Green = Green;
		m_Blue = Blue;
	}
	void GoDown() { m_cur_top++; }
	void GoLeft() { m_cur_left--; }
	void GoRight() { m_cur_left++; }
	int GetCurTop() { return m_cur_top; }
	int GetCurLeft() { return m_cur_left; }
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	int GetRed() { return m_Red; }
	int GetGreen() { return m_Green; }
	int GetBlue() { return m_Blue; }
};