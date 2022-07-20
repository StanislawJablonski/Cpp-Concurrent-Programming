#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>

#define key1 2137
#define key2 2420

typedef struct
{
	long typ;
	char slowo[256];
} Komunikat;

int main(int argc, char* argv[])
{
	Komunikat komunikat;
	int kolejka1;
	int kolejka2;

	kolejka1 = msgget(key1, 0777);
	kolejka2 = msgget(key2, 0777);

if(!argv[1])
{
	printf("Poprawne uzycie: ./klient PolskieSlowo\n");
	exit(1);
}

	strcpy(komunikat.slowo, argv[1]);
	komunikat.typ = getpid();

if(msgsnd(kolejka1, &komunikat, sizeof(char)*256, 0) == -1)
{
	printf("\nBlad wysylania slowa do serwera");
	exit(1);
}

	printf("Polski - %s\n", argv[1]);

if(msgrcv(kolejka2, &komunikat, sizeof(char)*256, getpid(), 0) == -1)
{
	printf("\nBlad odbiererania wiadomosci od serwera");
	exit(1);
}

	printf("Angielski - %s\n", komunikat.slowo);

return 0;
}