
def cargar_configuracion(archivo_configuracion):
    with open(archivo_configuracion, 'r') as f:
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
    estados, alfabeto, estado_inicial, estados_aceptacion, transiciones = cargar_configuracion('Configuracion.txt')
    
    with open('Cadenas.txt', 'r') as f:
        cadenas = [line.strip() for line in f if line.strip()]
    
    for cadena in cadenas:
        resultado = "ACEPTA" if evaluar_cadena(cadena, estado_inicial, estados_aceptacion, transiciones) else "NO ACEPTA"
        print(f"{cadena}: {resultado}")


if __name__ == "__main__":
    main()

