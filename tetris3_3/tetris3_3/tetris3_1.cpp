#include <iostream>
#include <windows.h> //GetStdHandle(), Sleep(), SetConsolePosition() �Լ� ���
#include <conio.h> //getch()�� kbhit() �Լ� ���

#define ESC 27 //���� ����
#define LEFT 75 //���� ȭ��ǥ Ű ASCII��
#define RIGHT 77 // ������ ȭ��ǥ Ű ASCII��
#define DOWN 80 //�Ʒ� ȭ��ǥ Ű ASCII��

#define BOARD_HEIGHT 22 //���� ���� ����
#define BOARD_WIDTH  12 //���� ���� ���� 

//���� ���� �׸��� ���� ��ġ
#define BOARD_STARTX 20  
#define BOARD_STARTY 2

//���� �� �׸��� ���� ��ġ
#define NEXTBLOCK_STARTX 50  
#define NEXTBLOCK_STARTY 3
#define NEXTBLOCK_SIZEX 8
#define NEXTBLOCK_SIZEY 5

//���� �׸��� ���� ��ġ
#define SCORE_STARTX 50
#define SCORE_STARTY 21
#define SCORE_SIZEX 8
#define SCORE_SIZEY 3

//�� ���� ��ġ
#define START_ARRAYX 4
#define START_ARRAYY 1

//������ ��ġ 
#define END_X 20
#define END_Y 25

using namespace std;

void initGame(); //���� �ʱ�ȭ
void gotoXY(int x, int y); //�ܼ� ȭ�鿡�� Ŀ���� Ư�� ��ġ�� �̵�
void drawGame(int blockX, int blockY); // ���� ȭ���� �׸��� �Լ�
int getGameAction(void); // ������ ����� �����ϴ� �Լ�
void move(int key);		// �� �̵� �Լ�
void reach();			// ���� ������ ĭ�� ������ �� ����
void fall();			// ������ �������� �������� �Լ�

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP, GAME_QUIT};
int  gameAction; //���� ��� ����
int blockX=4, blockY=1;	 //�����Ǵ� ���� �ʱ� ��ġ 
int  gameScore=0;

//�߰� 
int set[BOARD_WIDTH][BOARD_HEIGHT];		//��Ʈ���� �� ��ü�� 2���� �迭�� ����

int main(void){
    
	initGame();	// ���� �ʱ�ȭ

	while(gameAction != GAME_QUIT){			// esc�� �Է� �� �ݺ��� Ż��
		drawGame(blockX,blockY);			// ���� �׷���
		gameAction=getGameAction();			// ����� �Է� �� �Ǻ�
		move(gameAction);					// �Է� ���� ���� �̵�
		fall();								// ������ ������ �̵�
		Sleep(80);							// delay
	}

	gotoXY(END_X, END_Y);	// ���� ������ ��ġ�� �ʰ� Ŀ���� �̵�������
	return 0;
}

void gotoXY(int x, int y){
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
//��Ʈ���� ���� �׵θ��� 1, ���θ� 0���� �ʱ�ȭ�Ѵ�.
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
	//���ο� ����� set[4][1]���� ���۵ȴ�
	for(int i=0; i<BOARD_HEIGHT; i++){
		gotoXY(BOARD_STARTX, BOARD_STARTY+i);
		for(int j=0; j<BOARD_WIDTH; j++){   // ����� ��� set[4][1]�� ���� 
			if((j>=blockX&&j<blockX+4)&&((i==blockY)){
				cout << "��";
			}
			//0���� �Ҵ���� �κ� ����ó��
			else if(set[i][j]==0){   
				cout << "  ";
			}
			// 1�� ���� �κ� �ڽ� ( �׵θ� )
			else{
				cout << "��";
			}
		}
	}


	// ���� ��
	gotoXY(NEXTBLOCK_STARTX, NEXTBLOCK_STARTY-1);
	cout << "  [NEXT BLOCK]";
	for(int i=0; i<NEXTBLOCK_SIZEY; i++){
		gotoXY(NEXTBLOCK_STARTX, NEXTBLOCK_STARTY+i);
		for(int j=0; j<NEXTBLOCK_SIZEX; j++){
			if(i==0||j==0||i==(NEXTBLOCK_SIZEY-1)||j==(NEXTBLOCK_SIZEX-1)){
				cout << "��";
			}
			else if(i==2&&j>=2&&j<=5){
				cout << "��";
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
				cout << "��";
			}
			// ���ڸ� �� 0�� ���ԵǾ�� �ϹǷ� 
			// 100���� ������ �տ� 0�� 10���� ������ 0�ΰ��� ... 
			// �̿� ���� ������ ���ھ� ���
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
	if(kbhit() != 0){ //Ű���带 �������� Ȯ���� 
		
		keyValue=getch(); 
		if (keyValue == 224){ //Ư�� Ű�� ������ �� ���ۿ� 2Byte�� �߻���, ù��° ���� 224���� �߻��ϰ� �ι�° ���� Ư��Ű�� ���� �ٸ�
			keyValue=getch(); //Ư�� Ű�� Ȯ���ϱ� ���� 2���� getch()�Լ��� ȣ���ؾ� ��
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

//���ᰡ �ƴ� ����� �Է¿� ���� ��ĳ� ����� ��ǥ���� ������.
//Ʋ ������ ��� �� ���� ������ ������ �����Ѵ�.
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

// ������ �ֱ�� �������� �Լ� ��� Y���� �������� 
// �Ʒ��� �������� ȿ���� ����
void fall(){
	if(blockY<19){
		blockY++;
	}
	else if(blockY==19){
		reach();
	}
}

// down�� ���� Ȥ�� fall�� ���� �ٴڿ� ������ ��� 
// ���� �ʱⰪ���� ���� ��Ų �� ������ 10�� ������Ų��.
void reach(){
	blockX=START_ARRAYX;
	blockY=START_ARRAYY-1;
	gameScore += 10;
}p