#ifndef HOSPITAL_STRUCTS_H_
#define HOSPITAL_STRUCTS_H_
#include "abb.h"
#include "lista.h"

struct _entrenador_t{
	char* nombre;
	bool atendido;
	size_t cantidad_pokemon;
    abb_t* pokemones;
	size_t id;
	bool ya_fue_atendido;
};

struct _hospital_pkm_t{
    size_t cantidad_entrenadores;
	size_t cantidad_pokemon;
	abb_t* pokemones;
	lista_t* entrenadores;
};

struct _pkm_t{
	char* nombre;
    size_t nivel;
	size_t id;
	bool ya_fue_atendido;
};

#endif 