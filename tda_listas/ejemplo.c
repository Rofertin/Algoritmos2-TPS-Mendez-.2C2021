#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"

#include "pruebas.c"

#include <stdio.h>
#include <stdlib.h>

bool mostrar_elemento(void* elemento, void* contador){
    if(elemento && contador)
        printf("Elemento %i: %c \n", (*(int*)contador)++, *(char*)elemento);
    return true;
}

void probar_operaciones_lista(){
    lista_t* lista = lista_crear();
    char a='a',b='b', c='c', d='d', w='w';
  
    lista_insertar(lista, &a);
    lista_insertar(lista, &c);
    lista_insertar_en_posicion(lista, &d, 2);
    lista_insertar_en_posicion(lista, &b, 1);
    lista_insertar_en_posicion(lista, &w, 3);
    lista_insertar_en_posicion(lista, &w, 3);
    lista_insertar_en_posicion(lista, &w, 3);
    
    printf("\nCantidad de elementos: %li\n",lista_tamanio(lista));
    printf("Elementos en la lista: ");
    for(size_t i=0; i<lista_tamanio(lista); i++){
        if (lista_elemento_en_posicion(lista, i))
            printf("%c  ", *(char*)lista_elemento_en_posicion(lista, i));
    }
    printf("\n");

    char *valor_quitado = lista_quitar_de_posicion(lista, 3);
    printf("Elementos quitado de la posicion 3 de la lista: %c\n", *valor_quitado);
    
    printf("Primer elemento: %c\n",*(char*)lista_primero(lista));
    printf("Último elemento: %c\n",*(char*)lista_ultimo(lista));
    
    valor_quitado = lista_quitar(lista);
    printf("Elementos quitado de la última posición de la lista: %c\n", *valor_quitado);
    
    printf("\nCantidad de elementos: %li\n",lista_tamanio(lista));
   
    printf("Elementos en la lista: ");
    for(size_t i=0; i<lista_tamanio(lista); i++)
        printf("%c ", *(char*)lista_elemento_en_posicion(lista, i));
    printf("\n");
    printf("Primer elemento: %c\n",*(char*)lista_primero(lista));
    printf("Último elemento: %c\n",*(char*)lista_ultimo(lista));

    printf("\n\n");
    
    printf("Imprimo la lista usando el iterador externo: \n");
    lista_iterador_t* it = NULL;

    for(it = lista_iterador_crear(lista); lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it))
        printf("%c ",*(char*)lista_iterador_elemento_actual(it));
    printf("\n\n");

    lista_iterador_destruir(it);
    
    int contador=0;
    size_t elementos_recorridos = 0;
    printf("Imprimo la lista usando el iterador interno: \n");
    elementos_recorridos = lista_con_cada_elemento(lista, mostrar_elemento, (void*)&contador);

    printf("Recorri %lu elementos con el iterador interno y sume %i elementos\n", elementos_recorridos, contador);
    
    printf("\n");
    lista_destruir(lista);

    // OPERACIONES CON 1 ELEMENTO
    lista = lista_crear();
    int num = 24;
    
    printf("\nMIS PUREBAS:\n");
    printf("La lista deberia estar vacia, lista->cantidad: %li\n", lista_tamanio(lista));
    lista_insertar(lista,&num);
    printf("La lista NO deberia estar vacia, lista->cantidad: %li\n", lista_tamanio(lista));
    lista_quitar_de_posicion(lista,10);
    lista_insertar(lista,&num);
    lista_quitar(lista);
    printf("La lista deberia estar vacia, lista->cantidad: %li\n", lista_tamanio(lista));
    if(lista_vacia(lista))
        printf("Lista esta vacía\n");
    lista_destruir(lista);
    printf("\n\n");
}

void probar_operaciones_cola(){
    cola_t* cola = cola_crear();

    int numeros[]={1,2,3,4,5,6};

    for(size_t i=0; i<sizeof(numeros)/sizeof(int); i++){
        printf("Encolo %i\n", numeros[i]);
        cola_encolar(cola, &numeros[i]);
    }
    printf("\nTamaño de la cola: %li\n",cola_tamanio(cola));
    printf("\nDesencolo los numeros y los muestro: ");
    while(!cola_vacia(cola)){
        printf("%i ", *(int*)cola_frente(cola));
        cola_desencolar(cola);
    }
    printf("\nCola destruida\n");
    cola_destruir(cola);
}

void probar_operaciones_pila(){
    pila_t* pila = pila_crear();
    char* algo="somtirogla";

    for(int i=0; algo[i]!= 0; i++){
        printf("Apilo %c\n", algo[i]);
        pila_apilar(pila, &algo[i]);
    }
    
    printf("\nTamaño de la pila: %li\n",pila_tamanio(pila));

    printf("\nDesapilo y muestro los elementos apilados: ");
    while(!pila_vacia(pila)){
        printf("%c", *(char*)pila_tope(pila));
        pila_desapilar(pila);
    }
    printf("\n");
    pila_destruir(pila);
    printf("Pila destruida\n");
}

int main(){
    
    pa2m_nuevo_grupo("PRUEBAS CON PILA");
    creounapilavacia_y_devuelvonull();
    
    printf("\nEjemplo del uso de pila\n");
    probar_operaciones_pila();

    
    pa2m_nuevo_grupo("PRUEBAS CON COLA");
    creounacolavacia_y_devuelvonull();

    printf("\nEjemplo del uso de cola\n");
    probar_operaciones_cola();

    
    pa2m_nuevo_grupo("PRUEBAS CON LISTA");
    printf("Ejemplo del uso de lista\n");
    probar_operaciones_lista();
    
    return 0;
}
