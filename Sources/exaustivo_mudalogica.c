#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>

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
#define DIMENSAO_LINHA 20
#define LIMITELINHA 2
#define INICIOASCIILETRAS 55
#define INICIOASCIINUMEROS 48

#include "operacoes_gerais.h"
#include "exaustivo_mudalogica.h"
#include "exaustivos.h"

void inverte_vetor(int v[], int bits)
{
    int aux[bits];
    copia_vetor(v,aux,bits);

    for (int i = 0; i < bits; i++)
        v[bits-1-i] = aux[i];
}

void encerra_programa()
{
    system("cls");
    printf("Arquivo texto de entrada invalido. Fim do programa.\n");
    getch();
    exit(1);
}

void converte_hexbin(char valor_hex[], int valor_binario[], int bits)
{
    int valor_decimal = 0, indice_aux = 0;

    for (int i = strlen(valor_hex)-1; i >= 0; i--)
    {
        if (!isalnum(valor_hex[i]) || islower(valor_hex[i]))
            encerra_programa();

        if (isupper(valor_hex[i]))
            valor_decimal += (valor_hex[i] - INICIOASCIILETRAS)*pow(16,indice_aux);

        else
            valor_decimal += (valor_hex[i] - INICIOASCIINUMEROS)*pow(16,indice_aux);

        indice_aux++;
    }

   // printf("%s\n", valor_hex);

    converte_binario(valor_decimal,bits,valor_binario);
    inverte_vetor(valor_binario, bits);
    imprime_vetor(valor_binario,bits);
    printf("\n");
}

void saidacomum_mudalogica(Operacao parametros, int saida_exata[], int **logica_saida, int **logica_carry)
{
    int carry = 0;
    int indice_decimal = 0;
    int indice[3] = {0,0,0};

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        indice[0] = parametros.n1[i];
        indice[1] = parametros.n2[i];
        indice[2] = carry;

        indice_decimal = calcula_decimal(indice,3);
        printf("INDICE:\n%d\n", indice_decimal);

        saida_exata[i] = logica_saida[parametros.bits-1-i][indice_decimal];
      //  carry = logica_carry[parametros.bits-1-i][indice_decimal];
    }
}

long int interferencias_mudalogica(Operacao parametros, int interferencias[], int **logica_saida, int **logica_carry, int codigo_interferencia)
{
    int carry = 0;
    int indice_decimal = 0;
    int indice_aux = 0;
    int indice[3] = {0,0,0};
    int saida_exata[parametros.bits];
    long int decimal_final = 0;

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        indice[0] = parametros.n1[i];
        indice[1] = parametros.n2[i];

        if (codigo_interferencia == CODIGO_CORTES && (contem_vetor(interferencias,indice_aux,parametros.bits)))
            carry = 0;

        else if (codigo_interferencia == CODIGO_INDUCOES && (contem_vetor(interferencias,indice_aux,parametros.bits)))
            carry = 1;

        indice[2] = carry;

        indice_decimal = (int)calcula_decimal(indice,3);

        saida_exata[i] = logica_saida[parametros.bits-1-i][indice_decimal];
        carry = logica_carry[parametros.bits-1-i][indice_decimal];

        indice_aux++;
    }

    decimal_final = calcula_decimal(saida_exata,parametros.bits);
    return decimal_final;
}

void mudalogica_exaustivos(Operacao parametros,int **logica_saida, int **logica_carry)
{
    long double ER = 0, MAE = 0, relativo_parcial = 0, soma_relativos = 0, media_relativos = 0;

    int total_operacoes = (int)pow(2,parametros.bits*2);

    int bits_normal[parametros.bits];
    inicializa_vetor(bits_normal,parametros.bits);
    int bits_mudalogica[parametros.bits];
    inicializa_vetor(bits_mudalogica,parametros.bits);

    long int decimal_mudalogica = 0, decimal_exato = 0;
    long int erro_parcial = 0;
    long int diferentes = 0;
    long int amostra_erros[total_operacoes];

    for (int i = 0; i < total_operacoes; i++)
        amostra_erros[i] = 0;

    long int somas_erros = 0;

    printf("Calculando...\n");

    int indice_aux = 0;
    int aux_parametro = 0;

    int dimensao_combinacoes = parametros.bits*2;
    int operandos[dimensao_combinacoes];
    inicializa_vetor(operandos,dimensao_combinacoes);

    for (int i = 0; i < total_operacoes; i++)
    {

        for (int j = 0; j < parametros.bits; j++)
            parametros.n1[j] = operandos[j];

        aux_parametro = 0;

        for (int j = parametros.bits; j < dimensao_combinacoes; j++)
        {
            parametros.n2[aux_parametro] = operandos[j];
            aux_parametro++;
        }

        saida_comum(parametros,bits_normal);
        decimal_exato = calcula_decimal(bits_normal,parametros.bits);

        printf("\nOPERANDO 1:\n");
        imprime_vetor(parametros.n1,parametros.bits);
        printf("\nOPERANDO 2:\n");
        imprime_vetor(parametros.n2,parametros.bits);
        printf("\nDIMENSAO DOS OPERANDOS:\n%d\n", parametros.bits);

        saidacomum_mudalogica(parametros,bits_mudalogica,logica_saida,logica_carry); /* CRASHANDO AQUI PARA ALGUNS BITS */
        decimal_mudalogica = calcula_decimal(bits_mudalogica,parametros.bits);

        saida_comum(parametros,bits_normal);
        decimal_exato = calcula_decimal(bits_normal,parametros.bits);

        saidacomum_mudalogica(parametros,bits_mudalogica,logica_saida,logica_carry);
        decimal_mudalogica = calcula_decimal(bits_mudalogica,parametros.bits);

        // imprime_vetor(bits_mudalogica, parametros.bits);
        // printf("\n");

        erro_parcial = calcula_erroabsoluto(decimal_mudalogica, decimal_exato);
        somas_erros += (long double)erro_parcial;

        amostra_erros[indice_aux] = erro_parcial;
        indice_aux++;

        if (decimal_mudalogica != decimal_exato) /* Calculo de ER. */
            diferentes++;

        // printf("%ld && %ld\n", decimal_mudalogica, decimal_exato);
        //  imprime_vetor(operandos, dimensao_combinacoes);
        //   printf("\n");
        //  printf("Erro Absoluto Parcial:\n%ld\n", erro_parcial);
        //  __mingw_printf("Relativo Parcial:\n%0.4Lf\n",relativo_parcial);

        printf("\n");
        incrementa_vetor(operandos, dimensao_combinacoes);
        printf("INCREMENTOU\n");

        if (decimal_exato == 0)
            relativo_parcial = (long double)erro_parcial;

        else
            relativo_parcial = ((long double)erro_parcial/(long double)decimal_exato);

        soma_relativos += relativo_parcial;
    }

    //system("cls");

    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
    printf("\nRESULTADOS DAS METRICAS DE ERRO PADRONIZADAS COM TODOS OPERANDOS POSSIVEIS:\n");

    printf("\nTotal de Operacoes Realizadas:\n%d\n",indice_aux);
    printf("\nQuantidade de vezes em que o valor com interferencias divergiu do valor original:\n%ld\n", diferentes);
    printf("\nSoma dos erros absolutos parciais:\n%ld\n", somas_erros);
    __mingw_printf("\nSoma dos erros relativos parciais:\n%0.4Lf\n", soma_relativos);

    ER = (long double)diferentes/(long double)indice_aux;
    MAE = (long double)somas_erros/(long double)indice_aux;
    media_relativos = soma_relativos/(long double)indice_aux;

    __mingw_printf("\nER - Error Rate:\n%0.4Lf\n",ER);
    __mingw_printf("\nMAE - Mean Absolute Error:\n%0.4Lf\n",MAE);
    calcula_desviopadrao(amostra_erros,MAE,total_operacoes);
    __mingw_printf("\nMedia dos erros relativos:\n%0.4Lf\n",media_relativos);

    printf("\n\nPressione qualquer tecla para retornar ao menu inicial.\n");
    getch();
}

void mudalogica()
{
    FILE *arq_circuitos;
    char nome_arquivo[DIMENSAO], texto_arquivo[DIMENSAO_LINHA];
    int indice_aux = 0;

    Operacao parametros;
    inicializa_exaustivos(&parametros);
    int **logica_saida, **logica_carry;

    logica_saida = calloc(parametros.bits,sizeof(int*));
    for (int i = 0; i < parametros.bits; i++)
        logica_saida[i] = calloc(parametros.bits,sizeof(int));

    logica_carry = calloc(parametros.bits,sizeof(int*));
    for (int i = 0; i < parametros.bits; i++)
        logica_carry[i] = calloc(parametros.bits,sizeof(int));

    fflush(stdin);
    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
    printf("\nARQUIVO TEXTO DE ENTRADA COM AS LOGICAS DOS FULL ADDERS\n");
    printf("\nDigite o nome do arquivo texto contendo os valores hexadecimais das novas saidas S e C dos Full Adders:\n");
    gets(nome_arquivo);

    if (!(arq_circuitos = fopen(nome_arquivo,"r")))
    {
        system("cls");
        printf("Erro na abertura do arquivo texto de entrada. Fim do programa.\n");
        getch();
        exit(1);
    }

    while (!feof(arq_circuitos))
    {
        if (indice_aux >= parametros.bits)
        {
            fclose(arq_circuitos);
            encerra_programa();
        }

        fscanf(arq_circuitos,"%s",texto_arquivo);

        if (strlen(texto_arquivo) > LIMITELINHA)
        {
            fclose(arq_circuitos);
            encerra_programa();
        }

        converte_hexbin(texto_arquivo,logica_saida[indice_aux],parametros.bits);
        memset(texto_arquivo, 0, sizeof(texto_arquivo));

        fscanf(arq_circuitos,"%s",texto_arquivo);

        if (strlen(texto_arquivo) > LIMITELINHA)
        {
            fclose(arq_circuitos);
            encerra_programa();
        }

        converte_hexbin(texto_arquivo,logica_carry[indice_aux],parametros.bits);
        memset(texto_arquivo, 0, sizeof(texto_arquivo));

        indice_aux++;
    }

    if (indice_aux != parametros.bits)
    {
        fclose(arq_circuitos);
        encerra_programa();
    }


    mudalogica_exaustivos(parametros,logica_saida, logica_carry);

    for (int i = 0; i < parametros.bits; i++)
        free(logica_saida[i]);
    free(logica_saida);

    for (int i = 0; i < parametros.bits; i++)
        free(logica_carry[i]);
    free(logica_carry);

    fclose(arq_circuitos);
}
