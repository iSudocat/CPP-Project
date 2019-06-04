#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "controller.h"
#include "tools.h"
#include "startinterface.h"
#include "map.h"
#include "snake.h"
#include "food.h"

void Controller::Start()//��ʼ����
{
	SetWindowSize(41, 35);//���ô��ڴ�С
	SetColor(8);//���ÿ�ʼ������ɫ
	StartInterface *start = new StartInterface();//��̬����һ��StartInterface��start
	start->Action();//��ʼ����
	delete start;//�ͷ��ڴ�ռ�

	/*���ùر�λ�ã��������ʾ��ȴ�������������*/
	SetCursorPosition(5, 28);
	SetColor(15);//������ɫ
	std::cout << "Press any key to start... ";
	SetCursorPosition(5, 29);
	system("pause");
}

void Controller::Select()//ѡ�����
{
	/*��ʼ������ѡ��*/
	SetColor(15);
	SetCursorPosition(5, 28);
	std::cout << "                          ";
	SetCursorPosition(5, 29);
	std::cout << "                          ";
	SetCursorPosition(6, 23);
	std::cout << "����ѡ����Ϸ�Ѷȣ�";
	SetCursorPosition(6, 24);
	std::cout << "��(���¼�ѡ��,�س�ȷ��)";
	SetCursorPosition(27, 24);
	SetBackColor();//��һ��ѡ�����ñ���ɫ�Ա�ʾ��ǰѡ��
	std::cout << "��ģʽ";
	SetCursorPosition(27, 26);
	SetColor(15);
	std::cout << "��ͨģʽ";
	SetCursorPosition(27,28);
	std::cout << "����ģʽ";
	SetCursorPosition(27, 30);
	std::cout << "����ģʽ";
	SetCursorPosition(0, 33);
	score = 0;

	/*���·����ѡ��ģ��*/
	int ch;//��¼����ֵ
	key = 1;//��¼ѡ�����ʼѡ���һ��
	bool flag = false;//��¼�Ƿ����Enter����ǣ���ʼ��Ϊ��
	while ((ch = _getch()))
	{
		switch (ch)//��������
		{
		case 72://UP�Ϸ����
			if (key > 1)//����ʱѡ����Ϊ��һ��ʱ��UP�Ϸ������Ч
			{
				switch (key)
				{
				case 2:
					SetCursorPosition(27, 24);//����ѡ�������ñ���ɫ
					SetBackColor();
					std::cout << "��ģʽ";

					SetCursorPosition(27, 26);//����ѡ����ȡ������ɫ
					SetColor(15);
					std::cout << "��ͨģʽ";

					--key;
					break;
				case 3:
					SetCursorPosition(27, 26);
					SetBackColor();
					std::cout << "��ͨģʽ";

					SetCursorPosition(27, 28);
					SetColor(15);
					std::cout << "����ģʽ";

					--key;
					break;
				case 4:
					SetCursorPosition(27, 28);
					SetBackColor();
					std::cout << "����ģʽ";

					SetCursorPosition(27, 30);
					SetColor(15);
					std::cout << "����ģʽ";

					--key;
					break;
				}
			}
			break;

		case 80://DOWN�·����
			if (key < 4)
			{
				switch (key)
				{
				case 1:
					SetCursorPosition(27, 26);
					SetBackColor();
					std::cout << "��ͨģʽ";
					SetCursorPosition(27, 24);
					SetColor(15);
					std::cout << "��ģʽ";

					++key;
					break;
				case 2:
					SetCursorPosition(27, 28);
					SetBackColor();
					std::cout << "����ģʽ";
					SetCursorPosition(27, 26);
					SetColor(15);
					std::cout << "��ͨģʽ";

					++key;
					break;
				case 3:
					SetCursorPosition(27, 30);
					SetBackColor();
					std::cout << "����ģʽ";
					SetCursorPosition(27, 28);
					SetColor(15);
					std::cout << "����ģʽ";

					++key;
					break;
				}
			}
			break;

		case 13://Enter�س���
			flag = true;
			break;
		default://��Ч����
			break;
		}
		if (flag) break;//����Enter�س���ȷ�ϣ��˳��������ѭ��

		SetCursorPosition(0, 35);//������������½ǣ�����ر���˸Ӱ����Ϸ����
	}

	switch (key)//������ѡѡ�������ߵ��ƶ��ٶȣ�speedֵԽС���ٶ�Խ��
	{
	case 1:
		speed = 135;
		break;
	case 2:
		speed = 100;
		break;
	case 3:
		speed = 60;
		break;
	case 4:
		speed = 30;
		break;
	default:
		break;
	}
}

void Controller::DrawGame()//������Ϸ����
{
	system("cls");//����

	/*���Ƶ�ͼ*/
	SetColor(7);
	Map *init_map = new Map();
	init_map->PrintInitmap();
	delete init_map;

	/*���Ʋ����*/
	SetColor(15);
	SetCursorPosition(33, 2);
	std::cout << "��Greedy Snake";
	SetCursorPosition(32, 4);
	std::cout << "WHUer's ̰���ߡ�";
	SetCursorPosition(31, 7);
	std::cout << "���Ѷȣ�";
	SetColor(8);
	SetCursorPosition(1, 31);
	std::cout << "������Ϊһֻ��ǿ�����㡢���ǡ����µ�С�ߣ�����Ҫ�Ծ�����Ĺ��ӡ���";
	SetCursorPosition(1, 32);
	std::cout << "�����Ե�����ʱ���Ĵ���ӻ�����õ�����ķ���~��Ѽ~~~��";
	SetCursorPosition(36, 8);
	SetColor(15);
	switch (key)
	{
	case 1:
		std::cout << "��ģʽ";
		break;
	case 2:
		std::cout << "��ͨģʽ";
		break;
	case 3:
		std::cout << "����ģʽ";
		break;
	case 4:
		std::cout << "����ģʽ";
		break;
	default:
		break;
	}
	SetCursorPosition(31, 11);
	std::cout << "��÷֣�";
	SetCursorPosition(37, 12);
	std::cout << "     0";
	SetCursorPosition(32, 15);
	std::cout << " ��������ƶ���";
	SetCursorPosition(32, 17);
	std::cout << " ��ESC����ͣ��";
}

int Controller::PlayGame()//��Ϸ����ѭ��
{
	/*��ʼ���ߺ�ʳ��*/
	Snake *csnake = new Snake();
	Food *cfood = new Food();
	SetColor(10);
	csnake->InitSnake();
	srand((unsigned)time(NULL));//������������ӣ����û�� ʳ��ĳ���λ�ý���̶�
	cfood->DrawFood(*csnake);

	/*��Ϸѭ��*/
	while (csnake->OverEdge() && csnake->HitItself()) //�ж��Ƿ�ײǽ��ײ���������Ƿ�������
	{
		/*����ѡ��˵�*/
		if (!csnake->ChangeDirection()) //��Esc��ʱ
		{
			int tmp = Menu();//���Ʋ˵������õ�����ֵ
			switch (tmp)
			{
			case 1://������Ϸ
				break;

			case 2://���¿�ʼ
				delete csnake;
				delete cfood;
				return 1;//��1��ΪPlayGame�����ķ���ֵ���ص�Game�����У���ʾ���¿�ʼ

			case 3://�˳���Ϸ
				delete csnake;
				delete cfood;
				return 2;//��2��ΪPlayGame�����ķ���ֵ���ص�Game�����У���ʾ�˳���Ϸ

			default:
				break;
			}
		}

		if (csnake->GetFood(*cfood)) //�Ե�ʳ��
		{
			csnake->Move();//������
			UpdateScore(1);//���·�����1Ϊ����Ȩ��
			RewriteScore();//���»��Ʒ���
			cfood->DrawFood(*csnake);//������ʳ��
		}
		else
		{
			csnake->NormalMove();//�������ƶ�
		}

		if (csnake->GetBigFood(*cfood)) //�Ե���ʱʳ��
		{
			csnake->Move();
			UpdateScore(cfood->GetProgressBar() / 5);//����������ʱʳ�������ȷ��
			RewriteScore();
		}

		if (cfood->GetBigFlag()) //�����ʱ����ʱʳ���˸��
		{
			cfood->FlashBigFood();
		}

		Sleep(speed);//�����ߵ��ƶ�Ч��
	}

	/*������*/
	delete csnake;//�ͷŷ�����ڴ�ռ�
	delete cfood;
	int tmp = GameOver();//������Ϸ�������棬��������ѡ��
	switch (tmp)
	{
	case 1:
		return 1;//���¿�ʼ
	case 2:
		return 2;//�˳���Ϸ
	default:
		return 2;
	}
}

void Controller::UpdateScore(const int& tmp)//���·���
{
	score += key * 10 * tmp;//���÷���������Ϸ�Ѷȼ����˵Ĳ���tmpȷ��
}

void Controller::RewriteScore()//�ػ����
{
	/*Ϊ���ַ���β�����룬������������Ϊ6λ�����㵱ǰ����λ������ʣ��λ���ÿո�ȫ�����������*/
	SetCursorPosition(37,12);
	SetColor(15);
	int bit = 0;
	int tmp = score;
	while (tmp != 0)
	{
		++bit;
		tmp /= 10;
	}
	for (int i = 0; i < (6 - bit); ++i)
	{
		std::cout << " ";
	}
	std::cout << score;
}

int Controller::Menu()//ѡ��˵�
{
	/*���Ʋ˵�*/
	SetColor(11);
	SetCursorPosition(32, 19);
	std::cout << "�˵���";
	Sleep(100);
	SetCursorPosition(34, 21);
	SetBackColor();
	std::cout << "������Ϸ";
	Sleep(100);
	SetCursorPosition(34, 23);
	SetColor(11);
	std::cout << "���¿�ʼ";
	Sleep(100);
	SetCursorPosition(34, 25);
	std::cout << "�˳���Ϸ";
	SetCursorPosition(0, 31);

	/*ѡ�񲿷�*/
	int ch;
	int tmp_key = 1;
	bool flag = false;
	while ((ch = _getch()))
	{
		switch (ch)
		{
		case 72://UP
			if (tmp_key > 1)
			{
				switch (tmp_key)
				{
				case 2:
					SetCursorPosition(34, 21);
					SetBackColor();
					std::cout << "������Ϸ";
					SetCursorPosition(34, 23);
					SetColor(11);
					std::cout << "���¿�ʼ";

					--tmp_key;
					break;
				case 3:
					SetCursorPosition(34, 23);
					SetBackColor();
					std::cout << "���¿�ʼ";
					SetCursorPosition(34, 25);
					SetColor(11);
					std::cout << "�˳���Ϸ";

					--tmp_key;
					break;
				}
			}
			break;

		case 80://DOWN
			if (tmp_key < 3)
			{
				switch (tmp_key)
				{
				case 1:
					SetCursorPosition(34, 23);
					SetBackColor();
					std::cout << "���¿�ʼ";
					SetCursorPosition(34, 21);
					SetColor(11);
					std::cout << "������Ϸ";

					++tmp_key;
					break;
				case 2:
					SetCursorPosition(34, 25);
					SetBackColor();
					std::cout << "�˳���Ϸ";
					SetCursorPosition(34, 23);
					SetColor(11);
					std::cout << "���¿�ʼ";

					++tmp_key;
					break;
				}
			}
			break;

		case 13://Enter
			flag = true;
			break;

		default:
			break;
		}

		if (flag)
		{
			break;
		}
		SetCursorPosition(0, 31);
	}

	if (tmp_key == 1) //ѡ�������Ϸ���򽫲˵�����
	{
		SetCursorPosition(32, 19);
		std::cout << "      ";
		SetCursorPosition(34, 21);
		std::cout << "        ";
		SetCursorPosition(34, 23);
		std::cout << "        ";
		SetCursorPosition(34, 25);
		std::cout << "        ";
	}
	return tmp_key;
}

void Controller::Game()//��Ϸһ��ѭ��
{
	Start();//��ʼ����
	while (true)//��Ϸ����Ϊһ����ѭ����ֱ���˳���Ϸʱѭ������
	{
		Select();//ѡ�����
		DrawGame();//������Ϸ����
		int tmp = PlayGame();//������Ϸѭ���������¿�ʼ���˳���Ϸʱ������ѭ��������ֵ��tmp
		if (tmp == 1) //����ֵΪ1ʱ���¿�ʼ��Ϸ
		{
			system("cls");
			continue;
		}
		else if (tmp == 2) //����ֵΪ2ʱ�˳���Ϸ
		{
			break;
		}
		else
		{
			break;
		}
	}
}

int Controller::GameOver()//��Ϸ��������
{
	/*������Ϸ��������*/
	Sleep(500);
	SetColor(15);
	SetCursorPosition(10, 8);
	std::cout << "��������������������������������������������";
	Sleep(30);
	SetCursorPosition(9, 9);
	std::cout << " ��               Game Over !!!              ��";
	Sleep(30);
	SetCursorPosition(9, 10);
	std::cout << " ��                                          ��";
	Sleep(30);
	SetCursorPosition(9, 11);
	std::cout << " ��              ���ź��������              ��";
	Sleep(30);
	SetCursorPosition(9, 12);
	std::cout << " ��                                          ��";
	Sleep(30);
	SetCursorPosition(9, 13);
	std::cout << " ��             ��ķ���Ϊ��                 ��";
	SetCursorPosition(24, 13);
	std::cout << score;
	Sleep(30);
	SetCursorPosition(9, 14);
	std::cout << " ��                                          ��";
	Sleep(30);
	SetCursorPosition(9, 15);
	std::cout << " ��   �Ƿ�����һ�֣�                         ��";
	Sleep(30);
	SetCursorPosition(9, 16);
	std::cout << " ��                                          ��";
	Sleep(30);
	SetCursorPosition(9, 17);
	std::cout << " ��                                          ��";
	Sleep(30);
	SetCursorPosition(9, 18);
	std::cout << " ��    ���ѧ����������        ��ô��Һ���  ��";
	Sleep(30);
	SetCursorPosition(9, 19);
	std::cout << " ��                                          ��";
	Sleep(30);
	SetCursorPosition(9, 20);
	std::cout << " ��                                          ��";
	Sleep(30);
	SetCursorPosition(10, 21);
	std::cout << "��������������������������������������������";

	Sleep(100);
	SetCursorPosition(12, 18);
	SetBackColor();
	std::cout << "���ѧ����������";
	SetCursorPosition(0, 31);

	/*ѡ�񲿷�*/
	int ch;
	int tmp_key = 1;
	bool flag = false;
	while ((ch = _getch()))
	{
		switch (ch)
		{
		case 75://LEFT
			if (tmp_key > 1)
			{
				SetCursorPosition(12, 18);
				SetBackColor();
				std::cout << "���ѧ����������";
				SetCursorPosition(24, 18);
				SetColor(11);
				std::cout << "��ô��Һ���";
				--tmp_key;
			}
			break;

		case 77://RIGHT
			if (tmp_key < 2)
			{
				SetCursorPosition(24, 18);
				SetBackColor();
				std::cout << "��ô��Һ���";
				SetCursorPosition(12, 18);
				SetColor(11);
				std::cout << "���ѧ����������";
				++tmp_key;
			}
			break;

		case 13://Enter
			flag = true;
			break;

		default:
			break;
		}

		SetCursorPosition(0, 31);
		if (flag) {
			break;
		}
	}

	SetColor(11);
	switch (tmp_key)
	{
	case 1:
		return 1;//���¿�ʼ
	case 2:
		return 2;//�˳���Ϸ
	default:
		return 1;
	}
}