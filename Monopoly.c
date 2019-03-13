/******************************************
*
* Official Name:  Jason Aguilera
*
* Nickname:  Jay
*
* E-mail:  jdaguile@syr.edu
*
* Final Project:  Monopoly board game
*
* Compiler:  Visual Studio 2015
*
* Date: 5/3/17
*
*******************************************/

#include <stdio.h>
#include <time.h>
#define WIDTH 64
#define HEIGHT 22

typedef struct {
	int order; char name[20]; char character[5];
	int position; int balance; int properties[20];
} player; // STRUCT
typedef struct { int price; int rent; } property; // STRUCT

void playerAssignment(int playOrder[]); // USES RAND, MYOWN
void characterAssignment(char *character1[5], char *character2[5]); // *PARAM, STRUCT PARAM, MYOWN
void startboard(char board[HEIGHT][WIDTH]); // MYOWN
void displayboard(char board[HEIGHT][WIDTH], int *balance1, int *balance2,
	char name1[20], char name2[20], int prop1[20], int prop2[20]); // *PARAM, STRUCT PARAM, MYOWN
int rollDice(); // USES RAND, MYOWN
void setPositionP1(char board[HEIGHT][WIDTH], char character[5], int *position, int roll, int *balance, char name[]); // *PARAM, STRUCT PARAM, MYOWN
void setPositionP2(char board[HEIGHT][WIDTH], char character[5], int *position, int roll, int *balance, char name[]); // *PARAM, STRUCT PARAM, MYOWN
int newPosition(int *position, int *balance, int properties[], int playerProps[], char p2name[], char p1name[]); // *PARAM, STRUCT PARAM, MYOWN
void delay(double secs);
void printExternal(char p1name[], char p2name[], int p1balance, int p2balance, int p1properties[], int p2properties[]); // I/O, STRUCT PARAM, MYOWN
int mostValuable(int properties[]); // SEARCH, STRUCT PARAM, MYOWN

int main()
{
	srand(time(NULL)); //SRAND
	int i, j, dice, rent; int playOrder[2] = { 0, 0 };
	int allProperties[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	char board[HEIGHT][WIDTH], temp[20];
	player p1;
	player p2; // empty player structs declared

	printf("Welcome to Monopoly!\n");
	playerAssignment(playOrder); // randomizes order of players
	printf("Enter names of two players, separated by a space.\n");
	
	scanf("%s%s", p1.name, p2.name); // reads in name of players
	p1.order = playOrder[0]; p2.order = playOrder[1]; // assigns random number to player

	printf("\nWho goes first? ");
	// prints out order of players
	if (p1.order == 0)
		printf("1st player: %s. 2nd player: %s.\n\n", p1.name, p2.name);
	if (p2.order == 0)
	{
		printf("1st player: %s. 2nd player: %s.\n\n", p2.name, p1.name);
		strcpy(temp, p1.name); strcpy(p1.name, p2.name); strcpy(p2.name, temp);
	}
	characterAssignment(&p1.character, &p2.character); // asks players for types of characters
	
	printf("All players start off with $800!\n\n");
	p1.balance = 800; p2.balance = 800; // initializes players' balances

	for (i = 0; i < 20; i++) // clears players' respective property arrays
	{
		p1.properties[i] = 0; p2.properties[i] = 0;
	}
	p1.position = 0; p2.position = 0; // sets players' positions to 0 (GO!)

	startboard(board); dice = 0;
	setPositionP1(board, p1.character, &p1.position, dice, &p1.balance, p1.name);
	setPositionP2(board, p2.character, &p2.position, dice, &p2.balance, p2.name);
	displayboard(board, &p1.balance, &p2.balance, p1.name, p2.name, p1.properties, p2.properties); // ONE USAGE OF *PARAM
	delay(3.00);
	while (p1.balance >= 0 && p2.balance >= 0) // game begins
	{
		startboard(board); dice = 0; setPositionP2(board, p2.character, &p2.position, dice, &p2.balance, p2.name); // sets board with p2's former position
		dice = rollDice();
		printf("%s rolls dice and moves %d spaces!\n", p1.name, dice); // announces p1's displacement
		delay(2.00); setPositionP1(board, p1.character, &p1.position, dice, &p1.balance, p1.name); // sets p1's new position
		displayboard(board, &p1.balance, &p2.balance, p1.name, p2.name, p1.properties, p2.properties); // displays board with players' positions
		rent = newPosition(&p1.position, &p1.balance, allProperties, p1.properties, p2.name, p1.name); delay(4.00);
		// checks if property belongs to someone or if they can buy it
		p2.balance = p2.balance + rent; // pays player 2, if there is nothing to be paid, rent = 0
		displayboard(board, &p1.balance, &p2.balance, p1.name, p2.name, p1.properties, p2.properties); delay(5.00);
		if (p1.balance < 0 || p2.balance < 0)
			break;
		startboard(board); dice = 0; setPositionP1(board, p1.character, &p1.position, dice, &p1.balance, p1.name); // sets board with p1's former position
		dice = rollDice();
		printf("%s rolls dice and moves %d spaces!\n", p2.name, dice); // announces p2's displacement
		delay(2.00); setPositionP2(board, p2.character, &p2.position, dice, &p2.balance, p2.name); // sets p2's new position
		displayboard(board, &p1.balance, &p2.balance, p1.name, p2.name, p1.properties, p2.properties);
		rent = newPosition(&p2.position, &p2.balance, allProperties, p2.properties, p1.name, p2.name); delay(4.00);
		// checks if property belongs to someone or if they can buy it
		p1.balance = p1.balance + rent; // pays player 1, if there is nothing to be paid, rent = 0
		displayboard(board, &p1.balance, &p2.balance, p1.name, p2.name, p1.properties, p2.properties); delay(5.00);
	}
		printf("\n\nGAME OVER\n");
		if (p1.balance < 0)
			printf("%s wins!\n", p2.name);
		if (p2.balance < 0)
			printf("%s wins!\n", p1.name);
		printExternal(p1.name, p2.name, p1.balance, p2.balance, p1.properties, p2.properties); // ONE USAGE OF STRUCT PARAM
	return 0;
}
/******************** PLAYER ASSIGNMENT ********************/
void playerAssignment(int playOrder[])
{
	int i;
	for (i = 0; i < 2; i++)
	{
		playOrder[i] = (rand() % 2);
		if (i == 1) {
			do { playOrder[i] = (rand() % 2); } while (playOrder[i] == playOrder[i - 1]); // RAND
		}
	}
}
/******************** CHARACTER ASSIGNMENT ********************/
void characterAssignment(char *character1[5], char *character2[5])
{
	printf("Which character would player 1 like to be?\nEnter an item no greater than 4 letters.   ");
	scanf("%s", character1);
	printf("Which character would player 2 like to be?\nEnter an item no greater than 4 letters.   ");
	scanf("%s", character2); printf("\n");
}
/******************** START BOARD ********************/
void startboard(char board[HEIGHT][WIDTH])
{
	int row, col; char s0[] = "GO!", s1[] = "Albany", s2[] = "Boston", s3[] = "El Paso";
	char s4[] = "*Taxes", s5[] = "Orlando", s6[] = "LasVegas|", s7[] = "Denver", s8[] = "Atlanta";
	char s9[] = "Philly", s10[] = "*Fortune|", s11[] = "Miami", s12[] = "NYC", s13[] = "Houston|";
	char s14[] = "SanDiego|", s15[] = "Syracuse|", s16[] = "Portland|", s17[] = "LA", s18[] = "*JAIL";
	char s19[] = "Phoenix", s20[] = "Chicago", s21[] = "Columbus|", s22[] = "DC", s23[] = "Seattle";
	for (row = 0; row < HEIGHT; row++)
	{
		if (row == 0 || row == 3 || row == 18 || row == 21)
		{
			for (col = 0; col < WIDTH; col++)
				board[row][col] = '_';
		}
		else
		{
			for (col = 0; col < WIDTH; col++)
			{
				if (col > 8 && col < 55)
					board[row][col] = ' ';
				if (col >= 0 && col < 9)
					board[row][col] = '_';
				if (col > 54 && col <= 63)
					board[row][col] = '_';
			}
		}
	}
	for (row = 1; row < HEIGHT; row++)
	{
		for (col = 0; col < WIDTH; col++)
		{
			if (col == 0 || (col % 9) == 0)
				board[row][col] = '|';
			else if ((row % 3) != 0)
				board[row][col] = ' ';
			if (row > 3 && row < 18 && col > 9 && col < 54)
				board[row][col] = ' ';
			if (row == 18 && col > 9 && col < 54)
				board[row][col] = '_';
		}
	}
	strcpy(board[2] + 3, s0); strcpy(board[2] + 11, s1); strcpy(board[2] + 20, s2);
	strcpy(board[2] + 28, s3); strcpy(board[2] + 38, s4); strcpy(board[2] + 46, s5);
	strcpy(board[2] + 55, s6); strcpy(board[5] + 56, s7); strcpy(board[8] + 55, s8);
	strcpy(board[11] + 56, s9); strcpy(board[14] + 55, s10); strcpy(board[17] + 56, s11);
	strcpy(board[20] + 57, s12); strcpy(board[20] + 19, s16); strcpy(board[20] + 28, s15);
	strcpy(board[20] + 37, s14); strcpy(board[20] + 47, s13); strcpy(board[20] + 13, s17);
	strcpy(board[20] + 2, s18); strcpy(board[17] + 1, s19); strcpy(board[14] + 1, s20);
	strcpy(board[11] + 1, s21); strcpy(board[8] + 4, s22); strcpy(board[5] + 1, s23);
	board[3][0] = '|'; board[15][0] = '|';
}
/******************** DISPLAY BOARD ********************/
void displayboard(char board[HEIGHT][WIDTH], int *balance1, int *balance2,
	char name1[20], char name2[20], int prop1[20], int prop2[20])
{
	int row, col, i, j;
	printf("\n\nProperty name:  | %10s:  | %10s:\n", name1, name2);
	printf("Albany          |          %d   |      %d\n", prop1[0], prop2[0]);
	printf("Boston          |          %d   |      %d\n", prop1[1], prop2[1]);
	printf("El Paso         |          %d   |      %d\n", prop1[2], prop2[2]);
	printf("Orlando         |          %d   |      %d\n", prop1[3], prop2[3]);
	printf("Las Vegas       |          %d   |      %d\n", prop1[4], prop2[4]);
	printf("Denver          |          %d   |      %d\n", prop1[5], prop2[5]);
	printf("Atlanta         |          %d   |      %d\n", prop1[6], prop2[6]);
	printf("Philly          |          %d   |      %d\n", prop1[7], prop2[7]);
	printf("Miami           |          %d   |      %d\n", prop1[8], prop2[8]);
	printf("NYC             |          %d   |      %d\n", prop1[9], prop2[9]);
	printf("Houston         |          %d   |      %d\n", prop1[10], prop2[10]);
	printf("San Diego       |          %d   |      %d\n", prop1[11], prop2[11]);
	printf("Syracuse        |          %d   |      %d\n", prop1[12], prop2[12]);
	printf("Portland        |          %d   |      %d\n", prop1[13], prop2[13]);
	printf("LA              |          %d   |      %d\n", prop1[14], prop2[14]);
	printf("Phoenix         |          %d   |      %d\n", prop1[15], prop2[15]);
	printf("Chicago         |          %d   |      %d\n", prop1[16], prop2[16]);
	printf("Columbus        |          %d   |      %d\n", prop1[17], prop2[17]);
	printf("DC              |          %d   |      %d\n", prop1[18], prop2[18]);
	printf("Seattle         |          %d   |      %d\n", prop1[19], prop2[19]);

	for (row = 0; row < HEIGHT; row++)
	{
		for (col = 0; col < WIDTH; col++)
		{
			printf("%c", board[row][col]);
		} printf("\n");
	}
	printf("\n%s's balance: $%d. %s's balance: $%d.\n", name1, *balance1, name2, *balance2);
}
/******************** ROLL DICE ********************/
int rollDice()
{
	int num1, num2, sum;
	num1 = (rand() % 6) + 1; num2 = (rand() % 6) + 1; // RAND
	sum = num1 + num2;
	return sum;
}
/******************** SET POSITION FOR PLAYER 1 ********************/
void setPositionP1(char board[HEIGHT][WIDTH], char character[5], int *position, int roll, int *balance, char name[])
{
	*position = *position + roll;
	if (*position > 23)
	{
		*position = *position - 24;
		printf("%s passed GO! and earns $250!\n", name);
		*balance += 250; delay(3.00);
	}
	switch (*position)
	{
	case 0: strcpy(board[1] + 1, character); break;
	case 1: strcpy(board[1] + 10, character); break;
	case 2: strcpy(board[1] + 19, character); break;
	case 3: strcpy(board[1] + 28, character); break;
	case 4: strcpy(board[1] + 37, character); break;
	case 5: strcpy(board[1] + 46, character); break;
	case 6: strcpy(board[1] + 55, character); break;
	case 7: strcpy(board[4] + 55, character); break;
	case 8: strcpy(board[7] + 55, character); break;
	case 9: strcpy(board[10] + 55, character); break;
	case 10: strcpy(board[13] + 55, character); break;
	case 11: strcpy(board[16] + 55, character); break;
	case 12: strcpy(board[19] + 55, character); break;
	case 13: strcpy(board[19] + 46, character); break;
	case 14: strcpy(board[19] + 37, character); break;
	case 15: strcpy(board[19] + 28, character); break;
	case 16: strcpy(board[19] + 19, character); break;
	case 17: strcpy(board[19] + 10, character); break;
	case 18: strcpy(board[19] + 1, character); break;
	case 19: strcpy(board[16] + 1, character); break;
	case 20: strcpy(board[13] + 1, character); break;
	case 21: strcpy(board[10] + 1, character); break;
	case 22: strcpy(board[7] + 1, character); break;
	case 23: strcpy(board[4] + 1, character); break;
	}
}
/******************** SET POSITION FOR PLAYER 2 ********************/
void setPositionP2(char board[HEIGHT][WIDTH], char character[5], int *position, int roll, int *balance, char name[])
{
	*position = *position + roll;
	if (*position > 23)
	{
		*position = *position - 24;
		printf("%s passed GO! and earns $250!", name);
		*balance += 250; delay(3.00);
	}
	switch (*position)
	{
	case 0: strcpy(board[1] + 5, character); break;
	case 1: strcpy(board[1] + 14, character); break;
	case 2: strcpy(board[1] + 23, character); break;
	case 3: strcpy(board[1] + 32, character); break;
	case 4: strcpy(board[1] + 41, character); break;
	case 5: strcpy(board[1] + 50, character); break;
	case 6: strcpy(board[1] + 59, character); break;
	case 7: strcpy(board[4] + 59, character); break;
	case 8: strcpy(board[7] + 59, character); break;
	case 9: strcpy(board[10] + 59, character); break;
	case 10: strcpy(board[13] + 59, character); break;
	case 11: strcpy(board[16] + 59, character); break;
	case 12: strcpy(board[19] + 59, character); break;
	case 13: strcpy(board[19] + 50, character); break;
	case 14: strcpy(board[19] + 41, character); break;
	case 15: strcpy(board[19] + 32, character); break;
	case 16: strcpy(board[19] + 23, character); break;
	case 17: strcpy(board[19] + 14, character); break;
	case 18: strcpy(board[19] + 5, character); break;
	case 19: strcpy(board[16] + 5, character); break;
	case 20: strcpy(board[13] + 5, character); break;
	case 21: strcpy(board[10] + 5, character); break;
	case 22: strcpy(board[7] + 5, character); break;
	case 23: strcpy(board[4] + 5, character); break;
	}
}
/******************** BUY / PAY AT NEW POSITION ********************/
int newPosition(int *position, int *balance, int properties[], int playerProps[], char p2name[], char p1name[])
{
	property Albany = { 200, 150 }; property Boston = { 200, 150 }; property ElPaso = { 200, 150 }; property Orlando = { 300, 225 };
	property LasVegas = { 500, 375 }; property Denver = {200, 150}; property Atlanta = { 300, 225 }; property Philly = {300, 225};
	property Miami = { 450, 350 }; property NYC = { 500, 375 }; property Houston = { 300, 225 }; property SanDiego = {350, 250};
	property Syracuse = { 200, 150 }; property Portland = { 250, 175 }; property LA = { 500, 375 }; property Phoenix = {200, 150};
	property Chicago = { 350, 250 }; property Columbus = { 200, 150 }; property DC = { 350, 250 }; property Seattle = {400, 300};
	char answer; int tax, sum;
	switch (*position)
	{
	case 0:
		break;
	case 1: if (properties[0] == 0)
	{
		printf("Would you like to buy Albany? Price: %d (y/n)\n", Albany.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Albany.price;
			playerProps[0] = 1;
			properties[0] = 1;
		}
	}
			else if (properties[0] == 1 && playerProps[0] == 0)
			{
				printf("Albany belongs to %s!\n", p2name);
				printf("Rent is %d\n", Albany.rent);
				*balance = *balance - Albany.rent;
				return Albany.rent;
			}
			else
				printf("This property belongs to you.\n");
		break;
	case 2: if (properties[1] == 0)
	{
		printf("Would you like to buy Boston? Price: %d (y/n)\n", Boston.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Boston.price;
			playerProps[1] = 1;
			properties[1] = 1;
		}
		return 0;
	}
			else if (properties[1] == 1 && playerProps[1] == 0)
			{
				printf("Boston belongs to %s!\n", p2name);
				printf("Rent is %d\n", Boston.rent);
				*balance = *balance - Boston.rent;
				return Boston.rent;
			}
			else
				printf("This property belongs to you.\n");
			break;
	case 3: if (properties[2] == 0)
	{
		printf("Would you like to buy El Paso? Price: %d (y/n)\n", ElPaso.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - ElPaso.price;
			playerProps[2] = 1;
			properties[2] = 1;
		}
		return 0;
	}
			else if (properties[2] == 1 && playerProps[2] == 0)
			{
				printf("El Paso belongs to %s!\n", p2name);
				printf("Rent is %d\n", ElPaso.rent);
				*balance = *balance - ElPaso.rent;
				return ElPaso.rent;
			}
			else
				printf("This property belongs to you.\n");
		break;
	case 4:
		tax = *balance / 5;
		printf("%s must pay income tax!\n%d were paid.\n", p1name, tax);
		*balance = *balance - tax;
		break;
	case 5: if (properties[3] == 0)
		{
			printf("Would you like to buy Orlando? Price: %d (y/n)\n", Orlando.price);
			scanf(" %c", &answer);
			if (answer == 'n' || answer == 'N')
				break;
			else
			{
				*balance = *balance - Orlando.price;
				playerProps[3] = 1;
				properties[3] = 1;
			}
			return 0;
		}
		else if (properties[3] == 1 && playerProps[3] == 0)
		{
			printf("Orlando belongs to %s!\n", p2name);
			printf("Rent is %d\n", Orlando.rent);
			*balance = *balance - Orlando.rent;
			return Orlando.rent;
		}
		else
			printf("This property belongs to you.\n");
		break;
	case 6: if (properties[4] == 0)
		{
			printf("Would you like to buy Las Vegas? Price: %d (y/n)\n", LasVegas.price);
			scanf(" %c", &answer);
			if (answer == 'n' || answer == 'N')
				break;
			else
			{
				*balance = *balance - LasVegas.price;
				playerProps[4] = 1;
				properties[4] = 1;
			}
			return 0;
		}
		else if (properties[4] == 1 && playerProps[4] == 0)
		{
			printf("Las Vegas belongs to %s!\n", p2name);
			printf("Rent is %d\n", LasVegas.rent);
			*balance = *balance - LasVegas.rent;
			return LasVegas.rent;
		}
		else
			printf("This property belongs to you.\n");
		break;
	case 7: if (properties[5] == 0)
	{
		printf("Would you like to buy Denver? Price: %d (y/n)\n", Denver.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Denver.price;
			playerProps[5] = 1;
			properties[5] = 1;
		}
		return 0;
	}
			else if (properties[5] == 1 && playerProps[5] == 0)
			{
				printf("Denver belongs to %s!\n", p2name);
				printf("Rent is %d\n", Denver.rent);
				*balance = *balance - Denver.rent;
				return Denver.rent;
			}
			else
				printf("This property belongs to you.\n");
		break;
	case 8: if (properties[6] == 0)
	{
		printf("Would you like to buy Atlanta? Price: %d (y/n)\n", Atlanta.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Atlanta.price;
			playerProps[6] = 1;
			properties[6] = 1;
		}
		return 0;
	}
			else if (properties[6] == 1 && playerProps[6] == 0)
			{
				printf("Atlanta belongs to %s!\n", p2name);
				printf("Rent is %d\n", Atlanta.rent);
				*balance = *balance - Atlanta.rent;
				return Atlanta.rent;
			}
			else
				printf("This property belongs to you.\n");
		break;
	case 9: if (properties[7] == 0)
	{
		printf("Would you like to buy Philly? Price: %d (y/n)\n", Philly.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Philly.price;
			playerProps[7] = 1;
			properties[7] = 1;
		}
		return 0;
	}
			else if (properties[7] == 1 && playerProps[7] == 0)
			{
				printf("Philly belongs to %s!\n", p2name);
				printf("Rent is %d\n", Philly.rent);
				*balance = *balance - Philly.rent;
				return Philly.rent;
			}
			else
				printf("This property belongs to you.\n");
		break;
	case 10:
		printf("%s has landed on Fortune!\n", p1name);
		sum = rand() % 3;
		switch (sum)
		{
		case 0: sum = 100; break;
		case 1: sum = 250; break;
		case 2: sum = 400; break;
		}
		printf("%s wins %d\n", p1name, sum);
		*balance += sum;
		break;
	case 11: if (properties[8] == 0)
	{
		printf("Would you like to buy Miami? Price: %d (y/n)\n", Miami.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Miami.price;
			playerProps[8] = 1;
			properties[8] = 1;
		}
		return 0;
	}
			 else if (properties[8] == 1 && playerProps[8] == 0)
			 {
				 printf("Miami belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Miami.rent);
				 *balance = *balance - Miami.rent;
				 return Miami.rent;
			 }
			 else
				printf("This property belongs to you.\n");
		break;
	case 12: if (properties[9] == 0)
	{
		printf("Would you like to buy NYC? Price: %d (y/n)\n", NYC.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - NYC.price;
			playerProps[9] = 1;
			properties[9] = 1;
		}
		return 0;
	}
			 else if (properties[9] == 1 && playerProps[9] == 0)
			 {
				 printf("NYC belongs to %s!\n", p2name);
				 printf("Rent is %d\n", NYC.rent);
				 *balance = *balance - NYC.rent;
				 return NYC.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 13: if (properties[10] == 0)
	{
		printf("Would you like to buy Houston? Price: %d (y/n)\n", Houston.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Houston.price;
			playerProps[10] = 1;
			properties[10] = 1;
		}
		return 0;
	}
			 else if (properties[10] == 1 && playerProps[10] == 0)
			 {
				 printf("Houston belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Houston.rent);
				 *balance = *balance - Houston.rent;
				 return Houston.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 14: if (properties[11] == 0)
	{
		printf("Would you like to buy San Diego? Price: %d (y/n)\n", SanDiego.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - SanDiego.price;
			playerProps[11] = 1;
			properties[11] = 1;
		}
		return 0;
	}
			 else if (properties[11] == 1 && playerProps[11] == 0)
			 {
				 printf("San Diego belongs to %s!\n", p2name);
				 printf("Rent is %d\n", SanDiego.rent);
				 *balance = *balance - SanDiego.rent;
				 return SanDiego.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 15: if (properties[12] == 0)
	{
		printf("Would you like to buy Syracuse? Price: %d (y/n)\n", Syracuse.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Syracuse.price;
			playerProps[12] = 1;
			properties[12] = 1;
		}
		return 0;
	}
			 else if (properties[12] == 1 && playerProps[12] == 0)
			 {
				 printf("Syracuse belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Syracuse.rent);
				 *balance = *balance - Syracuse.rent;
				 return Syracuse.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 16: if (properties[13] == 0)
	{
		printf("Would you like to buy Portland? Price: %d (y/n)\n", Portland.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Portland.price;
			playerProps[13] = 1;
			properties[13] = 1;
		}
		return 0;
	}
			 else if (properties[13] == 1 && playerProps[13] == 0)
			 {
				 printf("Portland belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Portland.rent);
				 *balance = *balance - Portland.rent;
				 return Portland.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
		
	case 17: if (properties[14] == 0)
	{
		printf("Would you like to buy Los Angeles? Price: %d (y/n)\n", LA.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - LA.price;
			playerProps[14] = 1;
			properties[14] = 1;
		}
		return 0;
	}
			 else if (properties[14] == 1 && playerProps[14] == 0)
			 {
				 printf("Los Angeles belongs to %s!\n", p2name);
				 printf("Rent is %d\n", LA.rent);
				 *balance = *balance - LA.rent;
				 return LA.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 18: 
		printf("%s has landed in jail!\n", p1name);
		printf("%s sets bail at $200\n", p1name);
		*balance = *balance - 200;
		break;
	case 19: if (properties[15] == 0)
	{
		printf("Would you like to buy Phoenix? Price: %d (y/n)\n", Phoenix.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Phoenix.price;
			playerProps[15] = 1;
			properties[15] = 1;
		}
		return 0;
	}
			 else if (properties[15] == 1 && playerProps[15] == 0)
			 {
				 printf("Phoenix belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Phoenix.rent);
				 *balance = *balance - Phoenix.rent;
				 return Phoenix.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 20: if (properties[16] == 0)
	{
		printf("Would you like to buy Chicago? Price: %d (y/n)\n", Chicago.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Chicago.price;
			playerProps[16] = 1;
			properties[16] = 1;
		}
		return 0;
	}
			 else if (properties[16] == 1 && playerProps[16] == 0)
			 {
				 printf("Chicago belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Chicago.rent);
				 *balance = *balance - Chicago.rent;
				 return Chicago.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 21: if (properties[17] == 0)
	{
		printf("Would you like to buy Columbus? Price: %d (y/n)\n", Columbus.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Columbus.price;
			playerProps[17] = 1;
			properties[17] = 1;
		}
		return 0;
	}
			 else if (properties[17] == 1 && playerProps[17] == 0)
			 {
				 printf("Columbus belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Columbus.rent);
				 *balance = *balance - Columbus.rent;
				 return Columbus.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 22: if (properties[18] == 0)
	{
		printf("Would you like to buy Washington DC? Price: %d (y/n)\n", DC.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - DC.price;
			playerProps[18] = 1;
			properties[18] = 1;
		}
		return 0;
	}
			 else if (properties[18] == 1 && playerProps[18] == 0)
			 {
				 printf("Washington DC belongs to %s!\n", p2name);
				 printf("Rent is %d\n", DC.rent);
				 *balance = *balance - DC.rent;
				 return DC.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	case 23: if (properties[19] == 0)
	{
		printf("Would you like to buy Seattle? Price: %d (y/n)\n", Seattle.price);
		scanf(" %c", &answer);
		if (answer == 'n' || answer == 'N')
			break;
		else
		{
			*balance = *balance - Seattle.price;
			playerProps[19] = 1;
			properties[19] = 1;
		}
		return 0;
	}
			 else if (properties[19] == 1 && playerProps[19] == 0)
			 {
				 printf("Seattle belongs to %s!\n", p2name);
				 printf("Rent is %d\n", Seattle.rent);
				 *balance = *balance - Seattle.rent;
				 return Seattle.rent;
			 }
			 else
				 printf("This property belongs to you.\n");
		break;
	}
	return 0;
}
/******************** TIME DELAY ********************/
void delay(double secs)
{
	int ct, clicksToDelay, cps = CLOCKS_PER_SEC;
	clicksToDelay = cps*secs; ct = clock();
	while (clock() - ct < clicksToDelay);
}
/******************** PRINT TO EXTERNAL FILE ********************/
void printExternal(char p1name[], char p2name[], int p1balance, int p2balance, int p1properties[], int p2properties[])
{
	FILE * myout;
	myout = fopen("GameResult.txt", "w");
	int i = 0, p1props = 0, p2props = 0, highestIndex;

	if (p1balance < 0)
	{
		fprintf(myout, "   Winner: %s\n", p2name);
		fprintf(myout, "   Balance: %d\n", p2balance);
		while (i < 20)
		{
			if (p2properties[i] == 1)
				p2props++;
			i++;
		} fprintf(myout, "   Total # of properties: %d\n", p2props);
		highestIndex = mostValuable(p2properties);
		fprintf(myout, "   Most valuable property: ");
		switch (highestIndex)
		{
		case 0: fprintf(myout, "Albany"); break;
		case 1: fprintf(myout, "Boston"); break;
		case 2: fprintf(myout, "El Paso"); break;
		case 3: fprintf(myout, "Orlando"); break;
		case 4: fprintf(myout, "Las Vegas"); break;
		case 5: fprintf(myout, "Denver"); break;
		case 6: fprintf(myout, "Atlanta"); break;
		case 7: fprintf(myout, "Philly"); break;
		case 8: fprintf(myout, "Miami"); break;
		case 9: fprintf(myout, "New York City"); break;
		case 10: fprintf(myout, "Houston"); break;
		case 11: fprintf(myout, "San Diego"); break;
		case 12: fprintf(myout, "Syracuse"); break;
		case 13: fprintf(myout, "Portland"); break;
		case 14: fprintf(myout, "Los Angeles"); break;
		case 15: fprintf(myout, "Phoenix"); break;
		case 16: fprintf(myout, "Chicago"); break;
		case 17: fprintf(myout, "Columbus"); break;
		case 18: fprintf(myout, "DC"); break;
		case 19: fprintf(myout, "Seattle"); break;
		}
	}
	else if (p2balance < 0)
	{
		fprintf(myout, "   Winner: %s\n", p1name);
		fprintf(myout, "   Balance: %d\n", p1balance);
		while (i < 20)
		{
			if (p1properties[i] == 1)
				p1props++;
			i++;
		} fprintf(myout, "   Total # of properties: %d\n", p1props);
		highestIndex = mostValuable(p1properties);
		fprintf(myout, "   Most valuable property: ");
		switch (highestIndex)
		{
		case 0: fprintf(myout, "Albany"); break;
		case 1: fprintf(myout, "Boston"); break;
		case 2: fprintf(myout, "El Paso"); break;
		case 3: fprintf(myout, "Orlando"); break;
		case 4: fprintf(myout, "Las Vegas"); break;
		case 5: fprintf(myout, "Denver"); break;
		case 6: fprintf(myout, "Atlanta"); break;
		case 7: fprintf(myout, "Philly"); break;
		case 8: fprintf(myout, "Miami"); break;
		case 9: fprintf(myout, "New York City"); break;
		case 10: fprintf(myout, "Houston"); break;
		case 11: fprintf(myout, "San Diego"); break;
		case 12: fprintf(myout, "Syracuse"); break;
		case 13: fprintf(myout, "Portland"); break;
		case 14: fprintf(myout, "Los Angeles"); break;
		case 15: fprintf(myout, "Phoenix"); break;
		case 16: fprintf(myout, "Chicago"); break;
		case 17: fprintf(myout, "Columbus"); break;
		case 18: fprintf(myout, "DC"); break;
		case 19: fprintf(myout, "Seattle"); break;
		}
	}
	else
		printf("An error has occurred printing game results.\n");
	fclose(myout);
}
/******************** SEARCH FOR MOST VALUABLE PROPERTY INDEX ********************/
int mostValuable(int properties[])
{
	int propertyValues[] = { 200, 200, 200, 300, 500, 200, 300, 300, 450, 500, 300, 350, 200, 250, 500, 200, 350, 200, 350, 400 };
	int highestIndex, highestValue = 0, i = 0;

	while (i < 20)
	{
		if (properties[i] == 1 && propertyValues[i] > highestValue)
		{
			highestValue = propertyValues[i]; highestIndex = i;
		}
		i++;
	}
	return highestIndex;
}