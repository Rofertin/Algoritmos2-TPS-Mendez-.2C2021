#include "abb.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Va a reentrega

abb_t* abb_crear(abb_comparador comparador){
    if(!comparador)
      return NULL;
    abb_t* nuevo = calloc(1, sizeof(abb_t));
    if (!nuevo)
      return NULL; //calloc error
    nuevo->tamanio = 0;
    nuevo->nodo_raiz = NULL;
    nuevo->comparador = comparador;
    return nuevo;
}

nodo_abb_t* abb_insertar_nodo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){ 
  if(!comparador) return NULL;
  if(!nodo){
    nodo_abb_t* nuevo = calloc(1, sizeof(nodo_abb_t));
    nuevo->elemento = elemento;
    nuevo->derecha = NULL;
    nuevo->izquierda = NULL;
    return nuevo;
  }
  int comparacion = comparador(elemento, nodo->elemento);
  if (comparacion <= 0){
    nodo->izquierda = abb_insertar_nodo(nodo->izquierda, elemento, comparador);
  }
  if (comparacion > 0){
    nodo->derecha = abb_insertar_nodo(nodo->derecha, elemento, comparador);
  }
  return nodo;
}

abb_t* abb_insertar(abb_t* arbol, void* elemento){
  if(!arbol) return NULL;
  if(arbol->nodo_raiz == NULL){
    nodo_abb_t* nuevo = calloc(1, sizeof(nodo_abb_t));
    if(!nuevo) return NULL;
    nuevo->elemento = elemento;
    nuevo->derecha = NULL;
    nuevo->izquierda = NULL;
    arbol->nodo_raiz = nuevo;
    arbol->tamanio +=1;
    return arbol;
  }
  int comparacion = arbol->comparador(elemento, (arbol->nodo_raiz->elemento));
  if (comparacion <= 0){//duplicados y menores a la izquierda
    arbol->nodo_raiz->izquierda = abb_insertar_nodo(arbol->nodo_raiz->izquierda, elemento, arbol->comparador);
  }
  if(comparacion > 0){ //mayores a derecha
    arbol->nodo_raiz->derecha = abb_insertar_nodo(arbol->nodo_raiz->derecha, elemento, arbol->comparador);
  }
  arbol->tamanio += 1;
  return arbol;
}

nodo_abb_t* predecesor_inorden(nodo_abb_t* nodo) {
  nodo_abb_t* actual = nodo;
  while (actual->derecha){ 
    actual = actual->derecha;
  }
  return actual;
}

void* abb_quitar_nodo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador,void** elemento_a_devolver){
  if(nodo){
    int comparacion = comparador(elemento, nodo->elemento); 
    
    if(comparacion > 0){
      nodo->derecha = abb_quitar_nodo(nodo->derecha,elemento,comparador,elemento_a_devolver);
    }
    if(comparacion < 0){
      nodo->izquierda = abb_quitar_nodo(nodo->izquierda,elemento,comparador,elemento_a_devolver);
    }
    if(comparacion == 0){
      if(nodo->izquierda == NULL) {
            nodo_abb_t* temp = nodo->derecha;
            *elemento_a_devolver = nodo->elemento;
            free(nodo);
            return temp;
      }
      else if(nodo->derecha == NULL) {
            nodo_abb_t* temp = nodo->izquierda;
            *elemento_a_devolver = nodo->elemento;
            free(nodo);
            return temp;
      }
      *elemento_a_devolver = nodo->elemento;
      nodo_abb_t* predecesor = predecesor_inorden(nodo->izquierda);
      void* aux = nodo->elemento;
      nodo->elemento = predecesor->elemento;
      predecesor->elemento = aux;
      nodo->izquierda = abb_quitar_nodo(nodo->izquierda,predecesor->elemento,comparador,elemento_a_devolver);
    }
  }
  return nodo;
}

void* abb_quitar(abb_t* arbol, void *elemento){
  if(!arbol || abb_vacio(arbol))
    return NULL;
  
  void* elemento_a_devolver = NULL; 
  arbol->nodo_raiz = abb_quitar_nodo(arbol->nodo_raiz,elemento,arbol->comparador, &elemento_a_devolver);
  arbol->tamanio -= 1;
  return elemento_a_devolver;
}

void* abb_buscar_nodo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){
  while(nodo){
    int comparacion = comparador(elemento, nodo->elemento);
    if (comparacion == 0) return nodo->elemento; // encontrado
    if (comparacion < 0) nodo = nodo->izquierda;
    if (comparacion > 0) nodo = nodo->derecha;
  }
  return NULL;
}

void* abb_buscar(abb_t* arbol, void* elemento){
  if(!arbol || abb_vacio(arbol)|| !elemento) return NULL;
  void* elemento_a_buscar = NULL;
  int comparacion = arbol->comparador(elemento, (arbol->nodo_raiz->elemento));
  if(comparacion == 0) return arbol->nodo_raiz->elemento;
  if(comparacion < 0){
    elemento_a_buscar = abb_buscar_nodo(arbol->nodo_raiz->izquierda, elemento, arbol->comparador);
  }
  if(comparacion > 0){
    elemento_a_buscar = abb_buscar_nodo(arbol->nodo_raiz->derecha, elemento, arbol->comparador);
  }
  return elemento_a_buscar;
}

bool abb_vacio(abb_t* arbol){
  if(!arbol || arbol->tamanio == 0 || arbol->nodo_raiz == NULL)
    return true;
  return false;
}

size_t abb_tamanio(abb_t *arbol){
  if(!arbol ||abb_vacio(arbol))
    return 0;
  return arbol->tamanio;
}

void nodo_destruir(nodo_abb_t* nodo, void (*destructor)(void *)){
  if(nodo){
    nodo_destruir(nodo->izquierda,destructor);
    nodo_destruir(nodo->derecha,destructor);
    if(destructor) destructor(nodo->elemento);
    free(nodo);
    }
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *)){
  if(!arbol) return;
  nodo_destruir(arbol->nodo_raiz,destructor);
  free(arbol);
}

void abb_destruir(abb_t *arbol){
  abb_destruir_todo(arbol,NULL);
}

void abb_concadaelemento_inorden(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux, size_t* i, bool* terminado){
    if(!nodo || !*terminado){
      return;
    }
      abb_concadaelemento_inorden(nodo->izquierda, funcion, aux, i, terminado);
      if (*terminado){
        *terminado = funcion(nodo->elemento,aux);
        *i += 1;
      }
      abb_concadaelemento_inorden(nodo->derecha, funcion, aux, i, terminado);
    }


void abb_concadaelemento_preorden(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux, size_t* i, bool* terminado){
    if(!nodo || !*terminado){
      return;
    }
    if (*terminado){
        *terminado = funcion(nodo->elemento,aux);
        *i += 1;
    }
    abb_concadaelemento_preorden(nodo->izquierda, funcion, aux, i, terminado);
    abb_concadaelemento_preorden(nodo->derecha, funcion, aux, i, terminado);
  }


void abb_concadaelemento_postorden(nodo_abb_t* nodo, bool (*funcion)(void *, void *), void *aux, size_t* i, bool* terminado){
    if(!nodo || !*terminado){
      return;
    }
    abb_concadaelemento_postorden(nodo->izquierda, funcion, aux, i, terminado);
    abb_concadaelemento_postorden(nodo->derecha, funcion, aux, i, terminado);
    if (*terminado){
        *terminado = funcion(nodo->elemento,aux);
        *i += 1;
    }
  }


size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux){
  if(!arbol || !funcion ) return 0;
  size_t i = 0;
  bool terminado = true;
  if(recorrido == 0){ 
    abb_concadaelemento_inorden(arbol->nodo_raiz, funcion, aux, &i, &terminado);
  }
  if(recorrido == 1){ 
    abb_concadaelemento_preorden(arbol->nodo_raiz, funcion, aux, &i, &terminado);
  }
  if(recorrido == 2){ 
    abb_concadaelemento_postorden(arbol->nodo_raiz, funcion, aux, &i,  &terminado);
  }
  return i;
}

void abb_recorrido_inorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* i){
  if(!nodo || *i >= tamanio_array){
    return;
  }
  abb_recorrido_inorden(nodo->izquierda,array, tamanio_array, i);
  array[(*i)] = nodo->elemento;
  *i += 1;
  abb_recorrido_inorden(nodo->derecha,array, tamanio_array, i);
}

void abb_recorrido_preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* i){
  if(!nodo || *i >= tamanio_array){
    return;
  }
  array[(*i)] = nodo->elemento;
  *i += 1;
  abb_recorrido_preorden(nodo->izquierda,array, tamanio_array, i);
  abb_recorrido_preorden(nodo->derecha,array, tamanio_array, i);
  }


void abb_recorrido_postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* i){
  if(!nodo || *i >= tamanio_array){
    return;
  }
  abb_recorrido_postorden(nodo->izquierda,array, tamanio_array, i);
  abb_recorrido_postorden(nodo->derecha,array, tamanio_array, i);
  if(*i+1 <= tamanio_array){
    array[*i] = nodo->elemento;
    *i += 1;
  }
}

size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array){
  if(!arbol || abb_vacio(arbol) || tamanio_array == 0) return 0;
  size_t i = 0;
  if(recorrido == INORDEN){ 
    abb_recorrido_inorden(arbol->nodo_raiz, array, tamanio_array, &i);
  }
  if(recorrido == PREORDEN){ 
    abb_recorrido_preorden(arbol->nodo_raiz, array, tamanio_array, &i);
  }
  if(recorrido == POSTORDEN){ 
    abb_recorrido_postorden(arbol->nodo_raiz, array, tamanio_array, &i);
  }
  return i;
}


