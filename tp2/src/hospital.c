#include "hospital.h"
#include "hospital_structs.h"
#include "lista.h"
#include "abb.h"

#include <stdio.h>
#include <string.h>

//Agustín de la Rosa;
//ID_ENTRENADOR1;NOMBRE_ENTRENADOR1;POKEMON1;NIVEL;POKEMON2;NIVEL;POKEMON3;NIVEL;POKEMON_n;NIVEL;

int comparador_de_pokemones(void* a, void* b){
	pokemon_t* pok_a = a;
	pokemon_t* pok_b = b; 
	return strcmp(pok_a->nombre, pok_b->nombre);
};

hospital_t* hospital_crear(){
	hospital_t* hospital = calloc(1,sizeof(hospital_t));
	if(!hospital) return NULL;
	
	hospital->pokemones = abb_crear(comparador_de_pokemones);
	if(!hospital->pokemones){
		free(hospital);
		return NULL;
	}
	
	hospital->entrenadores = lista_crear();
	if(!hospital->entrenadores){
		free(hospital);
		abb_destruir(hospital->pokemones);
		return NULL;
	}
	return hospital;
}

char* fgets_dinamico(FILE *archivo){
    size_t tamanio = 0, tamanio_extra = 0;
    char linea[20]; //minimo
    char *dir_final = NULL, *temporal;

    while(fgets(linea, sizeof(linea), archivo)){

        int condicion_linea_terminada = 0;
        size_t largo_linea = strlen(linea);

        if(linea[largo_linea - 1] == '\n'){ // buffer alcanzó el tamaño
            //linea[largo_linea - 2] = 0;
            condicion_linea_terminada = 1;
        }

        if(tamanio_extra + largo_linea >= tamanio){
            tamanio += (sizeof linea) - (tamanio ? 1 : 0);
            temporal = realloc(dir_final, tamanio);
            if(temporal == NULL)
				break; 
            dir_final = temporal;
        }
        memcpy(dir_final + tamanio_extra, linea, largo_linea + 1); //le agrego memoria que me falta al buffer
        tamanio_extra += largo_linea;
        if(condicion_linea_terminada)
            break;
    }
    if(dir_final){
        temporal = realloc(dir_final, tamanio_extra + 1);
        if(temporal)
            dir_final = temporal;
    }
    return dir_final;
}

pokemon_t* guardar_pokemon(pokemon_t pokemon, size_t id){
	pokemon_t* pokemon_save = calloc(1, sizeof(pokemon_t));
	pokemon_save->nombre = malloc(strlen(pokemon.nombre)+1);
	strcpy(pokemon_save->nombre, pokemon.nombre);
	pokemon_save->nivel = pokemon.nivel;
	pokemon_save->id = id;
	pokemon_save->ya_fue_atendido = false;
	return pokemon_save;
}

entrenador_t* crear_entrenador(const char* nombre, size_t id){
	entrenador_t* entrenador_save = calloc(1, sizeof(entrenador_t));
	entrenador_save->pokemones = abb_crear(comparador_de_pokemones);
	entrenador_save->nombre = malloc(strlen(nombre)+1);
	entrenador_save->id = id;
	strcpy(entrenador_save->nombre, nombre);
	entrenador_save->ya_fue_atendido = false;
	entrenador_save->cantidad_pokemon = 0;
	return entrenador_save;
}

void agregar_pokemon_a_entrenador(hospital_t* hospital, pokemon_t* pokemon){
	lista_iterador_t* it = lista_iterador_crear(hospital->entrenadores);
	entrenador_t* entrenador = lista_iterador_elemento_actual(it);
	while(lista_iterador_tiene_siguiente(it) && entrenador->id != pokemon->id){
		lista_iterador_avanzar(it);
		entrenador = lista_iterador_elemento_actual(it);
	}
	if(entrenador->id == pokemon->id) abb_insertar(entrenador->pokemones, pokemon);
	lista_iterador_destruir(it);
}

void agregar_pokemon_a_hospital(hospital_t* hospital, pokemon_t pokemon, size_t id){
	pokemon_t* pokemon_save_abb = guardar_pokemon(pokemon, id);
	pokemon_t* pokemon_save_entrenador = guardar_pokemon(pokemon, id);
	
	abb_insertar(hospital->pokemones, pokemon_save_abb);
	hospital->cantidad_pokemon += 1;
	agregar_pokemon_a_entrenador(hospital, pokemon_save_entrenador);
}

void agregar_entrenador_a_hospital(hospital_t* hospital, const char* nombre, size_t id){
	entrenador_t* entrenador_ptr = crear_entrenador(nombre, id);
	hospital->entrenadores = lista_insertar(hospital->entrenadores, entrenador_ptr);
	hospital->cantidad_entrenadores += 1;
}


bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){
	if(!nombre_archivo) return false;
	FILE* archivo = fopen(nombre_archivo,"r");
	if (archivo == NULL) return false;
	
	size_t fila = 0, columna;
	char* buffer = NULL;
	pokemon_t pokemon;
	pokemon.nombre = NULL;
	size_t id = 0;

	buffer = fgets_dinamico(archivo);
	while(buffer){

		columna = 0;
		fila++;

		char* valor = strtok(buffer,";");		
		
		while(valor){
			
			if (columna == 1 && *valor != '\n'){
				id = (hospital->cantidad_entrenadores)+1;
				agregar_entrenador_a_hospital(hospital, valor, id);			
			}
			if (columna>1 && columna % 2 == 0){
				pokemon.nombre = malloc(strlen(valor)+1);
				strcpy(pokemon.nombre,valor);
			}	
			if (columna>1 && columna % 2 != 0){
				pokemon.nivel = (size_t)atoi(valor);
				agregar_pokemon_a_hospital(hospital, pokemon, id);
				free(pokemon.nombre);
			}
			columna++;
			valor = strtok(NULL, ";");
		}
		free(buffer);
		buffer = fgets_dinamico(archivo);
	}
	fclose(archivo);
	return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital){
	if(!hospital) return 0;
	return hospital->cantidad_pokemon;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if(!hospital) return 0;
	return hospital->cantidad_entrenadores;
}

bool funcion_para_abb(void* pokemon, void* funcion){
	if(!pokemon || !funcion) return false; 
	pokemon_t* p = pokemon;
	bool (*funcion_nueva)(pokemon_t *p) = funcion;
    return funcion_nueva(p);
}

size_t hospital_a_cada_pokemon(hospital_t *hospital, bool (*funcion)(pokemon_t *p)){
    if (!hospital || !hospital->pokemones || !funcion) return 0;
    size_t cantidad = abb_con_cada_elemento(hospital->pokemones, INORDEN, funcion_para_abb, funcion);
    return cantidad;
}

void destructor_pokemon(void* elemento){
    if(!elemento) return;
	pokemon_t* pokemon = elemento;
    free(pokemon->nombre);
    free(pokemon);
}

void entrenadores_destruir(lista_t* entrenadores){
	lista_iterador_t* iterador = lista_iterador_crear(entrenadores);
	while(lista_iterador_tiene_siguiente(iterador)){
        entrenador_t* entrenador = lista_iterador_elemento_actual(iterador);
        if(entrenador){
            free(entrenador->nombre);
            abb_destruir_todo(entrenador->pokemones, destructor_pokemon);
            free(entrenador);
        }
        lista_iterador_avanzar(iterador);
    }
	lista_iterador_destruir(iterador);
    lista_destruir(entrenadores);
}

void pokemones_abb_destruir(abb_t* pokemones){
	abb_destruir_todo(pokemones, destructor_pokemon);
}

void hospital_destruir(hospital_t* hospital){
	if(!hospital) return;
	if(hospital->entrenadores) entrenadores_destruir(hospital->entrenadores);
	if(hospital->pokemones) pokemones_abb_destruir(hospital->pokemones);
	free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon){
    if(!pokemon) return 0;
    return pokemon->nivel;
}

const char* pokemon_nombre(pokemon_t* pokemon){
	if(!pokemon) return NULL;
    return pokemon->nombre;	  
}

lista_t* devolver_lista_entrenadores(hospital_t* hospital){
	return (!hospital)? NULL: hospital->entrenadores; 
}

abb_t* devolver_abb_pokemones(hospital_t* hospital){
	return (!hospital)? NULL: hospital->pokemones;
}


