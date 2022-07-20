#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct ServerCommunicate {
	int size;
} ServerCommunicate;

typedef struct ClientCommunicate {
	int size;
	int id;
} ClientCommunicate;

int main(){
	
	int a;
	int f;
	ClientCommunicate messege1;
	ServerCommunicate messege2;
	const char* home = getenv("HOME");

	mkfifo("clientfifo", 0666);

	if((f = open("serverfifo", O_WRONLY, 0)) == -1){
		printf("Cant open serverfifo\n");
		exit(0);
	}

	printf("Enter person id: ");
	scanf("%3d", &a);
	messege1.id = a;
	messege1.size = strlen(home);

	int bufferLength = messege1.size + sizeof(messege1);
	char* buffer = (char*) malloc(bufferLength);
	memcpy(buffer, &messege1, sizeof(messege1));
	memcpy(buffer + sizeof(messege1), home, messege1.size);

	if(write(f, buffer, bufferLength) == -1){
		printf("Response saving error\n");
		exit(0);
	}
	close(f);

	f = open("clientfifo", O_RDONLY, 0);
	if(read(f, &messege2, sizeof(messege2)) == -1){
		printf("Cant read size\n"); 
		exit(0);
	}

	char* text = (char*) malloc(messege2.size);
	if(read(f, text, messege2.size) == -1) {
		printf("Communicate reading error\n");
		exit(0);
	}

	printf("Person of given id: %s\n", text);
	close(f);
	unlink("clientfifo");
}
