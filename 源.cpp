#include"my2048byJY.h"
void TZFE::newBoard()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			board[i][j].x = 100 * i + 70;
			board[i][j].y = 100 * j + 70;
			board[i][j].number = 0;
		}
}
int TZFE::readKeyboard()
{
	return _getch();
}

void TZFE::loopGame()
{
	while (1)
	{
		int cmd = readKeyboard();//接受标准输入流

		if (ifEndGame)
			if (cmd == 'Y' || cmd == 'y')
			{
				clear();
				return;
			}
			else if (cmd == 'N' || cmd == 'n')
			{
				ifEndGame = false;
				refreshShow();
				continue;
			}
			else
				continue;

		if (ifGameOver)
		{
			if (cmd == 'Y' || cmd == 'y')
			{
				resetGame();
				continue;
			}
			else if (cmd == 'N' || cmd == 'n')
			{
				clear();
				return;
			}
			else
				continue;
		}

		ifNeedRandomNumber = false;//缺省为0

		switch (cmd)
		{
		case'a':
		case 75:moveLeft();
			break;
		case's':
		case 80:moveDown();
			break;
		case'w':
		case 72:moveUp();
			break;
		case'd':
		case 77:moveRight();
			break;
		case'q':
		case 27:ifEndGame = true;
			break;
		default:continue;
		}

		if (score > bestScore)
		{
			bestScore = score;
			FILE* fp = fopen(configPath, "w");
			if (fp)
			{
				fwrite(&bestScore, sizeof(bestScore), 1, fp);
				fclose(fp);
			}//以上为文件储存部分???
		}

		if (ifNeedRandomNumber)
		{
			randomNumber();
			refreshShow();
		}
		else if (ifEndGame)
			refreshShow();
	}
}

void TZFE::resetGame()
{
	score = 0;
	ifNeedRandomNumber = true;
	ifGameOver = false;
	ifEndGame = false;

	int n = rand() % 16;
	int i;//i，j为第一个2所在的位置
	for (i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			board[i][j].number = (n-- == 0 ? 2 : 0);

	randomNumber();//生成一个新的随机数

	refreshShow();//刷新界面
}

void TZFE::randomNumber()
{
	srand(unsigned(time(0)));
	if (nullNumber() == 0)
		return;
	int n = rand() % nullNumber();//随机数位置
	int i;//i,j为出现数的位置
	for (i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (board[i][j].number == 0 && n-- == 0)
			{
				board[i][j].number = (rand() % 10 ? 2 : 4);
				return;
			}
}

int TZFE::nullNumber()
{
	int n = 0;//n为输出
	int i;//i,j为判断的坐标
	for (i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (board[i][j].number == 0)
				n++;
	return n;
}

void TZFE::ifLoseGame()
{
	int i;//i,j为判断的坐标
	for (i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			if (board[i][j].number == board[i][j + 1].number || board[j][i].number == board[j + 1][i].number)
			{
				ifGameOver = false;
				return;
			}
	ifGameOver = true;
}

void TZFE::moveUp()
{
	//i为行标
	for (int i = 0; i < 4; i++)
		//j为列标，k为比较项，其中就j>k
		for (int j = 1, k = 0; j < 4; j++)
			if (board[i][j].number > 0)//找出k后第一个非零项
				if (board[i][j].number == board[i][k].number)
				{
					score += board[i][k++].number *= 2;
					board[i][j].number = 0;
					ifNeedRandomNumber = true;//需要随机数了
				}
				else if (board[i][k].number == 0)
				{
					board[i][k].number = board[i][j].number;
					board[i][j].number = 0;
					ifNeedRandomNumber = true;//需要随机数了
				}
				else
				{
					board[i][++k].number = board[i][j].number;
					if (j != k)//这是判断二者是否一开始就相接
					{
						board[i][j].number = 0;
						ifNeedRandomNumber = true;//需要随机数了
					}
				}

}
//下面是另外三个方向移动函数，就不给注释了
void TZFE::moveDown()
{
	for (int i = 0; i < 4; i++)
		for (int j = 2, k = 3; j >= 0; j--)
			if (board[i][j].number > 0)
				if (board[i][j].number == board[i][k].number)
				{
					score += board[i][k--].number *= 2;
					board[i][j].number = 0;
					ifNeedRandomNumber = true;
				}
				else if (board[i][k].number == 0)
				{
					board[i][k].number = board[i][j].number;
					board[i][j].number = 0;
					ifNeedRandomNumber = true;
				}
				else
				{
					board[i][--k].number = board[i][j].number;
					if (j != k)
					{
						board[i][j].number = 0;
						ifNeedRandomNumber = true;
					}
				}
}

void TZFE::moveLeft()
{
	for (int i = 0; i < 4; i++)
		for (int j = 1, k = 0; j < 4; j++)
			if (board[j][i].number > 0)
				if (board[k][i].number == board[j][i].number)
				{
					score += board[k++][i].number *= 2;
					board[j][i].number = 0;
					ifNeedRandomNumber = true;
				}
				else if (board[k][i].number == 0)
				{
					board[k][i].number = board[j][i].number;
					board[j][i].number = 0;
					ifNeedRandomNumber = true;
				}
				else
				{
					board[++k][i].number = board[j][i].number;
					if (j != k)
					{
						board[j][i].number = 0;
						ifNeedRandomNumber = true;
					}
				}
}

void TZFE::moveRight()
{
	for (int i = 0; i < 4; i++)
		for (int j = 2, k = 3; j >= 0; j--)
			if (board[j][i].number > 0)
				if (board[j][i].number == board[k][i].number)
				{
					score += board[k--][i].number *= 2;
					board[j][i].number = 0;
					ifNeedRandomNumber = true;
				}
				else if (board[k][i].number == 0)
				{
					board[k][i].number= board[j][i].number;
					board[j][i].number = 0;
					ifNeedRandomNumber = true;
				}
				else
				{
					board[--k][i].number = board[j][i].number;
					if (j != k)
					{
						board[j][i].number = 0;
						ifNeedRandomNumber = true;
					}
				}
}

void TZFE::clear()
{
	//以下代码为windows平台代码
	COORD pos = { 0,0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	CONSOLE_CURSOR_INFO info = { 1,0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void TZFE::refreshShow()
{
	clear();
	
	if (nullNumber() == 0)
	{
		ifLoseGame();
		if (ifGameOver)
		{
			CONSOLE_CURSOR_INFO info = { 1,1 };
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		}
	}

	if (ifEndGame)
	{
		CONSOLE_CURSOR_INFO info = { 1, 1 };
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}

	fflush(0);
}



void TZFE::newGame()
{
	//获取最高分

	newBoard();
	FILE* fp = fopen(configPath, "r");
	if (fp)
	{
		fread(&bestScore, sizeof(bestScore), 1, fp);
		fclose(fp);
	}
	else
	{
		bestScore = 0;
		fp = fopen(configPath, "w");
		if (fp)
		{
			fwrite(&bestScore, sizeof(bestScore), 1, fp);
			fclose(fp);
		}
	}
	resetGame();
}

void TZFE::endGame(int signal)
{
	system("cls");
	CONSOLE_CURSOR_INFO info = { 1,1 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	exit(0);
}

void DisplayMessageBox(HWND hWnd, const TCHAR* szText, const TCHAR* szTitle, void (*OKFunction)(), void (*CancelFunction)())
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
//以下为语音识别所需代码
/*using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
string sentence;
void recognizeSpeech()
{
	// Creates an instance of a speech config with specified subscription key and service region.
	// Replace with your own subscription key and service region (e.g., "westus").
	auto config = SpeechConfig::FromSubscription("YourSubscriptionKey", "YourServiceRegion");

	// Creates a speech recognizer.
	auto recognizer = SpeechRecognizer::FromConfig(config);
	cout << "Say something...\n";

	// Starts speech recognition, and returns after a single utterance is recognized. The end of a
	// single utterance is determined by listening for silence at the end or until a maximum of 15
	// seconds of audio is processed.  The task returns the recognition text as result. 
	// Note: Since RecognizeOnceAsync() returns only a single utterance, it is suitable only for single
	// shot recognition like command or query. 
	// For long-running multi-utterance recognition, use StartContinuousRecognitionAsync() instead.
	auto result = recognizer->RecognizeOnceAsync().get();

	// Checks result.
	if (result->Reason == ResultReason::RecognizedSpeech)
	{
		cout << "We recognized: " << result->Text << std::endl;
		sentence=result->Text;
	}
	else if (result->Reason == ResultReason::NoMatch)
	{
		cout << "NOMATCH: Speech could not be recognized." << std::endl;
	}
	else if (result->Reason == ResultReason::Canceled)
	{
		auto cancellation = CancellationDetails::FromResult(result);
		cout << "CANCELED: Reason=" << (int)cancellation->Reason << std::endl;

		if (cancellation->Reason == CancellationReason::Error)
		{
			cout << "CANCELED: ErrorCode= " << (int)cancellation->ErrorCode << std::endl;
			cout << "CANCELED: ErrorDetails=" << cancellation->ErrorDetails << std::endl;
			cout << "CANCELED: Did you update the subscription info?" << std::endl;
		}
	}
}

*/