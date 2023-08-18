#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

int buscar(void*_matriz, int size, char* palabra, char*orientacion){
    char (*matriz)[size] = _matriz;
    int len = strlen(palabra);
    char upper[len];
    upper[len]='\0';
    char orientacion2[strlen(orientacion)];
    for(int i=0; i<len; i++){
        upper[i] = toupper(palabra[i]);
    }
    
    strcpy(orientacion2, orientacion);
    orientacion2[0] = toupper(orientacion[0]);
    int boolHorizontal = strcmp("Vertical", orientacion2);
    int k;
    if(boolHorizontal){
        for(int j=0; j<size; j++){
            //printf("\n");
            k=0;
            for(int i=0; i<size; i++){
                //printf("%c ", matriz[i][j]);
                if(upper[k]==matriz[i][j]){
                    k++;
                }else{
                    k=0;
                }
                if(k==len){
                    printf("\t %s encontrado en %d,%d con orientacion %s\n",upper,i-len+2,j+1, orientacion2);
                }
            }
        }
    }else{
        for(int i=0; i<size; i++){
            //printf("\n");
            k=0;
            for(int j=0; j<size; j++){
                //printf("%c ", matriz[i][j]);
                
                if(upper[k]==matriz[i][j]){
                    k++;
                }else{
                    k=0;
                }
                if(k==len){
                    printf("\t%s encontrado en %d,%d con orientacion %s\n",upper,i+1,j-len+2, orientacion2);
                }
            }
        }
    }
    
    return 1;
}

int leerArchivo(char* palabra, char* direccion, FILE* pointer, char* new_dir){
    clock_t start_t,end_t;
    pointer = fopen(direccion,"r"); // se abre el archivo
    start_t = clock();
    //printf("ESTE ES EL INICIO DE LA LECTURA DE %s:    %i \n",palabra,0);
    if (pointer == NULL){ // ?
        printf("No existe.");
        return 0;
    }
    int largo = 0; // dimension de la matriz, mal nombre de variable 
    int ch;
    char buffer[16]; // para guardar orientacion, ¿ cambiar nombre a orientacion?
    fgets(buffer, 16, pointer); // leer primera linea

    char opt[256],current_dir[256],current_dir2[256];
    buffer[0]= toupper(buffer[0]);
    buffer[strcspn(buffer,"\n")] = '\0';
    getcwd(current_dir2,sizeof(current_dir2));
    
    sprintf(current_dir2, "GWD/%s",buffer);
    errno = 0;
    if (opendir(current_dir2) == NULL && errno != EEXIST){
        mkdir(current_dir2, 0777);
    }


    while(1){ // se recorre primera linea, cambiar por largo de la linea dividido en 2
        ch = fgetc(pointer);
        if(ch == '\n'){
            break;
        }
        if(ch != ' '){
            largo++;
        }
    }
    // printf("Largo de la matriz de %s:\t\t%d\n", palabra, largo);
    
    char* sizenum,*sizenum2;
    sizenum = (char*)malloc((int)((ceil(log10(largo))+1)*sizeof(char))*2+1);
    sizenum2 = (char*)malloc((int)((ceil(log10(largo))+1)*sizeof(char))*2+2);
    sprintf(sizenum,"%d",largo);
    strcpy(sizenum2,sizenum);
    strcat(sizenum,"x");
    strcat(sizenum,sizenum2);
    getcwd(current_dir, sizeof(current_dir));
    sprintf(current_dir, "GWD/%s/%s",buffer,sizenum);
 
    errno = 0;

    if (opendir(current_dir) == NULL && errno != EEXIST){
        mkdir(current_dir, 0777);
    }
    //No tocar esto de arriba que crea bien las carpetas y me costo como 2 horas xd
    //ahora solo queda guardar los archivos de su lugar de origen a la carpeta

    rewind(pointer); // reseteo del puntero
    fgets(buffer, 16, pointer); // leer primera linea
    char matriz[largo][largo];


    int i = 0;
    int j = 0;
    while(1){
        ch = fgetc(pointer);
        if( ch == ' ' ){
            continue;
        }
        if(ch == '\n'){ // siguiente linea 
            i = 0;
            j++;
            continue;
        }
        if(feof(pointer)){ // EOF
            break;
        }
        matriz[i][j] = ch;
        i++;
    }
    for(int u=0; u<largo; u++){ // visualizar matrizz💤
        for(int v=0; v<largo; v++){
            //printf("%c ", matriz[u][v]);
        }
        //printf("\n");
    }
    end_t = clock();
    fclose(pointer);
    buffer[0]= toupper(buffer[0]);    
    buffer[strcspn(buffer,"\n")] = '\0';
    buscar(matriz,largo,palabra,buffer);
    
    getcwd(current_dir,sizeof(current_dir));
    sprintf(current_dir,"%s",direccion);
    printf("%s\n",current_dir);
    sprintf(current_dir2, "GWD/%s/%s/%s.txt",buffer,sizenum,palabra);
    printf("%s  \n",current_dir2);
    rename(current_dir,current_dir2);
    //printf("ESTE ES EL FINAL DE LA LECTURA DE %s:    %ld \n",palabra,end_t-start_t);
    free(sizenum);
    free(sizenum2);
    return 1;
}
int horizontal(FILE* pointer, char* palabra, int size){
    int i,j,k = 0;
    char letra;
    int largopal= strlen(palabra);
    int larglinea = size-largopal;
    while (j< size){

        while (i<size){

            letra= fgetc(pointer);
            
            if (palabra[k] == letra){
                k++;
            }
            if (k = largopal){
                return 1;
            }
            i++;
        }
        fgets(NULL,0,pointer);
        j++;        
    }

    return 0;
}


int main(){
    FILE* pointer;
    DIR *d;
    struct dirent *dir;
    char current_dir[256];
    char new_dir[256];
    getcwd(current_dir, sizeof(current_dir));

    sprintf(current_dir, "GWD");
    mkdir(current_dir, 0777);
     
    d = opendir("Archivos_de_prueba");
    char* direccion;
    int len = strlen("Archivos_de_prueba/\0");
    char* palabra;
  
    if (d){
        while ((dir = readdir(d)) != NULL){ // mientras queden archivos
            if (strstr(dir->d_name,".txt") != NULL){ // si existe el archivo
                direccion = (char*)malloc(len+strlen(dir->d_name)); // malloc para el string porque C
                strcpy(direccion,"Archivos_de_prueba/"); // carpeta fuente
                strcat(direccion,dir->d_name); // concatenar nombre para direccion
                palabra = (char*)malloc(strlen(dir->d_name));
                strcpy(palabra, dir->d_name);
                palabra[strlen(palabra)-4] = '\0';
                leerArchivo(palabra, direccion, pointer,new_dir);
                
                //sprintf("CWD", );
                //mkdir(folder, );
                free(palabra);
                free(direccion);
            }
        }
        closedir(d);
    }

    return 0;
}
