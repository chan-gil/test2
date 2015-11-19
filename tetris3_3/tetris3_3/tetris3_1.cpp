#include <iostream>
#include <windows.h> //GetStdHandle(), Sleep(), SetConsolePosition() 함수 사용
#include <conio.h> //getch()와 kbhit() 함수 사용

#define ESC 27 //게임 종료
#define LEFT 75 //왼쪽 화살표 키 ASCII값
#define RIGHT 77 // 오른쪽 화살표 키 ASCII값
#define DOWN 80 //아래 화살표 키 ASCII값
//
#define BOARD_HEIGHT 22 //게임 보드 높이
#define BOARD_WIDTH  12 //게임 보드 넓이 

//게임 보드 그리기 시작 위치
#define BOARD_STARTX 20  
#define BOARD_STARTY 2

//다음 블럭 그리기 시작 위치
#define NEXTBLOCK_STARTX 50  
#define NEXTBLOCK_STARTY 3
#define NEXTBLOCK_SIZEX 8
#define NEXTBLOCK_SIZEY 5

//점수 그리기 시작 위치
#define SCORE_STARTX 50
#define SCORE_STARTY 21
#define SCORE_SIZEX 8
#define SCORE_SIZEY 3

//블럭 시작 위치
#define START_ARRAYX 4
#define START_ARRAYY 1

//끝내는 위치 
#define END_X 20
#define END_Y 25

using namespace std;

void initGame(); //게임 초기화
void gotoXY(int x, int y); //콘솔 화면에서 커서를 특정 위치로 이동
void drawGame(int blockX, int blockY); // 게임 화면을 그리는 함수
int getGameAction(void); // 게임의 기능을 선택하는 함수
void move(int key);		// 블럭 이동 함수
void reach();			// 블럭이 마지막 칸에 도달할 때 실행
void fall();			// 일정한 간격으로 떨어지는 함수

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP, GAME_QUIT};
int  gameAction; //게임 기능 저장
int blockX=4, blockY=1;	 //생성되는 블럭의 초기 위치 
int  gameScore=0;

//추가 
int set[BOARD_WIDTH][BOARD_HEIGHT];		//테트리스 판 전체를 2차원 배열로 선언

int main(void){
    
	initGame();	// 게임 초기화

	while(gameAction != GAME_QUIT){			// esc를 입력 시 반복문 탈출
		drawGame(blockX,blockY);			// 블럭을 그려줌
		gameAction=getGameAction();			// 사용자 입력 값 판별
		move(gameAction);					// 입력 값에 따른 이동
		fall();								// 일정한 간격의 이동
		Sleep(80);							// delay
	}

	gotoXY(END_X, END_Y);	// 종료 문장이 겹치지 않게 커서를 이동시켜줌
	return 0;
}

void gotoXY(int x, int y){
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
//테트리스 게임 테두리에 1, 내부를 0으로 초기화한다.
void initGame(){
	for(int i=0; i<BOARD_HEIGHT; i++){
		for(int j=0; j<BOARD_WIDTH; j++){
			if(i==0||j==0||i==(BOARD_HEIGHT-1)||j==(BOARD_WIDTH-1)){
				set[i][j] = 1;
			}
			else{
				set[i][j] = 0;
			}
		}
	}
}

void drawGame(int blockX, int blockY){
	//새로운 블락은 set[4][1]에서 시작된다
	for(int i=0; i<BOARD_HEIGHT; i++){
		gotoXY(BOARD_STARTX, BOARD_STARTY+i);
		for(int j=0; j<BOARD_WIDTH; j++){   // ■■■■ 블록 set[4][1]에 생성 
			if((j>=blockX&&j<blockX+4)&&((i==blockY)){
				cout << "■";
			}
			//0으로 할당받은 부분 여백처리
			else if(set[i][j]==0){   
				cout << "  ";
			}
			// 1로 받은 부분 박스 ( 테두리 )
			else{
				cout << "□";
			}
		}
	}


	// 다음 블럭
	gotoXY(NEXTBLOCK_STARTX, NEXTBLOCK_STARTY-1);
	cout << "  [NEXT BLOCK]";
	for(int i=0; i<NEXTBLOCK_SIZEY; i++){
		gotoXY(NEXTBLOCK_STARTX, NEXTBLOCK_STARTY+i);
		for(int j=0; j<NEXTBLOCK_SIZEX; j++){
			if(i==0||j==0||i==(NEXTBLOCK_SIZEY-1)||j==(NEXTBLOCK_SIZEX-1)){
				cout << "□";
			}
			else if(i==2&&j>=2&&j<=5){
				cout << "■";
			}
			else{
				cout << "  ";
			}
		}
	}
	 

	gotoXY(SCORE_STARTX, SCORE_STARTY-1);		
	cout << " [GAME SCORE]";
	for(int i=0; i<SCORE_SIZEY; i++){
		gotoXY(SCORE_STARTX, SCORE_STARTY+i);
		for(int j=0; j<SCORE_SIZEX; j++){
			if(i==0||j==0||i==(SCORE_SIZEY-1)||j==(SCORE_SIZEX-1)){
				cout << "□";
			}
			// 앞자리 수 0도 포함되어야 하므로 
			// 100보다 작으면 앞에 0을 10보다 작으면 0두개를 ... 
			// 이와 같은 식으로 스코어 출력
			else if(i==1&&j==3){
				if(gameScore>=100){
					cout << "0" << gameScore;
				}
				else if(gameScore>=10){
					cout << "00" << gameScore;
				}
				else if(gameScore>0){
					cout << "000" << gameScore;
				}
				else if(gameScore==0){
					cout << "0000" ;
				}
				else if(gameScore>=1000){
					gameScore = 0;
				}
				else{
					cout << gameScore;
				}
			}
			else{
				cout << "  ";
			}
		}
	}  		
}

int getGameAction(void){
	int keyValue,act=0;
	if(kbhit() != 0){ //키보드를 눌렀는지 확인함 
		
		keyValue=getch(); 
		if (keyValue == 224){ //특수 키를 눌렀을 때 버퍼에 2Byte가 발생함, 첫번째 값은 224값을 발생하고 두번째 값은 특수키에 따라 다름
			keyValue=getch(); //특수 키를 확인하기 위해 2번의 getch()함수를 호출해야 함
		}
		switch (keyValue){
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
	}
  return act;
}

//종료가 아닌 사용자 입력에 따라 행렬내 블록의 좌표값을 내린다.
//틀 밖으로 벗어날 수 없기 문에 범위를 조정한다.
void move(int key){
	switch (key){
		case MOVE_LEFT:
			if(blockX>1){
				blockX--;
			}
			break;
		case MOVE_RIGHT:
			if(blockX<7){
				blockX++;
			}
			break;
		case MOVE_DOWN:
			if(blockY<20){
				blockY++;
			}
			else if(blockY==20){
				reach();
			}
			break;
		default :
			break;
	}
}

// 일정한 주기로 떨어지는 함수 행렬 Y값을 증가시켜 
// 아래로 떨어지는 효과를 낸다
void fall(){
	if(blockY<19){
		blockY++;
	}
	else if(blockY==19){
		reach();
	}
}

// down에 의해 혹은 fall에 의해 바닥에 도달할 경우 
// 블럭을 초기값으로 대응 시킨 후 점수를 10점 증가시킨다.
void reach(){
	blockX=START_ARRAYX;
	blockY=START_ARRAYY-1;
	gameScore += 10;
}p
