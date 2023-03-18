#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador_structs.h"
#include "src/simulador.h"

#include "string.h"
#include <stdbool.h>

bool ignorar_pokemon(pokemon_t* p){
    p = p;
    return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct{
    pokemon_t* pokemon[500];
    size_t cantidad;
} acumulados;

void resetear_acumulados(){
    acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto(){
    if(acumulados.cantidad < 2)
        return true;
    pokemon_t* anterior = acumulados.pokemon[0];
    for(int i=1;i<acumulados.cantidad;i++){
        pokemon_t* actual =  acumulados.pokemon[i];
        if(strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
            return false;
    }
    return true;
}

/* Pruebas */

void puedoCrearYDestruirUnHospital(){
    hospital_t* h=NULL;

    pa2m_afirmar((h=hospital_crear()), "Crear un hospital devuelve un hospital");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital se crea con cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital se crea con cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema(){
    hospital_t* h=NULL;

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital NULL tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital NULL tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital vacío tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital vacío tiene tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==1, "El hospital tiene 1 entrenador");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==3, "El hospital tiene 3 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==5, "El hospital tiene 5 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==24, "El hospital tiene 24 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==15, "El hospital tiene 15 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==72, "El hospital tiene 72 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void Pruebas_extras_TP1(){
    hospital_t* h=hospital_crear();
    
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_a_cada_pokemon(h, NULL)==0, "Recorrer los pokemon con funcion null da 0"); // extra agregada
    
    hospital_destruir(h);
}

void pruebas_TP1(){

    printf(AMARILLO"PRUEBAS TP1\n"NORMAL);
    pa2m_nuevo_grupo("Pruebas de  creación y destrucción");
    puedoCrearYDestruirUnHospital();

    pa2m_nuevo_grupo("Pruebas con NULL");
    dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

    pa2m_nuevo_grupo("Pruebas con un archivo vacío");
    dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
    dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
    dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con mas de un archivo");
    dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();

    pa2m_nuevo_grupo("Pruebas extras mías (TP1)");
    Pruebas_extras_TP1();

    printf("\n");
}

//-------------------------------------------------------------------------PRUEBAS_SIMULADOR-----------------------------------------------------------------------

void crear_una_simulacion_con_un_hospital_vacio(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h,NULL);
    
    ResultadoSimulacion r = false;
    simulador_t* simulador = simulador_crear(h);
    pa2m_afirmar(simulador,"Puedo crear un simulador con un hospital vacío");
    
    EstadisticasSimulacion e;
    r = simulador_simular_evento(simulador, ObtenerEstadisticas, &e);

    pa2m_afirmar(r == ExitoSimulacion,"Obtener estadísticas devuelve ExitoSimulacion");
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 1, "La cantidad de eventos simulados es 1");

    InformacionDificultad i;
    InformacionDificultad* i_ptr = &i;
    i.id = 0;
    r = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &i);
    pa2m_afirmar(r == ExitoSimulacion,"Obtener la informacion de la dificultad 0 devuelve ExitoSimulacion");
    pa2m_afirmar(i_ptr != NULL,"Los datos de la dificultad 0 son existentes");    


    simulador_destruir(simulador);
}

void crearhospitalconvariosentrenadores(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h,"");

    ResultadoSimulacion r = ExitoSimulacion;
    simulador_t* simulador = simulador_crear(h);

    r = simulador_simular_evento(simulador, ObtenerInformacionDificultad, NULL);
    pa2m_afirmar(r == ErrorSimulacion,"ObtenerInformacionDificultad con puntero NULL devuelve ErrorSimulacion");
    
    r = simulador_simular_evento(simulador, AgregarDificultad, NULL);
    pa2m_afirmar(r == ErrorSimulacion,"AgregarDificultad con puntero NULL devuelve ErrorSimulacion");
    
    simulador_destruir(simulador);
}

void crearsimulacionparapunterosnulls(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h,"ejemplos/varios_entrenadores.hospital");

    ResultadoSimulacion r = ExitoSimulacion;
    simulador_t* simulador = simulador_crear(h);

    r = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    pa2m_afirmar(r == ExitoSimulacion,"Atender proximo entrenador devuelve ExitoSimulación");
    
    InformacionPokemon i;
    r = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &i);
    pa2m_afirmar(r == ExitoSimulacion,"Obtener info pokemon devuelve ExitoSimulación");

    pa2m_afirmar(strcmp(simulador->sala_medica->pokemon_en_atencion->nombre,"rampardos")==0,"El pokemon que está siendo atendido sigue siendo rampardos por mas que haya uno de menor nivel");
    
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 2,"La cantidad de eventos simulados es 2");

    r = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    pa2m_afirmar(r == ExitoSimulacion,"Atender proximo entrenador devuelve ExitoSimulación");

    EstadisticasSimulacion e;
    r = simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
    pa2m_afirmar(r == ExitoSimulacion,"Obtener estádisticas pokemon devuelve ExitoSimulación");

    pa2m_afirmar(e.entrenadores_atendidos == 2,"La cantidad de entrenadores atendidos es 2");

    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 4,"La cantidad de eventos simulados es 4");

    simulador_destruir(simulador);
}

void adivinarnivel(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h,"ejemplos/varios_entrenadores.hospital");
    
    simulador_t* simulador = simulador_crear(h);
    pa2m_afirmar(simulador,"Puedo Crear un simulador");
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 0,"La cantidad de eventos simulados es 0");

    simulador_destruir(simulador);
}

void eventosinexistentes(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h,NULL);
    
    ResultadoSimulacion r = false;
    simulador_t* simulador = simulador_crear(h);
    
    int hola;
    r = simulador_simular_evento(simulador, 25, &hola);
    pa2m_afirmar(r == ErrorSimulacion, "Un evento que no existe debe retornar ErrorSimulacion");
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 1,"La cantidad de eventos simulados es 1");

    simulador_destruir(simulador);
}

void pruebas_TP2(){
    printf(AMARILLO"\nPRUEBAS TP2\n"NORMAL);
    pa2m_nuevo_grupo("Crear una simulación con un hospital vacío");
    crear_una_simulacion_con_un_hospital_vacio();

    pa2m_nuevo_grupo("Crear una simulación con un hospital con varios entrenadores");
    crearhospitalconvariosentrenadores();

    pa2m_nuevo_grupo("Adivinar Nivel");
    adivinarnivel();
    
    pa2m_nuevo_grupo("Pruebas con datos erróneos y NULLs");
    eventosinexistentes();

    printf("\n"); 
}