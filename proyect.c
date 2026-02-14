/*
  Proyecto FINAL: Uso de Sentencias de Control y Funciones
  Control de entrega de regalos por familia (hasta 100 familias)

  Compila en Windows:
    - MSVC (Developer Command Prompt):
        cl /W4 /EHsc proyecto_regalos.c
    - MinGW:
        gcc -Wall -Wextra -O2 -o proyecto_regalos.exe proyecto_regalos.c
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_FAMILIAS 100

typedef struct {
    char cedula[20];
    char nombre[50];
    int  num_ninos;
    int  entregados;
} Familia;

/* ---------- Utilidades de entrada ---------- */

static void trim_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

static void read_line(const char *prompt, char *out, size_t out_sz) {
    if (prompt) printf("%s", prompt);
    if (!fgets(out, (int)out_sz, stdin)) {
        /* EOF o error: dejar string vacío */
        out[0] = '\0';
        return;
    }
    trim_newline(out);

    /* Si el usuario escribió más de lo que cabe, limpiar el buffer */
    if (strlen(out) == out_sz - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
    }
}

static int parse_int(const char *s, int *out) {
    char *end = NULL;
    long v;

    if (!s) return 0;
    while (isspace((unsigned char)*s)) s++;

    if (*s == '\0') return 0;

    v = strtol(s, &end, 10);
    if (end == s) return 0;

    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;

    if (v < -2147483648L || v > 2147483647L) return 0;

    *out = (int)v;
    return 1;
}

static int read_int_range(const char *prompt, int minv, int maxv) {
    char buf[64];
    int v;

    for (;;) {
        read_line(prompt, buf, sizeof(buf));
        if (parse_int(buf, &v) && v >= minv && v <= maxv) return v;
        printf("  [!] Ingrese un numero valido (%d..%d).\n", minv, maxv);
    }
}

static int ask_yes_no(const char *prompt) {
    char buf[16];
    for (;;) {
        read_line(prompt, buf, sizeof(buf));
        if (buf[0] == '\0') continue;
        char c = (char)tolower((unsigned char)buf[0]);
        if (c == 's' || c == 'y') return 1; /* si / yes */
        if (c == 'n') return 0;
        printf("  [!] Responda con 's' (si) o 'n' (no).\n");
    }
}

/* ---------- Lógica de negocio ---------- */

static int buscar_familia_por_cedula(const Familia familias[], int total, const char *cedula) {
    for (int i = 0; i < total; i++) {
        if (strcmp(familias[i].cedula, cedula) == 0) return i;
    }
    return -1;
}

static void imprimir_menu(void) {
    printf("\n=============================================\n");
    printf("   SISTEMA DE CONTROL DE ENTREGA DE REGALOS\n");
    printf("=============================================\n");
    printf("1) Ingreso de datos del representante (familia)\n");
    printf("2) Ingreso de entrega de regalo (a un nino)\n");
    printf("3) Reporte: regalos FALTANTES por familia\n");
    printf("4) Reporte: regalos ENTREGADOS por familia\n");
    printf("5) Salir\n");
    printf("=============================================\n");
}

static void opcion_ingresar_familias(Familia familias[], int *total_ptr) {
    int total = *total_ptr;

    printf("\n--- INGRESO DE FAMILIAS ---\n");
    if (total >= MAX_FAMILIAS) {
        printf("  [!] Ya se alcanzo el maximo de %d familias.\n", MAX_FAMILIAS);
        return;
    }

    do {
        if (total >= MAX_FAMILIAS) {
            printf("  [!] Ya no hay espacio para mas familias (max %d).\n", MAX_FAMILIAS);
            break;
        }

        char cedula[20];
        char nombre[50];

        read_line("Cedula del representante: ", cedula, sizeof(cedula));
        if (cedula[0] == '\0') {
            printf("  [!] La cedula no puede estar vacia.\n");
            continue;
        }

        if (buscar_familia_por_cedula(familias, total, cedula) != -1) {
            printf("  [!] Ya existe una familia con esa cedula. No se permite duplicado.\n");
            continue;
        }

        read_line("Nombre del representante: ", nombre, sizeof(nombre));
        if (nombre[0] == '\0') {
            printf("  [!] El nombre no puede estar vacio.\n");
            continue;
        }

        int ninos = read_int_range("Numero de ninos (0..1000): ", 0, 1000);

        /* Guardar */
        strncpy(familias[total].cedula, cedula, sizeof(familias[total].cedula) - 1);
        familias[total].cedula[sizeof(familias[total].cedula) - 1] = '\0';

        strncpy(familias[total].nombre, nombre, sizeof(familias[total].nombre) - 1);
        familias[total].nombre[sizeof(familias[total].nombre) - 1] = '\0';

        familias[total].num_ninos = ninos;
        familias[total].entregados = 0;

        total++;
        printf("  [+] Familia registrada correctamente.\n");

    } while (ask_yes_no("Desea ingresar otra familia? (s/n): "));

    *total_ptr = total;
}

static void opcion_registrar_entrega(Familia familias[], int total) {
    printf("\n--- REGISTRO DE ENTREGA DE REGALO ---\n");

    if (total == 0) {
        printf("  [!] No hay familias registradas. Primero ingrese familias.\n");
        return;
    }

    do {
        char cedula[20];
        read_line("Cedula del representante de la familia: ", cedula, sizeof(cedula));
        if (cedula[0] == '\0') {
            printf("  [!] Cedula vacia.\n");
            continue;
        }

        int idx = buscar_familia_por_cedula(familias, total, cedula);
        if (idx == -1) {
            printf("  [!] No existe una familia con esa cedula.\n");
            continue;
        }

        Familia *f = &familias[idx];
        int faltan = f->num_ninos - f->entregados;

        printf("Familia: %s (%s)\n", f->nombre, f->cedula);
        printf("Ninos censados: %d | Entregados: %d | Faltan: %d\n",
               f->num_ninos, f->entregados, faltan);

        if (faltan <= 0) {
            printf("  [!] Ya se entregaron todos los regalos para esta familia.\n");
            continue;
        }

        /* El enunciado dice “una entrega a un niño”.
           Para no hacerlo eterno, permito ingresar cantidad,
           pero internamente equivale a registrar varias entregas. */
        int cant = read_int_range("Cuantos regalos desea registrar ahora? (1..1000): ", 1, 1000);
        if (cant > faltan) {
            printf("  [!] No puede entregar %d. Solo faltan %d para esta familia.\n", cant, faltan);
            continue;
        }

        f->entregados += cant;
        printf("  [+] Entrega registrada. Ahora entregados: %d | Faltan: %d\n",
               f->entregados, f->num_ninos - f->entregados);

    } while (ask_yes_no("Desea registrar otra entrega? (s/n): "));
}

static void opcion_reporte_faltantes(const Familia familias[], int total) {
    printf("\n--- REPORTE: REGALOS FALTANTES POR FAMILIA ---\n");

    if (total == 0) {
        printf("  [!] No hay familias registradas.\n");
        return;
    }

    printf("%-18s | %-30s | %-8s\n", "CEDULA", "NOMBRE", "FALTAN");
    printf("-------------------+--------------------------------+---------\n");

    for (int i = 0; i < total; i++) {
        int faltan = familias[i].num_ninos - familias[i].entregados;
        if (faltan < 0) faltan = 0; /* por seguridad */
        printf("%-18s | %-30s | %-8d\n",
               familias[i].cedula, familias[i].nombre, faltan);
    }
}

static void opcion_reporte_entregados(const Familia familias[], int total) {
    printf("\n--- REPORTE: REGALOS ENTREGADOS POR FAMILIA ---\n");

    if (total == 0) {
        printf("  [!] No hay familias registradas.\n");
        return;
    }

    printf("%-18s | %-30s | %-10s\n", "CEDULA", "NOMBRE", "ENTREGADOS");
    printf("-------------------+--------------------------------+-----------\n");

    for (int i = 0; i < total; i++) {
        int entregados = familias[i].entregados;
        if (entregados < 0) entregados = 0; /* por seguridad */
        printf("%-18s | %-30s | %-10d\n",
               familias[i].cedula, familias[i].nombre, entregados);
    }
}

int main(void) {
    Familia familias[MAX_FAMILIAS];
    int totalFamilias = 0;

    for (;;) {
        imprimir_menu();
        int op = read_int_range("Seleccione una opcion (1..5): ", 1, 5);

        switch (op) {
            case 1:
                opcion_ingresar_familias(familias, &totalFamilias);
                break;
            case 2:
                opcion_registrar_entrega(familias, totalFamilias);
                break;
            case 3:
                opcion_reporte_faltantes(familias, totalFamilias);
                break;
            case 4:
                opcion_reporte_entregados(familias, totalFamilias);
                break;
            case 5:
                printf("\nSaliendo del programa... Gracias.\n");
                return 0;
            default:
                /* No debería pasar por validación */
                printf("Opcion invalida.\n");
                break;
        }
    }
}

