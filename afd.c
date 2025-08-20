#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 256
#define MAX_ESTADOS 100
#define MAX_TRANSICIONES 200

typedef struct {
    char origen[20];
    char simbolo[5];
    char destino[20];
} Transicion;

int main() {
    FILE *fconf = fopen("Conf.txt", "r");
    FILE *fcadenas = fopen("Cadenas.txt", "r");
    if (!fconf || !fcadenas) {
        printf("Error al abrir archivos.\n");
        return 1;
    }

    char linea[MAX_LINEA];
    char estados[MAX_LINEA], alfabeto[MAX_LINEA], estado_inicial[20], estados_aceptacion[MAX_LINEA];
    Transicion transiciones[MAX_TRANSICIONES];
    int num_transiciones = 0;

    // Leer estados
    fgets(estados, MAX_LINEA, fconf);
    strtok(estados, "\n");

    // Leer alfabeto
    fgets(alfabeto, MAX_LINEA, fconf);
    strtok(alfabeto, "\n");

    // Leer estado inicial
    fgets(estado_inicial, MAX_LINEA, fconf);
    strtok(estado_inicial, "\n");

    // Leer estados de aceptación
    fgets(estados_aceptacion, MAX_LINEA, fconf);
    strtok(estados_aceptacion, "\n");

    // Leer transiciones
    while (fgets(linea, MAX_LINEA, fconf)) {
        if (strlen(linea) < 3) continue;
        strtok(linea, "\n");
        sscanf(linea, "%[^,],%[^,],%s", transiciones[num_transiciones].origen, transiciones[num_transiciones].simbolo, transiciones[num_transiciones].destino);
        num_transiciones++;
    }
    fclose(fconf);

    // Procesar cadenas
    while (fgets(linea, MAX_LINEA, fcadenas)) {
        strtok(linea, "\n");
        char cadena[MAX_LINEA];
        strcpy(cadena, linea);

        char estado_actual[20];
        strcpy(estado_actual, estado_inicial);

        int i, j;
        for (i = 0; i < strlen(cadena); i++) {
            char simbolo[2] = { cadena[i], '\0' };
            int encontrado = 0;

            for (j = 0; j < num_transiciones; j++) {
                if (strcmp(transiciones[j].origen, estado_actual) == 0 && strcmp(transiciones[j].simbolo, simbolo) == 0) {
                    strcpy(estado_actual, transiciones[j].destino);
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                strcpy(estado_actual, "ERROR");
                break;
            }
        }

        // Comprobar si es estado de aceptación
        int aceptada = 0;
        char copia[MAX_LINEA];
        strcpy(copia, estados_aceptacion);
        char *token = strtok(copia, ",");
        while (token != NULL) {
            if (strcmp(token, estado_actual) == 0) {
                aceptada = 1;
                break;
            }
            token = strtok(NULL, ",");
        }

        printf("%s: %s\n", cadena, aceptada ? "ACEPTA" : "NO ACEPTA");
    }

    fclose(fcadenas);
    return 0;
}

