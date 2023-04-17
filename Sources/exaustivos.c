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
#include "exaustivos.h"

int recebe_bits_exaustiva() /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria, para operacoes estatisticas. */
{
    int bits;
    int erro_entrada = 0;

    do
    {
        system("cls");
        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nQUANTIDADE DE BITS\n");

        printf("\nDigite a quantidade de bits dos operandos das somas (maximo %d bits):\n", MAXBITSEXAUSTIVA);
        scanf("%d", &bits); /* Obtem a quantidade de bits dos operandos e do resultado da soma binaria. */

        if (bits <= 0 || bits > MAXBITSEXAUSTIVA) /* Verifica se o usuario digitou a quantidade de bits correta em cada operando. */
        {
            printf("\nA quantidade de bits foi mal inserida. Pressione qualquer tecla para inserir novamente.\n");
            erro_entrada = 1;
            getch();
        }

        else
            erro_entrada = 0;

        system("cls");

    } while (erro_entrada == 1);

    return bits;
}

void inicializa_exaustivos(Operacao *parametros)
{
    parametros->bits = recebe_bits_exaustiva(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */
    parametros->n1 = calloc(parametros->bits,sizeof(int));
    parametros->n2 = calloc(parametros->bits,sizeof(int));
	inicializa_vetor(parametros->n1,parametros->bits);
    inicializa_vetor(parametros->n2,parametros->bits);
}

void incrementa_vetor (int v[], int dimensao)
{
    printf("\nENTRADA DA FUNCAO:\n");
    imprime_vetor(v,dimensao);

    for (int i = dimensao-1; i >= 0; i--)
    {
        if (v[i] == 0)
        {
            v[i] = 1;
            break;
        }

        else if (v[i] == 1)
            v[i] = 0;
    }

    printf("\nRESULTADO APOS INCREMENTO:\n");
    imprime_vetor(v,dimensao);
    printf("\nDIMENSAO:\n%d\n", dimensao);
}

int overflow_comum(Operacao parametros)
{
    int carry = 0;

    for (int i = parametros.bits-1; i >=0; i--)
    {
        if ((carry + parametros.n1[i] + parametros.n2[i]) >= 2)
            carry = 1;

        else
            carry = 0;
    }

    return carry;
}

int overflow_cortes(Operacao parametros, int cortes[])
{
    int carry = 0;

    int indice_interf = 0;

    for (int i = parametros.bits-1; i >=0; i--)
    {
        if (contem_vetor(cortes,indice_interf,parametros.bits))
            carry = 0;

        if ((carry + parametros.n1[i] + parametros.n2[i]) >= 2)
            carry = 1;

        else
            carry = 0;

        indice_interf++;
    }

    return carry;
}

int overflow_inducao(Operacao parametros, int inducoes[])
{
    int carry = 0;
    int indice_interf = 0;

    for (int i = parametros.bits-1; i >=0; i--)
    {
        if (contem_vetor(inducoes,indice_interf,parametros.bits))
            carry = 1;

        if ((carry + parametros.n1[i] + parametros.n2[i]) >= 2)
            carry = 1;

        else
            carry = 0;

        indice_interf++;
    }

    return carry;
}

void calculos_exaustivos(int codigo_interferencia)
{
    Operacao parametros;
    inicializa_exaustivos(&parametros);
    int interferencias[parametros.bits];
    interferencias_metricas(interferencias,parametros.bits,codigo_interferencia);

    long double ER = 0, MAE = 0, relativo_parcial = 0, soma_relativos = 0, media_relativos = 0;

    int total_operacoes = (int)pow(2,parametros.bits*2);
    int saida_exata[parametros.bits];
    inicializa_vetor(saida_exata,parametros.bits);

    long int decimal_exato = 0, decimal_interf = 0;
    long int erro_parcial = 0;
    long int diferentes = 0;
    long int amostra_erros[total_operacoes];

    int contaOF_cortes = 0, contaOF_inducoes = 0, contaOF_normal = 0, contaOF_ambos = 0;

    for (int i = 0; i < total_operacoes; i++)
        amostra_erros[i] = 0;

    long int somas_erros = 0;

    FILE *arq;

    printf("Calculando...\n");

    int indice_aux = 0;
    int aux_parametro = 0;

    int dimensao_combinacoes = parametros.bits*2;
    int operandos[dimensao_combinacoes];
    inicializa_vetor(operandos,dimensao_combinacoes);

    if (!(arq = fopen("somas_exaustivas.txt", "w")))
    {
        printf("Erro na abertura do arquivo texto. Fim do programa.\n");
        exit(1);
    }

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

        saida_comum(parametros,saida_exata);
        decimal_exato = calcula_decimal(saida_exata,parametros.bits);

        if (codigo_interferencia == CODIGO_CORTES)
        {
            decimal_interf = calcula_saidacortes(&parametros,interferencias);

            if (overflow_comum(parametros) && overflow_cortes(parametros, interferencias))
                contaOF_ambos++;


            else if (overflow_comum(parametros))
                contaOF_normal++;

            else if (overflow_cortes(parametros,interferencias))
                contaOF_cortes++;
        }

        else if (codigo_interferencia == CODIGO_INDUCOES)
        {
            decimal_interf = calcula_saidainducoes(parametros,interferencias);

            if (overflow_comum(parametros) && overflow_inducao(parametros, interferencias))
                contaOF_ambos++;

            else if (overflow_comum(parametros))
                contaOF_normal++;

            else if (overflow_inducao(parametros,interferencias))
                contaOF_inducoes++;
        }

        erro_parcial = calcula_erroabsoluto(decimal_exato, decimal_interf);
        somas_erros += (long double)erro_parcial;

        amostra_erros[indice_aux] = erro_parcial;
        indice_aux++;

        if (decimal_exato != decimal_interf)
            diferentes++;

        imprime_vetor(operandos, dimensao_combinacoes);
        printf("\n");
        printf("%ld\n", erro_parcial);

        incrementa_vetor(operandos, dimensao_combinacoes);

        if (decimal_exato == 0)
            relativo_parcial = (long double)erro_parcial;

        else
            relativo_parcial = ((long double)erro_parcial/(long double)decimal_exato);

        __mingw_fprintf(arq,"-- RELATIVO = %0.4Lf\n",relativo_parcial);

        soma_relativos += relativo_parcial;
    }

    fclose(arq);

    system("cls");

    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
    printf("\nRESULTADOS DAS METRICAS DE ERRO PADRONIZADAS COM TODOS OPERANDOS POSSIVEIS:\n");

    printf("\nTotal de Operacoes Realizadas:\n%d\n",indice_aux);
    printf("\nQuantidade de vezes em que o valor com interferencias diferiu do valor original:\n%ld\n", diferentes);
    printf("\nSoma dos erros absolutos parciais:\n%ld\n", somas_erros);
    __mingw_printf("\nSoma dos erros relativos parciais:\n%0.4Lf\n", soma_relativos);

    ER = (long double)diferentes/(long double)indice_aux;
    MAE = (long double)somas_erros/(long double)indice_aux;
    media_relativos = soma_relativos/(long double)indice_aux;

    __mingw_printf("\nER - Error Rate:\n%0.4Lf\n",ER);
    __mingw_printf("\nMAE - Mean Absolute Error:\n%0.4Lf\n",MAE);
    calcula_desviopadrao(amostra_erros,MAE,total_operacoes);
    __mingw_printf("\nMedia dos erros relativos:\n%0.4Lf\n",media_relativos);

    printf("\nQuantidade de estouros ocorridos com a soma comum:\n%d\n", contaOF_normal);
    printf("\nQuantidade de estouros ocorridos com a soma aproximada:");
    if (codigo_interferencia == CODIGO_CORTES)
        printf("\n%d\n", contaOF_cortes);
    else if (codigo_interferencia == CODIGO_INDUCOES)
        printf("\n%d\n", contaOF_inducoes);

    printf("\nQuantidade de estouros ocorridos com a soma comum e com a soma aproximada:\n%d\n", contaOF_ambos);

    printf("\n\nPressione qualquer tecla para retornar ao menu inicial.\n");
    getch();
}

void analise_exaustiva()
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
            calculos_exaustivos(CODIGO_CORTES);
            break;
        }

        else if (tecla == '2')
        {
            calculos_exaustivos(CODIGO_INDUCOES);
            break;
        }
    }
}
