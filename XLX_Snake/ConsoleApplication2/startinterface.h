#ifndef STRATINTERFACE_H
#define STARTINTERFACE_H

#include <deque>
#include <vector>
#include "point.h"

class StartInterface
{
public:
	StartInterface() : speed(35) {
		startsnake.emplace_back(Point(0, 14));//蛇初始部分
		startsnake.emplace_back(Point(1, 14));
		startsnake.emplace_back(Point(2, 15));
		startsnake.emplace_back(Point(3, 16));
		startsnake.emplace_back(Point(4, 17));
		startsnake.emplace_back(Point(5, 18));
		startsnake.emplace_back(Point(6, 17));
		startsnake.emplace_back(Point(7, 16));
		startsnake.emplace_back(Point(8, 15));
		startsnake.emplace_back(Point(9, 14));

		
		textsnake.emplace_back(Point(-22, 7));//W
		textsnake.emplace_back(Point(-22, 8));
		textsnake.emplace_back(Point(-22, 9));
		textsnake.emplace_back(Point(-22, 10));
		textsnake.emplace_back(Point(-21, 11));
		textsnake.emplace_back(Point(-20, 7));
		textsnake.emplace_back(Point(-20, 8));
		textsnake.emplace_back(Point(-20, 9));
		textsnake.emplace_back(Point(-20, 10));
		textsnake.emplace_back(Point(-19, 11));
		textsnake.emplace_back(Point(-18, 7));
		textsnake.emplace_back(Point(-18, 8));
		textsnake.emplace_back(Point(-18, 9));
		textsnake.emplace_back(Point(-18, 10));

		textsnake.emplace_back(Point(-15, 7));//H
		textsnake.emplace_back(Point(-15, 8));
		textsnake.emplace_back(Point(-15, 9));
		textsnake.emplace_back(Point(-15, 10));
		textsnake.emplace_back(Point(-15, 11));
		textsnake.emplace_back(Point(-14, 9));
		textsnake.emplace_back(Point(-13, 9));
		textsnake.emplace_back(Point(-12, 9));
		textsnake.emplace_back(Point(-11, 7));
		textsnake.emplace_back(Point(-11, 8));
		textsnake.emplace_back(Point(-11, 9));
		textsnake.emplace_back(Point(-11, 10));
		textsnake.emplace_back(Point(-11, 11));
		
		textsnake.emplace_back(Point(-8, 7));//U
		textsnake.emplace_back(Point(-8, 8));
		textsnake.emplace_back(Point(-8, 9));
		textsnake.emplace_back(Point(-8, 10));
		textsnake.emplace_back(Point(-7, 11));
		textsnake.emplace_back(Point(-6, 11));
		textsnake.emplace_back(Point(-5, 7));
		textsnake.emplace_back(Point(-5, 8));
		textsnake.emplace_back(Point(-5, 9));
		textsnake.emplace_back(Point(-5, 10));
		
		textsnake.emplace_back(Point(-16, 14));//——XLX
		textsnake.emplace_back(Point(-15, 14));
		textsnake.emplace_back(Point(-14, 14));
		textsnake.emplace_back(Point(-13, 14));
		textsnake.emplace_back(Point(-12, 14));
		textsnake.emplace_back(Point(-11, 14));

		textsnake.emplace_back(Point(-9, 13));
		textsnake.emplace_back(Point(-9, 15));
		textsnake.emplace_back(Point(-8, 14));
		textsnake.emplace_back(Point(-7, 13));
		textsnake.emplace_back(Point(-7, 15));
		textsnake.emplace_back(Point(-5, 13));
		textsnake.emplace_back(Point(-5, 14));
		textsnake.emplace_back(Point(-5, 15));
		textsnake.emplace_back(Point(-4, 15));
		textsnake.emplace_back(Point(-2, 13));
		textsnake.emplace_back(Point(-2, 15));
		textsnake.emplace_back(Point(-1, 14));
		textsnake.emplace_back(Point(0, 13));
		textsnake.emplace_back(Point(0, 15));
		

		textsnake.emplace_back(Point(-26, 17));//S
		textsnake.emplace_back(Point(-25, 17));
		textsnake.emplace_back(Point(-27, 18));
		textsnake.emplace_back(Point(-26, 19));
		textsnake.emplace_back(Point(-25, 20));
		textsnake.emplace_back(Point(-27, 21));
		textsnake.emplace_back(Point(-26, 21));

		textsnake.emplace_back(Point(-23, 17));//N
		textsnake.emplace_back(Point(-23, 18));
		textsnake.emplace_back(Point(-23, 19));
		textsnake.emplace_back(Point(-23, 20));
		textsnake.emplace_back(Point(-23, 21));
		textsnake.emplace_back(Point(-22, 18));
		textsnake.emplace_back(Point(-21, 19));
		textsnake.emplace_back(Point(-20, 20));
		textsnake.emplace_back(Point(-19, 17));
		textsnake.emplace_back(Point(-19, 18));
		textsnake.emplace_back(Point(-19, 19));
		textsnake.emplace_back(Point(-19, 20));
		textsnake.emplace_back(Point(-19, 21));

		textsnake.emplace_back(Point(-17, 21));//A
		textsnake.emplace_back(Point(-16, 20));
		textsnake.emplace_back(Point(-15, 19));
		textsnake.emplace_back(Point(-14, 18));
		textsnake.emplace_back(Point(-14, 19));
		textsnake.emplace_back(Point(-13, 17));
		textsnake.emplace_back(Point(-13, 19));
		textsnake.emplace_back(Point(-12, 18));
		textsnake.emplace_back(Point(-12, 19));
		textsnake.emplace_back(Point(-11, 19));
		textsnake.emplace_back(Point(-10, 20));
		textsnake.emplace_back(Point(-9, 21));

		textsnake.emplace_back(Point(-7, 17));//K
		textsnake.emplace_back(Point(-7, 18));
		textsnake.emplace_back(Point(-7, 19));
		textsnake.emplace_back(Point(-7, 20));
		textsnake.emplace_back(Point(-7, 21));
		textsnake.emplace_back(Point(-6, 19));
		textsnake.emplace_back(Point(-5, 18));
		textsnake.emplace_back(Point(-5, 20));
		textsnake.emplace_back(Point(-4, 17));
		textsnake.emplace_back(Point(-4, 21));

		textsnake.emplace_back(Point(-2, 17));//E
		textsnake.emplace_back(Point(-2, 18));
		textsnake.emplace_back(Point(-2, 19));
		textsnake.emplace_back(Point(-2, 20));
		textsnake.emplace_back(Point(-2, 21));
		textsnake.emplace_back(Point(-1, 17));
		textsnake.emplace_back(Point(-1, 19));
		textsnake.emplace_back(Point(-1, 21));
		textsnake.emplace_back(Point(0, 17));
		textsnake.emplace_back(Point(0, 19));
		textsnake.emplace_back(Point(0, 21));
	}
	void PrintFirst();
	void PrintSecond();
	void PrintThird();
	void PrintText();
	void ClearText();
	void Action();
private:
	std::deque<Point> startsnake;//开始动画中的蛇
	std::vector<Point> textsnake;//开始动画中的文字
	int speed;//动画的速度
};
#endif // STRATINTERFACE_H