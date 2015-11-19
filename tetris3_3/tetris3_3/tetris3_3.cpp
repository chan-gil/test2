#include <iostream>
#include <windows.h> //GetStdHandle(), Sleep(), SetConsolePosition() �Լ� ���
#include <conio.h> //getch()�� kbhit() �Լ� ���

#define ESC 27 //���� ����
#define LEFT 75 //���� ȭ��ǥ Ű ASCII��
#define RIGHT 77 // ������ ȭ��ǥ Ű ASCII��
#define DOWN 80 //�Ʒ� ȭ��ǥ Ű ASCII��
#define UP 72 //���� ȭ��ǥ Ű ASCII��

#define BOARD_HEIGHT 22 //���� ���� ����
#define BOARD_WIDTH  12 //���� ���� ���� 
#define NEXT_HEIGHT  8 //���� �� ���� ����
#define NEXT_WIDTH   8 //���� �� ���� ����

#define SCORE_HEIGHT 3 //���� �� ���� ����
#define SCORE_WIDTH  8 //���� �� ���� ����

#define BSIZE 4 //�� ���� & ����
#define BROTATION 4 // �� ȸ��


//���� ���� �迭���� ���� ��輱, ���� ����,  �� ǥ��
#define BOARD_BORDER 2 //���� ���� ��輱 ǥ��
#define BOARD_INSIDE 0 //���� ���� ���� ǥ��
#define BLOCK 1 //�� ǥ��


//���� ���� �׸��� ���� ��ġ
#define BOARD_STARTX 20  
#define BOARD_STARTY 2

//���� �� �׸��� ���� ��ġ
#define NEXT_STARTX  45 
#define NEXT_STARTY  3

//���� �׸��� ���� ��ġ
#define SCORE_STARTX 45
#define SCORE_STARTY 21




using namespace std;

//MOVE_LEFT: �� ���� 1ĭ �̵�, MOVE_RIGHT: �� ������ 1ĭ �̵�, MOVE_DOWN: �� �Ʒ� 1ĭ �̵�
//MOVE_DROP: �� �Ʒ� 1ĭ �ڵ� �̵�, GAME_QUIT: ��������

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP, ROTATION, GAME_QUIT};



void initGame(); //���� �ʱ�ȭ
void gotoXY(int x, int y); //�ܼ� ȭ�鿡�� Ŀ���� Ư�� ��ġ�� �̵�
int decodingKey(void); //���� Ű�� ��ȯ
void updateGame(void); //������ ���°� ������Ʈ �Լ�
int getGameAction(int action); // ������ ����� �����ϴ� �Լ�
void drawGame(int blockX, int blockY,int blockR); // ���� ȭ���� �׸��� �Լ�
bool collision(int blockX, int blockY, int rotation);  //����� ���Ӻ���� �浹�� ������ Ȯ���ϴ� �Լ�
void stack(int blockX, int blockY, int rotation); // ����� �Ʒ��� �̵��� �Ŀ� �浹Ȯ���� �߻��ϸ� ����� ���Ӻ��忡 �״� �Լ�


int blockX, blockY; //���� �ȿ� ���� ��ġ  
int blockR; //���� ȸ��
bool gameFlag; //���� ���� �÷���
int board[BOARD_HEIGHT][BOARD_WIDTH]; //���� ���带 ���� 2���� �迭 
int nextBoard[NEXT_HEIGHT][NEXT_WIDTH]; // ���� �� ǥ�� ���带 ���� 2���� �迭 
int scoreBoard[SCORE_HEIGHT][SCORE_WIDTH];// ���� ��� ���带 ���� 2���� �迭 


// ���� ȸ������ ����� 3���� �迭
int block[BROTATION][BSIZE][BSIZE]={
	{{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}}, //����� 0�� ȸ��
	{{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0}}, //����� 90�� ȸ��
	{{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}}, //����� 180�� ȸ��
	{{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0}} //����� 270�� ȸ��
};


int  gameScore;
bool gameStart;
int  gameAction; //�� � ���

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

//�ܼ� ȭ�鿡�� Ŀ���� Ư�� ��ġ�� �̵�
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

	//���Ӻ��� �ʱ�ȭ
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

	//���� �� ���� �ʱ�ȭ
	for(y=0; y< NEXT_HEIGHT; y++) {
		for(x=0; x<NEXT_WIDTH; x++) {
			if((x==0||x==NEXT_WIDTH-1)||(y==0||y==NEXT_HEIGHT-1)) {
				nextBoard[y][x]= BOARD_BORDER;
			} else {
				nextBoard[y][x]= BOARD_INSIDE;
			}
		}
	}

	//���� ���� �ʱ�ȭ
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
	if(kbhit() != 0) {//Ű���带 �������� Ȯ���� 
		keyValue=getch(); 
		if (keyValue == 224) { //Ư�� Ű�� ������ �� ���ۿ� 2Byte�� �߻���, ù��° ���� 224���� �߻��ϰ� �ι�° ���� Ư��//Ű�� ���� �ٸ�
			keyValue=getch();  //Ư�� Ű�� Ȯ���ϱ� ���� 2���� getch()�Լ��� ȣ���ؾ� ��
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

//Ű ���� ���� ������ ���¸� ������Ʈ ��
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
				cout << "��";
			} 
		}
	}
}




//����� ���Ӻ���� �浹�� ������ Ȯ���ϴ� �Լ�
bool collision(int blockX, int blockY, int rotation)
{
	//���� ������ �迭�� ����� �迭�� ���ؼ� �浹�� �߻��ߴ��� Ȯ��

}

// ����� �Ʒ��� �̵��� �Ŀ� �浹Ȯ���� �߻��ϸ� ����� ���Ӻ��忡 �״� �Լ�
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
