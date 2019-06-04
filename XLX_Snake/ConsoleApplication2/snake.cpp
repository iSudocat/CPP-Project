#include "snake.h"
#include <conio.h>
#include "tools.h"
#include <iostream>

void Snake::InitSnake()//初始化蛇
{
	for (auto& point : snake)
	{
		point.PrintCircular();
	}
}

void Snake::Move()//蛇增长
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

void Snake::NormalMove()//蛇正常移动，头增长，尾缩短
{
	Move();
	snake.front().Clear();
	snake.pop_front();
}

bool Snake::OverEdge()//超出边界
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

bool Snake::HitItself()//撞到自身
{
	std::deque<Point>::size_type cnt = 1;
	Point *head = new Point(snake.back().GetX(), snake.back().GetY());//获得头部坐标
	for (auto& point : snake) //如果整条蛇中与蛇头不相同的坐标不等于蛇长，则意味着蛇头碰撞到自身
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

bool Snake::ChangeDirection()//改变方向
{
	char ch;
	if (_kbhit())//kbhit函数返回值为两个，需注意
	{
		ch = _getch();
		switch (ch)
		{
		case -32:
			ch = _getch();
			switch (ch)
			{
			case 72:
				if (direction != Direction::DOWN)//如果方向与当前运动方向相反，无效
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
		std::cout << "                                                            ";//抹去时间条
		return true;
	}
	else
		return false;
}