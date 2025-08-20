# Taller-AFD
# Simulador de AFD (Autómata Finito Determinista)

Este proyecto implementa un **Autómata Finito Determinista (AFD)** en dos lenguajes: **Python** y **C**.  
El AFD lee su configuración desde un archivo `Conf.txt` y evalúa cadenas (números en binario) desde un archivo `Cadenas.txt`.

## Solucion en Python ##
```python
def cargar_configuracion(archivo_conf):
    with open(archivo_conf, 'r') as f:
        lineas = [line.strip() for line in f if line.strip() and not line.startswith("#")]

    estados = lineas[0].split(',')
    alfabeto = lineas[1].split(',')
    estado_inicial = lineas[2]
    estados_aceptacion = lineas[3].split(',')
    
    transiciones = {}
    for linea in lineas[4:]:
        origen, simbolo, destino = linea.split(',')
        transiciones[(origen, simbolo)] = destino

    return estados, alfabeto, estado_inicial, estados_aceptacion, transiciones


def evaluar_cadena(cadena, estado_inicial, estados_aceptacion, transiciones):
    estado_actual = estado_inicial
    for simbolo in cadena:
        if (estado_actual, simbolo) in transiciones:
            estado_actual = transiciones[(estado_actual, simbolo)]
        else:
            return False
    return estado_actual in estados_aceptacion


def main():
    estados, alfabeto, estado_inicial, estados_aceptacion, transiciones = cargar_configuracion('Conf.txt')
    
    with open('Cadenas.txt', 'r') as f:
        cadenas = [line.strip() for line in f if line.strip()]
    
    for cadena in cadenas:
        resultado = "ACEPTA" if evaluar_cadena(cadena, estado_inicial, estados_aceptacion, transiciones) else "NO ACEPTA"
        print(f"{cadena}: {resultado}")


if __name__ == "__main__":
    main()
```
## 1. cargar_configuracion(archivo_conf)
```txt
# Estados
q0,q1

# Alfabeto
0,1

# Estado inicial
q0

# Estados de aceptación
q0

# Transiciones
q0,0,q0
q0,1,q1
q1,0,q0
q1,1,q1

```
Objetivo: Cargar la configuración del autómata desde un archivo (En este caso, Conf.txt)


# 2. evaluar_cadena(cadena, estado_inicial, estados_aceptacion, transiciones)

Objetivo: Determinar si una cadena es aceptada por el autómata.
```python
def evaluar_cadena(cadena, estado_inicial, estados_aceptacion, transiciones):
    estado_actual = estado_inicial
    for simbolo in cadena:
        if (estado_actual, simbolo) in transiciones:
            estado_actual = transiciones[(estado_actual, simbolo)]
        else:
            return False
    return estado_actual in estados_aceptacion
```
## Cadenas de Prueba (Cadenas.txt).
```txt
0
1
10
11
101
1010
1111
1000
```
Empieza en el estado inicial.

Recorre cada símbolo de la cadena: Si existe una transición desde el estado actual con ese símbolo, cambia al estado destino.Si no existe, la cadena NO es aceptada.
Al final, la cadena es aceptada si el estado final pertenece a los estados de aceptación.
## 3. main()
Objetivo: Coordinar la ejecución del programa.
Proceso:
Llama a cargar_configuracion() para obtener la definición del autómata desde Conf.txt.
Lee el archivo Cadenas.txt, que contiene las cadenas a evaluar.
Para cada cadena:
Llama a evaluar_cadena().
Imprime ACEPTA o NO ACEPTA según el resultado.
```python
def main():
    estados, alfabeto, estado_inicial, estados_aceptacion, transiciones = cargar_configuracion('Conf.txt')
    
    with open('Cadenas.txt', 'r') as f:
        cadenas = [line.strip() for line in f if line.strip()]
    
    for cadena in cadenas:
        resultado = "ACEPTA" if evaluar_cadena(cadena, estado_inicial, estados_aceptacion, transiciones) else "NO ACEPTA"
        print(f"{cadena}: {resultado}")
```
## 4. Respuesta mostrada en terminal.

<img width="1068" height="326" alt="image" src="https://github.com/user-attachments/assets/d5325059-c292-4587-823c-5052c3544b40" />

# AFD - Autómata Finito Determinista en C

Este proyecto implementa un **Autómata Finito Determinista (AFD)** en lenguaje C.  
El programa lee una configuración desde un archivo (`Conf.txt`) y una lista de cadenas desde otro (`Cadenas.txt`).  
Después evalúa cada cadena según el autómata y muestra en pantalla si la cadena es **ACEPTADA** o **NO ACEPTA**.

---
## Codigo en C
```c
- **afd.c** → Código fuente en C del autómata.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 256
#define MAX_ITEMS 50
#define MAX_TRANSICIONES 200

typedef struct {
    char origen[50];
    char simbolo;
    char destino[50];
} Transicion;

char estados[MAX_ITEMS][50];
int num_estados = 0;

char alfabeto[MAX_ITEMS];
int num_alfabeto = 0;

char estado_inicial[50];

char estados_aceptacion[MAX_ITEMS][50];
int num_estados_aceptacion = 0;

Transicion transiciones[MAX_TRANSICIONES];
int num_transiciones = 0;

/* --- Utilidades --- */
static void rstrip(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t')) {
        s[--n] = '\0';
    }
}
static void lstrip(char *s) {
    size_t i = 0, n = strlen(s);
    while (i < n && (s[i] == ' ' || s[i] == '\t')) i++;
    if (i > 0) memmove(s, s + i, n - i + 1);
}
static void strip(char *s) {
    rstrip(s);
    lstrip(s);
}

/* Divide por separador, recortando espacios en cada token */
int split(const char *linea, const char *sep, char arr[][50]) {
    int count = 0;
    char buffer[MAX_LINE];
    strncpy(buffer, linea, sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';

    char *token = strtok(buffer, sep);
    while (token != NULL && count < MAX_ITEMS) {
        char tmp[50];
        strncpy(tmp, token, sizeof(tmp)-1);
        tmp[sizeof(tmp)-1] = '\0';
        strip(tmp);
        if (tmp[0] != '\0') {
            strncpy(arr[count++], tmp, 50);
            arr[count-1][49] = '\0';
        }
        token = strtok(NULL, sep);
    }
    return count;
}

/* Cargar configuración desde archivo */
void cargar_configuracion(const char *archivo_conf) {
    FILE *f = fopen(archivo_conf, "r");
    if (!f) {
        perror("No se pudo abrir archivo de configuración");
        exit(1);
    }

    char linea[MAX_LINE];
    int linea_num = 0;

    while (fgets(linea, sizeof(linea), f)) {
        strip(linea);
        if (linea[0] == '\0' || linea[0] == '#') continue;

        if (linea_num == 0) {
            num_estados = split(linea, ",", estados);
        } else if (linea_num == 1) {
            char arr[MAX_ITEMS][50];
            num_alfabeto = split(linea, ",", arr);
            for (int i = 0; i < num_alfabeto; i++) {
                alfabeto[i] = arr[i][0]; /* primer carácter */
            }
        } else if (linea_num == 2) {
            strncpy(estado_inicial, linea, sizeof(estado_inicial)-1);
            estado_inicial[sizeof(estado_inicial)-1] = '\0';
        } else if (linea_num == 3) {
            num_estados_aceptacion = split(linea, ",", estados_aceptacion);
        } else {
            char arr[3][50];
            int n = split(linea, ",", arr);
            if (n == 3 && num_transiciones < MAX_TRANSICIONES) {
                strncpy(transiciones[num_transiciones].origen, arr[0], 50);
                transiciones[num_transiciones].origen[49] = '\0';
                transiciones[num_transiciones].simbolo = arr[1][0];
                strncpy(transiciones[num_transiciones].destino, arr[2], 50);
                transiciones[num_transiciones].destino[49] = '\0';
                num_transiciones++;
            }
        }
        linea_num++;
    }
    fclose(f);
}

/* Buscar transición */
bool transicion(const char *estado, char simbolo, char *nuevo_estado) {
    for (int i = 0; i < num_transiciones; i++) {
        if (strcmp(transiciones[i].origen, estado) == 0 &&
            transiciones[i].simbolo == simbolo) {
            strncpy(nuevo_estado, transiciones[i].destino, 50);
            nuevo_estado[49] = '\0';
            return true;
        }
    }
    return false;
}

/* Evaluar una cadena */
bool evaluar_cadena(const char *cadena) {
    char estado_actual[50];
    strncpy(estado_actual, estado_inicial, sizeof(estado_actual)-1);
    estado_actual[sizeof(estado_actual)-1] = '\0';

    size_t len = strlen(cadena);
    for (size_t i = 0; i < len; i++) {
        char simbolo = cadena[i];
        char nuevo_estado[50];
        if (transicion(estado_actual, simbolo, nuevo_estado)) {
            strncpy(estado_actual, nuevo_estado, 50);
            estado_actual[49] = '\0';
        } else {
            return false;
        }
    }

    for (int i = 0; i < num_estados_aceptacion; i++) {
        if (strcmp(estado_actual, estados_aceptacion[i]) == 0) {
            return true;
        }
    }
    return false;
}

int main(void) {
    cargar_configuracion("Conf.txt");

    FILE *f = fopen("Cadenas.txt", "r");
    if (!f) {
        perror("No se pudo abrir archivo de cadenas");
        return 1;
    }

    char cadena[MAX_LINE];
    while (fgets(cadena, sizeof(cadena), f)) {
        strip(cadena);
        if (cadena[0] == '\0') continue;

        if (evaluar_cadena(cadena)) {
            printf("ACEPTA\n");
        } else {
            printf("NO ACEPTA\n");
        }
    }

    fclose(f);
    return 0;
}
```
- **Conf.txt** → Configuración del autómata (estados, alfabeto, transiciones, estado inicial y estados de aceptación).
```txt
# Estados
q0,q1

# Alfabeto
0,1

# Estado inicial
q0

# Estados de aceptación
q0

# Transiciones
q0,0,q0
q0,1,q1
q1,0,q0
q1,1,q1

```
- **Cadenas.txt** → Conjunto de cadenas que serán evaluadas.
```txt
0
1
10
11
101
1010
1111
1000
```
## Cómo compilar y ejecutar
2. Se compila el programa con `gcc`:
   ```bash
   gcc afd.c -o afd
