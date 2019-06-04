#include "point.h"
#include "tools.h"
#include <iostream>

void Point::Print()//输出方块
{
	SetCursorPosition(x, y);
	std::cout << "■";
}

void Point::PrintCircular()//输出圆形
{
	SetCursorPosition(x, y);
	std::cout << "●";
}

void Point::PrintStar1()//输出星形
{
	SetCursorPosition(x, y);
	std::cout << "★";
}

void Point::PrintStar2()//输出※
{
	SetCursorPosition(x, y);
	std::cout << "※";
}

void Point::PrintRectangular()//输出空白方框
{
	SetCursorPosition(x, y);
	std::cout << "□";
}



void Point::Clear()//清除输出
{
	SetCursorPosition(x, y);
	std::cout << "  ";
}

void Point::ChangePosition(const int x, const int y)//改变坐标
{
	this->x = x;
	this->y = y;
}