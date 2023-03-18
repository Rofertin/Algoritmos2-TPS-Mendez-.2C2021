#include "simulador_structs.h"
#include "simulador.h"
#include "hospital.h"
#include "lista.h"
#include "abb.h"
#include "heap.h"

#include <stdio.h>
#include <string.h>


#define NIVEL_ADIVINADO "Adivinaste!"

#define ERRAR_CERCA_MENOS_FACIL "Cerca, más grande"
#define ERRAR_CERCA_MAS_FACIL "Cerca, más chico"
#define ERRAR_LEJOS_MENOS_FACIL "Lejos, mucho más grande"
#define ERRAR_LEJOS_MAS_FACIL "Lejos, mucho más chico"

#define ERRAR_POCO_NORMAL "Más alto..."
#define ERRAR_MUCHO_NORMAL "Más bajo..."

#define ERRAR_DIFICIL "Pifiaste."

//----------------------------------------------------------------------INICIALIZAR SIMULLADOR-------------------------------------------------------------------------------

int verificar_nivel(unsigned intento, unsigned real){
    int diferencia = (int)real - (int)intento;
    if(real == intento)
        return 0;
    return diferencia;
}

unsigned calcular_puntaje_facil(unsigned intentos){
    return 1000-(intentos*50);
}

unsigned calcular_puntaje_normal(unsigned intentos){
    return 1000-(intentos*20);
}

unsigned calcular_puntaje_dificil(unsigned intentos){
    return 1000-(intentos*10);
}

char* duplicar_string(const char* clave){
    char* copia = malloc(strlen(clave) + 1);
    if (!copia) return NULL;
    strcpy(copia, clave);
    return copia;
}

const char* verificacion_a_string_facil(int resultado){
    if(resultado == 0) return NIVEL_ADIVINADO;
    if(resultado > 10) return ERRAR_LEJOS_MAS_FACIL;
    if(resultado > 0 ) return ERRAR_CERCA_MENOS_FACIL;
    if(resultado < -10) return ERRAR_LEJOS_MENOS_FACIL;
    if(resultado < 0 ) return ERRAR_CERCA_MAS_FACIL;
    return NULL;
}

const char* verificacion_a_string_normal(int resultado){
    if(resultado == 0) return NIVEL_ADIVINADO;
    if(resultado > 0 ) return ERRAR_POCO_NORMAL;
    return ERRAR_MUCHO_NORMAL;
}

const char* verificacion_a_string_dificil(int resultado){
    if(resultado == 0) return NIVEL_ADIVINADO;
    return ERRAR_DIFICIL;
}

int comparador_heap_minimal(void* pok_a, void* pok_b){
    int nivel_pok_a = (int)((pokemon_t*)pok_a)->nivel;
    int nivel_pok_b = (int)((pokemon_t*)pok_b)->nivel;
    return nivel_pok_a - nivel_pok_b;
}

void dificultad_crear_e_insertar(lista_t* lista_difs, const char* nombre, unsigned id, const char* (*verificacion_a_string)(int resultado), unsigned (*calcular_puntaje)(unsigned cantidad_intentos)){
    const char* nombre_save = duplicar_string(nombre);
    DatosDificultad* dificultad_nueva = calloc(1, sizeof(DatosDificultad));
    dificultad_nueva->nombre = nombre_save;
    dificultad_nueva->calcular_puntaje = calcular_puntaje;
    dificultad_nueva->verificar_nivel = verificar_nivel;
    dificultad_nueva->verificacion_a_string = verificacion_a_string;

    lista_insertar(lista_difs, dificultad_nueva);
}

void simulador_insertar_difs_basicas(simulador_t* simulador){
    simulador->dificultades_disponibles = lista_crear();
    dificultad_crear_e_insertar(simulador->dificultades_disponibles, "FACIL", 0, verificacion_a_string_facil, calcular_puntaje_facil);
    dificultad_crear_e_insertar(simulador->dificultades_disponibles, "NORMAL", 1, verificacion_a_string_normal, calcular_puntaje_normal);
    dificultad_crear_e_insertar(simulador->dificultades_disponibles, "DIFICIL", 2, verificacion_a_string_dificil, calcular_puntaje_dificil);
}

simulador_t* simulador_crear(hospital_t* hospital){
    if(!hospital) return NULL;

    simulador_t* simulador = calloc(1,sizeof(simulador_t));
    
    simulador->hospital = hospital;
    simulador->sala_espera = hospital->entrenadores;
    simulador->estadisticas.pokemon_totales = (unsigned)hospital_cantidad_pokemon(hospital);
    simulador->estadisticas.entrenadores_totales = (unsigned)hospital_cantidad_entrenadores(hospital);
    
    simulador_insertar_difs_basicas(simulador);
    simulador->datos_dificultad = (DatosDificultad*)lista_elemento_en_posicion(simulador->dificultades_disponibles, 1);
    simulador->id_dificultad = 1;

    simulador->sala_medica = calloc(1, sizeof(sala_medica_t));
    simulador->sala_medica->pokemon_en_espera = heap_crear(comparador_heap_minimal, 1);

    return simulador;
}

void obtener_estadisiticas_sim(simulador_t* simulador, EstadisticasSimulacion* datos, bool* error){
    if(!datos){
        *error = true;
        return;
    }
    datos->puntos = simulador->estadisticas.puntos;
    datos->cantidad_eventos_simulados = simulador->estadisticas.cantidad_eventos_simulados;
    datos->pokemon_totales = simulador->estadisticas.pokemon_totales;
    datos->pokemon_atendidos = simulador->estadisticas.pokemon_atendidos;
    datos->entrenadores_atendidos = simulador->estadisticas.entrenadores_atendidos;
    datos->pokemon_en_espera = simulador->estadisticas.pokemon_en_espera;
    datos->entrenadores_totales = simulador->estadisticas.entrenadores_totales;
}

//----------------------------------------------------------------------ATENDER PROXIMO ENTRENADOR---------------------------------------------------------------------------

pokemon_t* duplicar_pokemon(pokemon_t* pokemon){
	pokemon_t* pokemon_save = calloc(1, sizeof(pokemon_t));
	pokemon_save->nombre = malloc(strlen(pokemon->nombre)+1);
	strcpy(pokemon_save->nombre, pokemon->nombre);
	pokemon_save->nivel = pokemon->nivel;
	pokemon_save->id = pokemon->id;
	pokemon_save->ya_fue_atendido = pokemon->ya_fue_atendido;
	return pokemon_save;
}


bool abb_a_heap(void* elemento, void* heap){
    if(!elemento) return false;
    heap_insertar((heap_t*)heap, elemento);
    return true;
}

void insertar_en_sala_medica(simulador_t* simulador, entrenador_t* entrenador){
    size_t cantidad = abb_tamanio(entrenador->pokemones);
    sala_medica_t* sala_medica = simulador->sala_medica;
    
    sala_medica->nombre_entrenador = entrenador->nombre;
    abb_con_cada_elemento(entrenador->pokemones, POSTORDEN, abb_a_heap, (void*)(sala_medica->pokemon_en_espera));
    simulador->estadisticas.pokemon_en_espera += (unsigned int)cantidad;
    
    if(!sala_medica->pokemon_en_atencion){
        sala_medica->pokemon_en_atencion = (pokemon_t*)heap_extraer_raiz(sala_medica->pokemon_en_espera);
        simulador->estadisticas.pokemon_en_espera -= 1;
        }
    
}

void atender_proximo_entrenador(simulador_t* simulador, bool* error){
    if(simulador->estadisticas.entrenadores_atendidos >= simulador->estadisticas.entrenadores_totales){
        *error = true;
        return;
    }
    lista_iterador_t* it = lista_iterador_crear(simulador->sala_espera);
    bool encontrado = false;
    entrenador_t* entrenador_actual = NULL;

    while(lista_iterador_tiene_siguiente(it) && !encontrado){
        entrenador_actual = lista_iterador_elemento_actual(it);
        if(entrenador_actual){
            if(!entrenador_actual->atendido){
                encontrado = true;
                entrenador_actual->atendido = true;
                simulador->estadisticas.entrenadores_atendidos += 1;
                insertar_en_sala_medica(simulador, entrenador_actual);
            }
        }
            else {
                *error = true;
                return;
            }
        lista_iterador_avanzar(it);
    }
    lista_iterador_destruir(it);
}

//----------------------------------------------------------------------OBTENER INFORMACION POKEMON---------------------------------------------------------------------------

entrenador_t* obtener_entrenador_de_pokemon(lista_t* lista_entrenadores, pokemon_t* pokemon){
    lista_iterador_t* it = lista_iterador_crear(lista_entrenadores);
    bool encontrado = false;
    entrenador_t* entrenador_actual = NULL;

    while(lista_iterador_tiene_siguiente(it) && !encontrado){
        entrenador_actual = lista_iterador_elemento_actual(it);
        if(entrenador_actual){
            if(entrenador_actual->id == pokemon->id){
                encontrado = true;
            }
        }
        lista_iterador_avanzar(it); 
    }
    lista_iterador_destruir(it);
    return entrenador_actual;
}

void ObtenerInformacionPokemon(simulador_t* simulador,InformacionPokemon* datos, bool* error){
     if(!datos){
        *error = true;
        return;
    }
    pokemon_t* pokemon_atendido = simulador->sala_medica->pokemon_en_atencion;
    if(!pokemon_atendido){
        *error = true;
        datos->nombre_entrenador = NULL;
        datos->nombre_pokemon = NULL;
        return;
    }
    entrenador_t* entrenador = obtener_entrenador_de_pokemon(simulador->hospital->entrenadores, pokemon_atendido);
    if(!entrenador){
        *error = true;
        datos->nombre_entrenador = NULL;
        datos->nombre_pokemon = NULL;
        return;
    }
    datos->nombre_entrenador = entrenador->nombre;
    datos->nombre_pokemon = pokemon_atendido->nombre;
}

//----------------------------------------------------------------------ADIVINAR NIVEL POKEMON----------------------------------------------------------------------------------

void adivinar_nivel_pokemon(simulador_t* simulador, Intento* intento, bool* error){
    if(!intento || !simulador->sala_medica->pokemon_en_atencion){
        *error = true;
        return;
    }
    sala_medica_t* sala_medica = simulador->sala_medica;
    int resultado = simulador->datos_dificultad->verificar_nivel(intento->nivel_adivinado, (unsigned)sala_medica->pokemon_en_atencion->nivel);
    intento->resultado_string = simulador->datos_dificultad->verificacion_a_string(resultado);    
    simulador->intentos += 1;

    if(resultado == 0){
        sala_medica->pokemon_en_atencion->ya_fue_atendido = true;
        sala_medica->pokemon_en_atencion = (pokemon_t*)heap_extraer_raiz(sala_medica->pokemon_en_espera);
        simulador->estadisticas.puntos += simulador->datos_dificultad->calcular_puntaje(simulador->intentos);
        simulador->intentos = 0;
        
        simulador->estadisticas.pokemon_atendidos += 1;
        if (simulador->estadisticas.pokemon_en_espera > 0 ){
            simulador->estadisticas.pokemon_en_espera -= 1;
        }
        intento->es_correcto = true;
    }
    else{ 
        intento->es_correcto = false;
    }
}

//----------------------------------------------------------------------AGREGAR DIFICULTAD---------------------------------------------------------------------------------------

bool repetida_en_lista(void* elemento, void* aux){
    const char* nombre_aux = aux;
    DatosDificultad* dificultad = elemento;
    const char* nombre_en_lista = dificultad->nombre;
    if(!elemento || strcmp(nombre_aux,nombre_en_lista) == 0) return false;
    printf("COMPARACION %s vs %s\n",nombre_aux,nombre_en_lista);
    return true;
}

void agregar_dificultad(simulador_t* simulador, DatosDificultad* datos, bool* error){
    if(!datos || !datos->nombre) {
        *error = true;
        return;
    }
    DatosDificultad* nueva = calloc(1, sizeof(DatosDificultad));
    nueva->nombre = duplicar_string(datos->nombre);

    lista_iterador_t* it = lista_iterador_crear(simulador->dificultades_disponibles);
    bool no_repetido = true;
    while(lista_iterador_tiene_siguiente(it) && no_repetido){
        DatosDificultad* actual = lista_iterador_elemento_actual(it);
        if(actual){
            if(strcmp(actual->nombre, datos->nombre) == 0){
                *error = true;
                no_repetido = false;
            }
        }
        lista_iterador_avanzar(it);
    }
    lista_iterador_destruir(it);
    
    if(no_repetido){
        if(datos->verificacion_a_string != NULL){
            nueva->verificacion_a_string = datos->verificacion_a_string;
        }
        else {nueva->verificacion_a_string = verificacion_a_string_facil;}
        if(datos->verificacion_a_string != NULL){
            nueva->verificar_nivel = datos->verificar_nivel;
        }
        else {nueva->verificar_nivel = verificar_nivel;}
        if(datos->calcular_puntaje != NULL){
            nueva->calcular_puntaje = datos->calcular_puntaje;
        }
        else{nueva->calcular_puntaje = calcular_puntaje_dificil;}
        lista_insertar(simulador->dificultades_disponibles, nueva);
    }
    else{
        *error = true;
        free(nueva);
    }
}

//----------------------------------------------------------------------CAMBIAR DIFICULTAD---------------------------------------------------------------------------------------

void cambiar_dificultad(simulador_t* simulador, int* datos, bool* error){
    if(!datos){
        *error = true;
        return;
    }
    simulador->datos_dificultad = lista_elemento_en_posicion(simulador->dificultades_disponibles, (size_t)*datos);
    simulador->id_dificultad = (unsigned)*datos;
    if(!simulador->datos_dificultad) *error = true;
}

//----------------------------------------------------------------------OBTENER INFO DIFICULTAD----------------------------------------------------------------------------------

void obtener_info_dif(simulador_t* simulador, InformacionDificultad* datos, bool* error){
    if(!datos){
        *error = true;
        return;
    }
    DatosDificultad* dificultad_en_lista = lista_elemento_en_posicion(simulador->dificultades_disponibles, (size_t)datos->id);
    if(!dificultad_en_lista){
        datos->nombre_dificultad = NULL;
        datos->id = -1;
        *error = true;
        return;
    }
    datos->nombre_dificultad = dificultad_en_lista->nombre;
    datos->en_uso = (strcmp(dificultad_en_lista->nombre, simulador->datos_dificultad->nombre) == 0 )? true: false;
}

//----------------------------------------------------------------------FINALIZAR SIMULADOR--------------------------------------------------------------------------------------

void fin_sim(simulador_t* simulador, bool* error){
    if(simulador->terminado){
        *error = true;
        return;
    }
    simulador->terminado = true;
}

//----------------------------------------------------------------------SIMULAR EVENTOS------------------------------------------------------------------------------------------

ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
    if(!simulador || simulador->terminado) return ErrorSimulacion;
    bool algo_salio_mal = false;
    simulador->estadisticas.cantidad_eventos_simulados += 1;
    switch(evento){
        case ObtenerEstadisticas:
            obtener_estadisiticas_sim(simulador, (EstadisticasSimulacion*)datos, &algo_salio_mal);
            break;
        case AtenderProximoEntrenador:
            atender_proximo_entrenador(simulador, &algo_salio_mal);
            break;
        case ObtenerInformacionPokemonEnTratamiento:
            ObtenerInformacionPokemon(simulador, (InformacionPokemon*)datos, &algo_salio_mal);
            break;
        case AdivinarNivelPokemon:
            adivinar_nivel_pokemon(simulador, (Intento*)datos, &algo_salio_mal);
            break;
        case AgregarDificultad:
            agregar_dificultad(simulador, (DatosDificultad*) datos, &algo_salio_mal);
            break;
        case SeleccionarDificultad:
            cambiar_dificultad(simulador, (int*)datos, &algo_salio_mal);
            break;
        case ObtenerInformacionDificultad:
            obtener_info_dif(simulador, datos, &(algo_salio_mal));
            break;
        case FinalizarSimulacion:
            fin_sim(simulador, &algo_salio_mal);
            break;
        default:
            algo_salio_mal = true;
            break;
    }
    return (!algo_salio_mal)? ExitoSimulacion: ErrorSimulacion; 
}

//----------------------------------------------------------------------DESTRUCTORES---------------------------------------------------------------------------------------------

void limpiar_sala_medica(sala_medica_t* sala_medica){
    if(!sala_medica) return;
    if(sala_medica->pokemon_en_espera) heap_destruir(sala_medica->pokemon_en_espera);
    free(sala_medica);
}

bool dificultades_destruir(void* elemento, void* contexto){
    if(!elemento) return false;
    DatosDificultad* dificultad = elemento;
    if(dificultad->nombre) {free((void*)dificultad->nombre);}
    free(dificultad);
    return true; 
}

void destruir_dificultades_lista(lista_t* lista){
    if(!lista_vacia(lista)) lista_con_cada_elemento(lista, dificultades_destruir, NULL);
    lista_destruir(lista);
}

void simulador_destruir(simulador_t* simulador){
    if(!simulador) return;
    destruir_dificultades_lista(simulador->dificultades_disponibles);
    limpiar_sala_medica(simulador->sala_medica);
    hospital_destruir(simulador->hospital);
    free(simulador);
}
