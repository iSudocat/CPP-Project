#include "food.h"
#include "tools.h"
#include <cstdlib>
#include <iostream>
#include"snake.h"

//����ʳ��
void Food::DrawFood(Snake& csnake)
{
	/*����rand����������꣬�����䷶Χ������2-29�ڣ����ڵ�ͼ�ڣ������õ������������ص��������»�ȡ��
	  ͬʱÿ5��ʳ��ͳ���һ����ʱʳ��*/
	while (true)
	{
		int tmp_x = rand() % 28;
		int tmp_y = rand() % 28;
		
		int fflag = 1;//�ж�ʳ�����
		if (tmp_x >= 30 ||
			tmp_y >= 30 ||
			tmp_x <= 1 ||
			tmp_y <= 1) fflag = 0;

		if (tmp_x == 7 && tmp_y <= 10) fflag = 0;
		if (tmp_x >= 25 && tmp_x && tmp_y == 22) fflag = 0;
		if (tmp_x >= 22 && tmp_x <= 27 && tmp_y == 4) fflag = 0;
		if (tmp_x == 2 && tmp_y == 27) fflag = 0;
		if (tmp_x == 3 && tmp_y == 27) fflag = 0;
		if (tmp_x == 4 && tmp_y == 27)fflag = 0;
		if (tmp_x == 4 && tmp_y == 28) fflag = 0;
		if (tmp_x == 4 && tmp_y == 29)fflag = 0;
		if (fflag == 0) continue;

		bool flag = false;
		for (auto& point : csnake.snake)
		{
			if ((point.GetX() == tmp_x && point.GetY() == tmp_y) || (tmp_x == big_x && tmp_y == big_y)) {
				flag = true;
				break;
			}
		}
		if (flag)
			continue;
		x = tmp_x;
		y = tmp_y;
		SetCursorPosition(x, y);
		SetColor(13);
		std::cout << "��";
		++cnt;
		cnt %= 5;
		if (cnt == 0)
		{
			DrawBigFood(csnake);
		}
		break;
	}
}
//������ʱʳ��
void Food::DrawBigFood(Snake& csnake)
{
	SetCursorPosition(5, 0);
	SetColor(11);
	std::cout << "------------------------------------------";//������
	progress_bar = 42;
	while (true)
	{
		int tmp_x = rand() % 28;
		int tmp_y = rand() % 28;
		if (tmp_x < 2) tmp_x += 2;
		if (tmp_y < 2) tmp_y += 2;

		int fflag = 1;//�ж�ʳ�����
		if (tmp_x >= 30 ||
			tmp_y >= 30 ||
			tmp_x <= 1 ||
			tmp_y <= 1) fflag = 0;

		if (tmp_x == 7 && tmp_y <= 10) fflag = 0;
		if (tmp_x >= 25 && tmp_x && tmp_y == 22) fflag = 0;
		if (tmp_x >= 22 && tmp_x <= 27 && tmp_y == 4) fflag = 0;
		if (tmp_x == 2 && tmp_y == 27) fflag = 0;
		if (tmp_x == 3 && tmp_y == 27) fflag = 0;
		if (tmp_x == 4 && tmp_y == 27)fflag = 0;
		if (tmp_x == 4 && tmp_y == 28) fflag = 0;
		if (tmp_x == 4 && tmp_y == 29)fflag = 0;
		if (fflag == 0) continue;


		bool flag = false;
		for (auto& point : csnake.snake)
		{
			if ((point.GetX() == tmp_x && point.GetY() == tmp_y) || (tmp_x == x && tmp_y == y))
			{
				flag = true;
				break;
			}
		}
		if (flag)
			continue;

		big_x = tmp_x;
		big_y = tmp_y;
		SetCursorPosition(big_x, big_y);
		SetColor(18);
		std::cout << "��";
		big_flag = true;
		flash_flag = true;
		break;
	}
}

int Food::GetCnt()
{
	return cnt;
}
//��˸��ʱʳ��
void Food::FlashBigFood()
{
	SetCursorPosition(big_x, big_y);
	SetColor(18);
	if (flash_flag)
	{
		std::cout << "  ";
		flash_flag = false;
	}
	else
	{
		std::cout << "��";
		flash_flag = true;
	}

	SetCursorPosition(26, 0);
	SetColor(11);
	for (int i = 42; i >= progress_bar; --i)//����������
		std::cout << "\b \b";
	--progress_bar;
	if (progress_bar == 0) {
		SetCursorPosition(big_x, big_y);
		std::cout << "  ";
		big_flag = false;
		big_x = 0;
		big_y = 0;
	}
}

bool Food::GetBigFlag()
{
	return big_flag;
}

int Food::GetProgressBar()
{
	return progress_bar;
}