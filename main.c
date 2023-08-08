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
    char* nombre;
    int len= strlen("Archivos_de_prueba/\0");
    if (d){
        while ((dir = readdir(d)) != NULL){
            
            if (strstr(dir->d_name,".txt") != NULL){
                nombre = (char*)malloc(len+strlen(dir->d_name));
                strcpy(nombre,"Archivos_de_prueba/");
                strcat(nombre,dir->d_name);
                pointer = fopen(nombre,"r");
                if (pointer == NULL){
                    printf("No existe.");
                }
                
                free(nombre);
                fclose(pointer);
                
            }
        }
        closedir(d);
    }

    return 0;
}