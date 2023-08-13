#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


typedef struct {
    int max_actual;
    int mayor;
    char genero[256];
} FileData;

/**
 * @brief Consigue la información de un juego contenida en su archivo de texto correspondiente.
 * 
 * @param filename : Nombre del archivo, e.g. "Nombre_del_juego.txt".
 * @return FileData : struct con la información del juego.
 */
FileData get_file_data(char *filename) {
    FileData data = {0, 0, ""};
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fscanf(file, "%d\n%d\n%s", &data.max_actual, &data.mayor, data.genero);
        fclose(file);
    }
    return data;
}

/**
 * @brief Cuenta la cantidad de archivos por cada categoría y sub-categoría.
 * 
 * @param name : Género del juego.
 * @param choise : Opción (cantidad de jugadores o jugadores actuales).
 * @param choise0 : Texto que representa la cantidad de jugadores.
 * @param opti : Texto que describe la opción seleccionada.
 * @param opt : Nombre de la carpeta origen a crear en base a la opción.
 * @return int 
 */
int contador(char name[256],char choise[30],char choise0[30],char opti[50], char opt[20]){
    int count = 0;
    DIR *dir0;
    struct dirent *ent;
    char *dir_path0 = "./";
    char path[1024];
    sprintf(path, "%s/%s/%s/%s", opt, dir_path0, name, choise);
    if ((dir0 = opendir(path)) != NULL) {
        while ((ent = readdir(dir0)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                count++;
            }
        }
    closedir(dir0);
    printf("    Juegos con una %s %s: %d\n", opti, choise0, count);
    } 
    return count;
}

int main() {
    // Variables para buscar archivos .txt
    DIR *dir;
    struct dirent *entry;
    char *txt_extension = ".txt";
    char current_dir[256];
    char new_dir[256];
    getcwd(current_dir, sizeof(current_dir));
    dir = opendir(current_dir);
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        exit(EXIT_FAILURE);
    }
    char opcion[20];
    // input de usuario
    printf("ingrese (1) ”cantidad actual” o (2) ”mayor cantidad” para el ordenamiento:");
    scanf("%s",opcion);
    char opt[20];
    if (strcmp(opcion, "1") == 0){
    	sprintf(opt, "Cantidad_Actual");
    }
    else if (strcmp(opcion, "2") == 0){
    	sprintf(opt, "Mayor_Cantidad");
    }
    // creación de la carpeta origen
    char folder[256];
    sprintf(folder, opt);
    mkdir(folder, 0777);
    while ((entry = readdir(dir)) != NULL) { // termina al revisar todos los .txt
        if (strstr(entry->d_name, txt_extension) != NULL) {
            FileData data = get_file_data(entry->d_name);
            if (strlen(data.genero) > 0) { // si tiene genero continua
                char folder1[256], folder2[256], folder3[256];
                sprintf(new_dir, "%s/%s/%s", current_dir, opt, data.genero);
                mkdir(new_dir, 0777);
                sprintf(folder1, "%s/%s/Menor_a_40000", opt, data.genero);
                sprintf(folder2, "%s/%s/Entre_40000_y_80000", opt, data.genero);
                sprintf(folder3, "%s/%s/Mayor_a_80000", opt, data.genero);
                mkdir(folder1, 0777);
                mkdir(folder2, 0777);
                mkdir(folder3, 0777);
                // opcion 1
                if (strcmp(opcion, "1") == 0){
                    if (data.max_actual < 40000){
                        char new_path[256];
                	sprintf(new_path, "%s/%s", folder1, entry->d_name);
                	rename(entry->d_name, new_path);
                    }
                    else if (data.max_actual > 80000){
                        char new_path[256];
                	sprintf(new_path, "%s/%s", folder3, entry->d_name);
                	rename(entry->d_name, new_path);
                    }
                    else{
                        char new_path[256];
                	sprintf(new_path, "%s/%s", folder2, entry->d_name);
                	rename(entry->d_name, new_path);
                    }
                }
                //opcion 2
                if (strcmp(opcion, "2") == 0){
                    if (data.mayor < 40000){
                        char new_path[256];
                	sprintf(new_path, "%s/%s", folder1, entry->d_name);
                	rename(entry->d_name, new_path);
                    }
                    else if (data.mayor > 80000){
                        char new_path[256];
                	sprintf(new_path, "%s/%s", folder3, entry->d_name);
                	rename(entry->d_name, new_path);
                    }
                    else{
                        char new_path[256];
                	sprintf(new_path, "%s/%s", folder2, entry->d_name);
                	rename(entry->d_name, new_path);
                    }
                }
            }
        }
    }
    closedir(dir);
    // creacion de variables para entregar estadisticas
    DIR *dir0;
    struct dirent *ent;
    char *dir_path1 = "./"; 
    char dir_path[1024];
    sprintf(dir_path, "%s/%s", dir_path1, opt);
    struct stat filestat;
    int cont_40 = 0;
    int cont_40_80 = 0;
    int cont_80 = 0;
    if ((dir0 = opendir(dir_path)) != NULL) { 
        while ((ent = readdir(dir0)) != NULL) { // hasta que no queden .txt
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) { // hay archivos ?
                char path[1024];
                // Concatenamos la ruta de la carpeta con el nombre del directorio
                sprintf(path, "%s/%s", dir_path, ent->d_name);
                if (stat(path, &filestat) >= 0 && S_ISDIR(filestat.st_mode)) {
                    printf("\nCantidad de juegos de %s :\n", ent->d_name);
                    // estadisticas por categoria
                    if (strcmp(opcion, "1") == 0){
                        cont_40 += contador(ent->d_name,"Menor_a_40000", "menor a 40000", "cantidad de jugadores actuales", opt);
                        cont_40_80 += contador(ent->d_name,"Entre_40000_y_80000", "entre 40000 y 80000", "cantidad de jugadores actuales", opt);
                        cont_80 += contador(ent->d_name,"Mayor_a_80000", "mayor a 80000", "cantidad de jugadores actuales", opt);
                    }
                    else if (strcmp(opcion, "2") == 0){
                        cont_40 += contador(ent->d_name,"Menor_a_40000", "menor a 40000", "mayor cantidad de jugadores", opt);
                        cont_40_80 += contador(ent->d_name,"Entre_40000_y_80000", "entre 40000 y 80000", "mayor cantidad de jugadores", opt);
                        cont_80 += contador(ent->d_name,"Mayor_a_80000", "mayor a 80000", "mayor cantidad de jugadores", opt);
                    }                    
                }
            }
        }
        closedir(dir0);
    } else {
        printf("No se pudo abrir el directorio %s\n", dir_path);
        return -1;
    }
    // estadisticas totales
    printf("\nDel total de juegos:\n");
    if (strcmp(opcion, "1") == 0){
        printf("    Existen %d con una cantidad de jugadores actuales menor a 40000\n", cont_40);
        printf("    Existen %d con una cantidad de jugadores actuales entre 40000 y 80000\n", cont_40_80);
        printf("    Existen %d con una cantidad de jugadores actuales mayor a 80000\n", cont_80);
    }
    else if (strcmp(opcion, "2") == 0){
        printf("    Existen %d con una mayor cantidad de jugadores menor a 40000\n", cont_40);
        printf("    Existen %d con una mayor cantidad de jugadores entre 40000 y 80000\n", cont_40_80);
        printf("    Existen %d con una mayor cantidad de jugadores mayor a 80000\n", cont_80);
    }
    return 0;
}