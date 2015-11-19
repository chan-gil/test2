
#include <iostream>
#include <windows.h> //GetStdHandle(), Sleep(), SetConsolePosition() 함수 사용
#include <conio.h> //getch()와 kbhit() 함수 사용

#define ESC 27 //게임 종료
#define LEFT 75 //왼쪽 화살표 키 ASCII값
#define RIGHT 77 // 오른쪽 화살표 키 ASCII값
#define DOWN 80 //아래 화살표 키 ASCII값
#define UP 72 //위쪽 화살표 키 ASCII값

#define BOARD_HEIGHT 22 //게임 보드 높이
#define BOARD_WIDTH  12 //게임 보드 넓이 
#define NEXT_HEIGHT  8 //다음 블럭 보드 높이
#define NEXT_WIDTH   8 //다음 블럭 보드 넓이

#define SCORE_HEIGHT 3 //점수 블럭 보드 높이
#define SCORE_WIDTH  8 //점수 블럭 보드 넓이

#define BSIZE 4 //블럭 높이 & 넓이
#define BROTATION 4 // 블럭 회전


//게임 보드 배열에서 보드 경계선, 보드 내부,  블럭 표시
#define BOARD_BORDER 2 //게임 보드 경계선 표시
#define BOARD_INSIDE 0 //게임 보드 내부 표시
#define BLOCK 1 //블럭 표시


//게임 보드 그리기 시작 위치
#define BOARD_STARTX 20  
#define BOARD_STARTY 2

//다음 블럭 그리기 시작 위치
#define NEXT_STARTX  45 
#define NEXT_STARTY  3

//점수 그리기 시작 위치
#define SCORE_STARTX 45
#define SCORE_STARTY 21

using namespace std;



//MOVE_LEFT: 블럭 왼쪽 1칸 이동, MOVE_RIGHT: 블럭 오른쪽 1칸 이동, MOVE_DOWN: 블럭 아래 1칸 이동 MOVE_DROP: 블럭 아래 1칸 자동 이동, GAME_QUIT: 게임종료

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP, GAME_QUIT};



void initGame(); //게임 초기화
void gotoXY(int x, int y); //콘솔 화면에서 커서를 특정 위치로 이동
void updateGame(int move); //게임의 상태값 업데이트 함수
int getGameAction(); // 게임의 기능을 선택하는 함수
void drawGame(int blockX, int blockY); // 게임 화면을 그리는 함수
bool collision(int blockX, int blockY);  //블록이 게임보드와 충돌이 났는지 확인하는 함수

int blockX, blockY; //보드 안에 블럭의 위치  
int board[BOARD_HEIGHT][BOARD_WIDTH]; //게임 보드를 위한 2차원 배열 
int nextBoard[NEXT_HEIGHT][NEXT_WIDTH]; // 다음 블럭 표시 보드를 위한 2차원 배열 
int scoreBoard[SCORE_HEIGHT][SCORE_WIDTH];// 점수 출력 보드를 위한 2차원 배열 


// 블럭의 회전까지 고려한 3차원 배열
int block[BSIZE][BSIZE]=
         {{0,0,0,0},
	 {1,1,1,1},
	 {0,0,0,0},
	 {0,0,0,0}};

int  gameScore;
bool gameStart;
int  gameAction; //블럭 운영 방식

int main(void)
{
       
	initGame();
	
	while(gameAction != GAME_QUIT)
	{ 	
		gameAction=getGameAction();
		updateGame(gameAction);
		drawGame(blockX,blockY);
		Sleep(50); 
	}

	return 0;
}

void initGame() //게임 초기 상태 설정
{

	blockX = BOARD_WIDTH/2 -2;
	blockY = 1;
	gameScore = 0;
	gameAction = MOVE_DROP;
	int x, y;

	//게임보드 초기화
	for (y=0; y<BOARD_HEIGHT; y++)
	{
		for( x=0; x<BOARD_WIDTH; x++)
		{
			if ((x == 0 || x == BOARD_WIDTH-1) ||
				(y == 0 || y == BOARD_HEIGHT-1))
			{
				board[y][x]= BOARD_BORDER;
			} else
			{
				board[y][x]= BOARD_INSIDE;
			}
		}
	}

  //다음 블럭 보드 초기화
	for ( y=0; y< NEXT_HEIGHT; y++)
	{
		for( x=0; x<NEXT_WIDTH; x++)
		{
			if ((x == 0 || x == NEXT_WIDTH-1) ||
				(y == 0 || y == NEXT_HEIGHT-1))
			{
				nextBoard[y][x]= BOARD_BORDER;
			} else
			{
				nextBoard[y][x]= BOARD_INSIDE;
			}
		}
	}

   //점수 보드 초기화
	for ( y=0; y< SCORE_HEIGHT; y++)
	{
		for(x=0; x<SCORE_WIDTH; x++)
		{
			if ((x == 0 || x == SCORE_WIDTH-1) ||
				(y == 0 || y == SCORE_HEIGHT-1))
			{
				scoreBoard[y][x]= BOARD_BORDER;
			} else
			{
				scoreBoard[y][x]= BOARD_INSIDE;
			}
		}
	}

}

void drawGame(int blockX, int blockY)
{
	int x, y;
//1. 게임화면에 게임보드 창을 그리기
 for(y=0; y<BOARD_HEIGHT; y++){
		gotoXY(BOARD_STARTX, BOARD_STARTY+y);
		for(x=0; x<BOARD_WIDTH; x++){  
			//0으로 할당받은 부분 여백처리
			if(board[y][x]==0){   
				cout << "  ";
			}
			// 2로 받은 부분 박스 ( 테두리 )
			else if(board[y][x]==2){
				cout << "□";
			}
		}
	}

 //2. 게임보드 창에 블록 그리기
 for(int i=0; i<BOARD_HEIGHT; i++){
		gotoXY(BOARD_STARTX+8, BOARD_STARTY+i);
		for(int j=0; j<BOARD_WIDTH; j++){   
			if((j>=blockX&&j<blockX+4)&&(i==blockY)){
				cout << "■";
			}
		}
 }

//3. 다음 블록 보드 창을 그리기
  for(y=0; y<NEXT_HEIGHT; y++){
		gotoXY(NEXT_STARTX, NEXT_STARTY+y);
		for(x=0; x<NEXT_WIDTH; x++){   
			if(nextBoard[y][x]==0){   
				cout << "  ";
			}
			else if(nextBoard[y][x]==2){
				cout << "□";
			}
		}
	}

//4. 다음 블록 보드에 블럭 그리기
 for (y=0; y<BSIZE; y++)
 {
	for (x=0; x<BSIZE; x++)	
	{
		if (block[y][x] == BLOCK )
		{
			 gotoXY(NEXT_STARTX + x*2 + 4, NEXT_STARTY + y + 2);
			 cout << "■";
		}	
	}
 }

 //5. 게임 화면에 점수 보드 창을 그리기
 for (y=0; y<SCORE_HEIGHT; y++)
 {
	 gotoXY(SCORE_STARTX, SCORE_STARTY+y);
	 for (x=0; x<SCORE_WIDTH; x++)
		if(scoreBoard[y][x] ==  BOARD_BORDER) cout << "□";
	     else
			cout << "  ";	 
 }

 //점수 출력
   gotoXY(SCORE_STARTX + 3, SCORE_STARTY -1);
   cout << "S C O R E";
   gotoXY(SCORE_STARTX + 5, SCORE_STARTY + 1);
   cout << gameScore;
}

int getGameAction()
{
 int keyValue,act=gameAction;
 if(kbhit() != 0) //키보드를 눌렀는지 확인함 
 {
	  keyValue=getch(); 
	 if (keyValue == 224) //특수 키를 눌렀을 때 버퍼에 2Byte가 발생함, 첫번째 값은 224값을 발생하고 두번째 값은 특수키에 따라 다름
	 {
	   keyValue=getch(); //특수 키를 확인하기 위해 2번의 getch()함수를 호출해야 함
	 }
	 switch (keyValue)
	 {
		   case ESC:
			   act = GAME_QUIT;
				   break;
		   case LEFT: 
			  act = MOVE_LEFT;
			  break;
		   case RIGHT: 
			  act = MOVE_RIGHT;
			  break;
		   case DOWN:
			  act = MOVE_DOWN;
      			  break;
		   default :
			  break;
	  }
	 return act;
 }
 
 return act;
}

void updateGame(int move) 
{
  switch (gameAction)
  {
   case MOVE_LEFT: 
	   blockX--;
	   if (!collision(blockX, blockY)) 
		   blockX++;
           break;

   case MOVE_RIGHT: 
	   blockX++;
	   if (!collision(blockX, blockY))
		   blockX--;
      	   break;

   case MOVE_DOWN: 
	   blockY++;
	   if((collision(blockX, blockY))&&(blockY==(BOARD_HEIGHT-1))) 
	   {
		   blockY--;
		   blockX = BOARD_WIDTH/2 -2;
		   blockY = 1;
		   gameScore += 10;
	   }
	   break;
   case MOVE_DROP: 
		gameAction = MOVE_DOWN;
      	   break;
   default:
      	 break;
  }

 }


bool collision(int blockX, int blockY) //게임 객체 충돌 검사
//충돌이 발생하면 true 반환, 충돌일 발생하지 않으면 false 반환함
{
	if(blockX>=1||blockX>=7)
		return true;
	else
		return false;
//충돌 체크 

}


//콘솔 화면에서 커서를 특정 위치로 이동
void gotoXY(int x, int y) 
{
 COORD Pos = {x, y};
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
