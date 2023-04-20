/*
	Oğuzhan ÇALIŞKAN
	
	18.04.23(11 pm) - 19.04.23(3 am)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>

#define ROW 15 //min 10
#define COL 15 // min 10
#define I_SNAKE_LEN 3

#define FPS 3 // if its slow increase it and vice versa
#define FPM 0.5 // 1 / Move per frame

char gameBoard[ROW][COL];

enum Move {
	Up = 'w',
	Down = 's',
	Left = 'a',
	Right = 'd',
	No = 0
};

enum Move currentMove = Left;

struct node{
	int row;
	int col;
	struct node* next; 
};
struct node* head = NULL;
struct node* current = NULL;

struct node* appleFirst;

void add(int x, int y);
void initialGB();
void showGB();
void initialSnake();
void play();
void orderTheNodes();
void putTheSnake();
void addApple();
int mainMenu();
void gameOverMenu();
void freeSnake();

int point = 0;

int gameState = 1;

int main() {
	appleFirst = (struct node*)malloc(sizeof(struct node));

	addApple(); //first apple might spawn on the snake	
	initialGB();

	sleep(1);
	
	while(1) {
		system("color f");
		
		char ans = mainMenu();
		if(!ans) break;
		
		gameState = 1;
		
		initialSnake();
		while(gameState) {
			initialGB();
			putTheSnake();
			play();
			if(!gameState) break; 
			system("cls");
			showGB();
			sleep(1/FPS);
		}
		
		gameOverMenu();
	}

	return 0;
}

void play()  {
	static cnt = 0;
	
	if(cnt >= FPM) {
		enum Move order = currentMove;
		
		if(kbhit()) { order = getch(); }
		fflush(stdin);
		
		
		if(order != Up && order != Down && order != Left && order != Right) order = currentMove;
		if(!(currentMove == Up && order == Down) && !(order == Up && currentMove == Down) && !(order==Right && currentMove==Left) && !(currentMove==Right && order==Left)) currentMove = order;
		
		orderTheNodes();
		
		if(gameBoard[head->row][head->col] == 'o') {gameState = 0;}
		
		if(gameBoard[head->row][head->col] == '+') {
			if(current->row + 1 <= ROW - 1) {
				add(current->row + 1, current->col);
			}
			else {
				add(current->row, current->col + 1);
			}
			
			addApple();
			point++;
		}
		
		cnt = -1;
	}
	
	++cnt;

}

void gameOverMenu() {
	system("color c");
	sleep(1.5);
	system("PAUSE");
	
	system("cls");
	
	printf("GAME OVER!");
	printf("\n----------------\n");
	printf("Score: %d\n\n\n", point);
	
	sleep(1.5);
	system("PAUSE");
	point = 0;
	currentMove = Left;
	
	freeSnake();
}

int mainMenu() {
	char ans = '\0';
	while(ans != '1' && ans != '2') {
		system("cls");
		printf("Press 1 to play.\nPress 2 to exit.");
		ans = getch();
	}
	
	if(ans == '2') return 0;
	else if(ans == '1') return 1;
}

void putTheSnake() {
	struct node* cursor = head;	
	
	while(cursor != NULL) {
		gameBoard[cursor->row][cursor->col] = 'o';
		cursor = cursor->next;
	}
}

void orderTheNodes() {
	struct node* cursor = head->next;	
	
	int prevRow = head->row;
	int prevCol = head->col;
	
	int temp = 0;
	
	while(cursor != NULL) {
		temp = cursor->row;
		cursor->row = prevRow;
		prevRow = temp;
		
		temp = cursor->col;
		cursor->col = prevCol;
		prevCol = temp;
		
		cursor = cursor->next;	
	}
	
	if(currentMove == Up) {
		
		head->row = head->row > 0 ? (head->row - 1) : ROW - 1;
	}
	else if(currentMove == Down) {
		head->row = (head->row + 1) % ROW; 
	}
	else if(currentMove == Right) {
		head->col = (head->col + 1) % COL;
	}
	else if(currentMove == Left) {
		head->col = head->col > 0 ? (head->col - 1) : COL - 1;
	}
}

void add(int row, int col) {
	if(head == NULL) {
		head = (struct node*)malloc(sizeof(struct node));
		
		head->row = row;
		head->col = col;
		head->next = NULL;
		
		
		current = head;
	}
	else {
		struct node* newNode = (struct node*)malloc(sizeof(struct node));
		
		newNode->row = row;
		newNode->col = col;
		newNode->next = NULL;
		
		current->next = newNode;
		current = newNode;
	}
}

void initialGB() {
	int i;
	for(i = 0; i < ROW; ++i) {
		int j;
		for(j = 0; j < COL; ++j) {
			gameBoard[i][j] = ' ';
		}
	}
	
	gameBoard[appleFirst->row][appleFirst->col] = '+';
}

void addApple() {
	srand(time(NULL));
	int randRow = (rand() % ROW);
	int randCol = (rand() % COL);
	
	while(gameBoard[randRow][randCol] == 'o') {
		randRow = (rand() % ROW);
		randCol = (rand() % COL);
	}
	
	gameBoard[randRow][randCol] = '+';
	
	appleFirst->row = randRow;
	appleFirst->col = randCol;
}

void showGB() {
	printf("  ");
	
	int i;
	for(i = 0; i < COL; ++i) {
		printf("* ");
	}
	
	printf("\n");
	
	for(i = 0; i < ROW; ++i) {
		int j;
		printf("* ");
		for(j = 0; j < COL; ++j) {
			printf("%c ", gameBoard[i][j]);
		}
		printf("*\n");
	}
	
	printf("  ");
	
	for(i = 0; i < COL; ++i) {
		printf("* ");
	}
	
	printf("\n\n      Point: %d\n\n", point);
}

void initialSnake() {
	int i;
	for(i = 0; i < I_SNAKE_LEN; ++i) {
		add(ROW/2, (COL/2) + i);
		gameBoard[ROW/2][(COL/2)+i] = 'o';
	}
}

void freeSnake() {
	struct node* temp = head;
	
	while(head != NULL) {
		temp = temp->next;
		free(head);
		head = temp;
	} 
} 



