#include "snake.h"
#include <conio.h>
#include "tools.h"
#include <iostream>

void Snake::InitSnake()//��ʼ����
{
	for (auto& point : snake)
	{
		point.PrintCircular();
	}
}

void Snake::Move()//������
{
	switch (direction)
	{
	case Direction::UP:
		snake.emplace_back(Point(snake.back().GetX(), snake.back().GetY() - 1));
		break;
	case Direction::DOWN:
		snake.emplace_back(Point(snake.back().GetX(), snake.back().GetY() + 1));
		break;
	case Direction::LEFT:
		snake.emplace_back(Point(snake.back().GetX() - 1, snake.back().GetY()));
		break;
	case Direction::RIGHT:
		snake.emplace_back(Point(snake.back().GetX() + 1, snake.back().GetY()));
		break;
	default:
		break;
	}
	SetColor(14);
	snake.back().PrintCircular();
}

void Snake::NormalMove()//�������ƶ���ͷ������β����
{
	Move();
	snake.front().Clear();
	snake.pop_front();
}

bool Snake::OverEdge()//�����߽�
{
	int fflag = 1;
	if (snake.back().GetX() >= 30 ||
		snake.back().GetY() >= 30 ||
		snake.back().GetX() <= 1 ||
		snake.back().GetY() <= 1) fflag=0;

	if ((snake.back().GetX() == 7 && snake.back().GetY() <= 10)) fflag=0;
	if ((snake.back().GetX() >= 25 && snake.back().GetX() <= 29 && snake.back().GetY() == 22)) fflag = 0;
	if ((snake.back().GetX() >= 22 && snake.back().GetX() <= 27 && snake.back().GetY() == 4)) fflag = 0;
	if ((snake.back().GetX() == 2 && snake.back().GetY() == 27)) fflag = 0;
	if ((snake.back().GetX() == 3 && snake.back().GetY() == 27)) fflag = 0;
	if ((snake.back().GetX() == 4 && snake.back().GetY() == 27))fflag = 0;
	if ((snake.back().GetX() == 4 && snake.back().GetY() == 28)) fflag = 0;
	if ((snake.back().GetX() == 4 && snake.back().GetY() == 29))fflag = 0;
	return fflag;
}

bool Snake::HitItself()//ײ������
{
	std::deque<Point>::size_type cnt = 1;
	Point *head = new Point(snake.back().GetX(), snake.back().GetY());//���ͷ������
	for (auto& point : snake) //���������������ͷ����ͬ�����겻�����߳�������ζ����ͷ��ײ������
	{
		if (!(point == *head))
			++cnt;
		else
			break;
	}
	delete head;
	if (cnt == snake.size())
		return true;
	else
		return false;
}

bool Snake::ChangeDirection()//�ı䷽��
{
	char ch;
	if (_kbhit())//kbhit��������ֵΪ��������ע��
	{
		ch = _getch();
		switch (ch)
		{
		case -32:
			ch = _getch();
			switch (ch)
			{
			case 72:
				if (direction != Direction::DOWN)//��������뵱ǰ�˶������෴����Ч
					direction = Direction::UP;
				break;
			case 80:
				if (direction != Direction::UP)
					direction = Direction::DOWN;
				break;
			case 75:
				if (direction != Direction::RIGHT)
					direction = Direction::LEFT;
				break;
			case 77:
				if (direction != Direction::LEFT)
					direction = Direction::RIGHT;
				break;
			default:
				break;
			}
			return true;

		case 27://ESC
			return false;

		default:
			return true;

		}
	}
	return true;
}

bool Snake::GetFood(const Food& cfood)
{
	if (snake.back().GetX() == cfood.x && snake.back().GetY() == cfood.y)
		return true;
	else
		return false;
}

bool Snake::GetBigFood(Food& cfood)
{
	if (snake.back().GetX() == cfood.big_x && snake.back().GetY() == cfood.big_y)
	{
		cfood.big_flag = false;
		cfood.big_x = 0;
		cfood.big_y = 0;
		SetCursorPosition(1, 0);
		std::cout << "                                                            ";//Ĩȥʱ����
		return true;
	}
	else
		return false;
}