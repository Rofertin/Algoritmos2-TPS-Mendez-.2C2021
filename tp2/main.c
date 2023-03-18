#include "src/hospital_structs.h"
#include "src/hospital.h"
#include "src/simulador_structs.h"
#include "src/simulador.h"
#include "pruebas.c"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define FIN 'q'
#define OBTENER_INFO_DIF 'o'
#define MOSTRAR_STATS 'e'
#define PROXIMO_ENTRENADOR 'p'
#define OBTENER_INFO_POKEMON 'i'
#define ADIVINAR_NIVEL_POKEMON 'a'
#define SELECCIONAR_DIF 'd'
#define AGREGAR_DIF 'n'
#define LIMPIAR_CONSOLA 'c'

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct{
    simulador_t* simulador;
    bool jugando;
}Juego;

void intro(){
    system("clear");
    printf(AMARILLO"\nSIMULADOR POKEMÓN- Agustín de la Rosa\n \n" NORMAL);    
    // acá va un ascii art pero no sé si llego con la fecha
    sleep(2);
}

char* copiar_string(const char* clave){
    char* copia = malloc(strlen(clave) + 1);
    if (!copia) return NULL;
    strcpy(copia, clave);
    return copia;
}

void separador_pantalla(){
    printf("\n");
    printf("---------------------------------------------------------------------------------------------------");
    printf("\n");
}

char leer_comando(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}

int leer_numero(){
    char linea[100];
    char* leido;
    printf(">");
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return atoi(leido);
}

void inicializar_juego(Juego* juego, hospital_t* hospital){
    juego->simulador = simulador_crear(hospital);
    juego->jugando = true;
}

void main_menu(){
    printf(VERDE" OPCIONES DE JUEGO\n" NORMAL);
    char* eventos[4] = {"Obtener estadísticas (E)", "Obtener informacion pokemon en tratamiento (I)", "Atender próximo entrenador (P)", "Adivinar nivel pokemon (A)"};
    for(int i = 0; i < 4; i++)
        printf("%s%s\n", " -", eventos[i]);
    printf("\n");
    
    printf(VERDE" OPCIONES DIFICULTAD\n"NORMAL);
    char* eventos2[3] = {"Obtener información dificultad (O)","Agregar dificultad (N)", "Seleccionar dificultad (D)"};
    for(int i = 0; i < 3; i++)
        printf("%s%s\n", " -", eventos2[i]);
    printf("\n");

    printf(VERDE" OPCIONES UTILIDAD\n"NORMAL);
    char* eventos3[2] = {"Limpiar pantalla (C)","Finalizar simulacion (Q)"};
    for(int i = 0; i < 2; i++)
        printf("%s%s\n", " -", eventos3[i]);
    printf("\n");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

unsigned calcular_puntaje_creativo(unsigned intentos){
    return 10*intentos;
}

const char* verificacion_a_string_creativo(int resultado){
    return "Siempre es correcto en creativo.";
}

int verificar_nivel_creativo(unsigned intento, unsigned real){
    return 0; //siempre es correcto
}

void agregar_dificultad_creativo(Juego* juego){
    DatosDificultad dif_nueva;
    dif_nueva.nombre = "CREATIVO";
    dif_nueva.verificacion_a_string = verificacion_a_string_creativo;
    dif_nueva.verificar_nivel = verificar_nivel_creativo;
    dif_nueva.calcular_puntaje = calcular_puntaje_creativo;
    ResultadoSimulacion r = simulador_simular_evento(juego->simulador, AgregarDificultad, &dif_nueva);
    printf("%s",r?"Agregada dificultad [CREATIVO]":"Error, dificultad repetida");
}

unsigned calcular_puntaje_profesional(unsigned intetos){
    if(intetos > 2) return 0;
    return 1000/intetos;
}

const char* verificacion_a_string_profesional(int resultado){
    return "Capaz adivinaste, capaz no, ¿quien sabe?...";
}

int verificar_nivel_profesional(unsigned intento, unsigned real){
    int diferencia = (int)real - (int)intento;
    if(real == intento)
        return 0;
    return diferencia;
}

void agregar_dificultad_profesional(Juego* juego){
    DatosDificultad dif_nueva;
    dif_nueva.nombre = "PROFESIONAL";
    dif_nueva.verificacion_a_string = verificacion_a_string_profesional;
    dif_nueva.verificar_nivel = verificar_nivel_profesional;
    dif_nueva.calcular_puntaje = calcular_puntaje_profesional;
    ResultadoSimulacion r = simulador_simular_evento(juego->simulador, AgregarDificultad, &dif_nueva);
    printf("%s",r?"Agregada dificultad [PROFESIONAL]":"Error, dificultad repetida");
}

void agregar_dif(Juego* juego){
    if(!juego) return;
    printf(" Elija su dificultad: \n [PROFESIONAL]: 1 \n [CREATIVO]: 2\n");
    int numero = leer_numero();
    if(numero == 1){
        agregar_dificultad_profesional(juego);
        return;
    }
    if (numero == 2){
        agregar_dificultad_creativo(juego);
        return;
    }
    printf("Numero incorrecto.");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void mostrar_estadisticas(Juego* juego){
    if(!juego) return;
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);

    printf(VERDE"[DATOS HOSPITAL]\n"NORMAL);
    printf("Entrenadores totales: %i \n",e.entrenadores_totales);
    printf("Pokemones totales: %i \n",e.pokemon_totales);
    
    printf(VERDE"\n[DATOS SIMULADOR]\n"NORMAL);
    printf("Cantidad eventos simulados: %i \n",e.cantidad_eventos_simulados);
    printf("Entrenadores atendidos: %i\n",e.entrenadores_atendidos);
    printf("Pokemones atendidos: %i\n",e.pokemon_atendidos);
    printf("Puntos acumulados: %i\n",e.puntos);

    printf(VERDE"\n[DATOS SALA DE ESPERA]\n"NORMAL);
    printf("Pokemones en sala de espera: %i\n",e.pokemon_en_espera);
    
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool imprimir_difs_en_lista(void* elemento, void* aux){
    if(!elemento) return false;
    int* id = aux;
    DatosDificultad* dif = elemento;
    printf("ID: %i [%s]\n",*id,dif->nombre);
    *id += 1;
    return true;
}

void seleccionar_dif(Juego* juego){
    int id = 0;
    lista_con_cada_elemento(juego->simulador->dificultades_disponibles, imprimir_difs_en_lista, &id);
    int datos = leer_numero();
    ResultadoSimulacion r = simulador_simular_evento(juego->simulador, SeleccionarDificultad, &datos);
    if(r == ErrorSimulacion) printf("Dificultad no encontrada. ID INCORRECTO\n");
    else printf("Dificultad [%s] elegida correctamente\n",juego->simulador->datos_dificultad->nombre);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void primer_entrenador(Juego* juego){
    simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL);
    char* nombre_entrenador_nuevo = juego->simulador->sala_medica->nombre_entrenador;
    printf("Primer entrenador es: [%s]\n",nombre_entrenador_nuevo);
}

void proximo_entrenador(Juego* juego){
    if(!juego) return;
    char* nombre_entrenador_viejo = NULL;
    if(!juego->simulador->sala_medica->nombre_entrenador) {
        primer_entrenador(juego);
        return;
    }
    nombre_entrenador_viejo = copiar_string(juego->simulador->sala_medica->nombre_entrenador);
    ResultadoSimulacion r = simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL);
    char* nombre_entrenador_nuevo = juego->simulador->sala_medica->nombre_entrenador;
    if(r == ErrorSimulacion) {
        printf(ROJO"No quedan más entrenadores.\n"NORMAL);
        free(nombre_entrenador_viejo);
        return;
    }

    printf("[%s] -> [%s]\n",nombre_entrenador_viejo,nombre_entrenador_nuevo);
    printf("Cambio de entrenador correcto.\n");
    free(nombre_entrenador_viejo);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void obtener_info_dificultad(Juego* juego){
    InformacionDificultad i;
    simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &i);
    printf("OBTENER INFO DIFICULTAD \n");
    printf("ID: %i Nombre: [%s] \n", i.id,i.nombre_dificultad);
    printf("en uso-> %s \n",(i.en_uso)?"Si":"No");
    free((void*)i.nombre_dificultad);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void obtener_info_pok(Juego* juego){
    if(!juego) return;
    InformacionPokemon pokemon;
    simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &pokemon);
    
    printf("POKEMON EN SALA MÉDICA: %s. \n",pokemon.nombre_pokemon?pokemon.nombre_pokemon:"Ninguno actualmente");
    printf("ENTRENADOR EN SALA MÉDICA: %s. \n",pokemon.nombre_entrenador?pokemon.nombre_entrenador:"Ninguno actualmente");

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void adivinar_pokemon(Juego* juego){
    if(!juego) return;
    if(!juego->simulador->sala_medica->pokemon_en_atencion){
        printf(ROJO"No hay pokemon en atención.\n\n"NORMAL);
        printf("Primero atienda un entrenador.\n");
        return;
    }
    Intento intento;
    intento.resultado_string = NULL;
    printf("[%s]",juego->simulador->sala_medica->nombre_entrenador);
    printf("\nPokemon a adivinar: %s\n", juego->simulador->sala_medica->pokemon_en_atencion->nombre);
    intento.nivel_adivinado = (unsigned)leer_numero();
    printf("\n");
    
    simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &intento);
    printf("-%s\n",intento.resultado_string);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void limpiar_consola(){
    system("clear");
    printf("\n");
    printf(AMARILLO"\nSIMULADOR POKEMON"NORMAL);
    separador_pantalla();
    main_menu();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void destruir_juego(Juego* juego){
    if(juego) simulador_destruir(juego->simulador);
    juego->jugando = false;
}

void cerrar_todo(Juego* juego){
    if(!juego || !juego->simulador) return;
    simulador_simular_evento(juego->simulador, FinalizarSimulacion, NULL);
    destruir_juego(juego);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void ejecutar_comando(Juego *juego, char comando){
    switch (comando) {
        case FIN: //Salir del juego
            cerrar_todo(juego);
            break;
        case OBTENER_INFO_DIF:
            obtener_info_dificultad(juego);
            break;
        case AGREGAR_DIF:
            agregar_dif(juego);
            break;
        case SELECCIONAR_DIF:
            seleccionar_dif(juego);
            break;
        case OBTENER_INFO_POKEMON:
            obtener_info_pok(juego);
            break;
        case MOSTRAR_STATS:
            mostrar_estadisticas(juego);
            break;
        case PROXIMO_ENTRENADOR:
            proximo_entrenador(juego);
            break;
        case ADIVINAR_NIVEL_POKEMON:
            adivinar_pokemon(juego);
            break;
        case LIMPIAR_CONSOLA:
            limpiar_consola();
            break;
        default: //el resto de las cosas
            printf("\n COMANDO INVALIDO \n");
            break;
    }
}

hospital_t* hospital_varios_entrenadores(){
    hospital_t* hospital = hospital_crear();
    if(!hospital) return 0;
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");
    return hospital;
}

void simular_hospital(hospital_t* hospital){
    Juego juego;
    inicializar_juego(&juego, hospital);
    intro();
    limpiar_consola();
    while(juego.jugando){
        printf("> ");
        char c = leer_comando();
        printf("\n");
        ejecutar_comando(&juego, c);
        separador_pantalla();
    }
}

int main(int argc, char *argv[]) {
    pruebas_TP1();
    pruebas_TP2();

    printf("\nInserte S para pasar al simulador, cualquier otra tecla cerrará el programa...");
    printf("\n>");
    char avanzar = leer_comando();
    if(avanzar == 's'){ 
        hospital_t* h = hospital_varios_entrenadores();
        simular_hospital(h);
    }
    return 0;
}
