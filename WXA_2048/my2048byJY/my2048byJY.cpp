// my2048byJY.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "my2048byJY.h"


#define MAX_LOADSTRING 100
#define RGBYellow RGB(255, 242, 0)
#define RGBOrange RGB(255, 127, 39)
#define RGBRed RGB(237, 28, 36)
#define RGBBlack RGB(0,0,0)
#define RGBWhite RGB(255,255,255)

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HFONT hFont;                   // 字体句柄
HBRUSH hBrush[5];                               // 刷子句柄
TZFE tzfe;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void myLine(HDC ahdc, int a, int b, int x, int y) {
	MoveToEx(ahdc, a, b, NULL);
	LineTo(ahdc, x, y);
};
struct Number2
{
	const HFONT hFont = hFont;
	const HBRUSH hBrushY = hBrush[0];
}N2;
	

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
	hFont = CreateFont//2的参数
	(
		30, 0,							//高度20, 宽取0表示由系统选择最佳值
		0, 0,							//文本倾斜，与字体倾斜都为0
		FW_HEAVY,
		0, 0, 0,						//非斜体，无下划线，无中划线
		UNICODE,						//字符集
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,	//一系列的默认值
		L"2048字体"					//字体名称

	);

	hBrush[0] = CreateSolidBrush(RGBYellow);//黄色
	hBrush[1] = CreateSolidBrush(RGB(255, 127, 39));//橙色
	hBrush[2] = CreateSolidBrush(RGB(237, 28, 36));//红色
	hBrush[3] = CreateSolidBrush(RGBBlack);
	hBrush[4] = CreateSolidBrush(RGBWhite);


	tzfe.newGame();

	//tzfe.loopGame();
	//tzfe.endGame(0);


    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2048BYJY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048BYJY));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2048BYJY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2048BYJY);
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
		MoveWindow(hWnd, 0,0,500,650 ,FALSE);
	}
	break;
	case WM_KEYDOWN:
	{
		HDC hdc = GetDC(hWnd);
		switch (wParam)
		{
		case VK_LEFT:       //左移
			tzfe.ifNeedRandomNumber = false;
			tzfe.moveLeft();
			if (tzfe.ifNeedRandomNumber)
			{
				tzfe.randomNumber();
				tzfe.refreshShow();
			}
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
			if (tzfe.nullNumber() == 0)
			{
				tzfe.ifLoseGame();
				if (tzfe.ifGameOver == true)
				{
					TCHAR score[10];
					int size = wsprintf(score, TEXT("%2d"), tzfe.score);
					TCHAR END[20] = L"你的得分是 ";

					lstrcat(END, score);
					lstrcat(END, L" 还来不？");
					int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
					switch (msgboxID)
					{
					case IDCANCEL:
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
						SendMessage(hWnd, WM_CLOSE, 0, 0);
						break;
					case IDOK:
						for (int i = 0; i < 4; i++)
							for (int j = 0; j < 4; j++)
								Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
						tzfe.newGame();
						SelectObject(hdc, hFont);
						SetTextColor(hdc, RGBBlack);
						TextOut(hdc, 250, 550, L"分数：       ", sizeof(L"分数：       ") / 2);
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
						break;
					}
				}
			}
			break;
		case VK_RIGHT:       //右移
			tzfe.ifNeedRandomNumber = false;
			tzfe.moveRight();
			if (tzfe.ifNeedRandomNumber)
			{
				tzfe.randomNumber();
				tzfe.refreshShow();
			}
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
			if (tzfe.nullNumber() == 0)
			{
				tzfe.ifLoseGame();
				if (tzfe.ifGameOver == true)
				{
					TCHAR score[10];
					int size = wsprintf(score, TEXT("%2d"), tzfe.score);
					TCHAR END[20] = L"你的得分是 ";

					lstrcat(END, score);					lstrcat(END, L" 还来不？");
					int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
					switch (msgboxID)
					{
					case IDCANCEL:
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
						SendMessage(hWnd, WM_CLOSE, 0, 0);
						break;
					case IDOK:

						for (int i = 0; i < 4; i++)
							for (int j = 0; j < 4; j++)
								Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
						tzfe.newGame();
						SelectObject(hdc, hFont);
						SetTextColor(hdc, RGBBlack);
						TextOut(hdc, 250, 550, L"分数：       ", sizeof(L"分数：       ") / 2);
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
					break;
					}
				}
			}
			break;

		case VK_UP:        //转向
			tzfe.ifNeedRandomNumber = false;
			tzfe.moveUp();
			if (tzfe.ifNeedRandomNumber)
			{
				tzfe.randomNumber();
				tzfe.refreshShow();
			}
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
			if (tzfe.nullNumber() == 0)
			{
				tzfe.ifLoseGame();
				if (tzfe.ifGameOver == true)
				{

					TCHAR score[10];
					int size = wsprintf(score, TEXT("%2d"), tzfe.score);
					TCHAR END[20] = L"你的得分是 ";
					lstrcat(END, score);
					lstrcat(END, L" 还来不？");
					int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
					switch (msgboxID)
					{
					case IDCANCEL:
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
						SendMessage(hWnd, WM_CLOSE, 0, 0);
						break;
					case IDOK:

						for (int i = 0; i < 4; i++)
							for (int j = 0; j < 4; j++)
								Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
						tzfe.newGame();
						SelectObject(hdc, hFont);
						SetTextColor(hdc, RGBBlack);
						TextOut(hdc, 250, 550, L"分数：       ", sizeof(L"分数：       ") / 2);
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
						break;
					}
				}
			}
			break;
		case VK_DOWN:       //加速
			tzfe.ifNeedRandomNumber = false;
			tzfe.moveDown();
			if (tzfe.ifNeedRandomNumber)
			{
				tzfe.randomNumber();
				tzfe.refreshShow();
			}
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
			if (tzfe.nullNumber() == 0)
			{
				tzfe.ifLoseGame();
				if (tzfe.ifGameOver == true)
				{
					TCHAR score[10];
					int size = wsprintf(score, TEXT("%2d"), tzfe.score);
					TCHAR END[20] = L"你的得分是 ";
					lstrcat(END, score);
					lstrcat(END, L" 还来不？");
					int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
					switch (msgboxID)
					{
					case IDCANCEL:
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
						SendMessage(hWnd, WM_CLOSE, 0, 0);
						break;
					case IDOK:

						for (int i = 0; i < 4; i++)
							for (int j = 0; j < 4; j++)
								Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
						tzfe.newGame();
						SelectObject(hdc, hFont);
						SetTextColor(hdc, RGBBlack);
						TextOut(hdc, 250, 550, L"分数：       ", sizeof(L"分数：       ") / 2);
						InvalidateRect(hWnd, NULL, true);
						UpdateWindow(hWnd);
						break;
					}
				}
			}
			break;
		}
		ReleaseDC(hWnd, hdc);
	}
		return 0;
		//以下为语音识别代码
	/*case sentence:
			{
				HDC hdc = GetDC(hWnd);
				switch (wParam)
				{
				case ("Left" or "left"):       //左移
					tzfe.ifNeedRandomNumber = false;
					tzfe.moveLeft();
					if (tzfe.ifNeedRandomNumber)
					{
						tzfe.randomNumber();
						tzfe.refreshShow();
					}
					InvalidateRect(hWnd, NULL, true);
					UpdateWindow(hWnd);
					if (tzfe.nullNumber() == 0)
					{
						tzfe.ifLoseGame();
						if (tzfe.ifGameOver == true)
						{
							TCHAR score[10];
							int size = wsprintf(score, TEXT("%2d"), tzfe.score);
							TCHAR END[20] = L"你的得分是 ";

							lstrcat(END, score);
							lstrcat(END, L" 还来不？");
							int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
							switch (msgboxID)
							{
							case IDCANCEL:
								InvalidateRect(hWnd, NULL, true);
								UpdateWindow(hWnd);
								SendMessage(hWnd, WM_CLOSE, 0, 0);
								break;
							case IDOK:
								tzfe.newGame();
								for (int i = 0; i < 4; i++)
									for (int j = 0; j < 4; j++)
										Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
								break;
							}
						}
					}
					break;
				case ("Right" or "right"):       //右移
					tzfe.ifNeedRandomNumber = false;
					tzfe.moveRight();
					if (tzfe.ifNeedRandomNumber)
					{
						tzfe.randomNumber();
						tzfe.refreshShow();
					}
					InvalidateRect(hWnd, NULL, true);
					UpdateWindow(hWnd);
					if (tzfe.nullNumber() == 0)
					{
						tzfe.ifLoseGame();
						if (tzfe.ifGameOver == true)
						{
							TCHAR score[10];
							int size = wsprintf(score, TEXT("%2d"), tzfe.score);
							TCHAR END[20] = L"你的得分是 ";

							lstrcat(END, score);					lstrcat(END, L" 还来不？");
							int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
							switch (msgboxID)
							{
							case IDCANCEL:
								InvalidateRect(hWnd, NULL, true);
								UpdateWindow(hWnd);
								SendMessage(hWnd, WM_CLOSE, 0, 0);
								break;
							case IDOK:
								tzfe.newGame();
								for (int i = 0; i < 4; i++)
									for (int j = 0; j < 4; j++)
										Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
								break;
							}
						}
					}
					break;

				case ("Up" or "up"):        //转向
					tzfe.ifNeedRandomNumber = false;
					tzfe.moveUp();
					if (tzfe.ifNeedRandomNumber)
					{
						tzfe.randomNumber();
						tzfe.refreshShow();
					}
					InvalidateRect(hWnd, NULL, true);
					UpdateWindow(hWnd);
					if (tzfe.nullNumber() == 0)
					{
						tzfe.ifLoseGame();
						if (tzfe.ifGameOver == true)
						{

							TCHAR score[10];
							int size = wsprintf(score, TEXT("%2d"), tzfe.score);
							TCHAR END[20] = L"你的得分是 ";
							lstrcat(END, score);
							lstrcat(END, L" 还来不？");
							int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
							switch (msgboxID)
							{
							case IDCANCEL:
								InvalidateRect(hWnd, NULL, true);
								UpdateWindow(hWnd);
								SendMessage(hWnd, WM_CLOSE, 0, 0);
								break;
							case IDOK:
								tzfe.newGame();
								for (int i = 0; i < 4; i++)
									for (int j = 0; j < 4; j++)
										Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
								break;
							}
						}
					}
					break;
				case ("Down" or "down"):       //加速
					tzfe.ifNeedRandomNumber = false;
					tzfe.moveDown();
					if (tzfe.ifNeedRandomNumber)
					{
						tzfe.randomNumber();
						tzfe.refreshShow();
					}
					InvalidateRect(hWnd, NULL, true);
					UpdateWindow(hWnd);
					if (tzfe.nullNumber() == 0)
					{
						tzfe.ifLoseGame();
						if (tzfe.ifGameOver == true)
						{
							TCHAR score[10];
							int size = wsprintf(score, TEXT("%2d"), tzfe.score);
							TCHAR END[20] = L"你的得分是 ";
							lstrcat(END, score);
							lstrcat(END, L" 还来不？");
							int msgboxID = MessageBox(hWnd, END, L"你挂了！！！", MB_ICONINFORMATION | MB_OKCANCEL);
							switch (msgboxID)
							{
							case IDCANCEL:
								InvalidateRect(hWnd, NULL, true);
								UpdateWindow(hWnd);
								SendMessage(hWnd, WM_CLOSE, 0, 0);
								break;
							case IDOK:
								tzfe.newGame();
								for (int i = 0; i < 4; i++)
									for (int j = 0; j < 4; j++)
										Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
								break;
							}
						}
					}
					break;
				}
				ReleaseDC(hWnd, hdc);
			}
			return 0;*/
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
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++) 
					Rectangle(hdc, 20 + 100 * i, 20 + 100 * j, 120 + 100 * i, 120 + 100 * j);
			SelectObject(hdc, hFont);
			SetTextColor(hdc, RGBBlack);
			TCHAR score[10];
			int size = wsprintf(score, TEXT("%2d"), tzfe.score);
			TCHAR SCORE[20] = L"分数：";
			lstrcat(SCORE, score);
			TextOut(hdc, 250, 550, SCORE, sizeof(SCORE)/2); 
			struct Num2
			{
				RECT RE;
				HBRUSH HBR = hBrush[0];
				HFONT HF = hFont;
				wchar_t NUM[2] = L"2";
				//位置信息，以20，20，120，120为例，60，55
			}num2;

			struct Num4
			{
				RECT RE;
				HBRUSH HBR = hBrush[1];
				HFONT HF = hFont;
				wchar_t NUM[2] = L"4";
			}num4;

			struct Num8
			{
				RECT RE;
				HBRUSH HBR = hBrush[2];
				HFONT HF = hFont;
				wchar_t NUM[2] = L"8";
			}num8;

			struct Num16
			{
				RECT RE;
				HBRUSH HBR = hBrush[0];
				HFONT HF = hFont;
				wchar_t NUM[3] = L"16";
				//位置信息，53,55
			}num16;

			struct Num32
			{
				RECT RE;
				HBRUSH HBR = hBrush[1];
				HFONT HF = hFont;
				wchar_t NUM[3] = L"32";
			}num32;

			struct Num64
			{
				RECT RE;
				HBRUSH HBR = hBrush[2];
				HFONT HF = hFont;
				wchar_t NUM[3] = L"64";
			}num64;

			struct Num128
			{
				RECT RE;
				HBRUSH HBR = hBrush[0];
				HFONT HF = hFont;
				wchar_t NUM[4] = L"128";
				//位置信息 47,47 
			}num128;

			struct Num256
			{
				RECT RE;
				HBRUSH HBR = hBrush[1];
				HFONT HF = hFont;
				wchar_t NUM[4] = L"256";
			}num256;

			struct Num512
			{
				RECT RE;
				HBRUSH HBR = hBrush[2];
				HFONT HF = hFont;
				wchar_t NUM[4] = L"512";
			}num512;

			struct Num1024p
			{
				RECT RE;
				HBRUSH HBR = hBrush[3];
				HFONT HF = hFont;
				//位置信息 38，55
			}num1024p;

			for (int i=0;i<4;i++)
				for (int j = 0; j < 4; j++)
					if (tzfe.board[i][j].number == 0)
					{
						SelectObject(hdc, hBrush[4]);
						RECT RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &RE, hBrush[4]);
					}
					else if (tzfe.board[i][j].number == 2)
					{
						SelectObject(hdc, num2.HBR);
						num2.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num2.RE, num2.HBR);
						SelectObject(hdc, num2.HF);
						SetTextColor(hdc, RGBBlack);
						SetBkColor(hdc, RGBYellow);
						TextOut(hdc, 60 + 100 * i, 55 + 100 * j, num2.NUM, sizeof(num2.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 4)
					{
						SelectObject(hdc, num4.HBR);
						num4.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num4.RE, num4.HBR);
						SelectObject(hdc, num4.HF);
						SetTextColor(hdc, RGBBlack);
						SetBkColor(hdc, RGBOrange);
						TextOut(hdc, 60 + 100 * i, 55 + 100 * j, num4.NUM, sizeof(num4.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 8)
					{
						SelectObject(hdc, num8.HBR);
						num8.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num8.RE, num8.HBR);
						SelectObject(hdc, num8.HF);
						SetTextColor(hdc, RGBWhite);
						SetBkColor(hdc, RGBRed);
						TextOut(hdc, 60 + 100 * i, 55 + 100 * j, num8.NUM, sizeof(num8.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 16)
					{
						SelectObject(hdc, num16.HBR);
						num16.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num16.RE, num16.HBR);
						SelectObject(hdc, num16.HF);
						SetTextColor(hdc, RGBBlack);
						SetBkColor(hdc, RGBYellow);
						TextOut(hdc, 53 + 100 * i, 55 + 100 * j, num16.NUM, sizeof(num16.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 32)
					{
						SelectObject(hdc, num32.HBR);
						num32.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num32.RE, num32.HBR);
						SelectObject(hdc, num32.HF);
						SetTextColor(hdc, RGBBlack);
						SetBkColor(hdc, RGBOrange);
						TextOut(hdc, 53 + 100 * i, 55 + 100 * j, num32.NUM, sizeof(num32.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 64)
					{
						SelectObject(hdc, num64.HBR);
						num64.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num64.RE, num64.HBR);
						SelectObject(hdc, num64.HF);
						SetTextColor(hdc, RGBWhite);
						SetBkColor(hdc, RGBRed);
						TextOut(hdc, 53 + 100 * i, 55 + 100 * j, num64.NUM, sizeof(num64.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 128)
					{
						SelectObject(hdc, num128.HBR);
						num128.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num128.RE, num128.HBR);
						SelectObject(hdc, num128.HF);
						SetTextColor(hdc, RGBBlack);
						SetBkColor(hdc, RGBYellow);
						TextOut(hdc, 47 + 100 * i, 55 + 100 * j, num128.NUM, sizeof(num128.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 256)
					{
						SelectObject(hdc, num256.HBR);
						num256.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num256.RE, num256.HBR);
						SelectObject(hdc, num256.HF);
						SetTextColor(hdc, RGBBlack);
						SetBkColor(hdc, RGBOrange);
						TextOut(hdc, 47 + 100 * i, 55 + 100 * j, num256.NUM, sizeof(num256.NUM) / 2);
					}
					else if (tzfe.board[i][j].number == 512)
					{
						SelectObject(hdc, num512.HBR);
						num512.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num512.RE, num512.HBR);
						SelectObject(hdc, num512.HF);
						SetTextColor(hdc, RGBWhite);
						SetBkColor(hdc, RGBRed);
						TextOut(hdc, 47 + 100 * i, 55 + 100 * j, num512.NUM, sizeof(num512.NUM) / 2);
					}
					else
					{
						SelectObject(hdc, num1024p.HBR);
						num1024p.RE = { 21 + 100 * i,21 + 100 * j,119 + 100 * i,119 + 100 * j };
						FillRect(hdc, &num1024p.RE, num1024p.HBR);
						SelectObject(hdc, num1024p.HF);
						SetTextColor(hdc, RGBWhite);
						SetBkColor(hdc, RGBBlack);
						if(tzfe.board[i][j].number == 1024)
							TextOut(hdc, 38 + 100 * i, 55 + 100 * j, L"1024", sizeof(L"1024") / 2);
						else
							TextOut(hdc, 38 + 100 * i, 55 + 100 * j, L"2048", sizeof(L"2048") / 2);
					}
            EndPaint(hWnd, &ps);
			
        }
        break;
    case WM_DESTROY:
		DeleteObject(hFont);
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

