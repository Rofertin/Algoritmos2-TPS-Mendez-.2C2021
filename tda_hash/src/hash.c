#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1
#define EXITO 0

typedef struct casilla{
    char* clave;
    void* dato;
    struct casilla* siguiente;
}casilla_t;

struct hash{
    casilla_t** casilleros;
    hash_destruir_dato_t destructor;
    size_t cantidad_elementos;
    size_t maximo;
};

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){
    capacidad_inicial = (capacidad_inicial<3)?3:capacidad_inicial;

    hash_t* hash = (hash_t*)calloc(1,sizeof(hash_t));
    if(!hash) return NULL;
   
    hash->casilleros = (casilla_t**)calloc(capacidad_inicial, sizeof(casilla_t));
    if(!hash->casilleros){
        free(hash);
        return NULL;
    }
    hash->destructor = (destruir_elemento)?destruir_elemento:NULL;
    hash->cantidad_elementos = 0;
    hash->maximo = capacidad_inicial;
    
    return hash;
}

hash_t* agrandar_tabla(hash_t* hash, hash_destruir_dato_t destruir_elemento,size_t capacidad_extra){
    if(!hash) return NULL;

    hash_t* hash_nuevo = hash_crear(destruir_elemento,capacidad_extra);

    for(size_t i = 0; i < hash->cantidad_elementos; i++){
        hash_nuevo->casilleros[i] = hash->casilleros[i];
    }
    hash_destruir(hash);
    return hash_nuevo;

}

size_t funcion_hash(const char* clave){
    size_t numero = 0;
    while(*clave){
        numero += *(clave++);
    }
    return numero;
}

char* copiar_clave(const char* clave){
    char* copia = malloc(strlen(clave) + 1);
    if (!copia) return NULL;
    strcpy(copia, clave);
    return copia;
}

void destructor_hash(void (*destructor)(void *), void* elemento){
    if(destructor){
        destructor(elemento);
    }
}

int reemplazar_dato(hash_t* hash, casilla_t* casilla, casilla_t* nuevo){
    destructor_hash(hash->destructor, casilla->dato);
    casilla->dato = nuevo->dato;
    free(nuevo->clave);
    free(nuevo);
    return EXITO;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!hash || !clave) return ERROR;
    /*
    if((hash->cantidad_elementos+1) > (hash->maximo)){ 
        hash = agrandar_tabla(hash, hash->destructor, (hash->maximo*3));
        return hash_insertar(hash, clave, elemento);
    }
    */
    casilla_t* nuevo = malloc(sizeof(casilla_t));
    if(!nuevo) return ERROR;

    nuevo->clave = copiar_clave(clave);
    nuevo->dato = (elemento)?elemento:NULL;
    nuevo->siguiente = NULL;

    int posicion = funcion_hash(clave) % hash->maximo;
    if(!hash->casilleros[posicion]){
        hash->casilleros[posicion] = nuevo;
        hash->cantidad_elementos += 1;
    }
    else{
        casilla_t* ultimo = hash->casilleros[posicion];
        while(ultimo->siguiente){
            if(strcmp(ultimo->clave, clave) == 0){
                return reemplazar_dato(hash, ultimo, nuevo);
            }
            ultimo = ultimo->siguiente;
        }
        if(strcmp(ultimo->clave, clave) == 0){
            return reemplazar_dato(hash, ultimo, nuevo);
        }
        ultimo->siguiente = nuevo;
        hash->cantidad_elementos += 1;
    }
    return EXITO;
}

casilla_t* hash_quitar_en_medio_de_lista(hash_t* hash,casilla_t* actual_a_borrar, const char* clave, bool* condicion){
    if(!actual_a_borrar) return NULL;
    casilla_t* siguiente = NULL;

    if(strcmp(actual_a_borrar->clave,clave) == 0){
        *condicion = true;
        if(actual_a_borrar->siguiente){
            siguiente = actual_a_borrar->siguiente;
        }
        if(hash->destructor){
            hash->destructor(actual_a_borrar->dato);
        }
        free(actual_a_borrar->clave);
        free(actual_a_borrar);
        hash->cantidad_elementos -= 1;
        return siguiente;
    }
    actual_a_borrar->siguiente = hash_quitar_en_medio_de_lista(hash, actual_a_borrar->siguiente, clave, condicion);
    return actual_a_borrar;
}

int hash_quitar(hash_t* hash, const char* clave){
    if(!hash) return ERROR;
    int posicion = funcion_hash(clave) % hash->maximo;
    bool condicion = false;
    if(!hash->casilleros[posicion]){
        return ERROR;
    }
    else{
        casilla_t* actual_a_borrar = hash->casilleros[posicion];
        
        if(actual_a_borrar->siguiente){ // varios enlistados
            hash->casilleros[posicion] = hash_quitar_en_medio_de_lista(hash, actual_a_borrar, clave, &condicion);
        }
        else{ // 1 en la lista
            if(hash->destructor){
                hash->destructor(actual_a_borrar->dato);
            }
            condicion = true;
            free(actual_a_borrar->clave);
            free(actual_a_borrar);
            hash->cantidad_elementos -= 1;
            hash->casilleros[posicion] = NULL;
        }
    
    if(condicion)
        return EXITO;
    return ERROR;
    }

}

void* hash_obtener(hash_t* hash, const char* clave){
    if(!hash || !clave) return NULL;
    
    int posicion = funcion_hash(clave) % hash->maximo;
    
    casilla_t* actual = hash->casilleros[posicion];
    if(!actual) return NULL;
    
    while(actual){
        if(strcmp(actual->clave,clave) == 0){
            return actual->dato;
        }
        actual = actual->siguiente;
    }
    if(actual){
        return actual->dato;
    }
    return NULL;

}

bool hash_contiene(hash_t* hash, const char* clave){
    if(!hash || !clave) return false;
    
    int posicion = funcion_hash(clave) % hash->maximo;
    
    casilla_t* actual = hash->casilleros[posicion];
    if(!actual) return false;
    
    while(actual){
        if(strcmp(actual->clave,clave) == 0){
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

void destruir_clave_hash(casilla_t* hash_posicion, void (*destructor)(void *)){
    if(!hash_posicion) return;
    destruir_clave_hash(hash_posicion->siguiente, destructor);
    hash_posicion->siguiente = NULL;
    if(destructor){
        destructor(hash_posicion->dato);
    }
    free(hash_posicion->clave);
    free(hash_posicion);
}

void hash_destruir(hash_t* hash){
    if(!hash) return;
    for(size_t i = 0; i < hash->maximo; i++){
        destruir_clave_hash(hash->casilleros[i], hash->destructor);
    }
    free(hash->casilleros);
    free(hash);
}

size_t hash_cantidad(hash_t* hash){
    if(!hash) return 0;
    return hash->cantidad_elementos;
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    if(!hash || !funcion) return 0;
    size_t cantidad = 0;
    casilla_t* casillas_clave;
    for(size_t i = 0; i < hash->maximo;i++){
        casillas_clave = hash->casilleros[i];
        while(casillas_clave){
            if(funcion(hash,casillas_clave->clave,aux)){
                return cantidad + 1;
            }
            cantidad++;
            casillas_clave = casillas_clave->siguiente;
        }
    }
    return cantidad;
}
