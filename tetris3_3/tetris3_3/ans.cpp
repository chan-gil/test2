#include <iostream>
#include <windows.h> //GetStdHandle(), Sleep(), SetConsolePosition() �Լ� ���
#include <conio.h> //getch()�� kbhit() �Լ� ���
#include <ctime> //time()�Լ� ���
#include <cstdlib> //rand()�Լ��� srand()�Լ� ���

#define ESC 27 //���� ����
#define LEFT 75 //���� ȭ��ǥ Ű ASCII��
#define RIGHT 77 // ������ ȭ��ǥ Ű ASCII��
#define DOWN 80 //�Ʒ� ȭ��ǥ Ű ASCII��
#define UP 72 //���� ȭ��ǥ Ű ASCII��
#define SPACE 32 //�����̽� Ű ASCII�� 

#define BOARD_HEIGHT 22 //���� ���� ����
#define BOARD_WIDTH  12 //���� ���� ���� 
#define NEXT_HEIGHT  8 //���� �� ���� ����
#define NEXT_WIDTH   8 //���� �� ���� ����

#define SCORE_HEIGHT 3 //���� �� ���� ����
#define SCORE_WIDTH  8 //���� �� ���� ����

#define BSIZE 4 //�� ���� & ����
#define BROTATION 4 // �� ȸ��
#define BSHAPE 7


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

enum GameAction{MOVE_LEFT=1, MOVE_RIGHT, MOVE_DOWN, MOVE_DROP,ROTATION, GAME_QUIT};

struct Block {
	int X;//���� �ȿ� ���� X��ǥ  
	int Y; // ���� �ȿ� ���� Y��ǥ
	int R; // ���� ȸ��
	int S; // ���� ���
	int N; // ���� �� ���
};

struct GameInfo{
	int  action;//�� � ���
	int  score; // ��������
	bool flag; //���� ���� �÷���
};

void initGame(Block &blk, GameInfo &gm); //���� �ʱ�ȭ
void gotoXY(int x, int y); //�ܼ� ȭ�鿡�� Ŀ���� Ư�� ��ġ�� �̵�
int decodingKey(void); //���� Ű�� ��ȯ
void updateGame(Block &blk, GameInfo &gm); //������ ���°� ������Ʈ �Լ�
int getGameAction(int action); // ������ ����� �����ϴ� �Լ�
void drawGame(const Block &blk, const GameInfo &gm); // ���� ȭ���� �׸��� �Լ�
bool collision(const Block &blk);  //����� ���Ӻ���� �浹�� ������ Ȯ���ϴ� �Լ�
void removeFullLines(GameInfo &gm); //���Ӻ��忡 ���� �׿��� �ÿ� ������ ä���� ������ ������
void stack(const Block &blk); // ����� �Ʒ��� �̵��� �Ŀ� �浹Ȯ���� �߻��ϸ� ����� ���Ӻ��忡 �״� �Լ�

int board[BOARD_HEIGHT][BOARD_WIDTH]; //���� ���带 ���� 2���� �迭 
int nextBoard[NEXT_HEIGHT][NEXT_WIDTH]; // ���� �� ǥ�� ���带 ���� 2���� �迭 
int scoreBoard[SCORE_HEIGHT][SCORE_WIDTH];// ���� ��� ���带 ���� 2���� �迭 


// ���� ȸ������ ����� 3���� �迭
int block[BSHAPE][BROTATION][BSIZE][BSIZE]={
   {{0,0,0,0,
	 1,1,1,1,
	 0,0,0,0,
	 0,0,0,0}, //����� 0�� ȸ��
	{0,1,0,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,1,0,0}, //����� 90�� ȸ��
	{0,0,0,0,
	 1,1,1,1,
	 0,0,0,0,
	 0,0,0,0}, //����� 180�� ȸ��
	{0,1,0,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,1,0,0}}, //����� 270�� ȸ��
    {{0,1,0,0,   
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // ������ 0�� ȸ��
	{0,1,0,0,
	 0,1,1,0,
	 0,1,0,0,
	 0,0,0,0}, // ������ 90�� ȸ��
	{0,0,0,0,
	 1,1,1,0,
	 0,1,0,0,
	 0,0,0,0}, // ������ 180�� ȸ��
	{0,1,0,0,
	 1,1,0,0,
	 0,1,0,0,
	 0,0,0,0}}, // ������ 270�� ȸ��
   {{1,1,0,0,
	 0,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // Z���� 0�� ȸ��
	{0,1,0,0,
	 1,1,0,0,
	 1,0,0,0,
	 0,0,0,0}, // Z���� 90�� ȸ��
	{1,1,0,0,
	 0,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // Z���� 180�� ȸ��
	{0,1,0,0,
	 1,1,0,0,
	 1,0,0,0,
	 0,0,0,0}}, // Z���� 270�� ȸ��
   {{0,1,1,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // Z�ݻ����� 0�� ȸ��
	{0,1,0,0,
	 0,1,1,0,
	 0,0,1,0,
	 0,0,0,0},// Z�ݻ����� 90�� ȸ��
	{0,1,1,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // z�ݻ����� 180�� ȸ��
	{0,1,0,0,
	 0,1,1,0,
	 0,0,1,0,
	 0,0,0,0}}, // Z�ݻ����� 270�� ȸ��
   {{1,1,0,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,0,0,0}, // ������ 0�� ȸ�� 
	{0,0,1,0,
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0}, // ������ 90�� ȸ��
	{0,1,0,0,
	 0,1,0,0,
	 0,1,1,0,
	 0,0,0,0}, // ������ 180�� ȸ��
	{0,0,0,0,
	 1,1,1,0,
	 1,0,0,0,
	 0,0,0,0}}, // ������ 270�� ȸ��
   {{0,1,1,0,
	 0,1,0,0,
	 0,1,0,0,
	 0,0,0,0}, // L���� 0�� ȸ��
	{0,0,0,0,
	 1,1,1,0,
	 0,0,1,0,
	 0,0,0,0}, // L���� 90�� ȸ��
	{0,1,0,0,
	 0,1,0,0,
	 1,1,0,0,
	 0,0,0,0},// L���� 180�� ȸ��
	{1,0,0,0,
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0}},// L���� 270�� ȸ��
   {{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // ������ 0�� ȸ��
	{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0},//������ 90�� ȸ��
	{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}, // ������ 180�� ȸ��
	{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0}} // ������ 270�� ȸ��
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

void initGame(Block &blk, GameInfo &gm) //���� �ʱ� ���� ����
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

	//���Ӻ��� �ʱ�ȭ
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

  //���� �� ���� �ʱ�ȭ
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

   //���� ���� �ʱ�ȭ
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

 //���� �׸���
for (y=0; y<BOARD_HEIGHT; y++)
 {
	 gotoXY(BOARD_STARTX, BOARD_STARTY+y);
	 for (x=0; x<BOARD_WIDTH; x++)
	 {
		if(board[y][x] ==  BLOCK) cout << "��";
		else if(board[y][x]== BOARD_BORDER) cout << "��";
		else cout << "  ";
	 }
 }

 //��� �׸���

 for (y=0; y<BSIZE; y++)
 {
	for (x=0; x<BSIZE; x++)
	{
		if (block[blk.S][blk.R][y][x] == BLOCK )
		{
			 gotoXY(BOARD_STARTX + (blk.X + x)*2, BOARD_STARTY + blk.Y + y);
			 cout << "��";
		}	
	}
 }


  //���� ��� ���� �׸���
 gotoXY(NEXT_STARTX + 3, NEXT_STARTY -1);
 cout << "NEXT BLOCK";

 for (y=0; y<NEXT_HEIGHT; y++)
 {
	 gotoXY(NEXT_STARTX, NEXT_STARTY+y);
	 for (x=0; x<NEXT_WIDTH; x++)
		if(nextBoard[y][x] ==  BOARD_BORDER)
			cout << "��";
		else
			cout << "  ";

	 
 }
 // ���� ��� ���忡 �� �׸���
 for (y=0; y<BSIZE; y++)
 {
	for (x=0; x<BSIZE; x++)	
	{
		if (block[blk.N][0][y][x] == BLOCK )
		{
			 gotoXY(NEXT_STARTX + x*2 + 4, NEXT_STARTY + y + 2);
			 cout << "��";
		}	
	}
 }

 //���� ���� �׸���
 for (y=0; y<SCORE_HEIGHT; y++)
 {
	 gotoXY(SCORE_STARTX, SCORE_STARTY+y);
	 for (x=0; x<SCORE_WIDTH; x++)
		if(scoreBoard[y][x] ==  BOARD_BORDER)
			cout << "��";
	     else
			cout << "  ";	 
 }

 //���� ���
   gotoXY(SCORE_STARTX + 3, SCORE_STARTY -1);
   cout << "S C O R E";
   gotoXY(SCORE_STARTX + 5, SCORE_STARTY + 1);
   cout << gm.score;
}

int getGameAction(int action)
{
 int keyValue,act;
 act = action;
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
			   stack(blk); //���� ��ü ����
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
		   stack(blk); //���� ��ü ����
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

bool collision(const Block &blk) //���� ��ü �浹 �˻�
{
	 for (int y=0; y<BSIZE; y++)
	 {
		for (int x=0; x<BSIZE; x++)
		{
			if (block[blk.S][blk.R][y][x] == BLOCK )
			{
				if (board[blk.Y+y][blk.X+x] != BOARD_INSIDE)
				{
					return true; //�浹��			}
				}	
			 }
		 }
	 }
	 return false; //�浹 ����

}



//////////////////////////////////////////////////////////////
void stack(const Block &blk) //���� ��ü ����
{
 for (int y=0; y<BSIZE; y++)
	for (int x=0; x<BSIZE; x++)
		if (block[blk.S][blk.R][y][x] == BLOCK )
			board[blk.Y+y][blk.X+x] = BLOCK;
}


//////////////////////////////////////////////////////////////
void removeFullLines(GameInfo &gm) //���� ��� ���� ���� �ø���
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


//�ܼ� ȭ�鿡�� Ŀ���� Ư�� ��ġ�� �̵�
void gotoXY(int x, int y) 
{
 COORD Pos = {x, y};
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}