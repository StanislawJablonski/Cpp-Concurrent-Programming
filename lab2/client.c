#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

// uruchamiac z argumentem!

int main(int argc, char **argv) {
    FILE *fp;
    char lockfile[] = "lockfile";
    char buffer[] = "buffer";
    char clientFile[strlen(argv[1])]; strcpy(clientFile, argv[1]);
    char str[200];

    fp = fopen(clientFile, "w");
    fclose(fp);

    while(open(lockfile, O_CREAT|O_EXCL, 0) == -1) {
        printf("Serwer zajety...\n");
        sleep(2);
    }

    printf("Polaczono z serwerem.\n");

    fp = fopen(buffer, "w");

    fprintf(fp, "%s\n", clientFile);
    printf("Wpisz wiadomosc -1 konczy:\n");
    do {
        scanf("%[^\n]%*c", str);
        
        if( strcmp(str, "-1") == 0) {
            break;
        }
        fprintf(fp, "%s\n", str);
    } while (1);
    fclose(fp);
    

    int i=0;
    printf("Czekanie na odpowiedz serwera...");

    while(1) {

        fp = fopen(clientFile, "r");
        int c = fgetc(fp);

        if (c == EOF) {
            sleep(1);
        } else {
            rewind(fp);
            printf("Odpowiedz otrzymana:\n");
            
            char *bufptr = 0;
            size_t buflen = 0;
            while (getline(&bufptr, &buflen, fp) != -1) {
                printf("%s\n", bufptr);
            }

            fclose(fp);
            break;
        }
        fclose(fp);
    }

    remove(lockfile);

    return 0;
}