#include "src/abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"
#include "ejemplo.c"


int main(){
  pa2m_nuevo_grupo("Pruebas de ABB");
  executable();
  return pa2m_mostrar_reporte();
}
