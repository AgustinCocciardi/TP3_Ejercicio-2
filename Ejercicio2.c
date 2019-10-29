#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int main(int argc, char* argv[]){
    char *ayuda="-Help";
    if (argc == 2 && strcmp(argv[1],ayuda) == 0)
    {
        printf("\nEste programa tiene la funcionalidad de permitirle al usuario que lo invoque dividir la operación de suma de");
        printf("\ndos vectores (contenidos en un archivo) mediante el uso de hilos, cuya cantidad se indica por parámetro");
        printf("\nFormas de llamar al programa:");
        printf("\n./Ejercicio2 RutaDelArchivo NúmeroDeHilos");
        printf("\n./Ejemplo de ejecución: ");
        printf("\n./Ejercicio2 /home/user/Desktop/vector.txt 5 ");
        printf("\n");
        exit(3);
    }
    if (argc == 2 && strcmp(argv[1],ayuda) != 0)
    {
        printf("\nError en el pasaje de parámetros");
        printf("\nEscriba './Ejercicio2 -Help' (sin las comillas) para recibir ayuda\n");
        exit(4);
    }

    if (strchr(argv[2],'.') || strchr(argv[2],','))
        {
            printf("Error: el parámetro cantidad de hilos debe ser entero\n");
            exit(5);
        }

    int numeroDeHilos = atoi(argv[2]);
    char *nombreArchivo = argv[1];
    
        
        

        int cantidad_lineas_archivo = 0, i = 0;
        char palabra[100];
        char delimitador[] = ",";
        FILE *archivo; 
        archivo = fopen(nombreArchivo,"r");
        if (archivo == NULL)
        {
            printf("Error al intentar leer el archivo que usted pasó por parámetro. Es probable que no exista o que no se pueda abrir en modo lectura\n");
            exit(1);
        }
        while (feof(archivo) == 0)
        {
            fgets(palabra,100,archivo);
            cantidad_lineas_archivo++;
        }
        fclose(archivo);
        
        float vectorSuma[cantidad_lineas_archivo];
        float vector1[cantidad_lineas_archivo];
        float vector2[cantidad_lineas_archivo];

        FILE *lectura;
        lectura = fopen("vector.txt","r");
        if (lectura == NULL)
            exit(2);
        while (feof(lectura) == 0 && i < cantidad_lineas_archivo)
        {
            fgets(palabra,100,lectura);
            char *token = strtok(palabra,delimitador);
            vector1[i]=atof(token);
            token= strtok(NULL,delimitador); 
            vector2[i]=atof(token);
            i++;
        }
        fclose(lectura);

        for (int i = 0; i < cantidad_lineas_archivo; i++)
        {
            printf("%.2f + %.2f = %.2f\n", vector1[i], vector2[i], (vector1[i]+vector2[i]));
        }



    return 0;
}