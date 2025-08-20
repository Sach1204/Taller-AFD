# Taller-AFD
# Simulador de AFD (Autómata Finito Determinista)

Este proyecto implementa un **Autómata Finito Determinista (AFD)** en dos lenguajes: **Python** y **C**.  
El AFD lee su configuración desde un archivo `Conf.txt` y evalúa cadenas (números en binario) desde un archivo `Cadenas.txt`.

## Codigo en Python ##
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
## 4 Respuesta mostrada en terminal
<img width="1068" height="326" alt="image" src="https://github.com/user-attachments/assets/d5325059-c292-4587-823c-5052c3544b40" />
