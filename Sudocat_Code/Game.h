#pragma once
#include <Windows.h>
using namespace std;

#define CELL 20
#define ROWS 20		//����
#define COLS 15		//����

#define SCORE_LEVEL_INC 50	//���������ֵ


class Game
{
public:
	void DrawPanel(HDC hdc);  //���Ʊ��
	void RefreshPanel(HDC hdc);  //ˢ�����
	void DoDownShift(HDC hdc);  //����
	void DoLeftShift(HDC hdc);  //����
	void DoRightShift(HDC hdc);  //����
	void DoAccelerate(HDC hdc);  //����
	void DoRedirection(HDC hdc); //�ı䷽��
	void ClearRow(HDC hdc);   //����
	bool ExportBlock();  //�������
	bool IsTouchBottom(HDC hdc);   //���׼��
	void color(int a);	//����̨������ɫ
	void DeleteBlock(int** block);	//�ͷ�Block��̬������ڴ�
	int score_get() { return score; }
	int level_get() { return level; }
	int interval_unit_get() { return interval_unit; }
	int interval_base_get() { return interval_base; }
	int old_interval_get() { return old_interval; }
private:
	int score = 0;    //����
	int level = 0;    //����
	int interval_unit = 30;  //�漶��������ʱ��������
	int interval_base = 500;  //ʱ��������
	int old_interval;   //���浱ǰ��ʱ���������ڼ��ٲ���

};

class Block		//��ǰ�����еķ�����
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