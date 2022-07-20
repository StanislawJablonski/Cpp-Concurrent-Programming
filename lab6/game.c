#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>

#define key 10

int *board;

int memory;
int semaphores;
int score = -2;

void printBoard()
{
	printf("\n\n");
	int i, j;
	printf("  1 2 3\n");
	for (i = 0; i < 3; i++)
	{
		printf("%c", 'A' + i);
		for (j = 0; j < 3; j++)
		{
			printf("|");

			int val = *(board + i * 3 + j);
			switch (val)
			{
			case -1:
				printf("X");
				break;
			case 1:
				printf("O");
				break;
			default:
				printf(" ");
				break;
			}
		}
		printf("|\n");
	}
}

void turn(int n)
{
	printBoard();
	printf("\nWybierz pole A/B/C''1/2/3: ");
	char x;
	int y;
	int check = 0;
	do
	{
		int ix;
		scanf("%c%d", &x, &y);

		if (x == '\n'){}
		else if (x < 'A' || x > 'C' || y < 1 || y > 3) {printf("Niepoprawny ruch, pole poza plansza\n");}
		else
		{
			int ix = (int)x - (int)'A';
			y--;
			int val = *(board + ix * 3 + y);
			if (val != -1 && val != 1)
			{
				*(board + ix * 3 + y) = n;
				check = 1;
			}
			else {printf("Niepoprawny ruch, pole zajete\n");}
		}
	} while (check == 0);
	printBoard();
}

int tryFindWinner()
{
	int table[8];
	int i, j, n;
	for (n = 0; n < 8; n++)
	{
		table[n] = 0;
	}
	n = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			int val = *(board + i * 3 + j);
			table[0 + i] += val;
			table[3 + j] += val;
			if (i == j){table[6] += val;}
			else if (2 - i == j){table[7] += val;}
			if (val != 0){n++;}
		}
	}
	for (n = 0; n < 8; n++)
	{
		if (table[n] == 3){return 1;}
		else if (table[n] == -3){return -1;}
	}
	if (n == 9){return 0;}

	return -2;
}

void ending(int player, int winner)
{
	if (player != 0)
	{
		if (player == winner){printf("Wygrana!\n");}
		else if (winner == 0){printf("Remis!\n");}
		else {printf("Porazka!\n");}
	}
	semctl(semaphores, 0, IPC_RMID, 0);
	shmdt(board);
	shmctl(memory, IPC_RMID, 0);
	exit(0);
}

void intHandler(int dummy) {ending(0, 0);}

int main()
{

	printf("Nowa gra");
	signal(SIGINT, intHandler);

	struct sembuf operA1 = {0, -1, 0}, operA2 = {1, 1, 0}, *oper1, *oper2;
	struct sembuf operB1 = {1, -1, 0}, operB2 = {0, 1, 0};

	memory = shmget(key, 256, 0777 | IPC_CREAT);
	board = shmat(memory, 0, 0);

	int player;

	if ((semaphores = semget(key, 2, 0777 | IPC_CREAT | IPC_EXCL)) != -1)
	{
		printf("\nJestes pierwszy, Twoj znak - X. Rozpocznij rozgrywke\n");
		player = -1;

		oper1 = &operA1;
		oper2 = &operA2;

		semctl(semaphores, 0, SETVAL, 1);
		semctl(semaphores, 1, SETVAL, 0);
	}
	else
	{
		semaphores = semget(key, 2, 0777 | IPC_CREAT);

		printf("\nJestes drugi, twoj znak to O. Zaczekaj az pierwszy gracz rozpocznie\n");
		player = 1;

		oper1 = &operB1;
		oper2 = &operB2;
	}

	while (1)
	{
		semop(semaphores, oper1, 1);
		score = tryFindWinner();
		if (score != -2)
		{
			printBoard();
			ending(player, score);
			break;
		}

		turn(player);
		score = tryFindWinner();

		if (score != -2)
		{
			printBoard();
			ending(player, score);
			break;
		}

		semop(semaphores, oper2, 1);
	}
	return 0;
}