#ifndef HEAP_H_
#define HEAP_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct heap heap_t;

heap_t* heap_crear(int (*comparador)(void*, void*), size_t tamanio_inicial);

void sift_down(heap_t* heap, size_t pos);

void sift_up(heap_t* heap, size_t pos);

bool heap_insertar(heap_t* heap, void* elemento);

void* heap_extraer_raiz(heap_t* heap);

int heap_cantidad(heap_t* heap);

bool heap_vacio(heap_t* heap);

size_t heap_con_cada_elemento(heap_t* heap, bool (*funcion)(void*, void*), void *aux);

void heap_destruir(heap_t* heap);

#endif 