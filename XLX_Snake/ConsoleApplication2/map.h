#ifndef MAP_H
#define MAP_H

#include <vector>
#include "point.h"

class Map
{
public:
	Map()//Ĭ�Ϲ��캯�����������θ���ѹ��initmap
	{
		initmap.emplace_back(Point(1, 1));
		initmap.emplace_back(Point(2, 1));
		initmap.emplace_back(Point(3, 1));
		initmap.emplace_back(Point(4, 1));
		initmap.emplace_back(Point(5, 1));
		initmap.emplace_back(Point(6, 1));
		initmap.emplace_back(Point(7, 1));
		initmap.emplace_back(Point(8, 1));
		initmap.emplace_back(Point(9, 1));
		initmap.emplace_back(Point(10, 1));
		initmap.emplace_back(Point(11, 1));
		initmap.emplace_back(Point(12, 1));
		initmap.emplace_back(Point(13, 1));
		initmap.emplace_back(Point(14, 1));
		initmap.emplace_back(Point(15, 1));
		initmap.emplace_back(Point(16, 1));
		initmap.emplace_back(Point(17, 1));
		initmap.emplace_back(Point(18, 1));
		initmap.emplace_back(Point(19, 1));
		initmap.emplace_back(Point(20, 1));
		initmap.emplace_back(Point(21, 1));
		initmap.emplace_back(Point(22, 1));
		initmap.emplace_back(Point(23, 1));
		initmap.emplace_back(Point(24, 1));
		initmap.emplace_back(Point(25, 1));
		initmap.emplace_back(Point(26, 1));
		initmap.emplace_back(Point(27, 1));
		initmap.emplace_back(Point(28, 1));
		initmap.emplace_back(Point(29, 1));
		initmap.emplace_back(Point(30, 1));
		initmap.emplace_back(Point(1, 2));
		initmap.emplace_back(Point(30, 2));
		initmap.emplace_back(Point(1, 3));
		initmap.emplace_back(Point(30, 3));
		initmap.emplace_back(Point(1, 4));
		initmap.emplace_back(Point(30, 4));
		initmap.emplace_back(Point(1, 5));
		initmap.emplace_back(Point(30, 5));
		initmap.emplace_back(Point(1, 6));
		initmap.emplace_back(Point(30, 6));
		initmap.emplace_back(Point(1, 7));
		initmap.emplace_back(Point(30, 7));
		initmap.emplace_back(Point(1, 8));
		initmap.emplace_back(Point(30, 8));
		initmap.emplace_back(Point(1, 9));
		initmap.emplace_back(Point(30, 9));
		initmap.emplace_back(Point(1, 10));
		initmap.emplace_back(Point(30, 10));
		initmap.emplace_back(Point(1, 11));
		initmap.emplace_back(Point(30, 11));
		initmap.emplace_back(Point(1, 12));
		initmap.emplace_back(Point(30, 12));
		initmap.emplace_back(Point(1, 13));
		initmap.emplace_back(Point(30, 13));
		initmap.emplace_back(Point(1, 14));
		initmap.emplace_back(Point(30, 14));
		initmap.emplace_back(Point(1, 15));
		initmap.emplace_back(Point(30, 15));
		initmap.emplace_back(Point(1, 16));
		initmap.emplace_back(Point(30, 16));
		initmap.emplace_back(Point(1, 17));
		initmap.emplace_back(Point(30, 17));
		initmap.emplace_back(Point(1, 18));
		initmap.emplace_back(Point(30, 18));
		initmap.emplace_back(Point(1, 19));
		initmap.emplace_back(Point(30, 19));
		initmap.emplace_back(Point(1, 20));
		initmap.emplace_back(Point(30, 20));
		initmap.emplace_back(Point(1, 21));
		initmap.emplace_back(Point(30, 21));
		initmap.emplace_back(Point(1, 22));
		initmap.emplace_back(Point(30, 22));
		initmap.emplace_back(Point(1, 23));
		initmap.emplace_back(Point(30, 23));
		initmap.emplace_back(Point(1, 24));
		initmap.emplace_back(Point(30, 24));
		initmap.emplace_back(Point(1, 25));
		initmap.emplace_back(Point(30, 25));
		initmap.emplace_back(Point(1, 26));
		initmap.emplace_back(Point(30, 26));
		initmap.emplace_back(Point(1, 27));
		initmap.emplace_back(Point(30, 27));
		initmap.emplace_back(Point(1, 28));
		initmap.emplace_back(Point(30, 28));
		initmap.emplace_back(Point(1, 29));
		initmap.emplace_back(Point(30, 29));
		initmap.emplace_back(Point(1, 30));
		initmap.emplace_back(Point(2, 30));
		initmap.emplace_back(Point(3, 30));
		initmap.emplace_back(Point(4, 30));
		initmap.emplace_back(Point(5, 30));
		initmap.emplace_back(Point(6, 30));
		initmap.emplace_back(Point(7, 30));
		initmap.emplace_back(Point(8, 30));
		initmap.emplace_back(Point(9, 30));
		initmap.emplace_back(Point(10, 30));
		initmap.emplace_back(Point(11, 30));
		initmap.emplace_back(Point(12, 30));
		initmap.emplace_back(Point(13, 30));
		initmap.emplace_back(Point(14, 30));
		initmap.emplace_back(Point(15, 30));
		initmap.emplace_back(Point(16, 30));
		initmap.emplace_back(Point(17, 30));
		initmap.emplace_back(Point(18, 30));
		initmap.emplace_back(Point(19, 30));
		initmap.emplace_back(Point(20, 30));
		initmap.emplace_back(Point(21, 30));
		initmap.emplace_back(Point(22, 30));
		initmap.emplace_back(Point(23, 30));
		initmap.emplace_back(Point(24, 30));
		initmap.emplace_back(Point(25, 30));
		initmap.emplace_back(Point(26, 30));
		initmap.emplace_back(Point(27, 30));
		initmap.emplace_back(Point(28, 30));
		initmap.emplace_back(Point(29, 30));
		initmap.emplace_back(Point(30, 30));

		initmap.emplace_back(Point(7, 6));
		initmap.emplace_back(Point(7, 2));
		initmap.emplace_back(Point(7, 3));
		initmap.emplace_back(Point(7, 4));
		initmap.emplace_back(Point(7, 5));
		initmap.emplace_back(Point(7, 7));
		initmap.emplace_back(Point(7, 8));
		initmap.emplace_back(Point(7, 9));
		initmap.emplace_back(Point(7, 10));

		initmap.emplace_back(Point(26, 22));
		initmap.emplace_back(Point(27, 22));
		initmap.emplace_back(Point(28, 22));
		initmap.emplace_back(Point(29, 22));
		initmap.emplace_back(Point(25, 22));

		initmap.emplace_back(Point(26, 4));
		initmap.emplace_back(Point(27, 4));
		initmap.emplace_back(Point(25, 4));
		initmap.emplace_back(Point(24, 4));
		initmap.emplace_back(Point(23, 4));
		initmap.emplace_back(Point(22, 4));

		initmap.emplace_back(Point(2, 27));
		initmap.emplace_back(Point(3, 27));
		initmap.emplace_back(Point(4, 27));
		initmap.emplace_back(Point(4, 28));
		initmap.emplace_back(Point(4, 29));

	}
	void PrintInitmap();//���Ƴ�ʼ��ͼ
private:
	std::vector<Point> initmap;//�����ʼ��ͼ
	/*Map����Զ�����ֵ�ͼ��ֻ�轫��ʾ��ͼ�ĸ����㱣������Ӧ��map�У�����Snake����������Ӧ�ж�ײǽ��������
	std::vector<Point> map1;
	std::vector<Point> map2;
	*/
};
#endif // MAP_H
