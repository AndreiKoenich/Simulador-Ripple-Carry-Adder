/* O programa a seguir simula a operacao de um RIPPLE CARRY ADDER - Andrei Pochmann Koenich, informando tambem quais sao os valores obtidos
por meio de cortes de CARRY IN ou inducoes de CARRY IN ao longo do circuito. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>

#define BASE 10
#define MAXBITS 16
#define MAXBITSESTATISTICA 16
#define MAXBITSEXAUSTIVA 8
#define ESC 27
#define DIMENSAO 1000
#define SEPARADORES " ,.&*%\?!;/-'@\"$#=~><)(][}{:\n\t_"
#define MAXOPERACOES 20000
#define MUDANCA_OR 0
#define MUDANCA_AND 1
#define CODIGO_CORTES 1
#define CODIGO_INDUCOES 2

#include "operacoes_gerais.h"
#include "aleatorias_cortes.h"
#include "aleatorias_inducoes.h"
#include "aleatorias_mudaporta.h"
#include "faixavalores_cortes.h"
#include "faixavalores_inducoes.h"
#include "ieee_faixavalores.h"
#include "especificas.h"
#include "exaustivos.h"
#include "metricas_erros.h"
#include "exaustivo_mudalogica.h"

void inicia_programa() /* Mostra ao usuario as opcoes iniciais do programa. */
{
    while (1) /* Exibe o menu inicial ao usuario. */
    {
        system("cls");

        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nPressione a tecla para escolher a analise desejada:\n");
        printf("\n1 - Analise comum com dois operandos especificos.\n");
        printf("\n2 - Cortes de CARRY IN - Analise estatistica com operandos variaveis e um fixo, em uma faixa de valores.\n");
        printf("\n3 - Cortes de CARRY IN - Analise estatistica com operandos aleatorios.\n");
        printf("\n4 - Inducoes de CARRY IN - Analise estatistica com operandos variaveis e um fixo, em uma faixa de valores.\n");
        printf("\n5 - Inducoes de CARRY IN - Analise estatistica com operandos aleatorios.\n");
        printf("\n6 - Mudancas de Portas Logicas - Analise estatistica com operandos aleatorios.\n");
        printf("\n7 - Analise estatistica com metricas de erro padronizadas.\n");
        printf("\n8 - Analise estatistica exaustiva com todos os operandos possiveis.\n");
        printf("\nESC - Encerrar programa.\n");

        char tecla_escolha = getch();

        switch(tecla_escolha)
        {
            case '1':
                operacoes();
            break;

            case '2':
                cortes_faixavalores();
            break;

            case '3':
                analise_aleatoria_cortes();
            break;

            case '4':
                inducoes_faixavalores();
            break;

            case '5':
                analise_aleatoria_inducoes();
            break;

            case '6':
                analise_aleatoria_mudaporta();
            break;

            case '7':
                analise_metricas();
            break;

            case '8':
                analise_exaustiva();
            break;

            case ESC:
                system("cls");
                return;
            break;
        }
    }
}

int main ()
{
    srand(time(NULL));
    inicia_programa();
    system("pause");
    return 0;
}
