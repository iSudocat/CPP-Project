// Chess.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Chess.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND mainhWnd;			//保存窗口句柄
Board chessBoard;		//棋盘
int diff = 2;			//难度，默认normal
int turn = 1;			//先后手，默认先手（黑棋）
bool isStart = 0;		//判断比赛是否开始

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

    // TODO: 在此处放置代码。
	chessBoard.clear();
	/*chessBoard.SetChess(Point(1, 1), 1);
	chessBoard.SetChess(Point(1, 2), 1);
	chessBoard.SetChess(Point(2, 3), 2);
	chessBoard.SetChess(Point(2, 2), 2);*/
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHESS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHESS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHESS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
			int nScreenWidth, nScreenHeight;
			nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
			MoveWindow(hWnd, (nScreenWidth - (400 + (mCols - 1) * 50)) / 2, 10, 400 + (mCols - 1) * 50, 100 + nRows * 50, FALSE);
		}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

			HBRUSH brushBackground = CreateSolidBrush(RGB(211, 193, 155));
			HBRUSH brushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
			HBRUSH brushBlack = (HBRUSH)GetStockObject(BLACK_BRUSH);

			RECT rcClient;//区域结构
			GetClientRect(hWnd, &rcClient);//获得客户区域
			HBRUSH brushtemp = CreateSolidBrush(RGB(255, 255, 255));//新建临时白色画刷，用于清空整个界面
			//FillRect(hdc, &rcClient, brushtemp);//填充客户区域。

			//画出棋盘最外围边框
			MoveToEx(hdc, 200, 50, NULL);
			LineTo(hdc, 200, 750);
			LineTo(hdc, 900, 750);
			LineTo(hdc, 900, 50);
			LineTo(hdc, 200, 50);

			//输出行号列号
			int size;
			TCHAR szText[256];
			for (int i = 1; i <= nRows; i++)
			{
				size = wsprintf(szText, TEXT("%2d"), i);
				TextOut(hdc, (200 + (i - 1) * 50) - size * 5, 10, szText, size);
				TextOut(hdc, 160, (50 + (i - 1) * 50) - size * 5, szText, size);
			}

			//画棋盘网格
			SelectObject(hdc, brushBackground);
			int up = 50;
			for (int i = 1; i < nRows; i++)
			{
				int bottom = up + 50;
				int left = 200;
				for (int j = 1; j < mCols; j++)
				{
					Rectangle(hdc, left, up, left + 50, bottom);
					left += 50;
				}
				up += 50;
			}

			//画棋子
			for (int i = 1; i <= nRows; i++)
			{
				for (int j = 1; j <= mCols; j++)
				{
					int currentState = chessBoard.GetState(Point(i, j), 1, 0);
					if (!currentState) continue;
					if (currentState == 1)
					{
						SelectObject(hdc, brushBlack);
					}
					else
					{
						SelectObject(hdc, brushWhite);
					}
					Ellipse(hdc, 200 + (j - 1) * 50 - 22, 50 + (i - 1) * 50 - 22, 200 + (j - 1) * 50 + 22, 50 + (i - 1) * 50 + 22);
				}
			}

			//删除画刷
			DeleteObject(brushBackground);
			DeleteObject(brushBlack);
			DeleteObject(brushWhite);
			DeleteObject(brushtemp);

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
		{
			//chessBoard.PrintBoard(Point(0,0));
			HDC hdc = GetDC(hWnd);

			POINT cursor;
			GetCursorPos(&cursor);
			ScreenToClient(hWnd, &cursor);
			findPoint(cursor);


			int MAXDEP = 3;
			if (diff == 1) MAXDEP = 1;
			else if (diff == 2) MAXDEP = 3;
			else MAXDEP = 4;

			if (turn == 2 && !isStart)
			{
				chessBoard.SetChess(Point(nRows / 2, mCols / 2), (3 - turn));
				//chessBoard.PrintBoard(Point(nRows / 2, mCols / 2));
				InvalidateRect(hWnd, NULL, true);
				isStart = 1;
			}

			Point setPoint = Point(cursor.x, cursor.y);
			if (!setPoint.Check() || !chessBoard.SetChess(setPoint, turn))
			{
				break;
			}
			isStart = 1;

			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
			//InvalidateRect(hWnd, NULL, true);
			//UpdateWindow(hWnd);
			//chessBoard.PrintBoard(setPoint);

			int boardState = chessBoard.IsFinal(); // -1:平局 1:有一方胜利 0:还未决出胜负
			if (boardState > 0)
			{
				MessageBox(hWnd, TEXT("恭喜获胜！"), TEXT("游戏结束"), MB_OK);
				PostQuitMessage(0);
				break;
			}
			else if (boardState < 0)
			{
				MessageBox(hWnd, TEXT("平局！"), TEXT("游戏结束"), MB_OK);
				PostQuitMessage(0);
				break;
			}

			AI computerPlayer(chessBoard);
			Point best = Point(0, 0);
			computerPlayer.AlphaBeta((3 - turn), 1, MAXDEP, -0x7FFFFFFF, 0x7FFFFFFF, best);
			chessBoard.SetChess(best, (3 - turn));

			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
			//chessBoard.PrintBoard(best);

			boardState = chessBoard.IsFinal(); // -1:平局 1:有一方胜利 0:还未决出胜负
			if (boardState > 0)
			{
				MessageBox(hWnd, TEXT("电脑获胜！"), TEXT("游戏结束"), MB_OK);
				PostQuitMessage(0);
				break;
			}
			else if (boardState < 0)
			{
				MessageBox(hWnd, TEXT("平局！"), TEXT("游戏结束"), MB_OK);
				PostQuitMessage(0);
				break;
			}
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

Board::Board(const Board& board)
{
	memcpy(state, board.state, sizeof(board.state));
	empty = board.empty;
}

Board::~Board() { }

void Board::PrintBoard()
{
	printf("   ");
	for (int j = 1; j <= mCols; j++) printf("%2d ", j);
	printf("\n");
	for (int i = 1; i <= nRows; i++) {
		printf("%3d", i);
		for (int j = 1; j <= mCols; j++) {
			if (state[i][j]) printf(" %c ", (char)state[i][j] == 1 ? '*' : '#');
			else printf("   ");
		}
		printf("\n");
	}
}

void Board::PrintBoard(Point lastP)
{
	/*PAINTSTRUCT ps;
	HDC hdc = BeginPaint(mainhWnd, &ps);
	TextOut(hdc,0,0,L"TEST",4);
	// TODO: 在此处添加使用 hdc 的任何绘图代码...

	HBRUSH brushBackground = CreateSolidBrush(RGB(211, 193, 155));
	HBRUSH brushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH brushBlack = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RECT rcClient;//区域结构
	GetClientRect(mainhWnd, &rcClient);//获得客户区域
	HBRUSH brushtemp = CreateSolidBrush(RGB(255, 255, 255));//新建临时白色画刷，用于清空整个界面
	FillRect(hdc, &rcClient, brushtemp);//填充客户区域。

	//画出棋盘最外围边框
	MoveToEx(hdc, 200, 50, NULL);
	LineTo(hdc, 200, 750);
	LineTo(hdc, 900, 750);
	LineTo(hdc, 900, 50);
	LineTo(hdc, 200, 50);

	//输出行号列号
	int size;
	TCHAR szText[256];
	for (int i = 1; i <= nRows; i++)
	{
		size = wsprintf(szText, TEXT("%2d"), i);
		TextOut(hdc, (200 + (i - 1) * 50) - size * 5, 10, szText, size);
		TextOut(hdc, 160, (50 + (i - 1) * 50) - size * 5, szText, size);
	}

	//画棋盘网格
	SelectObject(hdc, brushBackground);
	int up = 50;
	for (int i = 1; i < nRows; i++)
	{
		int bottom = up + 50;
		int left = 200;
		for (int j = 1; j < mCols; j++)
		{
			Rectangle(hdc, left, up, left + 50, bottom);
			left += 50;
		}
		up += 50;
	}

	//画棋子
	for (int i = 1; i <= nRows; i++)
	{
		for (int j = 1; j <= mCols; j++)
		{
			int currentState = chessBoard.GetState(Point(i, j), 1, 0);
			if (!currentState) continue;
			if (currentState == 1)
			{
				SelectObject(hdc, brushBlack);
			}
			else
			{
				SelectObject(hdc, brushWhite);
			}
			Ellipse(hdc, 200 + (j - 1) * 50 - 22, 50 + (i - 1) * 50 - 22, 200 + (j - 1) * 50 + 22, 50 + (i - 1) * 50 + 22);
		}
	}

	//删除画刷
	DeleteObject(brushBackground);
	DeleteObject(brushBlack);
	DeleteObject(brushWhite);
	DeleteObject(brushtemp);

	EndPaint(mainhWnd, &ps);*/
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

//================================ class GAME =========================================//

void GAME::Run(int mode)
{
	chessBoard.clear();

	int MAXDEP = 3;
	if (mode == 1) MAXDEP = 1;
	else if (mode == 2) MAXDEP = 3;
	else MAXDEP = 4;

	printf("Choose black or white\n1.black\n2.white\n");
	int turn = 1;
	scanf_s("%d", &turn);
	if (turn == 2)
	{
		chessBoard.SetChess(Point(nRows / 2, mCols / 2), (3 - turn));
		printf("The computer choosed ( %d , %d )\n", nRows / 2, mCols / 2);
		chessBoard.PrintBoard();
	}

	int x, y;
	while (scanf_s("%d%d", &x, &y) != EOF)
	{
		Point setPoint = Point(x, y);
		if (!setPoint.Check() || !chessBoard.SetChess(setPoint, turn))
		{
			printf("Failed.Please choose a point again.\n");
			continue;
		}
		printf("Successfully!\n");
		chessBoard.PrintBoard();
		int boardState = chessBoard.IsFinal(); // -1:平局 1:有一方胜利 0:还未决出胜负
		if (boardState > 0) { printf("YOU WIN!\n"); break; }
		else if (boardState < 0) { printf("DRAW!\n"); break; }

		AI computerPlayer(chessBoard);
		Point best = Point(0, 0);
		computerPlayer.AlphaBeta((3 - turn), 1, MAXDEP, -0x7FFFFFFF, 0x7FFFFFFF, best);
		printf("The computer choosed ( %d , %d )\n", best.x, best.y);

		chessBoard.SetChess(best, (3 - turn));
		chessBoard.PrintBoard();
		boardState = chessBoard.IsFinal(); // -1:平局 1:有一方胜利 0:还未决出胜负
		if (boardState > 0) { printf("COMPUTER WIN!\n"); break; }
		else if (boardState < 0) { printf("DRAW!\n"); break; }
	}
}

//非成员函数
void findPoint(POINT& cursor)
{
	for (int x = 1; x <= nRows; x++)
	{
		for (int y = 1; y <= mCols; y++)
		{
			Point pointOnClient = Point( 200 + (y - 1) * 50 , 50 + (x - 1) * 50 );
			if (squareDist(pointOnClient, Point(cursor.x, cursor.y)) <= 25 * 25)
			{
				cursor.x = x;
				cursor.y = y;
				return;
			}
		}
	}
}

int squareDist(Point P, Point Q)
{
	return square(P.x - Q.x) + square(P.y - Q.y);
}

int square(int x)
{
	return x * x;
}