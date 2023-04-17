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
#include "metricas_erros.h"

void calcula_metricas(int codigo_interferencia)
{
    Operacao parametros;
    inicializa_parametros(&parametros);
    int interferencias[parametros.bits];
    interferencias_metricas(interferencias,parametros.bits, codigo_interferencia);

    long double ER = 0, MAE = 0, MSE = 0, RMSE = 0, relativo_parcial = 0, soma_relativos = 0, media_relativos = 0;

    int diferentes = 0;
    int saida_exata[parametros.bits];

    long int decimal_1 = 0, decimal_2 = 0;
    long int valor_max = (long int)pow(2,parametros.bits);
    long int total_operacoes = recebe_total(valor_max);
    long int decimal_exato = 0, decimal_interf = 0;
    long int erro_parcial = 0;
    long int amostra_erros[total_operacoes];

	for (int i = 0; i < total_operacoes; i++) /* Iteracao para calculo de media aritmetica. */
    {
		decimal_1 = rand() % valor_max;
		decimal_2 = rand() % valor_max;

		converte_binario(decimal_1,parametros.bits,parametros.n1);
		converte_binario(decimal_2,parametros.bits,parametros.n2);

		saida_comum(parametros,saida_exata);
        decimal_exato = calcula_decimal(saida_exata,parametros.bits);

        if (codigo_interferencia == CODIGO_CORTES)
            decimal_interf = calcula_saidacortes(&parametros, interferencias);

        else if (codigo_interferencia == CODIGO_INDUCOES)
            decimal_interf = calcula_saidainducoes(parametros, interferencias);

        if (decimal_exato != decimal_interf) /* Calculo de ER. */
            diferentes++;

        erro_parcial = calcula_erroabsoluto(decimal_exato,decimal_interf); /* Calcula o erro absoluto da vez. */
        amostra_erros[i] = erro_parcial;

        MAE += (long double)erro_parcial/(long double)total_operacoes;
        MSE += (long double)erro_parcial*(long double)erro_parcial/(long double)total_operacoes;

        if (decimal_exato == 0)
            relativo_parcial = (long double)erro_parcial;

        else
            relativo_parcial = ((long double)erro_parcial/(long double)decimal_exato);

        soma_relativos += relativo_parcial;

        //__mingw_printf("%0.4Lf\n", relativo_parcial);
    }

    RMSE = sqrt(MSE);
    ER = (long double)diferentes/(long double)total_operacoes;
    media_relativos = soma_relativos/(long double)total_operacoes;

    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
    printf("\nRESULTADOS DAS METRICAS DE ERRO PADRONIZADAS:\n");
    //printf("\nDiferentes:\n%d\n", diferentes);
    __mingw_printf("\nER - Error Rate:\n%0.4Lf\n",ER);
    __mingw_printf("\nMAE - Mean Absolute Error:\n%0.4Lf\n",MAE);
    calcula_desviopadrao(amostra_erros, MAE, total_operacoes);
    __mingw_printf("\nMSE - Mean Squared Error:\n%0.4Lf\n", MSE);
    __mingw_printf("\nRMSE - Root Mean Square Error:\n%0.4Lf\n",RMSE);
    __mingw_printf("\nMedia Aritmetica dos Erros Relativos:\n%0.4Lf\n", media_relativos);

    printf("\n\nPressione qualquer tecla para retornar ao menu inicial.\n");
    getch();
}

void analise_metricas()
{
    char tecla;

    while (1)
    {
        system("cls");

        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nESCOLHA DO TIPO DE INTERFERENCIA\n");
        printf("\nEscolha entre realizar cortes ou inducoes de CARRY IN em cada operacao:\n");
        printf("\n1 - Cortes de CARRY IN\n");
        printf("\n2 - Inducoes de CARRY IN\n");

        tecla = getch();

        if (tecla == '1')
        {
            calcula_metricas(CODIGO_CORTES);
            break;
        }

        else if (tecla == '2')
        {
            calcula_metricas(CODIGO_INDUCOES);
            break;
        }
    }
}
