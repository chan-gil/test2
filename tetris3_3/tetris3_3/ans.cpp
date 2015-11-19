#include <iostream>
#include <windows.h> //GetStdHandle(), Sleep(), SetConsolePosition() 함수 사용
#include <conio.h> //getch()와 kbhit() 함수 사용
#include <ctime> //time()함수 사용
#include <cstdlib> //rand()함수와 srand()함수 사용

#define ESC 27 //게임 종료
#define LEFT 75 //왼쪽 화살표 키 ASCII값
#define RIGHT 77 // 오른쪽 화살표 키 ASCII값
#define DOWN 80 //아래 화살표 키 ASCII값
#define UP 72 //위쪽 화살표 키 ASCII값
#define SPACE 32 //스페이스 키 ASCII값 

#define BOARD_HEIGHT 22 //게임 보드 높이
#define BOARD_WIDTH  12 //게임 보드 넓이 
#define NEXT_HEIGHT  8 //다음 블럭 보드 높이
#define NEXT_WIDTH   8 //다음 블럭 보드 넓이

#define SCORE_HEIGHT 3 //점수 블럭 보드 높이
#define SCORE_WIDTH  8 //점수 블럭 보드 넓이

#define BSIZE 4 //블럭 높이 & 넓이
#define BROTATION 4 // 블럭 회전
#define BSHAPE 7


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

//MOVE_LEFT: 블럭 왼쪽 1칸 이동, MOVE_RIGHT: 블럭 오른쪽 1칸 이동, MOVE_DOWN: 블럭 아래 1칸 이동
//MOVE_DROP: 블럭 아래 1칸 자동 이동, GAME_QUIT: 게임종료

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP,ROTATION, GAME_QUIT};

struct Block {
	int X;//보드 안에 블럭의 X좌표  
	int Y; // 보드 안에 블럭의 Y좌표
	int R; // 블럭의 회전
	int S; // 블럭의 모양
	int N; // 다음 블럭 모양
};

struct GameInfo{
	int  action;//블럭 운영 방식
	int  score; // 게임점수
	bool flag; //게임 종료 플래그
};

void initGame(Block &blk, GameInfo &gm); //게임 초기화
void gotoXY(int x, int y); //콘솔 화면에서 커서를 특정 위치로 이동
int decodingKey(void); //눌린 키값 반환
void updateGame(Block &blk, GameInfo &gm); //게임의 상태값 업데이트 함수
int getGameAction(int action); // 게임의 기능을 선택하는 함수
void drawGame(const Block &blk, const GameInfo &gm); // 게임 화면을 그리는 함수
bool collision(const Block &blk);  //블록이 게임보드와 충돌이 났는지 확인하는 함수
void removeFullLines(GameInfo &gm); //게임보드에 블럭이 쌓였을 시에 블럭으로 채워진 라인을 제거함
void stack(const Block &blk); // 블록이 아래도 이동한 후에 충돌확인이 발생하면 블록을 게임보드에 쌓는 함수

int board[BOARD_HEIGHT][BOARD_WIDTH]; //게임 보드를 위한 2차원 배열 
int nextBoard[NEXT_HEIGHT][NEXT_WIDTH]; // 다음 블럭 표시 보드를 위한 2차원 배열 
int scoreBoard[SCORE_HEIGHT][SCORE_WIDTH];// 점수 출력 보드를 위한 2차원 배열 


// 블럭의 회전까지 고려한 3차원 배열
int block[BSHAPE][BROTATION][BSIZE][BSIZE]={
   {{0,0,0,0,
	 1,1,1,1,
	 0,0,0,0,
	 0,0,0,0}, //수평블럭 0도 회전
	{0,1,0,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,1,0,0}, //수평블럭 90도 회전
	{0,0,0,0,
	 1,1,1,1,
	 0,0,0,0,
	 0,0,0,0}, //수평블럭 180도 회전
	{0,1,0,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,1,0,0}}, //수평블럭 270도 회전
    {{0,1,0,0,   
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // 오형블럭 0도 회전
	{0,1,0,0,
	 0,1,1,0,
	 0,1,0,0,
	 0,0,0,0}, // 오형블럭 90도 회전
	{0,0,0,0,
	 1,1,1,0,
	 0,1,0,0,
	 0,0,0,0}, // 오형블럭 180도 회전
	{0,1,0,0,
	 1,1,0,0,
	 0,1,0,0,
	 0,0,0,0}}, // 오형블럭 270도 회전
   {{1,1,0,0,
	 0,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // Z형블럭 0도 회전
	{0,1,0,0,
	 1,1,0,0,
	 1,0,0,0,
	 0,0,0,0}, // Z형블럭 90도 회전
	{1,1,0,0,
	 0,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // Z형블럭 180도 회전
	{0,1,0,0,
	 1,1,0,0,
	 1,0,0,0,
	 0,0,0,0}}, // Z형블럭 270도 회전
   {{0,1,1,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // Z반사형블럭 0도 회전
	{0,1,0,0,
	 0,1,1,0,
	 0,0,1,0,
	 0,0,0,0},// Z반사형블럭 90도 회전
	{0,1,1,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // z반사형블럭 180도 회전
	{0,1,0,0,
	 0,1,1,0,
	 0,0,1,0,
	 0,0,0,0}}, // Z반사형블럭 270도 회전
   {{1,1,0,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,0,0,0}, // ㄱ형블럭 0도 회전 
	{0,0,1,0,
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // ㄱ형블럭 90도 회전
	{0,1,0,0,
	 0,1,0,0,
	 0,1,1,0,
	 0,0,0,0}, // ㄱ형블럭 180도 회전
	{0,0,0,0,
	 1,1,1,0,
	 1,0,0,0,
	 0,0,0,0}}, // ㄱ형블럭 270도 회전
   {{0,1,1,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,0,0,0}, // L형블럭 0도 회전
	{0,0,0,0,
	 1,1,1,0,
	 0,0,1,0,
	 0,0,0,0}, // L형블럭 90도 회전
	{0,1,0,0,
	 0,1,0,0,
	 1,1,0,0,
	 0,0,0,0},// L형블럭 180도 회전
	{1,0,0,0,
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0}},// L형블럭 270도 회전
   {{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // ㅁ형블럭 0도 회전
	{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0},//ㅁ형블럭 90도 회전
	{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // ㅁ형블럭 180도 회전
	{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}} // ㅁ형블럭 270도 회전
};

int main(void)
{
	Block block;
	GameInfo game;

	initGame(block, game);
	
	while(game.flag != false)
	{ 	
		game.action=getGameAction(game.action);
		updateGame(block,game);
		drawGame(block, game);
		Sleep(80); 
	}

	return 0;
}

void initGame(Block &blk, GameInfo &gm) //게임 초기 상태 설정
{
	srand(time(NULL));

	blk.X = BOARD_WIDTH/2 -2;
	blk.Y = 1;
	blk.R = 0;
	blk.S = rand()%BSHAPE;
	blk.N = rand()%BSHAPE;
	gm.action = MOVE_DOWN;
	gm.flag = true;
	gm.score = 0;
	
	int x, y;

	//게임보드 초기화
	for (y=0; y<BOARD_HEIGHT; y++)
	{
		for( x=0; x<BOARD_WIDTH; x++)
		{
			if ((x == 0 || x == BOARD_WIDTH-1) || (y == 0 || y == BOARD_HEIGHT-1))
				board[y][x]= BOARD_BORDER;
			else
				board[y][x]= BOARD_INSIDE;
		}
	}

  //다음 블럭 보드 초기화
	for ( y=0; y< NEXT_HEIGHT; y++)
	{
		for( x=0; x<NEXT_WIDTH; x++)
		{
			if ((x == 0 || x == NEXT_WIDTH-1) || (y == 0 || y == NEXT_HEIGHT-1))
				nextBoard[y][x]= BOARD_BORDER;
			else
				nextBoard[y][x]= BOARD_INSIDE;
		}
	}

   //점수 보드 초기화
	for ( y=0; y< SCORE_HEIGHT; y++)
	{
		for(x=0; x<SCORE_WIDTH; x++)
		{
			if ((x == 0 || x == SCORE_WIDTH-1) || (y == 0 || y == SCORE_HEIGHT-1))
				scoreBoard[y][x]= BOARD_BORDER;
			 else
				scoreBoard[y][x]= BOARD_INSIDE;
		}
	}

}

void drawGame(const Block &blk, const GameInfo &gm)
{
 int x,y;

 //보드 그리기
for (y=0; y<BOARD_HEIGHT; y++)
 {
	 gotoXY(BOARD_STARTX, BOARD_STARTY+y);
	 for (x=0; x<BOARD_WIDTH; x++)
	 {
		if(board[y][x] ==  BLOCK) cout << "■";
		else if(board[y][x]== BOARD_BORDER) cout << "□";
		else cout << "  ";
	 }
 }

 //블록 그리기

 for (y=0; y<BSIZE; y++)
 {
	for (x=0; x<BSIZE; x++)
	{
		if (block[blk.S][blk.R][y][x] == BLOCK )
		{
			 gotoXY(BOARD_STARTX + (blk.X + x)*2, BOARD_STARTY + blk.Y + y);
			 cout << "■";
		}	
	}
 }


  //다음 블록 보드 그리기
 gotoXY(NEXT_STARTX + 3, NEXT_STARTY -1);
 cout << "NEXT BLOCK";

 for (y=0; y<NEXT_HEIGHT; y++)
 {
	 gotoXY(NEXT_STARTX, NEXT_STARTY+y);
	 for (x=0; x<NEXT_WIDTH; x++)
		if(nextBoard[y][x] ==  BOARD_BORDER)
			cout << "□";
		else
			cout << "  ";

	 
 }
 // 다음 블록 보드에 블럭 그리기
 for (y=0; y<BSIZE; y++)
 {
	for (x=0; x<BSIZE; x++)	
	{
		if (block[blk.N][0][y][x] == BLOCK )
		{
			 gotoXY(NEXT_STARTX + x*2 + 4, NEXT_STARTY + y + 2);
			 cout << "■";
		}	
	}
 }

 //점수 보드 그리기
 for (y=0; y<SCORE_HEIGHT; y++)
 {
	 gotoXY(SCORE_STARTX, SCORE_STARTY+y);
	 for (x=0; x<SCORE_WIDTH; x++)
		if(scoreBoard[y][x] ==  BOARD_BORDER)
			cout << "□";
	     else
			cout << "  ";	 
 }

 //점수 출력
   gotoXY(SCORE_STARTX + 3, SCORE_STARTY -1);
   cout << "S C O R E";
   gotoXY(SCORE_STARTX + 5, SCORE_STARTY + 1);
   cout << gm.score;
}

int getGameAction(int action)
{
 int keyValue,act;
 act = action;
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
		   case UP:
			  act = ROTATION;
      			  break;
		   case SPACE:
			  act = MOVE_DROP;
		   default :
			  break;
	  }
 }
 
 return act;
}

void updateGame(Block &blk, GameInfo &gm) 
{
	switch (gm.action)
  {
   case GAME_QUIT:
	   gm.flag = false;
	   break;
   case MOVE_LEFT: 
	   blk.X--;
	   if (collision(blk)) blk.X++;
	   gm.action = MOVE_DOWN;
           break;

   case MOVE_RIGHT: 
	   blk.X++;
	   if (collision(blk)) blk.X--;
	   gm.action = MOVE_DOWN;
      	   break;

   case MOVE_DOWN: 
	   blk.Y++;
	   if(collision(blk)) 
	   {
		   blk.Y--;
		   if (blk.Y == 1)
			   gm.flag = false;
		   else
		   {
			   stack(blk); //게임 객체 고정
			   removeFullLines(gm);
			   blk.X = BOARD_WIDTH/2 -2;
			   blk.Y = 1;
			   blk.R = 0;
			   blk.S = blk.N;
			   blk.N = rand()%BSHAPE;
		   }
	   }
	   gm.action = MOVE_DOWN;
	   break;
   case MOVE_DROP: 
	   while(!collision(blk))
		     blk.Y++;
	   blk.Y--;
	   if (blk.Y == 1) 
		   gm.flag = false;
	   else
	   {
		   stack(blk); //게임 객체 고정
		   removeFullLines(gm);
		   blk.X = BOARD_WIDTH/2 -2;
		   blk.Y = 1;
		   blk.R = 0;
		   blk.S = blk.N;
		   blk.N = rand()%BSHAPE;
		   gm.action = MOVE_DOWN;
	   }
   	   break;
   case ROTATION: 
		blk.R++;
	    if(blk.R > 3) blk.R= 0;
	    if (collision(blk))
	    {
		   blk.R--;
		   if(blk.R < 0) blk.R = 3;
	    }
	    gm.action = MOVE_DOWN;
      	break;
   default:
      	 break;
  }

 }

bool collision(const Block &blk) //게임 객체 충돌 검사
{
	 for (int y=0; y<BSIZE; y++)
	 {
		for (int x=0; x<BSIZE; x++)
		{
			if (block[blk.S][blk.R][y][x] == BLOCK )
			{
				if (board[blk.Y+y][blk.X+x] != BOARD_INSIDE)
				{
					return true; //충돌함			}
				}	
			 }
		 }
	 }
	 return false; //충돌 안함

}



//////////////////////////////////////////////////////////////
void stack(const Block &blk) //게임 객체 고정
{
 for (int y=0; y<BSIZE; y++)
	for (int x=0; x<BSIZE; x++)
		if (block[blk.S][blk.R][y][x] == BLOCK )
			board[blk.Y+y][blk.X+x] = BLOCK;
}


//////////////////////////////////////////////////////////////
void removeFullLines(GameInfo &gm) //누적 블록 제거 점수 올리기
{
	int line;

	for(int y=1; y<BOARD_HEIGHT-1; y++)
	{
		line = 0;
		for (int x=1; x<BOARD_WIDTH -1; x++)
			line += board[y][x];
		if (line == (BOARD_WIDTH-2))
		{
			gm.score += 10;
			for (int i=y-1; i>0; i--)
				for (int j=1; j< BOARD_WIDTH-1; j++)
					board[i+1][j] = board[i][j];
		}
	}
}


//콘솔 화면에서 커서를 특정 위치로 이동
void gotoXY(int x, int y) 
{
 COORD Pos = {x, y};
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}