#include "core.h"

// 全局变量
/*int nRows = 10 ;
int mCols = 10 ;
int MAXDEP = 4 ;*/

int main() {
	int op ;
	printf( "Choose difficulty\n1.easy\n2.normal\n3.hard\n" ) ;
	scanf( "%d" , &op ) ;
	GAME game ;
	game.Run(op) ;
	return 0 ;
}
