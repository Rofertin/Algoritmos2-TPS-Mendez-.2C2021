#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

struct heap{
  void** vector;
  int (*comparador)(void*,void*);
  size_t tamanio;
  size_t maximo;
};

heap_t* heap_crear(int (*comparador)(void*, void*), size_t tamanio_inicial){
    if(!comparador) return NULL;
    heap_t* heap = calloc(1, sizeof(heap_t));
    heap->comparador = comparador;
    heap->vector = malloc(tamanio_inicial*sizeof(void*));
    heap->tamanio = tamanio_inicial;
    return heap;
}

int heap_cantidad(heap_t* heap){
    if(!heap) return -1;
    return (int)heap->maximo;
}

void swap(void** vector, size_t i, size_t j){
    void* aux = vector[i];
    vector[i] = vector[j];
    vector[j] = aux;
}

bool heap_vacio(heap_t* heap){
    return (heap == NULL || heap->vector == NULL || heap->tamanio == 0);
}

void sift_up(heap_t* heap, size_t pos){
    if(heap_vacio(heap) || pos == 0) return;
    size_t padre = (pos-1)/2;
    if(heap->comparador(heap->vector[pos], heap->vector[padre]) < 0){
        swap(heap->vector,pos,padre);
        sift_up(heap,padre);
    }
}

void sift_down(heap_t* heap,size_t pos){
    if(heap_vacio(heap)) return;
    size_t hijo_izq = pos*2 + 1;;
    if(hijo_izq >= heap->maximo) return;
    
    size_t pos_menor = hijo_izq;
    size_t hijo_der = pos*2 + 2;
   
    if(hijo_der < heap->maximo){
        if(heap->comparador(heap->vector[hijo_der], heap->vector[pos_menor]) < 0)
            pos_menor = hijo_der;   
    }

    if(heap->comparador(heap->vector[pos], heap->vector[pos_menor]) > 0){
        swap(heap->vector, pos, pos_menor);
        sift_down(heap,pos_menor);
    }
}

bool heap_insertar(heap_t* heap, void* elemento){
    if(!heap || !elemento) return false;
    if(heap->tamanio == heap->maximo){
        void** heap_aux = realloc(heap->vector,sizeof(void*)*(heap->maximo+1)); 
        heap->vector = heap_aux;
        heap->tamanio++;
    }
    heap->vector[heap->maximo] = elemento;
    sift_up(heap,heap->maximo);
    heap->maximo++;
    return true;
}


void* heap_extraer_raiz(heap_t* heap){
    if(heap_vacio(heap)) return NULL;
    void* elemento = heap->vector[0];
    swap(heap->vector, 0, heap->maximo-1);
    heap->vector = realloc(heap->vector, sizeof(void*)*(heap->tamanio-1));
    heap->maximo--;
    heap->tamanio--;
    if(heap->maximo > 0)
        sift_down(heap, 0);
    return elemento;
}

size_t heap_con_cada_elemento(heap_t* heap, bool (*funcion)(void*, void*), void *aux){
    if(!heap || !funcion || heap_vacio(heap)) return 0;
    size_t cantidad = heap->tamanio;
    bool condicion = true;
    while(cantidad < heap->tamanio && condicion){
        condicion = funcion(heap->vector[cantidad], aux);
        cantidad += 1; 
    }
    return cantidad;
}

void heap_destruir(heap_t* heap){
    if(!heap) return;
    free(heap->vector);
    free(heap);
}

