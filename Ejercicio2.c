#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex; //Declaro un mutex para poder sincronizar los hilos

/* Variables globales que me van a ayudar a mantener el control */
int global = 1;         //esta variable me ayudará a conocer a que vector debo entrar
int iterador = 0;       //esta variable me ayudará a iterar por los vectores

typedef struct dato     //defino una estructura con los atributos que necesitaré para este ejercicio
{
    int tamanio;                //este atributo guardará el tamaño de los vectores, así no me iré de rango
    float* vector1;             //puntero al primer vector
    float* vector2;             //puntero al segundo vector
    float* vectorResultado;     //puntero al vector que guardará el resultado
    int valoresAOperar;         //esta variable es especial. Guardará la cantidad de valores que va a operar cada hilo. Su valor será diferente dependiendo de cuantos hilos ingrese el usuario
} vectores;             //este es el nombre que usaré para llamar a mi estructura

/*
    DATOS QUE SERÁN DE VITAL IMPORTANCIA PARA TRABAJAR CON HILOS EN C
    Al momento de usar un pthread_create para crear un hilo, debo pasarle 4 parámetros. Uno de esos parámetros es una función.
    La función que le pase por parámetro es la función que va a ejecutar el hilo al momento de ser creado.
    Esa función recibe un parámetro de tipo void* y retorna, de la misma manera un void* 
    La función solamente puede recibir un único parámetro. Si quiero mandarle más de uno, tendré que crear una estructura que contenga dichos parámetros
*/

//Esta es la función que ejecutarán los hilos al crearse
void* sumarVectores(void* args){
    vectores* datosHilo = (vectores*) args; //Tengo que guardar los parámetros que recibí, haciendo un casteo a la estructura
    int v= 0;                               //Voy a usar v para asegurarme de no operar más valores de los que correspondían
    while (v < datosHilo->valoresAOperar)   //Mientras v sea menor al número de valores a operar, continuo iterando
    {
        pthread_mutex_lock(&mutex);                                                     //Esta función es el equivalenta a un P(Mutex)
        if (global %2 != 0)                                                             //Esto if me dice si debo tomar el vector 1    
        {
            datosHilo->vectorResultado[iterador] += datosHilo->vector1[iterador];       //Sumo el vector 1 al vector resultado
        }
        if (global %2 == 0)                                                             //Este if me dice si debo tomar el vector 2
        {
            datosHilo->vectorResultado[iterador] += datosHilo->vector2[iterador];       //Sumo el vector 2 al vector resultado
            iterador++;                                                                 //Incremento el iterador
            if (iterador == datosHilo->tamanio)                                         //Me aseguro de que el iterador no se vaya de rango
            {
                iterador = 0;
            }
        }
        global++;                                                                       //Incremento la variable global
        pthread_mutex_unlock(&mutex);                                                   //Esta función es el equivalente a un V(Mutex)
        usleep(1000000);                                                                //Duermo el hilo durante 1 segundo
        v++;                                                                            //Incremento v para ver si puede seguir operando valores
    }
    
} 

/* Función main o hilo principal */
int main(int argc, char* argv[]){
    char *ayuda="-Help"; //Uso esta cadena para ver si el usuario quiere ver la ayuda
    if (argc == 2 && strcmp(argv[1],ayuda) == 0) //Muestro la ayuda al usuario
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
    if (argc == 2 && strcmp(argv[1],ayuda) != 0) //Este if es por si se equivocó al invocar la ayuda
    {
        printf("\nError en el pasaje de parámetros");
        printf("\nEscriba './Ejercicio2 -Help' (sin las comillas) para recibir ayuda\n");
        exit(4);
    }

    if (argc != 3) //Verifico que me haya pasado la cantidad de parámetros correcta. Nombre del script, nombre del archivo y cantidad de hilos son los parámetros a usar
    {
        printf("\nError en el pasaje de parámetros");
        printf("\nEscriba './Ejercicio2 -Help' (sin las comillas) para recibir ayuda\n");
        exit(5);
    }
    

    if (strchr(argv[2],'.') || strchr(argv[2],',')) //Con esto puedo validar que el número sea entero
        {
            printf("\nError: el parámetro cantidad de hilos debe ser entero\n");
            exit(5);
        }

    int numeroDeHilos = atoi(argv[2]); //extraigo de los parámetros la cantidad de hilos que quiere el usuario
    char *nombreArchivo = argv[1];     //extraigo de los parámetros el nombre del archivo que pasó el usuario
    
    if (numeroDeHilos <= 0) //verifico que la cantidad de hilos no sea cero
    {
        printf("\nError: el parámetro cantidad de hilos debe mayor a cero\n");
            exit(7);
    }
    
    int cantValoresPorHilo[numeroDeHilos]; //Creo este vector para saber cuantos valores va a operar cada hilo
    for (int i = 0; i < numeroDeHilos; i++)
    {
        cantValoresPorHilo[i]= 0;
    }

        //Voy a leer el archivo (2 veces) 
        //La primera para saber cuantos valores tendré 
        //La segunda para bajar esos valores al programa
        int cantidad_lineas_archivo = 0, i = 0; //Con estas variables voy a controlar la lectura y obtener las lineas del archivo
        char palabra[100];                      //Con esta variable voy a leer cada lìnea del archivo
        char delimitador[] = ",";               //Con esta variable voy a separar los valores. Recordemos que el vector se almacena así "129.43,514.32"
        FILE *archivo;                          //Con esta variable voy a abrir los archivos
        archivo = fopen(nombreArchivo,"r");     //Abro el archivo en modo lectura
        if (archivo == NULL)                    //Valido que el archivo existe y que lo pueda leer
        {
            printf("\nError al intentar leer el archivo que usted pasó por parámetro. Es probable que no exista o que no se pueda abrir en modo lectura\n");
            exit(1);
        }
        while (feof(archivo) == 0)              //Recorro hasta EndOfFile para contar la cantidad de lineas
        {
            fgets(palabra,100,archivo);
            cantidad_lineas_archivo++;
        }
        fclose(archivo);                        //Cierro el archivo. AVERIGUAR como volver al principio con la funcion FSEEK 
        
        float vectorSuma[cantidad_lineas_archivo];  //Creo los vectores con tantas posiciones como líneas haya encontrado
        float vector1[cantidad_lineas_archivo];
        float vector2[cantidad_lineas_archivo];

        int total = cantidad_lineas_archivo*2;      //Esta variable me dirá el total de valores que habrá 

        FILE *lectura;                          //Vuelvo a abrir el archivo para leerlo y bajar las variables a memoria. Averiguar como funciona FSEEK
        lectura = fopen("vector.txt","r");
        if (lectura == NULL)
            exit(2);
        while (feof(lectura) == 0 && i < cantidad_lineas_archivo)
        {
            fgets(palabra,100,lectura);                     //Con fgets obtengo el contenido de la linea
            char *token = strtok(palabra,delimitador);      //Con strktok obtengo toda la cadena hasta la primer coma "129.43,514.32" queda "514.32"
            vector1[i]=atof(token);                         //Guardo el numero en el primer vector
            token= strtok(NULL,delimitador);                //Repito lo mismo hasta que la cadena sea NULL 
            vector2[i]=atof(token);                         //Guardo el numero en el segundo vector    
            i++;
        }
        fclose(lectura);                                    //Cierro el archivo definitivamente
        
        i=0;
        while (total >= 1)                                  //Obtengo la cantidad de valores que operará cada hilo
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
        for (int i = 0; i < numeroDeHilos; i++)         //Informo por pantalla la cantidad de valores que operará cada hilo.
        {
            if (cantValoresPorHilo[i] > 0)
            {
                printf("El hilo %d va a operar %d valores\n", i+1, cantValoresPorHilo[i]);
            }
            else                                        //En caso de que no haya valores suficientes para todos los hilos, informo por pantalla
            {
                printf("No hay valores suficientes para los otros %d hilos\n", numeroDeHilos-i);
                numeroDeHilos=i;                        
                break;
            }
        }

        for (int i = 0; i < cantidad_lineas_archivo; i++) //Inicializo en 0 el vector donde almacenaré la suma. Si esto no se hace, la operación arrojará basura
        {
            vectorSuma[i]=0;
        }

        /*printf("\n");
        printf("Resultado REAL de la operación: \n");
        for (int i = 0; i < cantidad_lineas_archivo; i++)
        {
            printf("%.2f + %.2f = %.2f\n", vector1[i], vector2[i], (vector1[i]+vector2[i]));
        }*/

        //COMIENZA LA PARTE DIFICIL
        pthread_t hilo; //Declaro el hilo
        vectores datosHilo; //Declaro una estructura para que los hilos la usen

        datosHilo.vector1 = vector1;                    //guardo el vector 1 en la estructura
        datosHilo.vector2 = vector2;                    //guardo el vector 2 en la estructura
        datosHilo.vectorResultado = vectorSuma;         //guardo el vector resultado en la estructura
        datosHilo.tamanio = cantidad_lineas_archivo;    //guardo el tamaño de cada vector en la estructura

        for (int k = 0; k < numeroDeHilos ; k++)        //Este for lo usaré para crear hilos
        {
            datosHilo.valoresAOperar = cantValoresPorHilo[k];               //guardo la cantidad de valores a operar en la estructura. Recordar que pueden variar para cada hilo
            pthread_create(&hilo,NULL,sumarVectores,(void*)&datosHilo);     //creo el hilo. Le paso por parámetro: 
                                                                            //    1) El nombre del hilo
                                                                            //    2) NULL 
                                                                            //    3) La función que va a ejecutar al crearse
                                                                            //    4) La estructura que cree como parámetro
            pthread_join(hilo,NULL);        //Con el pthread_join, hago que el main espere a que terminen los hilos antes de seguir
        }
        
        FILE* salida;   //voy a crear un archivo para guardar el resultado de la suma
        salida=fopen(strcat(nombreArchivo,"salida.txt"),"w"); //abro un archivo en modo escritura. Le concateno un "salida.txt" al archivo de entrada
        if (salida == NULL)  //verifico que no haya tenido problemas con la creación del archivo
        {
            printf("\nSe ha producido un error al abrir un archivo para guardar la suma\n");
        }
        for (int i = 0; i < cantidad_lineas_archivo; i++)       //con este for guardo cada número de la suma
        {
            fprintf(salida,"%.2f\n",datosHilo.vectorResultado[i]); 
        }
        fclose(salida); //cierro el archivo de salida

    return 0;
}
