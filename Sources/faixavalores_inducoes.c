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
#include "faixavalores_inducoes.h"

void opinducoes_faixavalores(Operacao *parametros, int inducoes[]) /* Realiza as operacoes com somas entre os numeros binarios, e calcula as estatisticas. */
{
    int saida_exata[parametros->bits];
    int saida_inducao[parametros->bits];
    int diferentes = 0;

    inicializa_vetor(saida_exata,parametros->bits);
    inicializa_vetor(saida_inducao,parametros->bits);

	long int menor = calcula_decimal(parametros->n1,parametros->bits);
	long int maior = calcula_decimal(parametros->n2,parametros->bits);

    int quantidade_somas = maior-menor+1;
    double soma_erros = 0;
    long int saida_parcial = 0;
    long int erro_parcial = 0;
    long int soma_exata = 0;
	long int erros[quantidade_somas];

    int digito;
    int erro_insercao = 0;

    do
    {
        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nOPERANDO BINARIO FIXO\n");
        printf("\nDigite o operando binario fixo de %d bits:\n", parametros->bits);

        erro_insercao = 0;

        for (int i = 0; i < parametros->bits; i++) /* Obtem o operando binario fixo. */
        {
            digito = (int)getch() - '0';
            fflush(stdin);
            printf("%d", digito);
            parametros->n2[i] = digito;
        }

        if (controla_binario(parametros->n2,parametros->bits)) /* Verifica operandos invalidos. */
        {
            erro_insercao = 1;
            system("cls");
            printf("Um ou mais operandos foram mal inseridos.\n");
            printf("\nPressione qualquer tecla para inserir os operandos novamente.\n");
            getch();
            system("cls");
        }

    } while (erro_insercao != 0);

    int indice_erros = 0;

    for (int i = 0; i < quantidade_somas; i++) /* Iteracao para calculo de media aritmetica. */
    {
        saida_comum(*parametros,saida_exata); /* Calcula a saida com o operando binario menor atualizado. */
        soma_exata = calcula_decimal(saida_exata,parametros->bits);
        inicializa_vetor(saida_exata,parametros->bits); /* Reinicia o vetor contendo o valor da soma sem cortes, para cada iteracao. */

        calcula_inducoes(*parametros,inducoes,saida_inducao);
        saida_parcial = calcula_decimal(saida_inducao,parametros->bits);

        if (saida_parcial != soma_exata) /* Contabiliza quantas vezes o resultado com cortes foi diferente do resultado exato. */
            diferentes++;

        erro_parcial = calcula_erroabsoluto(saida_parcial,soma_exata); /* Calcula o erro absoluto da vez. */
        erros[indice_erros] = erro_parcial;
        indice_erros++;

      //  printf("\n-----------------------------------------------\n");
		//mostra_umporum(parametros, erro_parcial); /* Mostra os operandos e o erro parcial em cada operacao (opcional para fins de depuracao). */
      //  printf("\n-----------------------------------------------\n");

        soma_erros += (double)erro_parcial; /* Soma todos os erros absolutos de cada iteracao, para calculo da media aritmetica. */
        soma_um(parametros); /* Atualiza o operando binario menor, incrementando em uma unidade. */
    }

    double media_erros = soma_erros / quantidade_somas; /* Calculo da media aritmetica dos erros absolutos obtidos. */

    double desvio = 0;
    double soma_sqrdesvios = 0;

	for (int i = 0; i < quantidade_somas; i++) /* Iteracao para calculo de variancia e desvio padrao. */
    {
        desvio = pow((erros[i]-media_erros),2);
		soma_sqrdesvios += desvio;
    }

    double variancia = soma_sqrdesvios / (quantidade_somas-1); /* Calculo da variancia. */
    double desvio_padrao = sqrt(variancia); /* Calculo do desvio padrao. */
    long int fixo = calcula_decimal(parametros->n2,parametros->bits);

    while (1)
    {
        system("cls");
        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nRESULTADOS ESTATISTICOS DAS SOMAS COM INDUCOES DE CARRY IN:\n");
        printf("\nSomas com %ld ocorridas no intervalo de inteiros:\n[ %ld , %ld ]", fixo,menor,maior);
        printf("\n\nDurante as somas, ocorreram inducoes de CARRY IN fixas nos bits abaixo:\n");

        for (int i = 0; i < parametros->bits; i++)
            if(inducoes[i] != -1)
                printf("%d ", inducoes[i]);

        mostra_estatisticas(diferentes,media_erros,desvio_padrao, quantidade_somas);

        char tecla = getch();

        switch(tecla) /* Analisa a tecla pressionada, para continuar a execucao do programa. */
        {
            case 'x': /* Grava os dados estatisticos no arquivo texto de saida. */
            case 'X':
            {
                system("cls");

                FILE *arq;

                if (!(arq = fopen("inducoes_faixavalores.txt","a")))
                {
                    system("cls");
                    printf("Erro na abertura ou criacao do arquivo inducoes_faixavalores.txt. Fim do programa.\n");
                    getch();
                    exit(1);
                }

                fprintf(arq,"--------------------------------------------------------------------------------\n");
                fprintf(arq,"\nSomas com %ld ocorridas no intervalo de inteiros:\n[ %ld , %ld ]\n", fixo,menor,maior);
                fprintf(arq,"\nDurante as somas, ocorreram inducoes de CARRY IN fixas nos bits abaixo:\n");
                for (int i = 0; i < parametros->bits; i++)
                    if(inducoes[i] != -1)
                        fprintf(arq,"%d ", inducoes[i]);

                mostra_estatisticas_arq(arq,diferentes,media_erros,desvio_padrao, quantidade_somas);
                fclose(arq);

                printf("Arquivo texto de saida ""inducoes_faixavalores.txt"" atualizado com sucesso.\n");
                printf("\nPressione qualquer tecla para retornar para a tela anterior.\n");
                getch();
                system("cls");
            }
            break;

            case ESC: /* Encerra a execucao do programa. */
            {
                system("cls");
                exit(0);
            }
            break;

            default: /* Retorna para o menu anterior, caso o usuario aperte algo diferente de X ou ESC. */
                return;
            break;
        }
    }
}

void inducoes_faixavalores() /* Obtem as informacoes do usuario e realiza os calculos estatisticos sobre diferentes somas, com inducoes de CARRY IN. */
{
    Operacao parametros;

    parametros.bits = recebe_bits(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */
    parametros.n1 = calloc(parametros.bits,sizeof(int));
    parametros.n2 = calloc(parametros.bits,sizeof(int));

    preenche_maxmin(parametros); /* Preenche os vetores com os valores binarios dos operandos. */

    int inducoes[parametros.bits];
    inicializa_menosum(inducoes,parametros.bits);
    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich");
    printf("\n\nPONTOS DE INDUCAO:\n");
    recebe_inducoes(inducoes);
    system("cls");

    opinducoes_faixavalores(&parametros,inducoes); /* Realiza as operacoes envolvendo somas entre os operandos binarios. */
}
