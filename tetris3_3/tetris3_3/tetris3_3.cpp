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

//MOVE_LEFT: 블럭 왼쪽 1칸 이동, MOVE_RIGHT: 블럭 오른쪽 1칸 이동, MOVE_DOWN: 블럭 아래 1칸 이동
//MOVE_DROP: 블럭 아래 1칸 자동 이동, GAME_QUIT: 게임종료

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP, ROTATION, GAME_QUIT};



void initGame(); //게임 초기화
void gotoXY(int x, int y); //콘솔 화면에서 커서를 특정 위치로 이동
int decodingKey(void); //눌린 키값 반환
void updateGame(void); //게임의 상태값 업데이트 함수
int getGameAction(int action); // 게임의 기능을 선택하는 함수
void drawGame(int blockX, int blockY,int blockR); // 게임 화면을 그리는 함수
bool collision(int blockX, int blockY, int rotation);  //블록이 게임보드와 충돌이 났는지 확인하는 함수
void stack(int blockX, int blockY, int rotation); // 블록이 아래도 이동한 후에 충돌확인이 발생하면 블록을 게임보드에 쌓는 함수


int blockX, blockY; //보드 안에 블럭의 위치  
int blockR; //블럭의 회전
bool gameFlag; //게임 종료 플래그
int board[BOARD_HEIGHT][BOARD_WIDTH]; //게임 보드를 위한 2차원 배열 
int nextBoard[NEXT_HEIGHT][NEXT_WIDTH]; // 다음 블럭 표시 보드를 위한 2차원 배열 
int scoreBoard[SCORE_HEIGHT][SCORE_WIDTH];// 점수 출력 보드를 위한 2차원 배열 


// 블럭의 회전까지 고려한 3차원 배열
int block[BROTATION][BSIZE][BSIZE]={
	{{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}}, //수평블럭 0도 회전
	{{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0}}, //수평블럭 90도 회전
	{{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}}, //수평블럭 180도 회전
	{{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0}} //수평블럭 270도 회전
};


int  gameScore;
bool gameStart;
int  gameAction; //블럭 운영 방식

int main(void)
{

	initGame();

	while(gameFlag != false) { 	
		gameAction=getGameAction(gameAction);
		updateGame();
		drawGame(blockX,blockY,blockR);
		Sleep(50); 
	}

	return 0;
}

//콘솔 화면에서 커서를 특정 위치로 이동
void gotoXY(int x, int y) {
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void initGame() {

	blockX = BOARD_WIDTH/2 -2;
	blockY = 1;
	gameScore = 0;
	gameAction = MOVE_DROP;
	int x, y;

	//게임보드 초기화
	for(y=0; y<BOARD_HEIGHT; y++) {
		for(x=0; x<BOARD_WIDTH; x++) {
			if ((x==0||x==BOARD_WIDTH-1)||(y==0||y==BOARD_HEIGHT-1)) {
				board[y][x] = BOARD_BORDER;
			} else if (x==blockX&&y==blockY){
				board[y][x] = BLOCK;
			} else {
				board[y][x] = BOARD_INSIDE;
			}
		}
	}

	//다음 블럭 보드 초기화
	for(y=0; y< NEXT_HEIGHT; y++) {
		for(x=0; x<NEXT_WIDTH; x++) {
			if((x==0||x==NEXT_WIDTH-1)||(y==0||y==NEXT_HEIGHT-1)) {
				nextBoard[y][x]= BOARD_BORDER;
			} else {
				nextBoard[y][x]= BOARD_INSIDE;
			}
		}
	}

	//점수 보드 초기화
	for(y=0; y< SCORE_HEIGHT; y++) {
		for(x=0; x<SCORE_WIDTH; x++) {
			if ((x==0||x==SCORE_WIDTH-1)||(y == 0 || y == SCORE_HEIGHT-1)) {
				scoreBoard[y][x]= BOARD_BORDER;
			} else {
				scoreBoard[y][x]= BOARD_INSIDE;
			}
		}
	}
}

int getGameAction(int action) {
	int keyValue,act;
	act = action;
	if(kbhit() != 0) {//키보드를 눌렀는지 확인함 
		keyValue=getch(); 
		if (keyValue == 224) { //특수 키를 눌렀을 때 버퍼에 2Byte가 발생함, 첫번째 값은 224값을 발생하고 두번째 값은 특수//키에 따라 다름
			keyValue=getch();  //특수 키를 확인하기 위해 2번의 getch()함수를 호출해야 함
		}
		switch(keyValue) {
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
		default :
			break;
		}
	}
	return act;
}

//키 값에 따라 게임의 상태를 업데이트 함
void updateGame(void) {
	switch (gameAction) {
	case GAME_QUIT:
		gameFlag = false;
		break;
	case MOVE_LEFT: 
		blockX--;
		if (collision(blockX, blockY, blockR)) blockX++;
		gameAction = MOVE_DROP;
		break;
	case MOVE_RIGHT: 
		blockX++;
		if (collision(blockX, blockY, blockR)) blockX--;
		gameAction = MOVE_DROP;
		break;
	case MOVE_DOWN: 
		blockY++;
		if (collision(blockX, blockY, blockR)) {
			blockY--;
			if (blockY==1) {
				gameFlag = false;
			}
			else {
				stack(blockX, blockY, blockR);
				blockX = BOARD_WIDTH/2 -2;
				blockY = 1;
				blockR = 0;
			}
		}
		gameAction = MOVE_DROP;
		break;
	case MOVE_DROP:
		blockY++;
		if (collision(blockX, blockY, blockR)) {
			blockY--;
			if (blockY==1) {
				gameFlag = false;
			}
			else {
				stack(blockX, blockY, blockR);
				blockX = BOARD_WIDTH/2 -2;
				blockY = 1;
				blockR = 0;
			}
		}
		gameAction = MOVE_DOWN;
		break;
	case ROTATION: 
		blockR++;
		blockR %= 4;
		gameAction = MOVE_DROP;
		break;
	default:
		break;
	}


}


void drawGame(int blockX, int blockY) {
	int x, y;

	for(y=0; y<BOARD_HEIGHT; y++) {
		gotoXY(BOARD_STARTX, BOARD_STARTY+y);
		for(x=0; x<BOARD_WIDTH; x++) {  
			if(board[y][x]==BOARD_INSIDE) {   
				cout << "  ";33332100032..3
			} else if(board[y][x]==BOARD_BORDER) {
				cout << "□";
			} 
		}
	}
}




//블록이 게임보드와 충돌이 났는지 확인하는 함수
bool collision(int blockX, int blockY, int rotation)
{
	//게임 보드의 배열과 블록의 배열을 비교해서 충돌이 발생했는지 확인

}

// 블록이 아래도 이동한 후에 충돌확인이 발생하면 블록을 게임보드에 쌓는 함수
void stack(int blockX, int blockY, int rotation)
{
	for(int y=0; y<BSIZE; y++) {
		for(int x=0; x<BSIZE; x++) {
			if(block[blockR][y][x]==BLOCK) {
				board[blockY+y][blockX+x]=BLOCK;
			}
		}
	}
}
