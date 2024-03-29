// ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1 - 201920
// EL PROYECTO DEBE SER DESARROLLADO EN GRUPOS DE 3 PERSONAS
//
// DESARROLLADO POR:
// Maria Gabriela Torres - 201820251 - mg.torres
// Camila Parra - 201819464 - mc.parrad
// Nicolás Londoño  - 201821364 - n.londonoc

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ====================================================
// ESTRUCTURAS
// ====================================================

// Definición de la estructura en donde se almacenará la información a calcular el CRC
typedef struct informacion
{
    int longitudContenido;    // Longitud de la información leída del archivo
    unsigned char *contenido; // Apuntador al contenido de la información leída del archivo, sobre la cual se calculará el CRC
} Informacion;

// ====================================================
// PROTOTIPOS/DEFINICIONES DE FUNCIONES
// ====================================================

void calcularCRC(Informacion *, int, unsigned char);
unsigned char calcularByte(unsigned char *);
int cargarInfo(Informacion *, char *);

// ====================================================
// FUNCIONES
// ====================================================

/**
 * Calcula el CRC de la información almacenada en la estructura del archivo leído. Este se calcula
 * sobre el apuntador de contenido (datos->contenido) de la estructura del parámetro y 
 * DEBE dejar el byte con el CRC calculado en el último byte que ya se adicionó al final de dicho apuntador.
 * PARÁMETROS:
 * "datos" El apuntador a la estructura que almacena todo lo necesario para calcular el CRC
 * "lonDiv" El número de bits del divisor de entrada ingresado al programa
 * "divisor" El divisor a utilizar ya en su forma de caracter correspondiente a la entrada de unos y ceros
 **/
void calcularCRC(Informacion *datos, int lonDiv, unsigned char divisor)
{
    unsigned char a = divisor;
    unsigned char b = 0;

    for (int i = 0; i < datos->longitudContenido; i++)
    {
        unsigned char c = 128;
        while (a != 0)
        {
            if (datos->contenido[i] & c)
            {
                datos->contenido[i] = datos->contenido[i] ^ a;
                datos->contenido[i + 1] = datos->contenido[i + 1] ^ b;
            }
            char carry = a << 7;
            b = b >> 1;
            b = b | carry;
            a = a >> 1;
            c = c >> 1;
        }
        a = b;
        b = 0;
    }
}

/**
 * Calcula el apuntador con la cadena de caracteres correspondiente basado en la entrada de ceros y unos del usuario
 * PARÃMETROS:
 * "entrada" La entrada de ceros y unos del usuario
 * RETORNO: 
 * El char con el contenido respectivo de los bits de la entrada de ceros y unos del usuario
 **/
unsigned char calcularByte(unsigned char *entrada)
{

    /*Se calcula la longitud de la cadena  */
    int tam = strlen(entrada);
    /*//se guarda el tamaño del unsigned char dado por parámetro para saber cuantos ceros se le deben agregar con el fin de completar el byte.*/
    char bits[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};

    /* Se agregan los ceros adicionales al unsigned char para tener el dato como un byte*/
    for (char i = 0; i < tam; i++)
    {
        if (entrada[i] == '1')
            bits[i] = '1';
    }

    unsigned char byte = 0;

    for (int i = 0; i < 8; i++) /*//en este for se revisa cada carácter del unsigned char entrada para ir convirtiendo el número de binario a decimal*/
    {
        unsigned char potencia = pow(2, (7 - i));
        unsigned char bit = (bits[i] - '0') * potencia; /*//se saca la potencia de 2 por la que se debe multiplicar el 1 o el 0 del número binario de entrada*/
        byte = byte + bit;                              /*se suman los valores de los 1 o 0 multiplicados por las potencias de 2 para obtener el número en decimal*/
    }

    return byte;
}

/**
 * Carga la información ingresada del archivo dentro de la estructura para manejarla en un mismo lugar
 * ESTA FUNCIÓN NO SE DEBE MODIFICAR
 * PARÃMETROS:
 * "datos" El apuntador a la estructura que almacena todo lo necesario para calcular el CRC
 * "rutaArchivo" El apuntador a la cadena de texto con el nombre/ruta del archivo a leer
 * RETORNO: 
 * La longitud en bytes de la información leída del archivo que se carga en la estructura
 **/
int cargarInfo(Informacion *datos, char *rutaArchivo)
{

    // Declaración de todas las variables necesarias
    FILE *file;
    int n;

    // Lectura del archivo
    file = fopen(rutaArchivo, "rb");
    if (!file)
    {
        printf("\nNo se pudo abrir el archivo para leer: %s\n\n", rutaArchivo);
        system("pause");
        exit(EXIT_FAILURE);
    }
    //Verificación de la longitud en bytes del contenido
    fseek(file, 0, SEEK_END);
    n = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Se reserva el espacio necesario para el contenido donde se calculará el CRC
    // incluyendo el byte extra donde este quedará.
    datos->contenido = (unsigned char *)calloc(n + 1, sizeof(unsigned char));
    fread((datos->contenido), sizeof(unsigned char), n, file);
    datos->contenido[n] = 0;

    fclose(file);

    return n;
}

// ====================================================
// MAIN
// ====================================================

/**
 * Función principal de la aplicación
 * ESTA FUNCIÓN NO SE DEBE MODIFICAR
 * PARÃMETROS:
 * "argc" El número de argumentos de comando ingresado. Siempre es 1 o más.
 * "argv" El apuntador a las cadenas de texto con las diferentes entradas del usuario. La primera posición (0) siempre es la ruta del ejecutable.
 * RETORNO: 
 * Entero que indica si la ejecución se completo correctamente o con alguna anomalía
 **/
int main(int argc, char *argv[])
{
    // Declaración de todas las variables necesari<as antes que cualquier llamado a procedimientos
    int lonDiv, i;
    unsigned char divisor;
    Informacion *info;

    // Mensaje inicial
    printf("\nPROYECTO 1 INFRATEC - 201920\n\n");

    // Se verifica que se ingrese un número correcto de argumentos para el programa
    if (argc != 3)
    {
        printf("Se ingreso un numero incorrecto de argumentos (%d) o se ingresaron incorrectamente :(\n", argc);
        system("pause");
        return EXIT_FAILURE;
    }
    printf("Se ingresaron correctamente los argumentos (ruta '%s' y  divisor '%s') ! :D\n", argv[1], argv[2]);

    // Se verifica que el divisor tenga la longitud adecuada
    lonDiv = strlen(argv[2]);
    if (lonDiv < 3 || lonDiv > 8)
    {
        printf("La longitud del divisor para realizar el calculo es incorrecta (%d) :(\n", lonDiv);
        system("pause");
        return EXIT_FAILURE;
    }
    printf("El divisor tiene una longitud adecuada (%d) ! :D\n", lonDiv);

    // Reserva memoria para la estructura que almacenará la información y para el contenido
    info = (Informacion *)malloc(sizeof(Informacion));

    // Se carga la información dentro de la estructura según la entrada
    info->longitudContenido = cargarInfo(info, argv[1]);
    divisor = calcularByte(argv[2]);

    // Se calcula el CRC y se muestra en consola
    calcularCRC(info, lonDiv, divisor);
    printf("El CRC calculado en hexa es:\n\n");
    printf("%02x", info->contenido[info->longitudContenido]);
    printf("\n\n");

    system("pause");
    return EXIT_SUCCESS;
}