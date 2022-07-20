#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<signal.h>


typedef struct Record {
	int id;
	int size;
	char* messege;
} Record;

typedef struct ServerCommunicate {
	int size;
} ServerCommunicate;

typedef struct ClientCommunicate {
	int size;
	int id;
} ClientCommunicate;

Record data[20];

void InjectData(int i, int id, const char* messege){
	Record item;
	item.id = id;
	item.size = strlen(messege);
	item.messege = (char*) malloc(item.size);
	memcpy(item.messege, messege, item.size);
	data[i] = item;
}

void DataInit() {
	InjectData(0, 1, "Jablonski");
	InjectData(1, 2, "Kowalski");
	InjectData(2, 3, "Mickiewicz");
	InjectData(3, 4, "Sienkiewicz");
	InjectData(4, 5, "Nowak");
	InjectData(5, 6, "Slowacki");
	InjectData(6, 7, "Moniuszko");
	InjectData(7, 8, "Krosicki");
	InjectData(8, 9, "Sapkowski");
	InjectData(9, 10, "Wisniewski");
}

void main(){
	signal(SIGTERM, ignore);
    signal(SIGHUP, ignore);
    signal(SIGUSR1, end);
	
	int f1;
	int f2;
	ClientCommunicate messege1;
	ServerCommunicate messege2;

	DataInit();

	unlink("serverfifo");
	mkfifo("serverfifo", 0666);
	while(1){
		(f1 = open("serverfifo", O_RDONLY, 0))
		printf("test");
		if(read(f1, &messege1, sizeof(messege1)) == -1){
			printf("Invalid communicate\n");
			exit(0);
		}


		char* homePath = (char*) malloc(messege1.size);

		if(read(f1, homePath, messege1.size) == -1){
			printf("Wrong path\n");
			exit(0);
		}

		int i;
		int result;
		for(i=0; i < 20; i++){
			if(data[i].id == messege1.id){
				result = i;
				break;
			}
			else{
				result = -1;
			}
		}

		if((f2 = open("clientfifo", O_WRONLY, 0)) != -1){
			if(result >= 0){
				messege2.size = data[result].size;

				int bufferLength = messege2.size + sizeof(messege2);
				char* buffer = (char*) malloc(bufferLength);
				memcpy(buffer, &messege2, sizeof(messege2));
				memcpy(buffer + sizeof(messege2), data[result].messege, messege2.size);

				if(write(f2, buffer, bufferLength) == -1){
					printf("Communicate writting error\n");
					exit(0);
				}
			}
			else{
				const char* error_messege = "Nie ma";
				messege2.size = strlen(error_messege);
				if(write(f2, &messege2, 4) == -1){
					printf("Size writting error\n");
					exit(0);
				}
				if(write(f2, error_messege, messege2.size) == -1){
					printf("Communicate writting error\n");
					exit(0);
				}
			}
			close(f2);
		}
		else{
			printf("Opening clientfifo error\n");
			exit(0);
		}

		close(f1);
	}
}