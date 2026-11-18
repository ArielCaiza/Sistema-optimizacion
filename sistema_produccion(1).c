/*
 Sistema de optimizacion de produccion - Programacion 1
 Restricciones: no utiliza estructuras (struct) ni archivos.
 Manejo de datos mediante arreglos paralelos y punteros.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int leerEnteroPositivo(const char mensaje[]) {
    int valor, resultado;
    do {
        printf("%s", mensaje);
        resultado = scanf("%d", &valor);
        limpiarBuffer();
        if (resultado != 1 || valor <= 0) {
            printf("Error: ingrese un numero entero mayor que cero.\n");
        }
    } while (resultado != 1 || valor <= 0);
    return valor;
}

float leerRealPositivo(const char mensaje[]) {
    float valor;
    int resultado;
    do {
        printf("%s", mensaje);
        resultado = scanf("%f", &valor);
        limpiarBuffer();
        if (resultado != 1 || valor <= 0) {
            printf("Error: ingrese un valor mayor que cero.\n");
        }
    } while (resultado != 1 || valor <= 0);
    return valor;
}

void leerNombre(char nombre[]) {
    do {
        printf("Nombre del producto: ");
        if (fgets(nombre, MAX_NOMBRE, stdin) == NULL) {
            nombre[0] = '\0';
        } else {
            nombre[strcspn(nombre, "\n")] = '\0';
        }
        if (strlen(nombre) == 0) {
            printf("El nombre no puede estar vacio.\n");
        }
    } while (strlen(nombre) == 0);
}

/* Busqueda secuencial propia: devuelve la posicion o -1 si no existe. */
int buscarProducto(char nombres[][MAX_NOMBRE], int cantidad, const char buscado[]) {
    int i;
    for (i = 0; i < cantidad; i++) {
        if (strcmp(nombres[i], buscado) == 0) {
            return i;
        }
    }
    return -1;
}

void ingresarProducto(char nombres[][MAX_NOMBRE], int demanda[],
                      float tiempo[], float recursos[], int *cantidad) {
    char nuevoNombre[MAX_NOMBRE];

    if (*cantidad >= MAX_PRODUCTOS) {
        printf("\nLa lista ya tiene el maximo de %d productos.\n", MAX_PRODUCTOS);
        return;
    }

    printf("\n--- INGRESO DE PRODUCTO ---\n");
    leerNombre(nuevoNombre);

    if (buscarProducto(nombres, *cantidad, nuevoNombre) != -1) {
        printf("Ya existe un producto con ese nombre.\n");
        return;
    }

    strcpy(nombres[*cantidad], nuevoNombre);
    demanda[*cantidad] = leerEnteroPositivo("Cantidad demandada (unidades): ");
    tiempo[*cantidad] = leerRealPositivo("Tiempo por unidad (horas): ");
    recursos[*cantidad] = leerRealPositivo("Recursos por unidad (unidades de recurso): ");
    (*cantidad)++;

    printf("Producto ingresado correctamente.\n");
}

void mostrarProductos(char nombres[][MAX_NOMBRE], int demanda[],
                      float tiempo[], float recursos[], int cantidad) {
    int i;
    if (cantidad == 0) {
        printf("\nNo hay productos registrados.\n");
        return;
    }

    printf("\n%-4s %-22s %-12s %-15s %-15s\n",
           "No.", "Producto", "Demanda", "Horas/unidad", "Recurso/unidad");
    for (i = 0; i < cantidad; i++) {
        printf("%-4d %-22s %-12d %-15.2f %-15.2f\n",
               i + 1, nombres[i], demanda[i], tiempo[i], recursos[i]);
    }
}

void editarProducto(char nombres[][MAX_NOMBRE], int demanda[],
                    float tiempo[], float recursos[], int cantidad) {
    char buscado[MAX_NOMBRE];
    int posicion;

    if (cantidad == 0) {
        printf("\nNo existen productos para editar.\n");
        return;
    }

    printf("\n--- EDICION DE PRODUCTO ---\n");
    leerNombre(buscado);
    posicion = buscarProducto(nombres, cantidad, buscado);

    if (posicion == -1) {
        printf("No se encontro el producto.\n");
        return;
    }

    printf("Ingrese los nuevos datos del producto.\n");
    printf("Nuevo nombre (escriba el actual si no desea cambiarlo): ");
    if (fgets(buscado, MAX_NOMBRE, stdin) != NULL) {
        buscado[strcspn(buscado, "\n")] = '\0';
        if (strlen(buscado) > 0) {
            int repetido = buscarProducto(nombres, cantidad, buscado);
            if (repetido == -1 || repetido == posicion) {
                strcpy(nombres[posicion], buscado);
            } else {
                printf("Ese nombre ya pertenece a otro producto. Se conserva el anterior.\n");
            }
        }
    }

    demanda[posicion] = leerEnteroPositivo("Nueva cantidad demandada: ");
    tiempo[posicion] = leerRealPositivo("Nuevo tiempo por unidad (horas): ");
    recursos[posicion] = leerRealPositivo("Nuevos recursos por unidad: ");
    printf("Producto actualizado correctamente.\n");
}

void eliminarProducto(char nombres[][MAX_NOMBRE], int demanda[],
                      float tiempo[], float recursos[], int *cantidad) {
    char buscado[MAX_NOMBRE];
    int posicion, i;

    if (*cantidad == 0) {
        printf("\nNo existen productos para eliminar.\n");
        return;
    }

    printf("\n--- ELIMINACION DE PRODUCTO ---\n");
    leerNombre(buscado);
    posicion = buscarProducto(nombres, *cantidad, buscado);

    if (posicion == -1) {
        printf("No se encontro el producto.\n");
        return;
    }

    for (i = posicion; i < *cantidad - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        demanda[i] = demanda[i + 1];
        tiempo[i] = tiempo[i + 1];
        recursos[i] = recursos[i + 1];
    }

    (*cantidad)--;
    printf("Producto eliminado correctamente.\n");
}

void calcularProduccion(int demanda[], float tiempo[], float recursos[], int cantidad) {
    int i;
    float tiempoTotal = 0.0f, recursosTotales = 0.0f;
    float tiempoDisponible, recursosDisponibles;

    if (cantidad == 0) {
        printf("\nNo hay productos registrados para calcular.\n");
        return;
    }

    printf("\n--- ANALISIS DE PRODUCCION ---\n");
    for (i = 0; i < cantidad; i++) {
        tiempoTotal += demanda[i] * tiempo[i];
        recursosTotales += demanda[i] * recursos[i];
    }

    printf("Tiempo total requerido: %.2f horas\n", tiempoTotal);
    printf("Recursos totales requeridos: %.2f unidades\n", recursosTotales);

    tiempoDisponible = leerRealPositivo("Tiempo de produccion disponible (horas): ");
    recursosDisponibles = leerRealPositivo("Cantidad de recursos disponibles: ");

    if (tiempoTotal <= tiempoDisponible && recursosTotales <= recursosDisponibles) {
        printf("\nRESULTADO: La fabrica SI puede cumplir la demanda.\n");
    } else {
        printf("\nRESULTADO: La fabrica NO puede cumplir toda la demanda.\n");
        if (tiempoTotal > tiempoDisponible) {
            printf("- Falta tiempo de produccion: %.2f horas.\n",
                   tiempoTotal - tiempoDisponible);
        }
        if (recursosTotales > recursosDisponibles) {
            printf("- Faltan recursos: %.2f unidades.\n",
                   recursosTotales - recursosDisponibles);
        }
    }
}

int main(void) {
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE];
    int demanda[MAX_PRODUCTOS];
    float tiempo[MAX_PRODUCTOS], recursos[MAX_PRODUCTOS];
    int cantidad = 0, opcion;

    do {
        printf("\n========================================\n");
        printf(" SISTEMA DE OPTIMIZACION DE PRODUCCION\n");
        printf("========================================\n");
        printf("1. Ingresar producto\n");
        printf("2. Mostrar productos\n");
        printf("3. Editar producto\n");
        printf("4. Eliminar producto\n");
        printf("5. Calcular produccion y verificar capacidad\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();
            opcion = -1;
        } else {
            limpiarBuffer();
        }

        switch (opcion) {
            case 1:
                ingresarProducto(nombres, demanda, tiempo, recursos, &cantidad);
                break;
            case 2:
                mostrarProductos(nombres, demanda, tiempo, recursos, cantidad);
                break;
            case 3:
                editarProducto(nombres, demanda, tiempo, recursos, cantidad);
                break;
            case 4:
                eliminarProducto(nombres, demanda, tiempo, recursos, &cantidad);
                break;
            case 5:
                calcularProduccion(demanda, tiempo, recursos, cantidad);
                break;
            case 0:
                printf("\nPrograma finalizado.\n");
                break;
            default:
                printf("\nOpcion no valida. Intente nuevamente.\n");
        }
    } while (opcion != 0);

    return 0;
}
