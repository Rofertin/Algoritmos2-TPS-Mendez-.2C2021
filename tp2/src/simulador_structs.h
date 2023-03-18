#ifndef SIMULADOR_STRUCTS_H_
#define SIMULADOR_STRUCTS_H_

#include "hospital_structs.h"
#include "simulador.h"
#include "heap.h"
#include "lista.h"

typedef struct sala_medica_t_{
    char* nombre_entrenador;
    heap_t* pokemon_en_espera;
    pokemon_t* pokemon_en_atencion;
} sala_medica_t;

struct _simulador_t{
    hospital_t* hospital;
    EstadisticasSimulacion estadisticas;
    
    lista_t* dificultades_disponibles;
    DatosDificultad* datos_dificultad;
    unsigned id_dificultad;

    sala_medica_t* sala_medica;
    lista_t* sala_espera;
    unsigned intentos;

    bool terminado;
};

#endif 
