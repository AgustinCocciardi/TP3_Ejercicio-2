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

    if (strchr(argv[2],'.') || strchr(argv[2],',') || strchr(argv[2],'0') || strchr(argv[2],'-'))
        {
            printf("\nError: el parámetro cantidad de hilos debe ser entero y mayor que 0\n");
            exit(5);
        }

    int numeroDeHilos = atoi(argv[2]);
    char *nombreArchivo = argv[1];
    
    int cantValoresPorHilo[numeroDeHilos];
    for (int i = 0; i < numeroDeHilos; i++)
    {
        cantValoresPorHilo[i]= 0;
    }

        int cantidad_lineas_archivo = 0, i = 0;
        char palabra[100];
        char delimitador[] = ",";
        FILE *archivo; 
        archivo = fopen(nombreArchivo,"r");
        if (archivo == NULL)
        {
            printf("\nError al intentar leer el archivo que usted pasó por parámetro. Es probable que no exista o que no se pueda abrir en modo lectura\n");
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

        int total = cantidad_lineas_archivo*2;

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

        if (numeroDeHilos > (cantidad_lineas_archivo*2))
        {
            printf("\nEl archivo tiene un total de %d elementos y usted quiere trabajar con %d hilos\nHay más hilos que elementos\n", cantidad_lineas_archivo*2, numeroDeHilos);
            exit(5);
        }
        
        i=0;
        while (total >= 1)
        {
            cantValoresPorHilo[i]++;
            total--;
            i++;
            if (i == numeroDeHilos)
            {
                i=0;
            }
        }
        
        printf("\n");
        for (int i = 0; i < numeroDeHilos; i++)
        {
            printf("El hilo %d va a operar %d valores\n", i+1, cantValoresPorHilo[i]);
        }
        
        printf("\n");
        printf("Resultado de la operación: \n");
        for (int i = 0; i < cantidad_lineas_archivo; i++)
        {
            printf("%.2f + %.2f = %.2f\n", vector1[i], vector2[i], (vector1[i]+vector2[i]));
        }



    return 0;
}