#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <atlstr.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include "Game.h"

#define ID_TIMER 1
#define MAX_PATH 1024

/////////////////ȫ�ֱ���/////////////////////////////
HWND hwnd;     //���ھ������
UINT timer_id = 0;    //�����ʱ��ID
byte g_panel[ROWS][COLS] = { 0 };
bool isPause = false;    //��ͣ��ʶ
bool isover = false;	//��Ϸ������ʶ
int** block_new;
string username;
LPTSTR iniscore = new char[10];
string path = "";
string inipath = "";
int maxscore = 0;
////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	//�ص�����ԭ��
void SpeechControl(HDC hdc);

Game Tetris;
Block cur_block;

void string2tchar(std::string& src, TCHAR* buf)
{
#ifdef UNICODE  
	_stprintf_s(buf, MAX_PATH, _T("%S"), src.c_str());//%S���ַ�  
#else  
	_stprintf_s(buf, MAX_PATH, _T("%s"), src.c_str());//%s���ַ�
#endif
}


int main()
{
	std::cout << "��ӭ��������˹���飡" << endl;
	std::cout << "���̿��ƣ�" << endl;
	std::cout << "������ת  ��:�����½�  ���������ƶ�  ���������ƶ�  �ո���ͣ" << endl << endl;
	std::cout << "�������ƣ�" << endl;
	std::cout << "��Delete���ɽ�������ʶ����ƣ����Σ�����������Ҫ�����ſ�ʹ�á�" << endl;
	std::cout << "�����������x��/����x��/����x��/��ת" << endl << endl;
	std::cout << "�����������û�����" << endl;
	
	std::cin >> username;
	string tpath = _pgmptr;
	int	where = tpath.rfind("\\");
	path = tpath.substr(0, where);
	inipath = path + "\\Tetris.ini";
	cout << inipath<< endl;

	GetPrivateProfileString("Score", username.c_str(), "-1", iniscore, 10, inipath.c_str());

	if ((string)iniscore == "-1")
	{
		WritePrivateProfileString("Score", username.c_str(), "0", inipath.c_str());
		maxscore = 0;
	}
	else
	{
		maxscore = atoi(iniscore);
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);//ȡ�ó�����
	TCHAR szAppName[] = TEXT("teris");
	MSG msg;
	WNDCLASS wc;//����WNDCLASS��Ķ���wc �·���������
	wc.style = CS_HREDRAW | CS_VREDRAW;// ��������
	wc.lpfnWndProc = WndProc;//���ڴ�����
	wc.cbClsExtra = 0;//������չ
	wc.cbWndExtra = 0;//����ʵ����չ
	wc.hInstance = hInstance;//���ھ��
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//����ͼ��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//���ڹ��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//���ڱ���
	wc.lpszMenuName = NULL;//���ڲ˵�
	wc.lpszClassName = szAppName;//��������

	if (!RegisterClass(&wc))//ע�ᴰ��
	{
		printf("RegisterClass occur errors!");
		return 0;
	}
	hwnd = CreateWindow //ʵ��������
	(
		szAppName,
		TEXT("����˹����"),
		WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, SW_SHOW);//��ʾ����
	UpdateWindow(hwnd);//���´��ڣ�����һЩ��ʼ��Ϣ������GDI��

/*************************************************************************************************
	ÿ�����ڶ���һ���Լ��Ĵ�����Ϣ���У�����������Ҫ����Ĵ�����Ϣ��
	���������ǽ���һ�����ڵ���Ϣѭ�����ڴ����˳�֮ǰһֱ���ڹ���״̬��
	���ϵĴ���Ϣ�����л�ȡ��Ϣ
	��ȡ��Ϣ�ĺ����ǣ�
	BOOL GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax );
	�ú�����Ҫ��while()��ִ�У���while(GetMessage(&msg, NULL, 0, 0)),������򵥵�д��

	��GetMessage()��ȡ����Ϣ���������Ϣ����Ҫ���д�������TranslateMessage(MSG *lpMsg)��
	��������Ĺ����ǽ��������Ϣת�����ַ���Ϣͨ��˵���Ƿ�����Ϣ��

	TranslateMessage()֮�󣬻���Ҫ����Ϣ�����ɷ�����ÿ����Ϣ���͵���Ӧ����Ҫ�����Ŀ�괰�ڻ�ؼ���
	�ɷ�������LRESULT DispatchMessage(MSG *lpMsg);
*************************************************************************************************/

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	std::cout << username + "����ӭ�´�������" << endl << endl;

	GetPrivateProfileString("Score", username.c_str(), "0", iniscore, 10, inipath.c_str());
	if (Tetris.score_get() > std::stoi((string)iniscore))
	{
		string s = to_string(maxscore);
		WritePrivateProfileString("Score", username.c_str(), s.c_str(), inipath.c_str());
	}
	
	return 0;
	//return msg.wParam;
}

/*************************************************************************************************
	�������ڵ���Ҫ�����Ѿ�����ˣ��������ǻ���Ҫ����һ�����ڵĻص�������
	�ڳ�ʼ���������ʱ������Ҫ������ص������ģ�����ص���������windowsϵͳ���лص���
	�ص������ĸ�ʽ���밴�����¸�ʽ��
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	message����ϢID
	wParam����������Ϣ����ʱ��ֵΪ���°������������
	lParam���洢���������״̬��Ϣ
*************************************************************************************************/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		MoveWindow(hwnd, 600, 200, CELL * COLS + 20, CELL * ROWS + 100, FALSE); 
		srand(time(NULL));
		Tetris.ExportBlock();
		timer_id = SetTimer(hwnd, ID_TIMER, Tetris.interval_base_get() - Tetris.level_get() * Tetris.interval_unit_get(), NULL);
		break;
	case WM_TIMER:
	{
		if (isover == false)
		{
			hdc = GetDC(hwnd);
			Tetris.DoDownShift(hdc);
			if (Tetris.score_get()> maxscore)
			{
				maxscore = Tetris.score_get();
			}

			string out = "��ǰ�ȼ���" + std::to_string(Tetris.level_get()) + " �÷�:" + std::to_string(Tetris.score_get()) + " ��߷�:" + std::to_string(maxscore);
			TCHAR szText[30];
			string2tchar(out, szText);
			SetTextColor(hdc, RGB(30, 144, 255));
			TextOut(hdc, CELL * COLS / 2 - 100, CELL * ROWS + 20, szText, strlen(szText));
			ReleaseDC(hwnd, hdc);
			break;
		}
		break;
	}
		
	case WM_KEYDOWN:
		if (isover == false) 
		{
			hdc = GetDC(hwnd);
			switch (wParam)
			{
			case VK_DELETE:		//��Delete����������ʶ�𣨵��Σ�
			{   //���Ҫ��case���涨���������Ҫ��{}������
				//Ϊ��ֹʶ�������ɴ��ڿ��٣���ʹ�ö��߳�ִ��ʶ����
				std::thread t(SpeechControl, hdc); //����һ���߳�t������1Ϊ���еĺ���������2Ϊ���ݵĲ������ɿգ�
				t.detach(); //��ʾ���߳��ں�̨��������ȴ����̵߳ĺ������أ�����ִ�к�������
				break;
			}
			case VK_LEFT:       //����
				if (!isPause) Tetris.DoLeftShift(hdc);
				break;
			case VK_RIGHT:       //����
				if (!isPause) Tetris.DoRightShift(hdc);
				break;
			case VK_UP:        //ת��
				if (!isPause) Tetris.DoRedirection(hdc);
				break;
			case VK_DOWN:       //����
				if (!isPause) Tetris.DoAccelerate(hdc);
				break;
			case VK_SPACE:  //��ͣ
				isPause = !isPause;
				if (isPause)
				{
					if (timer_id) KillTimer(hwnd, timer_id);
					timer_id = 0;
				}
				else
				{
					timer_id = SetTimer(hwnd, ID_TIMER, Tetris.interval_base_get() - Tetris.level_get() * Tetris.interval_unit_get(), FALSE);
				}
				break;
			}
			ReleaseDC(hwnd, hdc);
			
		}
		break;
		
	case WM_PAINT:
	{
		if (isover == false)
		{
			hdc = BeginPaint(hwnd, &ps);
			Tetris.DrawPanel(hdc);   //������Ϸ���
			Tetris.RefreshPanel(hdc);  //ˢ��


			EndPaint(hwnd, &ps);
			break;
		}
		break;
	}
		
	case WM_DESTROY:
		if (timer_id)	KillTimer(hwnd, timer_id);
		if (block_new != NULL)	Tetris.DeleteBlock(block_new);
		PostQuitMessage(0);
		
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}



void Game::DrawPanel(HDC hdc)    //������Ϸ���
{
	color(13);
	int x, y;
	RECT rect;

	for (y = 0; y < ROWS; y++)
	{
		for (x = 0; x < COLS; x++)
		{
			//�趨���α߿�Χ
			rect.top = y * CELL + 1;
			rect.bottom = (y + 1) * CELL - 1;
			rect.left = x * CELL + 1;
			rect.right = (x + 1) * CELL - 1;
			//���ƾ��α߿�
			FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}
	}
}

void Game::DoDownShift(HDC hdc)    //����
{
	if (block_new == NULL) return;

	//�ж��Ƿ񵽴�ײ�
	if (IsTouchBottom(hdc))   //���ײ�
	{
		if (isover == false)
		{
			//���д���
			ClearRow(hdc);
			ExportBlock();  //�����һ������
		}
	
	}

	if (isover == false)
	{
		cur_block.GoDown();
		//cur_top++;
		RefreshPanel(hdc);
	}
}

void Game::DoLeftShift(HDC hdc)    //����
{
	int x, y;
	if (block_new == NULL) return;

	if (0 == cur_block.GetCurLeft() ) return;
	
	if (cur_block.GetCurTop()  < 0) return; //����û��������ʾǰ����������
	for (y = 0; y < cur_block.GetHeight() ; y++)
	{
		for (x = 0; x < cur_block.GetWidth(); x++)    
		{
			if (block_new[y][x] == 1)	//����߿�ʼɨ�裬�ҵ���y������ߵ�ʵ�ķ���
			{
				//�жϵ�ǰ����������������һ�������Ƿ�Ϊʵ�ģ��Ǿʹ�����������
				if (g_panel[cur_block.GetCurTop()  + y][cur_block.GetCurLeft () + x - 1] == 1) return;


				break;  //��y�д�������һ��ʵ�ķ�������û�з��飬����ɨ����һ��
			}
		}
	}
	//cur_left--;
	cur_block.GoLeft();
	RefreshPanel(hdc);
}

void Game::DoRightShift(HDC hdc)    //����
{
	int x, y;
	if (block_new == NULL) return;

	if (COLS - cur_block.GetWidth () == cur_block.GetCurLeft ()) return;
	if (cur_block.GetCurTop () < 0) return;  //����������ʾǰ��������
	for (y = 0; y < cur_block.GetHeight (); y++)
	{
		for (x = cur_block.GetWidth () - 1; x >= 0; x--)   
		{
			if (block_new[y][x] == 1)	//���ұ߿�ʼɨ�裬�ҵ���y�����ұߵ�ʵ�ķ���
			{
				//�жϵ�ǰ������������ұ�һ�������Ƿ�Ϊʵ�ģ��Ǿʹ�����������
				if (g_panel[cur_block.GetCurTop () + y][cur_block.GetCurLeft () + x + 1]) return;

				break;  //��y�д�������һ��ʵ�ķ�����ұ�û�з��飬����ɨ����һ��
			}
		}
	}
	//cur_left++;
	cur_block.GoRight();
	RefreshPanel(hdc);
}

void Game::DoRedirection(HDC hdc)   //�ı䷽��
{
	int i, j;
	//byte* temp = NULL;
	if (block_new == NULL) return;
	if (cur_block.GetCurTop () < 0) return;  //����������ʾǰ����ת��

	int** block_temp = NULL;
	block_temp = new int* [cur_block.GetWidth()];
	for (int i = 0; i < cur_block.GetWidth(); i++)
	{
		block_temp[i] = new int[cur_block.GetHeight()];
	}


	for (i = 0; i < cur_block.GetWidth (); i++)
	{
		for (j = 0; j < cur_block.GetHeight (); j++)
		{
			//temp[i][j]=block[height_block-j-1][i];block_new[j][i]
			block_temp[i][j] = block_new[j][cur_block.GetWidth() - i - 1];
		}
	}

	

	//���¶�λ����
	int incHeight = cur_block.GetWidth () - cur_block.GetHeight ();
	int incWidth = cur_block.GetHeight() - cur_block.GetWidth();
	int temp_cur_top = cur_block.GetCurTop () - incHeight / 2;
	int temp_cur_left = cur_block.GetCurLeft () - incWidth / 2;

	//system("cls");
	//printf("temp_top=%d, temp_left=%d",temp_cur_top,temp_cur_left);

	//�жϵ�ǰ�ռ��Ƿ��㹻�÷���ı䷽��
	int max_len = max(cur_block.GetWidth (), cur_block.GetHeight ());
	//��ֹ�±����Խ��
	if (temp_cur_top + max_len - 1 >= ROWS || temp_cur_left < 0 || temp_cur_left + max_len - 1 >= COLS)
	{
		Tetris.DeleteBlocktemp(block_temp, cur_block.GetWidth());  //�˳�ǰ�������ͷ��ڴ�
		return;
	}
	for (i = 0; i < max_len; i++)
	{
		for (j = 0; j < max_len; j++)
		{
			//ת������Ŀռ������ѱ�ռ�õ�ʵ�ķ�����ڣ���ת��ʧ��
			if (g_panel[temp_cur_top + i][temp_cur_left + j])
			{
				Tetris.DeleteBlocktemp(block_temp,cur_block.GetWidth());  //�˳�ǰ�������ͷ��ڴ�
				return;
			}
		}
	}
	if(block_new!=NULL)
	Tetris.DeleteBlock(block_new);

	block_new = new int* [cur_block.GetWidth()];
	for (int i = 0; i < cur_block.GetWidth(); i++)
	{
		block_new[i] = new int[cur_block.GetHeight()];
	}

	//����ʱ������ֵ����block
	for (i = 0; i < cur_block.GetWidth(); i++)
	{
		for (j = 0; j < cur_block.GetHeight(); j++)
		{
			block_new[i][j] = block_temp[i][j];
		}
	}

	//���轻������
	cur_block.SetBlock(temp_cur_top, temp_cur_left, cur_block.GetHeight(), cur_block.GetWidth(), cur_block.GetRed(), cur_block.GetGreen(), cur_block.GetBlue());

	Tetris.DeleteBlocktemp(block_temp, cur_block.GetHeight());  //�ͷ�Ϊ��ʱ����������ڴ�
	RefreshPanel(hdc);
}

void Game::DoAccelerate(HDC hdc)    //����
{
	if (block_new == NULL) return;

	if (IsTouchBottom(hdc))
	{
		//���д���
		ClearRow(hdc);
		ExportBlock();
	}
	//cur_top++;
	cur_block.GoDown();
	RefreshPanel(hdc);
}

bool Game::IsTouchBottom(HDC hdc)	//���׼��
{
	int x, y;
	int i, j;

	if (block_new == NULL) return false;
	if (ROWS == cur_block.GetCurTop () + cur_block.GetHeight ())
	{
		//�̶�����
		for (i = 0; i < cur_block.GetHeight (); i++)
		{
			for (j = 0; j < cur_block.GetWidth (); j++)
			{
				//block_new[y][x] = 1  block_new[i][j]
				if (block_new[i][j])
					g_panel[cur_block.GetCurTop () + i][cur_block.GetCurLeft () + j] = 1;
			}
		}
		return true;
	}

	for (y = cur_block.GetHeight () - 1; y >= 0; y--)    //�ӵ��п�ʼɨ��
	{
		//�жϵ�һ��ʵ�ķ�����������ڽӵ��·������Ƿ�Ϊʵ�ģ��Ǿʹ����Ѿ�����ײ�
		for (x = 0; x < cur_block.GetWidth (); x++)
		{
			if (block_new[y][x])
			{
				if (cur_block.GetCurTop () + y + 1 < 0) return false;
				if (g_panel[cur_block.GetCurTop ()+ y + 1][cur_block.GetCurLeft () + x])
				{
					//�ж��Ƿ�gameover
					if (cur_block.GetCurTop () <= 0)
					{
						isover = true;
						if (timer_id)
						{
							KillTimer(hwnd, timer_id);
							timer_id = 0;
						}
						if (block_new != NULL)	Tetris.DeleteBlock(block_new);
						MessageBox(hwnd, TEXT("��Ϸ����"), TEXT("��Ϸ������"), MB_OK | MB_ICONEXCLAMATION);
						//SendMessage(hwnd, WM_CLOSE, 0, 0);
						PostQuitMessage(0);
						return false;
						
					}
					
					//�̶�����
					for (i = 0; i < cur_block.GetHeight (); i++)
					{
						for (j = 0; j < cur_block.GetWidth (); j++)
						{
							if (block_new[i][j]) 
								g_panel[cur_block.GetCurTop() + i][cur_block.GetCurLeft() + j] = 1;
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

void Game::ClearRow(HDC hdc)      //����
{
	int i, j, k;
	int count = 0;  //���д���
	bool isFilled;
	//���д���
	for (i = ROWS - 1; i >= 0; i--)
	{
		isFilled = true;
		for (j = 0; j < COLS; j++)
		{
			if (!g_panel[i][j])
			{
				isFilled = false;
				break;
			}
		}
		if (isFilled)
		{
			for (j = 0; j < COLS; j++)
			{
				g_panel[i][j] = 0;
			}
			//���з���������
			for (k = i - 1; k >= 0; k--)
			{
				for (j = 0; j < COLS; j++)
				{
					g_panel[k + 1][j] = g_panel[k][j];
				}
			}
			i = i + 1;
			count++;
		}
	}

	//��߼���Ϊ9�������Է�������Ϊ(9+1)*SCORE_LEVEL_INC-1
	if (score >= 10 * SCORE_LEVEL_INC - 1) return;

	//�ӷֹ�������������1�м�10�֣�2�м�15��,3�м�20��,4�м�30��
	switch (count)
	{
	case 1:
		score += 10;
		break;
	case 2:
		score += 15;
		break;
	case 3:
		score += 20;
		break;
	case 4:
		score += 30;
		break;
	}

	int temp_level = score / SCORE_LEVEL_INC;
	if (temp_level > level)
	{
		level = temp_level;
		//������ǰ��ʱ����Ȼ������
		if (timer_id) KillTimer(hwnd, timer_id);
		timer_id = SetTimer(hwnd, ID_TIMER, interval_base - level * interval_unit, NULL);
	}

	//system("cls");
	//printf("score: %d, level: %d ", score, level);
}

void Game::RefreshPanel(HDC hdc)     //ˢ�����
{
	int x, y;
	RECT rect;	//���������
	HBRUSH h_bSolid = (HBRUSH)GetStockObject(GRAY_BRUSH);		//׼����仭ˢ���
	HBRUSH h_bEmpty = (HBRUSH)GetStockObject(WHITE_BRUSH);		//׼��ȡ����仭ˢ���
	if (block_new == NULL) return;
	if (isover == true) return;

	//�������з���
	for (y = 0; y < ROWS; y++)
	{
		for (x = 0; x < COLS; x++)
		{
			//Ϊ����ˢ������ı߿�rect��Χ�����趨�ıȱ߿�ΧС1������Ӧ+-2
			rect.top = y * CELL + 2;
			rect.bottom = (y + 1) * CELL - 2;
			rect.left = x * CELL + 2;
			rect.right = (x + 1) * CELL - 2;
			if (g_panel[y][x])
				FillRect(hdc, &rect, h_bSolid);		//��䷽��
			else
				FillRect(hdc, &rect, h_bEmpty);		//ȡ���������
		}
	}
	//���Ƶ�ǰ����
	for (y = 0; y < cur_block.GetHeight (); y++)
	{
		for (x = 0; x < cur_block.GetWidth (); x++)
		{
			//if (*(block + y * cur_block.GetWidth() + x))	//ʵ��
			if (block_new[y][x] == 1)
			{
				rect.top = (y + cur_block.GetCurTop ()) * CELL + 2;
				rect.bottom = (y + cur_block.GetCurTop() + 1) * CELL - 2;
				rect.left = (x + cur_block.GetCurLeft()) * CELL + 2;
				rect.right = (x + cur_block.GetCurLeft() + 1) * CELL - 2;
				HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(cur_block.GetRed (), cur_block.GetGreen (), cur_block.GetBlue ()));
				FillRect(hdc, &rect, hBrush);
			}
		}
	}
}

bool Game::ExportBlock()    //�������
{
	int sel;
	color(12);
	if (block_new != NULL)
	{
		Tetris.DeleteBlock(block_new);  //�ͷ�֮ǰ������ڴ�
	}

	int cur_left, cur_top;  //��¼���鵱ǰ��λ��
	int width_block, height_block; //����Ŀ�Ⱥ͸߶�
	
	sel = rand() % 7;
	switch (sel)
	{
	case 0:  //ˮƽ��
		width_block = 4;
		height_block = 1;


		block_new = new int * [height_block];
		for (int i = 0; i < height_block; i++)
		{
			block_new[i] = new int [width_block];
		}

		block_new[0][0] = 1;
		block_new[0][1] = 1;
		block_new[0][2] = 1;
		block_new[0][3] = 1;
		
		
		cur_top = 0 - height_block;
		cur_left = (COLS - width_block) / 2;
		cur_block.SetBlock(cur_top, cur_left, width_block, height_block, 30, 144, 255);
		break;
	case 1:  //����
		color(13);
		width_block = 3;
		height_block = 2;

		block_new = new int* [height_block];
		for (int i = 0; i < height_block; i++)
		{
			block_new[i] = new int[width_block];
		}

		block_new[0][0] = 0;
		block_new[0][1] = 1;
		block_new[0][2] = 0;
		block_new[1][0] = 1;
		block_new[1][1] = 1;
		block_new[1][2] = 1;

		cur_top = 0 - height_block;
		cur_left = (COLS - width_block) / 2;
		cur_block.SetBlock(cur_top, cur_left, width_block, height_block, 254, 67, 101);
		break;
	case 2:  //�����
		color(10);
		width_block = 3;
		height_block = 2;

		block_new = new int* [height_block];
		for (int i = 0; i < height_block; i++)
		{
			block_new[i] = new int[width_block];
		}

		block_new[0][0] = 1;
		block_new[0][1] = 0;
		block_new[0][2] = 0;
		block_new[1][0] = 1;
		block_new[1][1] = 1;
		block_new[1][2] = 1;

		cur_top = 0 - height_block;
		cur_left = (COLS - width_block) / 2;
		cur_block.SetBlock(cur_top, cur_left, width_block, height_block, 255, 127, 36);
		break;
	case 3:  //�Һ���
		color(9);
		width_block = 3;
		height_block = 2;

		block_new = new int* [height_block];
		for (int i = 0; i < height_block; i++)
		{
			block_new[i] = new int[width_block];
		}

		block_new[0][0] = 0;
		block_new[0][1] = 0;
		block_new[0][2] = 1;
		block_new[1][0] = 1;
		block_new[1][1] = 1;
		block_new[1][2] = 1;

		cur_top = 0 - height_block;
		cur_left = (COLS - width_block) / 2;
		cur_block.SetBlock(cur_top, cur_left, width_block, height_block, 230, 155, 3);
		break;
	case 4:  //������
		color(9);
		width_block = 3;
		height_block = 2;

		block_new = new int* [height_block];
		for (int i = 0; i < height_block; i++)
		{
			block_new[i] = new int[width_block];
		}

		block_new[0][0] = 1;
		block_new[0][1] = 1;
		block_new[0][2] = 0;
		block_new[1][0] = 0;
		block_new[1][1] = 1;
		block_new[1][2] = 1;

		cur_top = 0 - height_block;
		cur_left = (COLS - width_block) / 2;
		cur_block.SetBlock(cur_top, cur_left, width_block, height_block, 255, 105, 180);
		break;
	case 5:  //������
		color(8);
		width_block = 3;
		height_block = 2;

		block_new = new int* [height_block];
		for (int i = 0; i < height_block; i++)
		{
			block_new[i] = new int[width_block];
		}

		block_new[0][0] = 0;
		block_new[0][1] = 1;
		block_new[0][2] = 1;
		block_new[1][0] = 1;
		block_new[1][1] = 1;
		block_new[1][2] = 0;

		cur_top = 0 - height_block;
		cur_left = (COLS - width_block) / 2;
		cur_block.SetBlock(cur_top, cur_left, width_block, height_block, 255, 105, 180);
		break;
	case 6:  //����
		color(13);
		width_block = 2;
		height_block = 2;

		block_new = new int* [height_block];
		for (int i = 0; i < height_block; i++)
		{
			block_new[i] = new int[width_block];
		}

		block_new[0][0] = 1;
		block_new[0][1] = 1;
		block_new[1][0] = 1;
		block_new[1][1] = 1;

		cur_top = 0 - height_block;
		cur_left = (COLS - width_block) / 2;
		cur_block.SetBlock(cur_top, cur_left, width_block, height_block, 0, 139, 139);
		break;
	}
	return block_new != NULL;
}

void Game::DeleteBlock(int** block)	//�ͷ�Block��̬������ڴ�
{
	for (int i = 0; i < cur_block.GetHeight(); i++)
	{
		delete[] block[i];
	}
	delete[] block;
	block = NULL;
}

void Game::DeleteBlocktemp(int** block,int height)	//�ͷ�ת��ʱtempBlock��̬������ڴ�
{
	for (int i = 0; i < height; i++)
	{
		delete[] block[i];
	}
	delete[] block;
	block = NULL;
}

void Game::color(int a)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);//��������ڸı����̨����������ǰ��ɫ������2=aʱ���ͱ���ɫ������2=a|bʱ�����Ǳ��롣
}

void SpeechControl(HDC hdc)
{
	using namespace std;
	extern wstring SpeechRecognition();
	wstring wstr = SpeechRecognition();
	int times = 1;//�ƶ�����
	string::size_type idx = wstr.find(L"һ");
	if (idx != string::npos)
	{
		times = 1;
	}
	else
	{
		idx = wstr.find(L"��");
		if (idx != string::npos)
		{
			times = 2;
		}
		else
		{
			idx = wstr.find(L"��");
			if (idx != string::npos)
			{
				times = 3;
			}
			else
			{
				idx = wstr.find(L"��");
				if (idx != string::npos)
				{
					times = 4;
				}
				else
				{
					idx = wstr.find(L"��");
					if (idx != string::npos)
					{
						times = 5;
					}
				}
			}
		}
	}

	int i;
	idx = wstr.find(L"��");
	if (idx != string::npos)
	{
		for (i = 1; i <= times; i++)
			if (!isPause) Tetris.DoLeftShift(hdc);
	}
	else
	{
		idx = wstr.find(L"��");
		if (idx != string::npos)
		{
			for (i = 1; i <= times; i++)
				if (!isPause) Tetris.DoRightShift(hdc);
		}
		else
		{
			idx = wstr.find(L"ת");
			if (idx != string::npos)
			{
				if (!isPause) Tetris.DoRedirection(hdc);
			}
			else
			{
				idx = wstr.find(L"����");
				if (idx != string::npos)
				{
					for (i = 1; i <= times; i++)
						if (!isPause) Tetris.DoAccelerate(hdc);
				}
				else
				{
					cout << "��ǰ����ʶ��δ�ҵ�ƥ��ָ��" << endl;
				}
			}
		}
	}

}