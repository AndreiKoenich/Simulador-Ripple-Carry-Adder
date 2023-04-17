#ifndef METRICAS_ERROS_H_INCLUDED
#define METRICAS_ERROS_H_INCLUDED

void interferencias_metricas(int interferencias[], int bits, int codigo_interferencia); /* Obtem do usuario os bits em que ocorrerao interferencias de CARRY IN fixos. */

void calcula_metricas(int codigo_interferencia);

void analise_metricas();

#endif // METRICAS_ERROS_H_INCLUDED
