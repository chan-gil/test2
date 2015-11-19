
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



//MOVE_LEFT: �� ���� 1ĭ �̵�, MOVE_RIGHT: �� ������ 1ĭ �̵�, MOVE_DOWN: �� �Ʒ� 1ĭ �̵� MOVE_DROP: �� �Ʒ� 1ĭ �ڵ� �̵�, GAME_QUIT: ��������

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP, GAME_QUIT};



void initGame(); //���� �ʱ�ȭ
void gotoXY(int x, int y); //�ܼ� ȭ�鿡�� Ŀ���� Ư�� ��ġ�� �̵�
void updateGame(int move); //������ ���°� ������Ʈ �Լ�
int getGameAction(); // ������ ����� �����ϴ� �Լ�
void drawGame(int blockX, int blockY); // ���� ȭ���� �׸��� �Լ�
bool collision(int blockX, int blockY);  //����� ���Ӻ���� �浹�� ������ Ȯ���ϴ� �Լ�

int blockX, blockY; //���� �ȿ� ���� ��ġ  
int board[BOARD_HEIGHT][BOARD_WIDTH]; //���� ���带 ���� 2���� �迭 
int nextBoard[NEXT_HEIGHT][NEXT_WIDTH]; // ���� �� ǥ�� ���带 ���� 2���� �迭 
int scoreBoard[SCORE_HEIGHT][SCORE_WIDTH];// ���� ��� ���带 ���� 2���� �迭 


// ���� ȸ������ ����� 3���� �迭
int block[BSIZE][BSIZE]=
         {{0,0,0,0},
	 {1,1,1,1},
	 {0,0,0,0},
	 {0,0,0,0}};

int  gameScore;
bool gameStart;
int  gameAction; //�� � ���

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

void initGame() //���� �ʱ� ���� ����
{

	blockX = BOARD_WIDTH/2 -2;
	blockY = 1;
	gameScore = 0;
	gameAction = MOVE_DROP;
	int x, y;

	//���Ӻ��� �ʱ�ȭ
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

  //���� �� ���� �ʱ�ȭ
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

   //���� ���� �ʱ�ȭ
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
//1. ����ȭ�鿡 ���Ӻ��� â�� �׸���
 for(y=0; y<BOARD_HEIGHT; y++){
		gotoXY(BOARD_STARTX, BOARD_STARTY+y);
		for(x=0; x<BOARD_WIDTH; x++){  
			//0���� �Ҵ���� �κ� ����ó��
			if(board[y][x]==0){   
				cout << "  ";
			}
			// 2�� ���� �κ� �ڽ� ( �׵θ� )
			else if(board[y][x]==2){
				cout << "��";
			}
		}
	}

 //2. ���Ӻ��� â�� ��� �׸���
 for(int i=0; i<BOARD_HEIGHT; i++){
		gotoXY(BOARD_STARTX+8, BOARD_STARTY+i);
		for(int j=0; j<BOARD_WIDTH; j++){   
			if((j>=blockX&&j<blockX+4)&&(i==blockY)){
				cout << "��";
			}
		}
 }

//3. ���� ��� ���� â�� �׸���
  for(y=0; y<NEXT_HEIGHT; y++){
		gotoXY(NEXT_STARTX, NEXT_STARTY+y);
		for(x=0; x<NEXT_WIDTH; x++){   
			if(nextBoard[y][x]==0){   
				cout << "  ";
			}
			else if(nextBoard[y][x]==2){
				cout << "��";
			}
		}
	}

//4. ���� ��� ���忡 �� �׸���
 for (y=0; y<BSIZE; y++)
 {
	for (x=0; x<BSIZE; x++)	
	{
		if (block[y][x] == BLOCK )
		{
			 gotoXY(NEXT_STARTX + x*2 + 4, NEXT_STARTY + y + 2);
			 cout << "��";
		}	
	}
 }

 //5. ���� ȭ�鿡 ���� ���� â�� �׸���
 for (y=0; y<SCORE_HEIGHT; y++)
 {
	 gotoXY(SCORE_STARTX, SCORE_STARTY+y);
	 for (x=0; x<SCORE_WIDTH; x++)
		if(scoreBoard[y][x] ==  BOARD_BORDER) cout << "��";
	     else
			cout << "  ";	 
 }

 //���� ���
   gotoXY(SCORE_STARTX + 3, SCORE_STARTY -1);
   cout << "S C O R E";
   gotoXY(SCORE_STARTX + 5, SCORE_STARTY + 1);
   cout << gameScore;
}

int getGameAction()
{
 int keyValue,act=gameAction;
 if(kbhit() != 0) //Ű���带 �������� Ȯ���� 
 {
	  keyValue=getch(); 
	 if (keyValue == 224) //Ư�� Ű�� ������ �� ���ۿ� 2Byte�� �߻���, ù��° ���� 224���� �߻��ϰ� �ι�° ���� Ư��Ű�� ���� �ٸ�
	 {
	   keyValue=getch(); //Ư�� Ű�� Ȯ���ϱ� ���� 2���� getch()�Լ��� ȣ���ؾ� ��
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


bool collision(int blockX, int blockY) //���� ��ü �浹 �˻�
//�浹�� �߻��ϸ� true ��ȯ, �浹�� �߻����� ������ false ��ȯ��
{
	if(blockX>=1||blockX>=7)
		return true;
	else
		return false;
//�浹 üũ 

}


//�ܼ� ȭ�鿡�� Ŀ���� Ư�� ��ġ�� �̵�
void gotoXY(int x, int y) 
{
 COORD Pos = {x, y};
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
