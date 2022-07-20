#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>

#define slownik_size 10
#define key1 2137
#define key2 2420

typedef struct {
  char pl[256];
  char en[256];
} Slownik;

typedef struct {
  long typ;
  char slowo[256];
} Komunikat;

Slownik slownik[slownik_size] = {
	{"kot","cat"},
	{"pies","dog"},
	{"dziecko","baby"},
	{"tygrys","tiger"},
	{"lew","lion"},
	{"biurko","desk"},
	{"kwiat","flower"},
	{"oko","eye"},
	{"drzewo","tree"},
	{"drzwi","door"}
};

char *tlumaczenie(char* slowo)
{
  int i;
  for(i = 0; i < slownik_size; i++)
  {
    if(!strcmp(slowo, slownik[i].pl))
      return slownik[i].en;
  }
  return "Brak tlumaczenia w slowniku";
}

int main(int argc, char *argv[])
{

	msgctl(key1, IPC_RMID, 0);
	msgctl(key2, IPC_RMID, 0);
	Komunikat komunikat;
	int kolejka1;
	int kolejka2;

	kolejka1 = msgget(key1, 0777 | IPC_CREAT);
	kolejka2 = msgget(key2, 0777 | IPC_CREAT);

	printf("Serwer rozpoczal dzialanie poprawnie...\n");

 while(1)
 {


	printf("\nOdebrano - %s - od %ld \n", komunikat.slowo, komunikat.typ);

	strcpy(komunikat.slowo, tlumaczenie(komunikat.slowo));
	printf("Tlumaczenie - %s\n", komunikat.slowo);
	printf("\n Oczekiwanie na kolejne slowo... \n");

	if(msgsnd(kolejka2, &komunikat, sizeof(char)*256, 0) == -1)
	{
	printf("\n Blad wysylania tlumaczenia");
	exit(1);
	}
}
   return 0;
}