#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>

/**
 * @brief Recibe la matriz, el tamaño de la matriz, la palabra a buscar dentro de la matriz, y la orientación de la palabra a buscar. Con estos datos recorre la matriz hasta que encuentra la palabra. 
 * 
 * @param _matriz : Matriz recibida donde hay que buscar la palabra
 * @param size : TAmaño de la matriz recibida
 * @param palabra : Palabra a buscar
 * @param orientacion : Orientación de la palabra que se debe buscar
 * @return int 
 */
int buscar(void*_matriz, int size, char* palabra, char*orientacion){
    char (*matriz)[size] = _matriz;
    int len = strlen(palabra);
    int boolHorizontal = ((orientacion[0] == 'H') || (orientacion[0] == 'h')); 
    int k;
    if(boolHorizontal){
        for(int j=0; j<size; j++){
            k=0;
            for(int i=0; i<size; i++){
                if(palabra[k]==matriz[i][j]){
                    k++;
                }else{
                    k=0;
                }
                if(k==len){
                    printf("\t %s encontrado en %d,%d con orientacion %s\n",palabra,i-len+2,j+1, orientacion);
                }
            }
        }
    }else{
        for(int i=0; i<size; i++){
            k=0;
            for(int j=0; j<size; j++){
                if(palabra[k]==matriz[i][j]){
                    k++;
                }else{
                    k=0;
                }
                if(k==len){
                    printf("\t%s encontrado en %d,%d con orientacion %s\n",palabra,i+1,j-len+2, orientacion);
                }
            }
        }
    }
    return 1;
}

/**
 * @brief Recibiendo la palabra, la orientación, y el puntero del archivo, trabaja con el puntero del archivo y con eso consigue acceso aleatorio.
 * 
 * @param palabra : Palabra que hay que buscar
 * @param orientacion : Orientación de la palabra que se debe buscar
 * @param pointer : Puntero del archivo donde se busca la palabra
 * @return int 
 */
int buscar2(char* palabra, char* orientacion, FILE* pointer){
    char buffer[512];
    fgets(buffer, 512, pointer); //leer primera fila
    int size = strlen(buffer) / 2, len = strlen(palabra), ch;
    int startingPos = strlen(orientacion) + 1;
    fseek(pointer,startingPos, 0);

    int boolHorizontal = ((orientacion[0] == 'H') || (orientacion[0] == 'h'));
    int k;
    if(boolHorizontal){
        for(int j=0; j<size; j++){
            k=0;
            for(int i=0; i<size; i++){
                fseek(pointer, startingPos + 2*j*size + 2*i, 0);
                ch = getc(pointer);
                if(palabra[k]==ch){
                    k++;
                }else{
                    k=0;
                }
                if(k==len){
                    printf("\t %s encontrado en x=%d,y=%d con orientacion %s\n",palabra,i-len+2,j+1, orientacion);
                }
            }
        }
    }else{
        for(int i=0; i<size; i++){
            k=0;
            for(int j=0; j<size; j++){
                fseek(pointer, startingPos + 2*j*size + 2*i, 0);
                ch = getc(pointer);
                if(palabra[k]==ch){
                    k++;
                }else{
                    k=0;
                }
                if(k==len){
                    printf("\t%s encontrado en x=%d,y=%d con orientacion %s\n",palabra,i+1,j-len+2, orientacion);
                }
            }
        }
    }
    return 1;
}

/**
 * @brief Lee el archivo recibiendo la palabra, la direccion del archivo .txt de la palabra y el puntero de lectura del archivo
 * 
 * @param palabra : Palabra a buscar dentro del archivo
 * @param direccion : La dirección del archivo .txt a leer
 * @param pointer : Puntero para leer el archivo
 * @return int 
 */
int leerArchivo(char* palabra, char* direccion, FILE* pointer){
    int metodo = 1; // metodo de busqueda, 0 para primer metodo con matriz, 1 para metodo con random access.
    pointer = fopen(direccion,"r"); // se abre el archivo
    
    if (pointer == NULL){ // ?
        printf("No existe.");
        return 0;
    }

    clock_t start_t,end_t;
    int ch = strlen(palabra);
    char sizenum[8], current_dir[256],current_dir2[256];
    char buffer[16]; // para guardar orientacion

    fgets(buffer, 16, pointer); // leer primera linea
    buffer[0]= toupper(buffer[0]);
    buffer[strcspn(buffer,"\n")] = '\0';
    getcwd(current_dir,sizeof(current_dir));

    sprintf(current_dir, "GWD/%s",buffer);
    errno = 0;
    if (opendir(current_dir) == NULL && errno != EEXIST){
        mkdir(current_dir, 0777);
    }

    char primeraFila[512];
    fgets(primeraFila, 512, pointer); //leer primera fila
    int largo = strlen(primeraFila) / 2; // dimension de la matriz
    sprintf(sizenum,"%dx%d",largo,largo);
    sizenum[7] = '\0';

    getcwd(current_dir2, sizeof(current_dir));
    sprintf(current_dir2, "GWD/%s/%s",buffer,sizenum);

    errno = 0;
    if (opendir(current_dir2) == NULL && errno != EEXIST){
        mkdir(current_dir2, 0777);
    }

    start_t = clock(); //  *** inicio cronometro ***

    if(!metodo){
        rewind(pointer); // reseteo del puntero
        fgets(buffer, 16, pointer); // leer primera linea
        char matriz[largo][largo]; // matriz con la info del archivo
        int i = 0;
        int j = 0;
        while(1){
            ch = fgetc(pointer);
            if( ch == ' ' ){
                continue;
            }if(ch == '\n'){ // siguiente linea 
                i = 0;
                j++;
                continue;
            }if(feof(pointer)){ // EOF
                break;
            } // else
            matriz[i][j] = ch;
            i++;
        }
        buffer[0]= toupper(buffer[0]);
        buffer[strcspn(buffer,"\n")] = '\0';
        buscar(matriz,largo,palabra,buffer);
    }else{
        buscar2(palabra, buffer, pointer);
    }

    end_t = clock(); // *** final cronometro ***
    
    getcwd(current_dir,sizeof(current_dir));
    sprintf(current_dir,"%s",direccion);
    printf("%s\n",current_dir);
    sprintf(current_dir2, "GWD/%s/%s/%s.txt",buffer,sizenum,palabra);
    printf("%s  \n",current_dir2);
    rename(current_dir,current_dir2);
    printf("ESTE ES EL FINAL DE LA LECTURA DE %s:    %Lf \n",palabra,((long double)(end_t-start_t))/CLOCKS_PER_SEC); // printea tiempo en segundos
    fclose(pointer);
    return 1;
}

int main(){
    FILE* pointer;
    DIR *d;
    struct dirent *dir;
    char current_dir[256];
    getcwd(current_dir, sizeof(current_dir));

    sprintf(current_dir, "GWD");
    mkdir(current_dir, 0777);
     
    d = opendir("Archivos_de_prueba");
    char* direccion;
    int len = strlen("Archivos_de_prueba/\0");
    char* palabra;
  
    if (d){ // si el directorio existe
        while ((dir = readdir(d)) != NULL){ // mientras queden archivos
            if (strstr(dir->d_name,".txt") != NULL){ // si existe el archivo
                direccion = (char*)malloc(len+strlen(dir->d_name));
                strcpy(direccion,"Archivos_de_prueba/"); // carpeta fuente
                strcat(direccion,dir->d_name); // concatenar nombre para direccion
                palabra = (char*)malloc(strlen(dir->d_name));
                strcpy(palabra, dir->d_name);
                palabra[strlen(palabra)-4] = '\0';
                leerArchivo(palabra, direccion, pointer);
                free(palabra);
                free(direccion);
            }
        }
        closedir(d);
    }

    return 0;
}
