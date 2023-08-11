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
    int len = strlen("Archivos_de_prueba/\0");
    if (d){
        while ((dir = readdir(d)) != NULL){ // mientras queden archivos
            
            if (strstr(dir->d_name,".txt") != NULL){ // si existe el archivo
                nombre = (char*)malloc(len+strlen(dir->d_name)); // malloc para el string porque C
                strcpy(nombre,"Archivos_de_prueba/"); // carpeta fuente
                strcat(nombre,dir->d_name); // concatenar nombre para direccion
                pointer = fopen(nombre,"r"); // se abre el archivo
                if (pointer == NULL){ // ?
                    printf("No existe.");
                }

                int largo = 0; // dimension de la matriz, mal nombre de variable 
                int ch;
                printf("Se abrio %s\t", dir->d_name);
                char buffer[16]; // para guardar orientacion, ¿cambiar nombre a orientacion?
                fgets(buffer, 16, pointer); // leer primera linea
                while(1){ // se recorre primera linea, cambiar por largo de la linea dividido en 2
                    ch = fgetc(pointer);
                    if(ch == '\r'){ // revisar el \r , parece q no hay
                        printf("esta wea tiene backslash erre XD !!\n");
                    }
                    if(ch == '\n'){
                        break;
                    }
                    if(ch != ' '){
                        largo++;
                    }
                }
                rewind(pointer); // reseteo del puntero
                printf("el tamanio de la matriz es %d\n", largo);
                fgets(buffer, 16, pointer); // leer primera linea
                char matriz[largo][largo];
                int i = 0;
                int j = 0;
                while(1){
                    ch = fgetc(pointer);
                    if(ch == '\r'){
                        printf("Nashe\n"); // Nashe
                    }
                    if(ch == '\n'){ // siguiente linea
                        i = 0;
                        j++;
                        continue;
                    }
                    if(feof(ch)){ // EOF
                        break;
                    }
                    matriz[i][j] = ch;

                }
                for(int u=0; u<largo; u++){ // visualizar matrizz💤
                    for(int v=0; v<largo; v++){
                        printf("%d ", matriz[u][v]);
                    }
                    printf("\n");
                }

                
                free(nombre);
                fclose(pointer);
                
            }
        }
        closedir(d);
    }

    return 0;
}