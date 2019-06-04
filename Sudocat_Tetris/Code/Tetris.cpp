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

/////////////////全局变量/////////////////////////////
HWND hwnd;     //窗口句柄变量
UINT timer_id = 0;    //保存计时器ID
byte g_panel[ROWS][COLS] = { 0 };
bool isPause = false;    //暂停标识
bool isover = false;	//游戏结束标识
int** block_new;
string username;
LPTSTR iniscore = new char[10];
string path = "";
string inipath = "";
int maxscore = 0;
////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	//回调函数原型
void SpeechControl(HDC hdc);

Game Tetris;
Block cur_block;

void string2tchar(std::string& src, TCHAR* buf)
{
#ifdef UNICODE  
	_stprintf_s(buf, MAX_PATH, _T("%S"), src.c_str());//%S宽字符  
#else  
	_stprintf_s(buf, MAX_PATH, _T("%s"), src.c_str());//%s单字符
#endif
}


int main()
{
	std::cout << "欢迎来到俄罗斯方块！" << endl;
	std::cout << "键盘控制：" << endl;
	std::cout << "↑：翻转  ↓:加速下降  ←：向左移动  →：向右移动  空格：暂停" << endl << endl;
	std::cout << "语音控制：" << endl;
	std::cout << "按Delete键可进行语音识别控制（单次），本功能需要联网才可使用。" << endl;
	std::cout << "语音命令：向左x格/向右x格/加速x格/翻转" << endl << endl;
	std::cout << "请输入您的用户名：" << endl;
	
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

	HINSTANCE hInstance = GetModuleHandle(NULL);//取得程序句柄
	TCHAR szAppName[] = TEXT("teris");
	MSG msg;
	WNDCLASS wc;//创建WNDCLASS类的对象wc 下方设置属性
	wc.style = CS_HREDRAW | CS_VREDRAW;// 窗口类型
	wc.lpfnWndProc = WndProc;//窗口处理函数
	wc.cbClsExtra = 0;//窗口扩展
	wc.cbWndExtra = 0;//窗口实例扩展
	wc.hInstance = hInstance;//窗口句柄
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//窗口图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//窗口光标
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//窗口背景
	wc.lpszMenuName = NULL;//窗口菜单
	wc.lpszClassName = szAppName;//窗口类名

	if (!RegisterClass(&wc))//注册窗口
	{
		printf("RegisterClass occur errors!");
		return 0;
	}
	hwnd = CreateWindow //实例化窗口
	(
		szAppName,
		TEXT("俄罗斯方块"),
		WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, SW_SHOW);//显示窗口
	UpdateWindow(hwnd);//更新窗口，发送一些初始消息，更新GDI等

/*************************************************************************************************
	每个窗口都有一个自己的窗口消息队列，里面存放了需要处理的窗口消息，
	接下来就是建立一个窗口的消息循环，在窗口退出之前一直处于工作状态，
	不断的从消息队列中获取消息
	获取消息的函数是：
	BOOL GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax );
	该函数需要在while()中执行，如while(GetMessage(&msg, NULL, 0, 0)),这是最简单的写法

	在GetMessage()获取到消息是虚拟键消息，需要进行处理，调用TranslateMessage(MSG *lpMsg)，
	这个函数的功能是将虚拟键消息转换成字符消息通俗说就是翻译消息。

	TranslateMessage()之后，还需要对消息进行派发，将每个消息发送到相应的需要到达的目标窗口或控件，
	派发函数：LRESULT DispatchMessage(MSG *lpMsg);
*************************************************************************************************/

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	std::cout << username + "，欢迎下次再来！" << endl << endl;

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
	创建窗口的主要步骤已经完成了，但是我们还需要创建一个窗口的回调函数，
	在初始化窗口类的时候，是需要有这个回调函数的，这个回调函数将被windows系统进行回调。
	回调函数的格式必须按照如下格式：
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	message：消息ID
	wParam：当键盘消息发出时，值为按下按键的虚拟键码
	lParam：存储按键的相关状态信息
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

			string out = "当前等级：" + std::to_string(Tetris.level_get()) + " 得分:" + std::to_string(Tetris.score_get()) + " 最高分:" + std::to_string(maxscore);
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
			case VK_DELETE:		//按Delete键启动语音识别（单次）
			{   //如果要在case里面定义变量，需要用{}括起来
				//为防止识别调用造成窗口卡顿，将使用多线程执行识别函数
				std::thread t(SpeechControl, hdc); //创建一个线程t，参数1为运行的函数，参数2为传递的参数（可空）
				t.detach(); //表示该线程在后台允许，无需等待该线程的函数返回，继续执行后面的语句
				break;
			}
			case VK_LEFT:       //左移
				if (!isPause) Tetris.DoLeftShift(hdc);
				break;
			case VK_RIGHT:       //右移
				if (!isPause) Tetris.DoRightShift(hdc);
				break;
			case VK_UP:        //转向
				if (!isPause) Tetris.DoRedirection(hdc);
				break;
			case VK_DOWN:       //加速
				if (!isPause) Tetris.DoAccelerate(hdc);
				break;
			case VK_SPACE:  //暂停
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
			Tetris.DrawPanel(hdc);   //绘制游戏面板
			Tetris.RefreshPanel(hdc);  //刷新


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



void Game::DrawPanel(HDC hdc)    //绘制游戏面板
{
	color(13);
	int x, y;
	RECT rect;

	for (y = 0; y < ROWS; y++)
	{
		for (x = 0; x < COLS; x++)
		{
			//设定矩形边框范围
			rect.top = y * CELL + 1;
			rect.bottom = (y + 1) * CELL - 1;
			rect.left = x * CELL + 1;
			rect.right = (x + 1) * CELL - 1;
			//绘制矩形边框
			FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}
	}
}

void Game::DoDownShift(HDC hdc)    //下移
{
	if (block_new == NULL) return;

	//判断是否到达底部
	if (IsTouchBottom(hdc))   //到底部
	{
		if (isover == false)
		{
			//消行处理
			ClearRow(hdc);
			ExportBlock();  //输出下一个方块
		}
	
	}

	if (isover == false)
	{
		cur_block.GoDown();
		//cur_top++;
		RefreshPanel(hdc);
	}
}

void Game::DoLeftShift(HDC hdc)    //左移
{
	int x, y;
	if (block_new == NULL) return;

	if (0 == cur_block.GetCurLeft() ) return;
	
	if (cur_block.GetCurTop()  < 0) return; //方块没有完整显示前，不能左移
	for (y = 0; y < cur_block.GetHeight() ; y++)
	{
		for (x = 0; x < cur_block.GetWidth(); x++)    
		{
			if (block_new[y][x] == 1)	//从左边开始扫描，找到第y行最左边的实心方块
			{
				//判断当前方格在面板上面左边一个方格是否为实心，是就代表不能再左移
				if (g_panel[cur_block.GetCurTop()  + y][cur_block.GetCurLeft () + x - 1] == 1) return;


				break;  //第y行从左数第一个实心方块的左边没有方块，继续扫描下一行
			}
		}
	}
	//cur_left--;
	cur_block.GoLeft();
	RefreshPanel(hdc);
}

void Game::DoRightShift(HDC hdc)    //右移
{
	int x, y;
	if (block_new == NULL) return;

	if (COLS - cur_block.GetWidth () == cur_block.GetCurLeft ()) return;
	if (cur_block.GetCurTop () < 0) return;  //方块完整显示前不能右移
	for (y = 0; y < cur_block.GetHeight (); y++)
	{
		for (x = cur_block.GetWidth () - 1; x >= 0; x--)   
		{
			if (block_new[y][x] == 1)	//从右边开始扫描，找到第y行最右边的实心方块
			{
				//判断当前方格在面板上右边一个方格是否为实心，是就代表不能再右移
				if (g_panel[cur_block.GetCurTop () + y][cur_block.GetCurLeft () + x + 1]) return;

				break;  //第y行从右数第一个实心方块的右边没有方块，继续扫描下一行
			}
		}
	}
	//cur_left++;
	cur_block.GoRight();
	RefreshPanel(hdc);
}

void Game::DoRedirection(HDC hdc)   //改变方向
{
	int i, j;
	//byte* temp = NULL;
	if (block_new == NULL) return;
	if (cur_block.GetCurTop () < 0) return;  //方块完整显示前不能转向

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

	

	//重新定位方块
	int incHeight = cur_block.GetWidth () - cur_block.GetHeight ();
	int incWidth = cur_block.GetHeight() - cur_block.GetWidth();
	int temp_cur_top = cur_block.GetCurTop () - incHeight / 2;
	int temp_cur_left = cur_block.GetCurLeft () - incWidth / 2;

	//system("cls");
	//printf("temp_top=%d, temp_left=%d",temp_cur_top,temp_cur_left);

	//判断当前空间是否足够让方块改变方向
	int max_len = max(cur_block.GetWidth (), cur_block.GetHeight ());
	//防止下标访问越界
	if (temp_cur_top + max_len - 1 >= ROWS || temp_cur_left < 0 || temp_cur_left + max_len - 1 >= COLS)
	{
		Tetris.DeleteBlocktemp(block_temp, cur_block.GetWidth());  //退出前必须先释放内存
		return;
	}
	for (i = 0; i < max_len; i++)
	{
		for (j = 0; j < max_len; j++)
		{
			//转向所需的空间内有已被占用的实心方格存在，即转向失败
			if (g_panel[temp_cur_top + i][temp_cur_left + j])
			{
				Tetris.DeleteBlocktemp(block_temp,cur_block.GetWidth());  //退出前必须先释放内存
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

	//把临时变量的值赋给block
	for (i = 0; i < cur_block.GetWidth(); i++)
	{
		for (j = 0; j < cur_block.GetHeight(); j++)
		{
			block_new[i][j] = block_temp[i][j];
		}
	}

	//重设交换方块
	cur_block.SetBlock(temp_cur_top, temp_cur_left, cur_block.GetHeight(), cur_block.GetWidth(), cur_block.GetRed(), cur_block.GetGreen(), cur_block.GetBlue());

	Tetris.DeleteBlocktemp(block_temp, cur_block.GetHeight());  //释放为临时变量分配的内存
	RefreshPanel(hdc);
}

void Game::DoAccelerate(HDC hdc)    //加速
{
	if (block_new == NULL) return;

	if (IsTouchBottom(hdc))
	{
		//消行处理
		ClearRow(hdc);
		ExportBlock();
	}
	//cur_top++;
	cur_block.GoDown();
	RefreshPanel(hdc);
}

bool Game::IsTouchBottom(HDC hdc)	//触底检测
{
	int x, y;
	int i, j;

	if (block_new == NULL) return false;
	if (ROWS == cur_block.GetCurTop () + cur_block.GetHeight ())
	{
		//固定方块
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

	for (y = cur_block.GetHeight () - 1; y >= 0; y--)    //从底行开始扫描
	{
		//判断第一个实心方块在面板上邻接的下方方格是否为实心，是就代表已经到达底部
		for (x = 0; x < cur_block.GetWidth (); x++)
		{
			if (block_new[y][x])
			{
				if (cur_block.GetCurTop () + y + 1 < 0) return false;
				if (g_panel[cur_block.GetCurTop ()+ y + 1][cur_block.GetCurLeft () + x])
				{
					//判断是否gameover
					if (cur_block.GetCurTop () <= 0)
					{
						isover = true;
						if (timer_id)
						{
							KillTimer(hwnd, timer_id);
							timer_id = 0;
						}
						if (block_new != NULL)	Tetris.DeleteBlock(block_new);
						MessageBox(hwnd, TEXT("游戏结束"), TEXT("游戏结束！"), MB_OK | MB_ICONEXCLAMATION);
						//SendMessage(hwnd, WM_CLOSE, 0, 0);
						PostQuitMessage(0);
						return false;
						
					}
					
					//固定方块
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

void Game::ClearRow(HDC hdc)      //消行
{
	int i, j, k;
	int count = 0;  //消行次数
	bool isFilled;
	//消行处理
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
			//所有方块往下移
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

	//最高级别为9级，所以分数极限为(9+1)*SCORE_LEVEL_INC-1
	if (score >= 10 * SCORE_LEVEL_INC - 1) return;

	//加分规则：消除行数，1行加10分，2行加15分,3行加20分,4行加30分
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
		//撤销当前计时器，然后重设
		if (timer_id) KillTimer(hwnd, timer_id);
		timer_id = SetTimer(hwnd, ID_TIMER, interval_base - level * interval_unit, NULL);
	}

	//system("cls");
	//printf("score: %d, level: %d ", score, level);
}

void Game::RefreshPanel(HDC hdc)     //刷新面板
{
	int x, y;
	RECT rect;	//定义矩形类
	HBRUSH h_bSolid = (HBRUSH)GetStockObject(GRAY_BRUSH);		//准备填充画刷句柄
	HBRUSH h_bEmpty = (HBRUSH)GetStockObject(WHITE_BRUSH);		//准备取消填充画刷句柄
	if (block_new == NULL) return;
	if (isover == true) return;

	//绘制已有方块
	for (y = 0; y < ROWS; y++)
	{
		for (x = 0; x < COLS; x++)
		{
			//为避免刷掉方块的边框，rect范围必须设定的比边框范围小1，即对应+-2
			rect.top = y * CELL + 2;
			rect.bottom = (y + 1) * CELL - 2;
			rect.left = x * CELL + 2;
			rect.right = (x + 1) * CELL - 2;
			if (g_panel[y][x])
				FillRect(hdc, &rect, h_bSolid);		//填充方块
			else
				FillRect(hdc, &rect, h_bEmpty);		//取消方块填充
		}
	}
	//绘制当前方块
	for (y = 0; y < cur_block.GetHeight (); y++)
	{
		for (x = 0; x < cur_block.GetWidth (); x++)
		{
			//if (*(block + y * cur_block.GetWidth() + x))	//实心
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

bool Game::ExportBlock()    //输出方块
{
	int sel;
	color(12);
	if (block_new != NULL)
	{
		Tetris.DeleteBlock(block_new);  //释放之前分配的内存
	}

	int cur_left, cur_top;  //记录方块当前的位置
	int width_block, height_block; //方块的宽度和高度
	
	sel = rand() % 7;
	switch (sel)
	{
	case 0:  //水平条
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
	case 1:  //三角
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
	case 2:  //左横折
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
	case 3:  //右横折
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
	case 4:  //左闪电
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
	case 5:  //右闪电
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
	case 6:  //正方
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

void Game::DeleteBlock(int** block)	//释放Block动态分配的内存
{
	for (int i = 0; i < cur_block.GetHeight(); i++)
	{
		delete[] block[i];
	}
	delete[] block;
	block = NULL;
}

void Game::DeleteBlocktemp(int** block,int height)	//释放转向时tempBlock动态分配的内存
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);//此语句用于改变控制台窗口内文字前景色（参数2=a时）和背景色（参数2=a|b时），非必须。
}

void SpeechControl(HDC hdc)
{
	using namespace std;
	extern wstring SpeechRecognition();
	wstring wstr = SpeechRecognition();
	int times = 1;//移动次数
	string::size_type idx = wstr.find(L"一");
	if (idx != string::npos)
	{
		times = 1;
	}
	else
	{
		idx = wstr.find(L"二");
		if (idx != string::npos)
		{
			times = 2;
		}
		else
		{
			idx = wstr.find(L"三");
			if (idx != string::npos)
			{
				times = 3;
			}
			else
			{
				idx = wstr.find(L"四");
				if (idx != string::npos)
				{
					times = 4;
				}
				else
				{
					idx = wstr.find(L"五");
					if (idx != string::npos)
					{
						times = 5;
					}
				}
			}
		}
	}

	int i;
	idx = wstr.find(L"左");
	if (idx != string::npos)
	{
		for (i = 1; i <= times; i++)
			if (!isPause) Tetris.DoLeftShift(hdc);
	}
	else
	{
		idx = wstr.find(L"右");
		if (idx != string::npos)
		{
			for (i = 1; i <= times; i++)
				if (!isPause) Tetris.DoRightShift(hdc);
		}
		else
		{
			idx = wstr.find(L"转");
			if (idx != string::npos)
			{
				if (!isPause) Tetris.DoRedirection(hdc);
			}
			else
			{
				idx = wstr.find(L"加速");
				if (idx != string::npos)
				{
					for (i = 1; i <= times; i++)
						if (!isPause) Tetris.DoAccelerate(hdc);
				}
				else
				{
					cout << "当前语音识别未找到匹配指令" << endl;
				}
			}
		}
	}

}