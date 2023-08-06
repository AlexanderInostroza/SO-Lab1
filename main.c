#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
    DIR *d;
    struct dirent *dir;
    d = opendir("./Archivos_de_prueba");
    if (d){
        while ((dir = readdir(d)) != NULL){
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return 0;
}