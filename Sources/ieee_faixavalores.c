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
#include "ieee_faixavalores.h"

void operacoesIEEE_faixavalores(Operacao *parametros, int cortes[]) /* Realiza a analise estatistica segundo os parametros do artigo da IEEE. */
{
    int saida_exata[parametros->bits];

    inicializa_vetor(parametros->n1,parametros->bits);
    inicializa_vetor(saida_exata,parametros->bits);

    int quantidade_somas = (int)pow(2,parametros->bits);
    long double probabilidade = 1/(long double)quantidade_somas;

    long int saida_parcial = 0;
    long int erro_parcial = 0;
    long int soma_exata = 0;

    //long double ER = 0;
    long double MED = 0;
    long double MRED = 0;
    long double MSE = 0;
    long double RMSE = 0;

    for (int i = 0; i < quantidade_somas; i++)
    {
        saida_comum(*parametros,saida_exata); /* Calcula a saida com o operando binario menor atualizado. */
        soma_exata = calcula_decimal(saida_exata,parametros->bits);
        inicializa_vetor(saida_exata,parametros->bits); /* Reinicia o vetor contendo o valor da soma sem cortes, para cada iteracao. */

        saida_parcial = calcula_saidacortes(parametros,cortes); /* Recebe o valor decimal da operacao com cortes. */
        erro_parcial = calcula_erroabsoluto(saida_parcial,soma_exata); /* Calcula o erro absoluto da vez. */

        MED += (long double)erro_parcial*probabilidade;
        MSE += (long double)erro_parcial*(long double)erro_parcial*probabilidade;

        if (soma_exata == 0)
            soma_exata = 1;

        MRED += ((long double)erro_parcial)/soma_exata;

        soma_um(parametros); /* Atualiza o operando binario menor, para a proxima iteracao. */
    }

    MRED /= (long double)quantidade_somas;
    RMSE = sqrt(MSE);

    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
    printf("\nRESULTADOS DAS OPERACOES PROPOSTAS NO ARTIGO DA IEEE:\n");
    __mingw_printf("\nMED - Mean Error Distance:\n%0.4Lf\n",MED);
    __mingw_printf("\nMRED - Mean Relative Error Distance:\n%0.4Lf\n",MRED);
    __mingw_printf("\nMSE - Mean Squared Error:\n%0.4Lf\n", MSE);
    __mingw_printf("\nRMSE - Root Mean Square Error:\n%0.4Lf\n",RMSE);

    printf("\n\nPressione qualquer tecla para retornar ao menu inicial.\n");
    getch();
}

void analiseIEEE_faixavalores() /* Inicia a analise estatistica segundo os parametros do artigo da IEEE. */
{
    Operacao parametros;
    parametros.bits = recebe_bits_estatistica(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */
    parametros.n1 = calloc(parametros.bits,sizeof(int));
    parametros.n2 = calloc(parametros.bits,sizeof(int));

    int digito;
    int erro_insercao = 0;

    do
    {
        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nOPERANDO BINARIO FIXO\n");
        printf("\nDigite o operando binario fixo de %d bits:\n", parametros.bits);

        erro_insercao = 0;

        for (int i = 0; i < parametros.bits; i++) /* Obtem o operando binario fixo. */
        {
            digito = (int)getch() - '0';
            fflush(stdin);
            printf("%d", digito);
            parametros.n2[i] = digito;
        }

        if (controla_binario(parametros.n2,parametros.bits)) /* Verifica operandos invalidos. */
        {
            erro_insercao = 1;
            system("cls");
            printf("Um ou mais operandos foram mal inseridos.\n");
            printf("\nPressione qualquer tecla para inserir os operandos novamente.\n");
            getch();
            system("cls");
        }

    } while (erro_insercao != 0);

    system("cls");
    int cortes[parametros.bits];
    interferencias_estatisticas(cortes,parametros.bits);
    operacoesIEEE_faixavalores(&parametros,cortes); /* Realiza a analise estatistica segundo os parametros do artigo da IEEE. */
    free(parametros.n1);
    free(parametros.n2);
}
