#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std ;

#define N 30
#define M 30
const int nRows = 10 ;
const int mCols = 10 ;
const int MAXDEP = 4 ;
const int Direction[8][2] = { { -1 , -1 } , { -1 , 0 } , { -1 , 1 } , { 0 , -1 } , { 0 , 1 } , { 1 , -1} , { 1 , 0 } , { 1 , 1 } } ; //�˸�����

struct Point
{
	int x , y ;
	Point ( int X = 0 , int Y = 0 ) { x = X , y = Y ; }
	bool Check()
	{
		if ( x < 0 || y < 0 || x > nRows || y > mCols ) return 0 ;
		return 1 ;
	}
} ;

class Board
{
private:
	int state[N][M] ;
	int empty ;	//����ĸ�������
public:
	Board() { } ;
	Board(int *board) ;
	Board(const Board &board) ;
	~Board() ;
	
	void PrintBoard() ;
	void ResetChess( Point w ) ;
	bool SetChess( Point w , int player ) ;
	int GetState( Point w , int direct , int dist ) ;		//ȡ��ĳһ�����Ͼ൱ǰ��distλ�õ�״̬
	void Getline( int *line , Point p , int direct , int num ) ;  	//ȡ������num�����ӵ�״̬
	int CalculateValue( int nPlayer , int nEmpty ) ;		//���ز�ͬ���͵Ĺ���
	int Calculate( int *line , int player ) ;					//����һ��ֱ�ߵĹ���
	int IsFinal() ;  						//�ж���Ϸ�Ƿ����
	int Evaluate() ;  						//���ۺ�����������ǰ���
	void clear() ;
} ;

class AI
{
private:
	Board virtualBoard ;		//AI��ģ�����
public:
	AI(const Board board) ;
	AI(const AI &ai) ;
	~AI() ;

	void GenerateQueue( vector <Point> *decide ) ; 	//���ɾ��߶���
	int AlphaBeta( int player , int depth , int alpha , int beta , Point &best ) ; //����С���� player=1 ���� player=2 ����
} ;

class GAME
{
private:
	Board chessBoard ;
public:
	void Run() ;
} ;

//================================ class Board =========================================//

Board::Board(int *board)
{
	memcpy( state , board , sizeof(board) ) ;
}

Board::Board(const Board &board)
{
	memcpy( state , board.state , sizeof(board.state) ) ;
	empty = board.empty ;
}

Board::~Board() { }

void Board::PrintBoard()
{
	printf( "   " ) ;
	for (int j = 1 ; j <= mCols ; j ++ ) printf( "%2d " , j ) ;
	printf( "\n" ) ;
	for (int i = 1 ; i <= nRows ; i ++ ) {
		printf( "%3d" , i ) ;
		for (int j = 1 ; j <= mCols ; j ++ ) {
			if ( state[i][j] ) printf( " %c " , (char)state[i][j] == 1 ? '*' : '#' ) ;
			else printf( "   " ) ;
		}
		printf( "\n" ) ;
	}
}

int Board::GetState( Point w , int direct , int dist )
{
	w.x += Direction[direct][0] * dist ;
	w.y += Direction[direct][1] * dist ;
	if ( !w.Check() ) return -1 ;
	return state[w.x][w.y] ;
}

void Board::ResetChess( Point w )
{
	state[w.x][w.y] = 0 ;
}

bool Board::SetChess( Point w , int player )
{
	if ( !state[w.x][w.y] ) {
		empty -- ;
		return state[w.x][w.y] = player ;
	}
	return 0 ;
}

void Board::Getline( int *line , Point p , int direct , int num )
{
	int delx = Direction[direct][0] ;
	int dely = Direction[direct][1] ;
	line[1] = state[p.x][p.y] ;
	line[0] = 1 ;
	for (int tot = 2 ; tot <= num ; tot ++ )
	{
		p.x += delx , p.y += dely ;
		if ( !p.Check() ) break ;
		line[tot] = state[p.x][p.y] ;
		line[0] = tot ;
	}
}

int Board::IsFinal()
{
	int line[8] ;
	for (int i = 1 ; i <= nRows ; i ++ )
	{
		for (int j = 1 ; j <= mCols ; j ++ )
		{
			if ( !state[i][j] ) continue ;
			for (int k = 0 ; k < 8 ; k ++ )
			{
				Getline( line ,  Point( i , j ) , k , 5 ) ;
				if ( line[0] < 5 ) continue ;
				bool flag = 1 ;
				for (int x = 2 ; x <= 5 ; x ++ )
				{
					if ( line[x] != line[x-1] ) { flag = 0 ; break ; }
				}
				if ( flag ) return 1 ;
			}
		}
	}
	if ( empty == 0 ) return -1 ;
	return 0 ;
}

int Board::CalculateValue( int nPlayer , int nEmpty ) {
	// ���ز�ͬ���͵Ĺ���
	
	if ( nPlayer >= 5 ) return 100000 ;		// ����
	else if ( nPlayer >= 4 )
	{
		if ( nEmpty >= 2 ) return 10000 ;	// ������	011110��022220
		else if ( nEmpty ) return 3000 ;	// ������	011112��022221
	}
	else if ( nPlayer >= 3 )
	{
		if ( nEmpty >= 2 ) return 3000 ;	// ������	01110��02220
		else if ( nEmpty ) return 300 ;		// ������	01112��02221
	}
	else if ( nPlayer >= 2 )
	{
		if ( nEmpty >= 2 ) return 400 ;		// �����	0110��0220
		else if ( nEmpty ) return 200 ;		// ������	0112��0221
	}
	else if ( nPlayer >= 1 )
	{
		if ( nEmpty >= 2 ) return 40 ;		// ��һ��	010��020
		else if ( nEmpty ) return 20 ;		// ��һ��	012��021
	}
	return 0 ;
}

int Board::Calculate( int *line , int player ) {

	int watch[100] ;
	for (int i = 0 ; i <= line[0] ; i ++ ) watch[i] = line[i] ;

	// ����ֱ���ϵĹ���
	int value = 0 ;
	int num_of_empty = 0 ;
	int num_of_player = 0 ;
	for (int i = 1 ; i <= line[0] ; i ++ )
	{
		if ( line[i] == player ) num_of_player ++ ;
		else if ( line[i] == 0 )
		{
			value += CalculateValue( num_of_player , num_of_empty + 1 ) ;
			num_of_player = 0 ;
			num_of_empty = 1 ;
		} else
		{
			value += CalculateValue( num_of_player , num_of_empty ) ;
			num_of_player = 0 ;
			num_of_empty = 0 ;
		}
	}
	value += CalculateValue( num_of_player , num_of_empty ) ;
	return value ;
}

int Board::Evaluate()
{
	// ������ǰ���
	int firstValue = 0 ;    // ���ֵĹ���
	int secondValue = 0 ;	// ���ֵĹ���
	int line[N+M] ;
	line[0] = 0 ;
	
	// �� k = 4
	for (int i = 1 ; i <= nRows ; i ++ )
	{
		line[0] = 0 ;
		Getline( line , Point( i , 1 ) , 4 , mCols ) ;
		firstValue += Calculate( line , 1 ) ;
		secondValue += Calculate( line , 2 ) ;
	}
	
	// �� k = 6
	for (int j = 1 ; j <= mCols ; j ++ )
	{
		line[0] = 0 ;
		Getline( line , Point( 1 , j ) , 6 , nRows ) ;
		firstValue += Calculate( line , 1 ) ;
		secondValue += Calculate( line , 2 ) ;
	}

	// ��б��(�ϰ벿��) k = 7
	for (int j = 1 ; j <= mCols ; j ++ )
	{
		line[0] = 0 ;
		Getline( line , Point( 1 , j ) , 7 , N + M ) ;
		firstValue += Calculate( line , 1 ) ;
		secondValue += Calculate( line , 2 ) ;
	}

	// ��б��(�°벿��) k = 7
	for (int i = 2 ; i <= nRows ; i ++ )
	{
		line[0] = 0 ;
		Getline( line , Point( i , 1 ) , 7 , N + M ) ;
		firstValue += Calculate( line , 1 ) ;
		secondValue += Calculate( line , 2 ) ;
	}

	// ��б��(�ϰ벿��) k = 5
	for (int j = 1 ; j <= mCols ; j ++ )
	{
		line[0] = 0 ;
		Getline( line , Point( 1 , j ) , 5 , N + M ) ;
		firstValue += Calculate( line , 1 ) ;
		secondValue += Calculate( line , 2 ) ;
	}

	// ��б��(�°벿��) k = 5
	for (int i = 2 ; i <= nRows ; i ++ )
	{
		line[0] = 0 ;
		Getline( line , Point( i , mCols ) , 5 , N + M ) ;
		firstValue += Calculate( line , 1 ) ;
		secondValue += Calculate( line , 2 ) ;
	}

	return firstValue - secondValue ; // ���ֵĹ���-���ֵĹ�����Ϊȫ�ֹ���
}

void Board::clear()
{
	memset( state , 0 , sizeof(state) ) ;
	empty = nRows * mCols ;
}

//================================ class AI =========================================//

AI::AI(const Board board)
{
	virtualBoard = board ;
}

AI::AI(const AI &ai)
{
	virtualBoard = ai.virtualBoard ;
}
AI::~AI() { }

void AI::GenerateQueue( vector <Point> *decide )
{
	for (int i = 1 ; i <= nRows ; i ++ ) 
	{
		for (int j = 1 ; j <= mCols ; j ++ ) 
		{
			Point w = Point( i , j ) ;
			if ( virtualBoard.GetState( w , 1 , 0 ) != 0 ) continue ;
			for (int k = 0 ; k < 8 ; k ++ ) 
			{
				if ( virtualBoard.GetState( w , k , 1 ) > 0 )
				{
					decide->push_back(w) ;
					break ;
				}
			}
		}
	}
}

int AI::AlphaBeta( int player , int depth , int alpha , int beta , Point &best )
{
	if ( depth > MAXDEP || virtualBoard.IsFinal() ) return virtualBoard.Evaluate() ;
	vector <Point> decide ;		//���߶���
	decide.clear() ;
	GenerateQueue( &decide ) ;
	int size = decide.size() ;
	for (int i = 0 ; i < size ; i ++ )
	{
		Point w = decide[i] ;
		if ( !virtualBoard.SetChess( w , player ) ) continue ;
		if ( player == 1 )
		{
			int ret = AlphaBeta( 2 , depth + 1 , alpha , beta , best ) ;
			virtualBoard.ResetChess( w ) ;
			if ( depth == 1 && ret > alpha ) best = w ;
			alpha = max( alpha , ret ) ;
			if ( beta <= alpha ) return alpha ;
		} else
		{
			int ret = AlphaBeta( 1 , depth + 1 , alpha , beta , best ) ;
			virtualBoard.ResetChess( w ) ;
			if ( depth == 1 && ret < beta ) best = w ;
			beta = min( beta , ret ) ;
			if ( beta <= alpha ) return beta ;
		}
	}
	if ( player == 1 ) return alpha ;
	else return beta ;
}

//================================ class GAME =========================================//

void GAME::Run()
{
	chessBoard.clear() ;
	int x , y ;
	while ( scanf( "%d%d" ,&x , &y ) != EOF )
	{
		Point setPoint = Point( x , y ) ;
		if ( !setPoint.Check() || !chessBoard.SetChess( setPoint , 1 ) )
		{
			printf( "Failed.Please choose a point again.\n" ) ;
			continue ;
		}
		printf( "Successfully!\n" ) ;
		chessBoard.PrintBoard() ;
		int boardState = chessBoard.IsFinal() ; // -1 ƽ�� 1 ��һ��ʤ�� 0 ��δ����ʤ��
		if ( boardState > 0 ) { printf( "YOU WIN!\n" ) ; break ; }
		else if ( boardState < 0 ) { printf( "DRAW!\n" ) ; break ; }

		AI computerPlayer(chessBoard) ;
		Point best = Point( 0 , 0 ) ;
		computerPlayer.AlphaBeta( 2 , 1 , -0x7FFFFFFF , 0x7FFFFFFF , best ) ;
		printf( "The computer choosed ( %d , %d )\n" , best.x , best.y ) ;

		chessBoard.SetChess( best , 2 ) ;
		chessBoard.PrintBoard() ;
		boardState = chessBoard.IsFinal() ; // -1 ƽ�� 1 ��һ��ʤ�� 0 ��δ����ʤ��
		if ( boardState > 0 ) { printf( "COMPUTER WIN!\n" ) ; break ; }
		else if ( boardState < 0 ) { printf( "DRAW!\n" ) ; break ; }
	}
}

int main() {
	GAME game ;
	game.Run() ;
	return 0 ;
}
