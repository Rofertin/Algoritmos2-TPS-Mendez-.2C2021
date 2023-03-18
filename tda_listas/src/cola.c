#include "cola.h"
#include "lista.h"

struct _cola_t{
    nodo_t* final;
    nodo_t* principio;
    size_t tamanio;
};

cola_t* cola_crear(){
    cola_t* cola = calloc(1,sizeof(cola_t));
    if(!cola) //fallo en calloc
        return NULL;
    cola->final = NULL;
    cola->principio = NULL;
    cola->tamanio = 0;
    return cola;
}

cola_t* cola_encolar(cola_t* cola, void* elemento){
    if(!cola)
        return NULL;
    nodo_t* temporal = malloc(sizeof(nodo_t));
    if(!temporal){ //fallo malloc
        return cola;
    }
    temporal->elemento = elemento;
    temporal->siguiente = NULL;

    if (cola->principio == NULL){    //primer elemento
        cola->principio = temporal;
        cola->final = cola->principio;
        cola->tamanio += 1;
        return cola;
    }
    cola->final->siguiente = temporal;
    cola->final = temporal;
    cola->tamanio += 1;
    return cola; 
}

void* cola_desencolar(cola_t* cola){
    if(!cola)
        return NULL;
    nodo_t* nodo_a_borrar = cola->principio;
    void* elemento_a_devolver = nodo_a_borrar->elemento;
    cola->principio = nodo_a_borrar->siguiente;
    cola->tamanio -= 1;
    free(nodo_a_borrar);
    return elemento_a_devolver;
}

void* cola_frente(cola_t* cola){
    if(!cola)
        return NULL;
    if(!cola_vacia(cola)){
        return cola->principio->elemento;
    }
    return NULL;
}

size_t cola_tamanio(cola_t* cola){
    if(!cola)
        return 0;
    return cola->tamanio;
}

bool cola_vacia(cola_t* cola){
    if(!cola)
        return true;
    if(cola->principio == NULL)
        return true;
    return false;
}

void cola_destruir(cola_t* cola){
    free(cola);
}
