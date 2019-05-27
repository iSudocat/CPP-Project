// Chess.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Chess.h"

#define MAX_LOADSTRING 100

// 全局变量:
int nScreenWidth;
int nScreenHeight;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND mainhWnd;			//保存窗口句柄
Board chessBoard;		//棋盘
int diff = 2;			//难度，默认normal
int turn = 1;			//先后手，默认先手（黑棋）
bool isStart = 0;		//判断比赛是否开始
bool isEnd = 0;			//判断比赛是否结束
bool isPlaying = 0;		//判断电脑是否正在计算
Point currentPosition;	//记录当前落子位置
Point lastCursorPos;	//记录上一次鼠标位置

vector <Record> setChessRecords;

POINT polygonUp[4], polygonRt[4];

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	chessBoard.clear();

	polygonUp[0].x = 50, polygonUp[0].y = 50;
	polygonUp[1].x = 60, polygonUp[1].y = 40;
	polygonUp[2].x = 760, polygonUp[2].y = 40;
	polygonUp[3].x = 750, polygonUp[3].y = 50;

	polygonRt[0].x = 750, polygonRt[0].y = 50;
	polygonRt[1].x = 760, polygonRt[1].y = 40;
	polygonRt[2].x = 760, polygonRt[2].y = 740;
	polygonRt[3].x = 750, polygonRt[3].y = 750;

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CHESS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHESS));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHESS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CHESS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, TEXT("五子棋"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	mainhWnd = hWnd;		//保存窗口句柄

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		MoveWindow(hWnd, (nScreenWidth - (100 + (mCols - 1) * 50)) / 2, 0, 100 + (mCols - 1) * 50 + 30, 100 + nRows * 50, FALSE);
	}
	break;
	case WM_COMMAND:
	{
		HMENU hMenu = GetMenu(hWnd);
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case ID_RESTART:
			DisplayMessageBox(hWnd, TEXT("确定重新开始？"), TEXT("重新开始"), TryAgain, NULL);
			break;
		case ID_EASY:
		{
			ChangeDifficulty(hMenu, ID_EASY, ID_NORMAL, ID_HARD);
			diff = 1;
		}
		break;
		case ID_NORMAL:
		{
			ChangeDifficulty(hMenu, ID_NORMAL, ID_EASY, ID_HARD);
			diff = 2;
		}
		break;
		case ID_HARD:
		{
			ChangeDifficulty(hMenu, ID_HARD, ID_EASY, ID_NORMAL);
			diff = 3;
		}
		break;
		case ID_BLACK:
		{
			if (isStart)
			{
				MessageBox(hWnd, TEXT("游戏已经开始\n请重新开始游戏以选择颜色"), TEXT("失败"), MB_OK | MB_ICONWARNING);
			}
			else
			{
				turn = 1;
				CheckMenuItem(hMenu, ID_WHITE, MF_UNCHECKED);
				CheckMenuItem(hMenu, ID_BLACK, MF_CHECKED);
			}
		}
		break;
		case ID_WHITE:
		{
			if (isStart)
			{
				MessageBox(hWnd, TEXT("游戏已经开始\n请重新开始游戏以选择颜色"), TEXT("失败"), MB_OK | MB_ICONWARNING);
			}
			else
			{
				turn = 2;
				CheckMenuItem(hMenu, ID_WHITE, MF_CHECKED);
				CheckMenuItem(hMenu, ID_BLACK, MF_UNCHECKED);
				chessBoard.SetChess(Point(nRows / 2 + 1, mCols / 2 + 1), (3 - turn));
				NewRecord(Point(nRows / 2 + 1, mCols / 2 + 1), (3 - turn));
				chessBoard.RePaintBoard(hWnd, Point(nRows / 2 + 1, mCols / 2 + 1));
				isStart = 1;
			}
		}
		break;
		case ID_READ:
		{
			DisplayMessageBox(hWnd, TEXT("确定读取棋局？"), TEXT("读取棋局"), ReadGame_Mulitithread, NULL);
		}
		break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DisplayMessageBox(hWnd, TEXT("确定退出游戏？"), TEXT("退出游戏"), ExitGame, NULL);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC hdcBuffer = CreateCompatibleDC(hdc);									//给设备分配一个内存空间
		RECT rcClient;																//区域结构
		GetClientRect(hWnd, &rcClient);												//获得客户区域
		int windowHeight = rcClient.bottom - rcClient.top;							//客户区域高度
		int windowWidth = rcClient.right - rcClient.left;							//客户区域宽度
		HBITMAP hBitMap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);	//创建位图
		SelectObject(hdcBuffer, hBitMap);											//将画布贴到绘图设备上

		HBRUSH brushBackground = CreateSolidBrush(RGB(211, 193, 155));
		HBRUSH brushShadow = CreateSolidBrush(RGB(221, 158, 89));
		HBRUSH brushGray = CreateSolidBrush(RGB(195, 195, 195));
		HBRUSH brushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
		HBRUSH brushBlack = (HBRUSH)GetStockObject(BLACK_BRUSH);


		HBRUSH brushtemp = CreateSolidBrush(RGB(255, 255, 255));		//新建临时白色画刷，用于清空整个界面
		FillRect(hdcBuffer, &rcClient, brushtemp);						//填充客户区域。

		HPEN penBoldBlack = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		//画出棋盘最外围边框
		//SelectObject(hdcBuffer, penBoldBlack);
		MoveToEx(hdcBuffer, 50, 50, NULL);
		LineTo(hdcBuffer, 50, 750);
		LineTo(hdcBuffer, 750, 750);
		LineTo(hdcBuffer, 750, 50);
		LineTo(hdcBuffer, 50, 50);

		//输出行号列号
		int size;
		TCHAR szText[256];
		for (int i = 1; i <= nRows; i++)
		{
			size = wsprintf(szText, TEXT("%2d"), i);
			TextOut(hdcBuffer, (50 + (i - 1) * 50) - size * 5, 10, szText, size);
			TextOut(hdcBuffer, 10, (50 + (i - 1) * 50) - size * 5, szText, size);
		}

		//输出落子记录
		SelectObject(hdcBuffer, brushGray);
		Rectangle(hdcBuffer, 900, 90, min(1400, nScreenWidth - 50), 710);
		int titlex = (900 + min(1400, nScreenWidth - 50)) / 2 - 30;
		int titley = 40;
		HFONT hFont = CreateFont
		(
			40, 0,							//高度40, 宽取0表示由系统选择最佳值
			0, 0,							//文本倾斜，与字体倾斜都为0
			FW_HEAVY,
			0, 0, 0,						//非斜体，无下划线，无中划线
			UNICODE,						//字符集
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,	//一系列的默认值
			L"落子记录字体"					//字体名称
		);
		SelectObject(hdcBuffer, hFont);
		size = wsprintf(szText, TEXT("记录"));
		TextOut(hdcBuffer, titlex, titley, szText, size);
		hFont = CreateFont
		(
			20, 0,							//高度20, 宽取0表示由系统选择最佳值
			0, 0,							//文本倾斜，与字体倾斜都为0
			FW_HEAVY,
			0, 0, 0,						//非斜体，无下划线，无中划线
			UNICODE,						//字符集
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,	//一系列的默认值
			L"落子记录字体"					//字体名称
		);
		SelectObject(hdcBuffer, hFont);
		SetTextColor(hdcBuffer, RGB(63, 72, 204));
		SetBkColor(hdcBuffer, RGB(195, 195, 195));
		int textx = 930;
		int texty = 100;
		int num = setChessRecords.size();
		for (int i = max(0, num - 30); i < num; i++, texty += 20)
		{
			Record currentRecord = setChessRecords[i];
			size = wsprintf(szText, TEXT("%02d:%02d:%02d"), currentRecord.localTime.wHour, currentRecord.localTime.wMinute, currentRecord.localTime.wSecond);
			TextOut(hdcBuffer, textx, texty, szText, size);
			if (currentRecord.player == turn) size = wsprintf(szText, TEXT("玩家落子"));
			else size = wsprintf(szText, TEXT("电脑落子"));
			TextOut(hdcBuffer, textx + 100, texty, szText, size);
			size = wsprintf(szText, TEXT("(%02d,%02d)"), currentRecord.chessCoordinate.x, currentRecord.chessCoordinate.y);
			TextOut(hdcBuffer, textx + 200, texty, szText, size);
		}
		DeleteObject(hFont);

		//画棋盘网格
		SelectObject(hdcBuffer, brushBackground);
		int up = 50;
		for (int i = 1; i < nRows; i++)
		{
			int bottom = up + 50;
			int left = 50;
			for (int j = 1; j < mCols; j++)
			{
				Rectangle(hdcBuffer, left, up, left + 50, bottom);
				left += 50;
			}
			up += 50;
		}

		//画棋盘阴影
		SelectObject(hdcBuffer, brushShadow);
		Polygon(hdcBuffer, polygonUp, 4);
		Polygon(hdcBuffer, polygonRt, 4);

		//画棋子
		for (int i = 1; i <= nRows; i++)
		{
			for (int j = 1; j <= mCols; j++)
			{
				int currentState = chessBoard.GetState(Point(i, j), 1, 0);
				if (!currentState) continue;
				if (currentState == 1)
				{
					SelectObject(hdcBuffer, brushBlack);
				}
				else
				{
					SelectObject(hdcBuffer, brushWhite);
				}
				Ellipse(hdcBuffer, 50 + (j - 1) * 50 - 22, 50 + (i - 1) * 50 - 22, 50 + (j - 1) * 50 + 22, 50 + (i - 1) * 50 + 22);
			}
		}

		//框出当前棋子
		if (currentPosition.x && currentPosition.y)
		{
			HPEN penRed = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			SelectObject(hdcBuffer, penRed);

			Point upLeft = Point(50 + (currentPosition.y - 1) * 50 - 25, 50 + (currentPosition.x - 1) * 50 - 25);		//框的左上角坐标
			MoveToEx(hdcBuffer, upLeft.x - 1, upLeft.y, NULL);
			LineTo(hdcBuffer, upLeft.x + 15, upLeft.y);
			MoveToEx(hdcBuffer, upLeft.x + 35, upLeft.y, NULL);
			LineTo(hdcBuffer, upLeft.x + 50, upLeft.y);
			LineTo(hdcBuffer, upLeft.x + 50, upLeft.y + 15);
			MoveToEx(hdcBuffer, upLeft.x + 50, upLeft.y + 35, NULL);
			LineTo(hdcBuffer, upLeft.x + 50, upLeft.y + 50);
			LineTo(hdcBuffer, upLeft.x + 35, upLeft.y + 50);
			MoveToEx(hdcBuffer, upLeft.x + 15, upLeft.y + 50, NULL);
			LineTo(hdcBuffer, upLeft.x, upLeft.y + 50);
			LineTo(hdcBuffer, upLeft.x, upLeft.y + 35);
			MoveToEx(hdcBuffer, upLeft.x, upLeft.y + 15, NULL);
			LineTo(hdcBuffer, upLeft.x, upLeft.y);

			DeleteObject(penRed);
		}

		//突出显示当前鼠标位置
		POINT currentCursor;
		GetCursorPos(&currentCursor);
		ScreenToClient(hWnd, &currentCursor);
		findPoint(currentCursor);
		Point mousePosition = Point(currentCursor.x, currentCursor.y);

		if (mousePosition.Check())
		{
			HPEN penRed = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			SelectObject(hdcBuffer, penRed);

			Point upLeft = Point(50 + (mousePosition.y - 1) * 50 - 25, 50 + (mousePosition.x - 1) * 50 - 25);		//框的左上角坐标
			MoveToEx(hdcBuffer, upLeft.x - 1, upLeft.y, NULL);
			LineTo(hdcBuffer, upLeft.x + 15, upLeft.y);
			MoveToEx(hdcBuffer, upLeft.x + 35, upLeft.y, NULL);
			LineTo(hdcBuffer, upLeft.x + 50, upLeft.y);
			LineTo(hdcBuffer, upLeft.x + 50, upLeft.y + 15);
			MoveToEx(hdcBuffer, upLeft.x + 50, upLeft.y + 35, NULL);
			LineTo(hdcBuffer, upLeft.x + 50, upLeft.y + 50);
			LineTo(hdcBuffer, upLeft.x + 35, upLeft.y + 50);
			MoveToEx(hdcBuffer, upLeft.x + 15, upLeft.y + 50, NULL);
			LineTo(hdcBuffer, upLeft.x, upLeft.y + 50);
			LineTo(hdcBuffer, upLeft.x, upLeft.y + 35);
			MoveToEx(hdcBuffer, upLeft.x, upLeft.y + 15, NULL);
			LineTo(hdcBuffer, upLeft.x, upLeft.y);

			DeleteObject(penRed);
		}

		BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcBuffer, 0, 0, SRCCOPY);//复制到系统设备上显示
		//删除画刷 画笔
		DeleteObject(brushBackground);
		DeleteObject(brushShadow);
		DeleteObject(brushBlack);
		DeleteObject(brushWhite);
		DeleteObject(brushGray);
		DeleteObject(brushtemp);
		DeleteObject(penBoldBlack);
		//删除内存中的hdcBuffer
		DeleteDC(hdcBuffer);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT currentCursor;
		GetCursorPos(&currentCursor);
		ScreenToClient(hWnd, &currentCursor);
		findPoint(currentCursor);
		Point currentCursorPos = Point(currentCursor.x, currentCursor.y);
		if (currentCursorPos.x != lastCursorPos.x || currentCursorPos.y != lastCursorPos.y)
		{
			Point upLeft = Point(50 + (lastCursorPos.y - 1) * 50 - 25, 50 + (lastCursorPos.x - 1) * 50 - 25);
			RECT lastRECTUp, lastRECTRt, lastRECTBm, lastRECTLt;
			lastRECTUp.left = upLeft.x - 2;
			lastRECTUp.top = upLeft.y - 2;
			lastRECTUp.right = upLeft.x + 48;
			lastRECTUp.bottom = upLeft.y + 2;

			lastRECTRt.left = upLeft.x + 48;
			lastRECTRt.top = upLeft.y - 2;
			lastRECTRt.right = upLeft.x + 52;
			lastRECTRt.bottom = upLeft.y + 48;
			
			lastRECTBm.left = upLeft.x + 2;
			lastRECTBm.top = upLeft.y + 48;
			lastRECTBm.right = upLeft.x + 52;
			lastRECTBm.bottom = upLeft.y + 52;

			lastRECTLt.left = upLeft.x - 2;
			lastRECTLt.top = upLeft.y + 2;
			lastRECTLt.right = upLeft.x + 2;
			lastRECTLt.bottom = upLeft.y + 52;

			InvalidateRect(hWnd, &lastRECTUp, true);
			InvalidateRect(hWnd, &lastRECTRt, true);
			InvalidateRect(hWnd, &lastRECTBm, true);
			InvalidateRect(hWnd, &lastRECTLt, true);

			upLeft = Point(50 + (currentCursorPos.y - 1) * 50 - 25, 50 + (currentCursorPos.x - 1) * 50 - 25);
			RECT currRECTUp, currRECTRt, currRECTBm, currRECTLt;
			currRECTUp.left = upLeft.x - 2;
			currRECTUp.top = upLeft.y - 2;
			currRECTUp.right = upLeft.x + 48;
			currRECTUp.bottom = upLeft.y + 2;

			currRECTRt.left = upLeft.x + 48;
			currRECTRt.top = upLeft.y - 2;
			currRECTRt.right = upLeft.x + 52;
			currRECTRt.bottom = upLeft.y + 48;

			currRECTBm.left = upLeft.x + 2;
			currRECTBm.top = upLeft.y + 48;
			currRECTBm.right = upLeft.x + 52;
			currRECTBm.bottom = upLeft.y + 52;

			currRECTLt.left = upLeft.x - 2;
			currRECTLt.top = upLeft.y + 2;
			currRECTLt.right = upLeft.x + 2;
			currRECTLt.bottom = upLeft.y + 52;

			InvalidateRect(hWnd, &currRECTUp, true);
			InvalidateRect(hWnd, &currRECTRt, true);
			InvalidateRect(hWnd, &currRECTBm, true);
			InvalidateRect(hWnd, &currRECTLt, true);

			UpdateWindow(hWnd);
			lastCursorPos = currentCursorPos;
		}
	}
	break;
	case WM_LBUTTONDOWN:
		{
			if (isEnd || isPlaying) break;
			POINT cursor;
			GetCursorPos(&cursor);
			ScreenToClient(hWnd, &cursor);
			findPoint(cursor);


			int MAXDEP = 3;
			if (diff == 1) MAXDEP = 1;
			else if (diff == 2) MAXDEP = 3;
			else MAXDEP = 4;

			Point setPoint = Point(cursor.x, cursor.y);
			if (!setPoint.Check() || !chessBoard.SetChess(setPoint, turn))
			{
				break;
			}
			isStart = 1;

			NewRecord(setPoint, turn);
			chessBoard.RePaintBoard(hWnd, setPoint);

			int boardState = chessBoard.IsFinal(); // -1:平局 1:有一方胜利 0:还未决出胜负
			if (boardState > 0)
			{
				isEnd = 1;
				TCHAR szText[256];
				wsprintf(szText, TEXT("恭喜获胜 !\n再来一局？"));
				DisplayMessageBox(hWnd, szText, TEXT("游戏结束"), TryAgain, NULL);
				break;
			}
			else if (boardState < 0)
			{
				isEnd = 1;
				TCHAR szText[256];
				wsprintf(szText, TEXT("平局 .\n再来一局？"));
				DisplayMessageBox(hWnd, szText, TEXT("游戏结束"), TryAgain, NULL);
				break;
			}

			//多线程计算电脑落子位置
			//为防止计算造成窗口卡顿，将使用多线程执行搜索函数
			std::thread t(ComputersTurn); //创建一个线程t，参数1为运行的函数，参数2为传递的参数（可空）
			t.detach(); //表示该线程在后台允许，无需等待该线程的函数返回，继续执行后面的语句
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//类成员函数定义
//================================ class Board =========================================//

Board::Board(int* board)
{
	memcpy(state, board, sizeof(board));
}

Board::Board(const Board & board)
{
	memcpy(state, board.state, sizeof(board.state));
	empty = board.empty;
}

Board::~Board() { }

void Board::RePaintBoard(HWND hWnd, Point nowP)
{
	currentPosition = nowP;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
	return;
}

int Board::GetState(Point w, int direct, int dist)
{
	w.x += Direction[direct][0] * dist;
	w.y += Direction[direct][1] * dist;
	if (!w.Check()) return -1;
	return state[w.x][w.y];
}

void Board::ResetChess(Point w)
{
	state[w.x][w.y] = 0;
}

bool Board::SetChess(Point w, int player)
{
	if (!state[w.x][w.y]) {
		empty--;
		return state[w.x][w.y] = player;
	}
	return 0;
}

void Board::Getline(int* line, Point p, int direct, int num)
{
	int delx = Direction[direct][0];
	int dely = Direction[direct][1];
	line[1] = state[p.x][p.y];
	line[0] = 1;
	for (int tot = 2; tot <= num; tot++)
	{
		p.x += delx, p.y += dely;
		if (!p.Check()) break;
		line[tot] = state[p.x][p.y];
		line[0] = tot;
	}
}

int Board::IsFinal()
{
	int line[8];
	for (int i = 1; i <= nRows; i++)
	{
		for (int j = 1; j <= mCols; j++)
		{
			if (!state[i][j]) continue;
			for (int k = 0; k < 8; k++)
			{
				Getline(line, Point(i, j), k, 5);
				if (line[0] < 5) continue;
				bool flag = 1;
				for (int x = 2; x <= 5; x++)
				{
					if (line[x] != line[x - 1]) { flag = 0; break; }
				}
				if (flag) return 1;
			}
		}
	}
	if (empty == 0) return -1;
	return 0;
}

int Board::CalculateValue(int nPlayer, int nEmpty) {
	// 返回不同棋型的估价

	if (nPlayer >= 5) return 100000;		// 五连
	else if (nPlayer >= 4)
	{
		if (nEmpty >= 2) return 10000;	// 活四连	011110、022220
		else if (nEmpty) return 3000;	// 死四连	011112、022221
	}
	else if (nPlayer >= 3)
	{
		if (nEmpty >= 2) return 3000;	// 活三连	01110、02220
		else if (nEmpty) return 300;		// 死三连	01112、02221
	}
	else if (nPlayer >= 2)
	{
		if (nEmpty >= 2) return 400;		// 活二连	0110、0220
		else if (nEmpty) return 200;		// 死二连	0112、0221
	}
	else if (nPlayer >= 1)
	{
		if (nEmpty >= 2) return 40;		// 活一连	010、020
		else if (nEmpty) return 20;		// 死一连	012、021
	}
	return 0;
}

int Board::Calculate(int* line, int player) {

	int watch[100];
	for (int i = 0; i <= line[0]; i++) watch[i] = line[i];

	// 计算直线上的估价
	int value = 0;
	int num_of_empty = 0;
	int num_of_player = 0;
	for (int i = 1; i <= line[0]; i++)
	{
		if (line[i] == player) num_of_player++;
		else if (line[i] == 0)
		{
			value += CalculateValue(num_of_player, num_of_empty + 1);
			num_of_player = 0;
			num_of_empty = 1;
		}
		else
		{
			value += CalculateValue(num_of_player, num_of_empty);
			num_of_player = 0;
			num_of_empty = 0;
		}
	}
	value += CalculateValue(num_of_player, num_of_empty);
	return value;
}

int Board::Evaluate()
{
	// 评估当前棋局
	int firstValue = 0;    // 先手的估价
	int secondValue = 0;	// 后手的估价
	int line[N + M];
	line[0] = 0;

	// 行 k = 4
	for (int i = 1; i <= nRows; i++)
	{
		line[0] = 0;
		Getline(line, Point(i, 1), 4, mCols);
		firstValue += Calculate(line, 1);
		secondValue += Calculate(line, 2);
	}

	// 列 k = 6
	for (int j = 1; j <= mCols; j++)
	{
		line[0] = 0;
		Getline(line, Point(1, j), 6, nRows);
		firstValue += Calculate(line, 1);
		secondValue += Calculate(line, 2);
	}

	// 正斜线(上半部分) k = 7
	for (int j = 1; j <= mCols; j++)
	{
		line[0] = 0;
		Getline(line, Point(1, j), 7, N + M);
		firstValue += Calculate(line, 1);
		secondValue += Calculate(line, 2);
	}

	// 正斜线(下半部分) k = 7
	for (int i = 2; i <= nRows; i++)
	{
		line[0] = 0;
		Getline(line, Point(i, 1), 7, N + M);
		firstValue += Calculate(line, 1);
		secondValue += Calculate(line, 2);
	}

	// 反斜线(上半部分) k = 5
	for (int j = 1; j <= mCols; j++)
	{
		line[0] = 0;
		Getline(line, Point(1, j), 5, N + M);
		firstValue += Calculate(line, 1);
		secondValue += Calculate(line, 2);
	}

	// 反斜线(下半部分) k = 5
	for (int i = 2; i <= nRows; i++)
	{
		line[0] = 0;
		Getline(line, Point(i, mCols), 5, N + M);
		firstValue += Calculate(line, 1);
		secondValue += Calculate(line, 2);
	}

	return firstValue - secondValue; // 先手的估价-后手的估价作为全局估价
}

void Board::clear()
{
	memset(state, 0, sizeof(state));
	empty = nRows * mCols;
}

//================================ class AI =========================================//

AI::AI(const Board board)
{
	virtualBoard = board;
}

AI::AI(const AI & ai)
{
	virtualBoard = ai.virtualBoard;
}
AI::~AI() { }

void AI::GenerateQueue(vector <Point> * decide)
{
	for (int i = 1; i <= nRows; i++)
	{
		for (int j = 1; j <= mCols; j++)
		{
			Point w = Point(i, j);
			if (virtualBoard.GetState(w, 1, 0) != 0) continue;
			for (int k = 0; k < 8; k++)
			{
				if (virtualBoard.GetState(w, k, 1) > 0)
				{
					decide->push_back(w);
					break;
				}
			}
		}
	}
}

int AI::AlphaBeta(int player, int depth, int MAXDEP, int alpha, int beta, Point & best)
{
	if (depth > MAXDEP || virtualBoard.IsFinal()) return virtualBoard.Evaluate();
	vector <Point> decide;		//决策队列
	decide.clear();
	GenerateQueue(&decide);
	int size = decide.size();
	for (int i = 0; i < size; i++)
	{
		Point w = decide[i];
		if (!virtualBoard.SetChess(w, player)) continue;
		if (player == 1)
		{
			int ret = AlphaBeta(2, depth + 1, MAXDEP, alpha, beta, best);
			virtualBoard.ResetChess(w);
			if (depth == 1 && ret > alpha) best = w;
			alpha = max(alpha, ret);
			if (beta <= alpha) return alpha;
		}
		else
		{
			int ret = AlphaBeta(1, depth + 1, MAXDEP, alpha, beta, best);
			virtualBoard.ResetChess(w);
			if (depth == 1 && ret < beta) best = w;
			beta = min(beta, ret);
			if (beta <= alpha) return beta;
		}
	}
	if (player == 1) return alpha;
	else return beta;
}

//非成员函数
void NewRecord(Point w, int player)
{
	Record tempRecord;
	tempRecord.chessCoordinate = w;
	tempRecord.player = player;
	GetLocalTime(&tempRecord.localTime);
	setChessRecords.push_back(tempRecord);
}
void DisplayMessageBox(HWND hWnd, const TCHAR * szText, const TCHAR * szTitle, void (* OKFunction)( ), void (*CancelFunction)( ))
{
	int msgboxID = MessageBox(hWnd, szText, szTitle, MB_ICONINFORMATION | MB_OKCANCEL);
	switch (msgboxID)
	{
	case IDCANCEL:
		if (CancelFunction) CancelFunction();
		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
		break;
	case IDOK:
	{
		if (OKFunction) OKFunction();
	}
	break;
	}
}

void TryAgain()
{
	HWND hWnd = mainhWnd;
	isStart = 0;
	chessBoard.clear();

	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu, ID_WHITE, MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_BLACK, MF_CHECKED);
	turn = 1;						//先后手，重置为默认先手（黑棋）

	isStart = 0;					//重置比赛开始状态
	isEnd = 0;						//重置比赛结束状态
	currentPosition = Point(0, 0);	//清空当前落子位置
	lastCursorPos = Point(0, 0);	//清空上一次鼠标位置
	setChessRecords.clear();		//清空落子记录
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
}


void ReadGame_Mulitithread()
{
	std::thread t(ReadGame);
	t.detach();
	return;
}

void ReadGame()
{
	FILE* fp;
	if ( fopen_s(&fp, "chess.log", "r") == 0 )
	{
		MessageBox(mainhWnd, TEXT("读取成功"), TEXT("成功"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(mainhWnd, TEXT("读取失败\n错误信息：文件不存在"), TEXT("失败"), MB_OK | MB_ICONERROR);
		return;
	}
	int tot = 0;
	int num_of_com = 0;
	int num_of_man = 0;
	fscanf_s(fp, "%d%d%d\n", &diff, &turn, &tot);

	chessBoard.clear();
	setChessRecords.clear();		//清空落子记录
	for (int i = 0; i < tot; i++)
	{
		int h, m, s, x, y, p;
		fscanf_s(fp, "%02d:%02d:%02d (%02d,%02d) %d", &h, &m, &s, &x, &y, &p);
		Record tempRecord;
		tempRecord.localTime.wHour = h;
		tempRecord.localTime.wMinute = m;
		tempRecord.localTime.wSecond = s;
		tempRecord.chessCoordinate = Point(x, y);
		tempRecord.player = p;
		chessBoard.SetChess(Point(x, y), p);
		setChessRecords.push_back(tempRecord);
		num_of_man += p == turn;
		num_of_com += p != turn;
	}

	if (chessBoard.IsFinal())
	{
		isStart = 1;
		isEnd = 1;
	}
	else
	{
		if (tot == 0) isStart = 0;
		else isStart = 1;
		isEnd = 0;
	}

	HWND hWnd = mainhWnd;
	HMENU hMenu = GetMenu(hWnd);
	if (turn == 1)
	{
		CheckMenuItem(hMenu, ID_WHITE, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_BLACK, MF_CHECKED);
	}
	else
	{
		CheckMenuItem(hMenu, ID_WHITE, MF_CHECKED);
		CheckMenuItem(hMenu, ID_BLACK, MF_UNCHECKED);
	}
	
	switch (diff)
	{
	case 1:
		ChangeDifficulty(hMenu, ID_EASY, ID_NORMAL, ID_HARD);
		break;
	case 2:
		ChangeDifficulty(hMenu, ID_NORMAL, ID_EASY, ID_HARD);
		break;
	case 3:
		ChangeDifficulty(hMenu, ID_HARD, ID_EASY, ID_NORMAL);
		break;
	}

	if (tot) currentPosition = setChessRecords.back().chessCoordinate;	//清空当前落子位置
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
	Sleep(1000);
	if (num_of_com < num_of_man || (num_of_com == num_of_man && turn == 2)) ComputersTurn();
}

void ExitGame()
{
	DestroyWindow(mainhWnd);
	return;
}

void ChangeDifficulty(HMENU hMenu, UINT checked, UINT unchecked_one, UINT unchecked_two)
{
	CheckMenuItem(hMenu, checked, MF_CHECKED);
	CheckMenuItem(hMenu, unchecked_one, MF_UNCHECKED);
	CheckMenuItem(hMenu, unchecked_two, MF_UNCHECKED);
}

void ComputersTurn()
{
	HWND hWnd = mainhWnd;
	isPlaying = 1;
	
	int MAXDEP = 3;
	if (diff == 1) MAXDEP = 1;
	else if (diff == 2) MAXDEP = 3;
	else MAXDEP = 4;

	AI computerPlayer(chessBoard);
	Point best = Point(0, 0);

	computerPlayer.AlphaBeta((3 - turn), 1, MAXDEP, -0x7FFFFFFF, 0x7FFFFFFF, best);
	chessBoard.SetChess(best, (3 - turn));
	NewRecord(best, (3 - turn));
	chessBoard.RePaintBoard(hWnd, best);

	int boardState = chessBoard.IsFinal(); // -1:平局 1:有一方胜利 0:还未决出胜负
	if (boardState > 0)
	{
		isEnd = 1;
		TCHAR szText[256];
		wsprintf(szText, TEXT("电脑获胜 !\n再来一局？"));
		DisplayMessageBox(hWnd, szText, TEXT("游戏结束"), TryAgain, NULL);
	}
	else if (boardState < 0)
	{
		isEnd = 1;
		TCHAR szText[256];
		wsprintf(szText, TEXT("平局 .\n再来一局？"));
		DisplayMessageBox(hWnd, szText, TEXT("游戏结束"), TryAgain, NULL);
	}
	isPlaying = 0;
}

void findPoint(POINT & cursor)
{
	for (int x = 1; x <= nRows; x++)
	{
		for (int y = 1; y <= mCols; y++)
		{
			Point pointOnClient = Point(50 + (y - 1) * 50, 50 + (x - 1) * 50);
			if (squareDist(pointOnClient, Point(cursor.x, cursor.y)) <= 25 * 25)
			{
				cursor.x = x;
				cursor.y = y;
				return;
			}
		}
	}
	cursor.x = cursor.y = -1;
}

int squareDist(Point P, Point Q)
{
	return square(P.x - Q.x) + square(P.y - Q.y);
}

int square(int x)
{
	return x * x;
}