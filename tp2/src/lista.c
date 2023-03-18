#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

lista_t* lista_crear(){
    lista_t* lista = calloc(1,sizeof(lista_t));
    if(!lista)
        return NULL;
    
    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    lista->cantidad = 0;
    return lista;
}

lista_t* lista_termino_vacia(lista_t* lista){
    if(lista_vacia(lista)){
        free(lista);
        lista = lista_crear();
    }
    return lista;
}

lista_t* lista_insertar_primero(lista_t* lista, void* elemento){
    nodo_t* nodo_a_agregar = malloc(sizeof(nodo_t));
    nodo_a_agregar->elemento = elemento;
    nodo_a_agregar->siguiente = lista->nodo_inicio;
    lista->nodo_inicio = nodo_a_agregar;
    lista->cantidad += 1;
    return lista;
}

lista_t* lista_insertar(lista_t* lista, void* elemento){
    if(!lista)
        return NULL;
    
    nodo_t* nodo_a_agregar = malloc(sizeof(nodo_t));
    nodo_a_agregar->elemento = elemento;
    nodo_a_agregar->siguiente = NULL; //ultima posicion

    if(lista_vacia(lista)){
        lista->nodo_inicio = nodo_a_agregar;
        lista->nodo_fin = nodo_a_agregar;
        lista->cantidad += 1;
        return lista;
    }
    
    lista->nodo_fin->siguiente = nodo_a_agregar;
    lista->nodo_fin = nodo_a_agregar;
    lista->cantidad += 1;
    return lista;
}

lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(!lista){
        return NULL;
    }
    if (lista_vacia(lista) || posicion >= lista->cantidad){ 
        lista = lista_insertar(lista,elemento);
        return lista;
    }

    if (posicion == 0){ 
        lista = lista_insertar_primero(lista, elemento);
        return lista;
    }
    
    size_t i = 1;
    nodo_t* nodo_posicion = lista->nodo_inicio; //empieza desde el inicio
 
    while(i < posicion && nodo_posicion->siguiente){
        nodo_posicion = nodo_posicion->siguiente;      
        i++;
    }
    nodo_t* nodo_a_insertar = malloc(sizeof(nodo_t));
    nodo_a_insertar->elemento = elemento;

    nodo_a_insertar->siguiente = nodo_posicion->siguiente;
    nodo_posicion->siguiente = nodo_a_insertar;
    lista->cantidad += 1;

    return lista;
}

void* lista_quitar_primero(lista_t* lista){
    nodo_t* auxiliar = lista->nodo_inicio;
    void* elemento_a_devolver = lista->nodo_inicio->elemento;
    lista->nodo_inicio = lista->nodo_inicio->siguiente;
    lista->cantidad -= 1;
    free(auxiliar);
    return elemento_a_devolver;
}

void* lista_quitar_primero_pero_es_ultimo(lista_t* lista){
    void* elemento_a_devolver = NULL;
    nodo_t* auxiliar = lista->nodo_fin;
    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    elemento_a_devolver = auxiliar->elemento;
    
    lista->cantidad -= 1;
    free(auxiliar);
    return elemento_a_devolver;
}

void* lista_quitar(lista_t* lista){
    if(!lista || lista_vacia(lista))
        return NULL;
    void* elemento_a_devolver = NULL;
    
    if(lista->cantidad == 1){
        elemento_a_devolver = lista_quitar_primero_pero_es_ultimo(lista);
        return elemento_a_devolver;
    }
    
    // cuando cantidad > 1
    size_t i = 0;
    nodo_t* nodo_previo_a_quitar = lista->nodo_inicio;
    nodo_t* nodo_a_quitar = NULL;
    while(nodo_previo_a_quitar->siguiente->siguiente){ //consigo el aunteultimo
        nodo_previo_a_quitar = nodo_previo_a_quitar->siguiente;
        i++;
    }
    
    lista->nodo_fin = nodo_previo_a_quitar;
    nodo_a_quitar = lista->nodo_fin->siguiente; //deja de ser el previo
    elemento_a_devolver = nodo_a_quitar->elemento;
    free(nodo_a_quitar);
    
    lista->nodo_fin->siguiente = NULL;
    lista->cantidad -= 1;
    
    lista_termino_vacia(lista);
    return elemento_a_devolver;
}

void* lista_quitar_de_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista_vacia(lista))
        return NULL;
    
    if(posicion == 0){
        return lista_quitar_primero(lista);
    }

    size_t i = 1;
    nodo_t* nodo_a_quitar = NULL;
    nodo_t* nodo_buscado = lista->nodo_inicio;
    void* elemento_a_devolver = NULL; //para hacer free al nodo

    while(i < posicion && nodo_buscado->siguiente){
        nodo_buscado = nodo_buscado->siguiente;      
        i++;
    }
    if (i != posicion){
        lista = lista_quitar(lista);
        return lista;
    }
    nodo_a_quitar = nodo_buscado->siguiente;
    nodo_buscado->siguiente = nodo_a_quitar->siguiente;
    elemento_a_devolver = nodo_a_quitar->elemento;
    
    lista->cantidad -= 1;
    free(nodo_a_quitar);

    lista_termino_vacia(lista);
    return elemento_a_devolver;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista_vacia(lista))
        return NULL;
    
    size_t i = 0;
    nodo_t* nodo_buscado = lista->nodo_inicio;

    while(i < posicion && nodo_buscado->siguiente){
        nodo_buscado = nodo_buscado->siguiente;      
        i++;
    }
    if (i != posicion){
        return NULL;
    }

    return nodo_buscado->elemento;
}

void* lista_primero(lista_t* lista){
    if(!lista || lista_vacia(lista))
        return NULL;
    return lista->nodo_inicio->elemento;
}

void* lista_ultimo(lista_t* lista){   
    if(!lista || lista_vacia(lista))
        return NULL;
    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    if (!lista ||lista->cantidad == 0 || lista->nodo_inicio == NULL)
        return true;
    return false;
}

size_t lista_tamanio(lista_t* lista){
    if(!lista || lista_vacia(lista))
        return 0;
    return lista->cantidad;
}

void lista_destruir(lista_t* lista){
    nodo_t* posicion = NULL;
    nodo_t* aux_nodo = NULL;
    if(!lista_vacia(lista)){
        posicion = lista->nodo_inicio;
        while(posicion->siguiente){
            aux_nodo = posicion;
            posicion = posicion->siguiente;
            free(aux_nodo);
            
        }free(posicion);
    }
    free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista)
        return NULL;
    lista_iterador_t* iterador = calloc(1,sizeof(lista_iterador_t));
    if(!iterador)
        return NULL;
    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;
    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if (lista_vacia(iterador->lista) || !iterador->corriente)
        return false;
    return true;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    bool avanzar = false;
    if(lista_iterador_tiene_siguiente(iterador)){
        if(iterador->corriente->siguiente)
            avanzar = true;
        iterador->corriente = iterador->corriente->siguiente;
    }
    return avanzar;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if(!iterador || lista_vacia(iterador->lista) || !iterador->corriente)
        return NULL;
    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if(!lista || !funcion)
        return 0;
    nodo_t* nodo_actual = lista->nodo_inicio;
    size_t i = 1;
    while(funcion(nodo_actual->elemento,contexto) && nodo_actual->siguiente){
        nodo_actual = nodo_actual->siguiente;
        i++;
    }
    return i;
}
