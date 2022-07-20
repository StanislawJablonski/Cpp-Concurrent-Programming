#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>


int main(void) {
    FILE *fp;
    char buffer[] = "buffer";
    char clientFile[200];
    char str[200];

    while(1) {
        fp = fopen(buffer, "w");
        fclose(fp);

        printf("Oczekiwanie na wiadomosc\n");

        while(1) {
            fp = fopen(buffer, "r");
            int c = fgetc(fp);

            if (c == EOF) {
                // pusty
                sleep(1);
            } else {
                // nie pusty
                rewind(fp);
                printf("Wiadomosc otrzymana:\n");
                
                char *bufptr = 0;
                size_t buflen = 0;
                getline(&bufptr, &buflen, fp);
                strcpy(clientFile, bufptr);
                clientFile[strlen(clientFile)-1] = '\0';
                while (getline(&bufptr, &buflen, fp) != -1) {
                    printf("%s\n", bufptr);
                }

                fclose(fp);
                break;
            }
            fclose(fp);
        }

        fp = fopen(clientFile, "w");
        printf("Wpisz wiadomosc -1 konczy:\n");
        do {
            scanf("%[^\n]%*c", str);
            
            if( strcmp(str, "-1") == 0 ) {
                break;
            }
            fprintf(fp, "%s\n", str);
        } while (1);
        fclose(fp);
        
    }


}