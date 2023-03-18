#include "pila.h"
#include "lista.h"
#include <stdio.h>

struct _pila_t{
    nodo_t* tope;
    size_t tamanio;
};

pila_t* pila_crear(){
    pila_t* pila = calloc(1,sizeof(pila_t));
    if(!pila)
        return NULL;
    pila->tope = NULL;
    pila->tamanio = 0;
    return pila;
}

pila_t* pila_apilar(pila_t* pila, void* elemento){
    pila->tamanio += 1;
    nodo_t* nuevo = (nodo_t*)malloc(sizeof(nodo_t));
    nuevo->elemento = elemento;
    nuevo->siguiente = pila->tope;
    pila->tope = nuevo;
    return pila;
}

void* pila_desapilar(pila_t* pila){
    if(!pila){
        return NULL;
    }
    if(!pila_vacia(pila)){
        pila->tamanio -= 1;
        nodo_t* nodo_a_elimiar = pila->tope;
        void* elemento_a_devolver = nodo_a_elimiar->elemento;
        pila->tope = nodo_a_elimiar->siguiente;
        free(nodo_a_elimiar);
        return elemento_a_devolver;
    }
    return NULL; // no había elementos
}

void* pila_tope(pila_t* pila){
    if(!pila  || pila_vacia(pila)){
        return NULL;
    }
    return pila->tope->elemento;
}

size_t pila_tamanio(pila_t* pila){
    if (!pila)
        return 0;
    return pila->tamanio;
}

bool pila_vacia(pila_t* pila){
    if (!pila){ //no existe pila
        return true;
    }
    if(pila->tope == NULL){ // creada sin elementos
        return true;
    }    
    return false;
}

void* pila_elemento(pila_t* pila){
    return pila->tope->elemento;
}

void pila_destruir(pila_t* pila){
    free(pila);
}

