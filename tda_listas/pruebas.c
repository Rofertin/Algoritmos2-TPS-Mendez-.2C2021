#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include "pa2mm.h"

void creounapilavacia_y_devuelvonull(){
  pila_t* pila = pila_crear();
  pa2m_afirmar(pila_vacia(pila)==true, "Pila vacia, vacia = True");
  pa2m_afirmar(pila_tope(pila)==NULL, "Pila vacía, tope = NULL");
  pa2m_afirmar(pila_tamanio(pila)==0, "Pila vacia, tamaño = 0");
  pila_destruir(pila);
}

void creounacolavacia_y_devuelvonull(){
  cola_t* cola = cola_crear();
  pa2m_afirmar(cola_vacia(cola)==true, "Cola vacia, vacia = True");
  pa2m_afirmar(cola_frente(cola)==NULL, "Cola vacía, tope = NULL");
  pa2m_afirmar(cola_tamanio(cola)==0, "Cola vacia, tamaño = 0");
  cola_destruir(cola);
}

// int main() {
//   pa2m_nuevo_grupo("Pruebas.c");
//   creounapilavacia_y_devuelvonull();


//   return pa2m_mostrar_reporte();
// }

