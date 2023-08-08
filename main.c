#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
    DIR *d;
    struct dirent *dir;
    FILE* pointer;
    d = opendir("./Archivos_de_prueba");
    char prueba[70] = "Archivos_de_prueba/\0";

    if (d){
        while ((dir = readdir(d)) != NULL){
            
            if (strstr(dir->d_name,".txt") != NULL){
                memcpy(prueba,"Archivos_de_prueba/\0",strlen(prueba));
                printf("%s\n", prueba);
                strcat(prueba,dir->d_name);
                printf("%s\n", prueba);
                pointer = fopen(prueba,"r");
                if (pointer == NULL){
                    printf("No existe.");
                }
                
                
                
            }
        }
        fclose(pointer);
        closedir(d);
    }

    return 0;
}