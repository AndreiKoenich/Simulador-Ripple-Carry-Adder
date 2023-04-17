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
#include "aleatorias_inducoes.h"

void operacoes_aleatorias_inducoes(int bits, int inducoes[]) /* Realiza somas com inducoes de CARRY IN, envolvendo operandos aleatorios, e determina as estatisticas. */
{
    long int valor_max = (int)pow(2,bits);
    int total_operacoes = recebe_total(valor_max);

    int saida_exata[bits];
    int saida_inducao[bits];
    int diferentes = 0;

    long double soma_erros = 0;
    long int interf_parcial = 0;
    long int erro_parcial = 0;
    long int soma_exata = 0;
	long int erros[total_operacoes];

	Operacao parametros;
	parametros.bits = bits;
    parametros.n1 = calloc(parametros.bits,sizeof(int));
    parametros.n2 = calloc(parametros.bits,sizeof(int));
	inicializa_vetor(parametros.n1,parametros.bits);
    inicializa_vetor(parametros.n2,parametros.bits);

	int decimal1 = 0, decimal2 = 0, indice_erros = 0;

	for (int i = 0; i < total_operacoes; i++) /* Iteracao para calculo de media aritmetica. */
    {
		decimal1 = rand() % valor_max;
		decimal2 = rand() % valor_max;

		converte_binario(decimal1,bits,parametros.n1);
		converte_binario(decimal2,bits,parametros.n2);

		saida_comum(parametros,saida_exata);
        soma_exata = calcula_decimal(saida_exata,bits);
		calcula_inducoes(parametros,inducoes,saida_inducao);

		if (interf_parcial != soma_exata) /* Contabiliza quantas vezes o resultado com inducoes foi diferente do resultado exato. */
            diferentes++;

        interf_parcial = calcula_decimal(saida_inducao,bits);
		erro_parcial = calcula_erroabsoluto(interf_parcial,soma_exata); /* Calcula o erro absoluto da vez. */

		erros[indice_erros] = erro_parcial;
		indice_erros++;

      //  printf("\n-----------------------------------------------\n");
		//mostra_umporum(parametros, erro_parcial); /* Mostra os operandos e o erro parcial em cada operacao (opcional para fins de depuracao). */
      //  printf("\n-----------------------------------------------\n");

		soma_erros += (double)erro_parcial; /* Soma todos os erros absolutos de cada iteracao, para calculo da media aritmetica. */
    }

    double media_erros = soma_erros / total_operacoes; /* Calculo da media aritmetica dos erros absolutos obtidos. */

    long double desvio = 0;
    long double soma_sqrdesvios = 0;

	for (int i = 0; i < total_operacoes; i++) /* Iteracao para calculo de variancia e desvio padrao. */
    {
        desvio = pow((erros[i]-media_erros),2);
		soma_sqrdesvios += desvio;
    }

    long double variancia = soma_sqrdesvios / (total_operacoes-1); /* Calculo da variancia. */
    double desvio_padrao = sqrt(variancia); /* Calculo do desvio padrao. */

    while (1)
    {
        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nRESULTADOS ESTATISTICOS DAS SOMAS ALEATORIAS COM INDUCOES DE CARRY IN:\n");
        printf("\nIntervalo de valores dos operandos:\n[0 , %ld)\n", valor_max);
        printf("\nDurante as somas, ocorreram inducoes de CARRY IN fixas nos bits abaixo:\n");

        for (int i = 0; i < bits; i++)
            if(inducoes[i] != -1)
                printf("%d ", inducoes[i]);

        mostra_estatisticas(diferentes,media_erros,desvio_padrao, total_operacoes);

        char tecla = getch();

        switch(tecla) /* Analisa a tecla pressionada, para continuar a execucao do programa. */
        {
            case 'x': /* Grava os dados estatisticos no arquivo texto de saida. */
            case 'X':
            {
                system("cls");

                FILE *arq;

                if (!(arq = fopen("somasinducoes_aleatorias.txt","a")))
                {
                    system("cls");
                    printf("Erro na abertura ou criacao do arquivo somasinducoes_aleatorias.txt. Fim do programa.\n");
                    getch();
                    exit(1);
                }

                fprintf(arq,"------------------------------\n");
                fprintf(arq,"OPERACAO ANALISADA:\n\n");
                fprintf(arq,"Foram realizadas %d somas com operandos aleatorios de %d bits, no intervalo [0 , %ld)\n", total_operacoes, bits, valor_max);

                fprintf(arq,"\nDurante as somas, ocorreram inducoes de CARRY IN fixas nos bits abaixo:\n");
                for (int i = 0; i < bits; i++)
                    if(inducoes[i] != -1)
                        fprintf(arq,"%d ", inducoes[i]);

                mostra_estatisticas_arq(arq,diferentes,media_erros,desvio_padrao, total_operacoes);
                fclose(arq);

                printf("Arquivo texto de saida ""somasinducoes_aleatorias.txt"" atualizado com sucesso.\n");
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

    free(parametros.n1);
    free(parametros.n2);
}

void analise_aleatoria_inducoes() /* Realiza calculos estatisticos com somas envolvendo operandos aleatorios e inducoes de CARRY IN. */
{
    int bits = recebe_bits_estatistica(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */

    int inducoes[bits];
    inicializa_menosum(inducoes,bits);

    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich");
    printf("\n\nPONTOS DE INDUCAO:\n");
    recebe_inducoes(inducoes);
    system("cls");

    operacoes_aleatorias_inducoes(bits,inducoes); /* Realiza as operacoes envolvendo somas entre os operandos binarios. */
}
