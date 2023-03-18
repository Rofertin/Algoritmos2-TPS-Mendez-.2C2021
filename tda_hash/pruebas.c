#include "pa2mm.h"
#include "src/hash.h"
#include "ejemplo.c"
#include <stdio.h>

void casteoYprintf(void* puntero){
    printf("%s",(char*)puntero);
}

void verificar_hash(hash_t* hash){
    printf("%s\n", hash?"Hash creado":"Hash ERROR");
}

void guardar_alumno(hash_t* clase, char* nombre, const char* nota){
  int retorno = hash_insertar(clase, nombre, duplicar_string(nota));
  printf("Inserto clave %s (%s): ", nombre, nota);
  printf("%s\n", retorno==0?"OK":"ERROR");
}

int mis_pruebas(){
    hash_t* clase = hash_crear(destruir_string, 1);
    hash_t* clase2 = hash_crear(NULL, 10);
    printf("Verifico hash: ");
    verificar_hash(clase);
    printf("Verifico hash: ");
    verificar_hash(clase2);
    printf("\n");
    
    guardar_alumno(clase,"Clave1", "ocho");
    guardar_alumno(clase,"Alumno-2", "nueve");

    
    guardar_alumno(clase2,"Clave1", "ocho");
    guardar_alumno(clase2,"Alumno-2", "nueve");

    guardar_alumno(clase,"Alumno-3", "diez");
    guardar_alumno(clase,"Alumno-4", "once");
    guardar_alumno(clase,"Alumno-5", "siete");
    printf("\n");

    void* elemento = hash_obtener(clase, "Clave1");
    printf("Busco <Clave1: ");
    casteoYprintf(elemento);
    printf(">\n");
    
    guardar_alumno(clase,"Clave1", "veinte");

    elemento = hash_obtener(clase, "Clave1");
    printf("Vuelvo a buscar <Clave1: ");
    casteoYprintf(elemento);
    printf(">\n\n");

    hash_destruir(clase);
    hash_destruir(clase2);
    return 0;
}

int main(){
    pa2m_nuevo_grupo("MIS PRUEBAS"); 
    //mis_pruebas();
    pa2m_nuevo_grupo("PRUEBAS PROFESORES"); 
    ejemplo_profesores();
    printf("\n"); 
    return pa2m_mostrar_reporte();
}