#include "point.h"
#include "tools.h"
#include <iostream>

void Point::Print()//�������
{
	SetCursorPosition(x, y);
	std::cout << "��";
}

void Point::PrintCircular()//���Բ��
{
	SetCursorPosition(x, y);
	std::cout << "��";
}

void Point::PrintStar1()//�������
{
	SetCursorPosition(x, y);
	std::cout << "��";
}

void Point::PrintStar2()//�����
{
	SetCursorPosition(x, y);
	std::cout << "��";
}

void Point::PrintRectangular()//����հ׷���
{
	SetCursorPosition(x, y);
	std::cout << "��";
}



void Point::Clear()//������
{
	SetCursorPosition(x, y);
	std::cout << "  ";
}

void Point::ChangePosition(const int x, const int y)//�ı�����
{
	this->x = x;
	this->y = y;
}