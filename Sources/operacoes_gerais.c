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

void calcula_desviopadrao(long int amostra[], long double media, int total_operacoes)
{
    long double desvio = 0, soma_sqrdesvios = 0;

    for (int i = 0; i < total_operacoes; i++)
    {
        //__mingw_printf("\n%ld\n", amostra[i]);
        desvio = (long double)amostra[i] - media;
        soma_sqrdesvios += desvio*desvio;
    }

    long double variancia = soma_sqrdesvios/(total_operacoes-1);
    long double desvio_padrao = sqrt(variancia);
    __mingw_printf("\nDesvio Padrao dos Erros Absolutos:\n%0.4Lf\n",desvio_padrao);
}

void converte_binario(int entrada, int bits, int saida[]) /* Converte um numero inteiro decimal de entrada em um numero binario de saida, embutido em um vetor. */
{
    unsigned int mascara = 1U << (bits-1);

    for (int i = 0; i < bits; i++)
    {
        saida[i] = (entrada & mascara) ? 1 : 0;
        entrada <<= 1;
    }
}

int controla_binario(int binario[], int bits) /* Verifica se o numero recebido e binario. */
{
    for (int i = 0; i < bits; i++)
        if (binario[i] != 0 && binario[i] != 1)
            return 1;

    return 0;
}

void inicializa_parametros(Operacao *parametros)
{
    parametros->bits = recebe_bits_estatistica(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */
    parametros->n1 = calloc(parametros->bits,sizeof(int));
    parametros->n2 = calloc(parametros->bits,sizeof(int));
	inicializa_vetor(parametros->n1,parametros->bits);
    inicializa_vetor(parametros->n2,parametros->bits);
}

int recebe_bits() /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */
{
    int bits;
    int erro_entrada = 0;

    do
    {
        system("cls");
        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nQUANTIDADE DE BITS\n");

        printf("\nDigite a quantidade de bits dos dois numeros binarios a serem somados (maximo %d bits):\n", MAXBITS);
        scanf("%d", &bits); /* Obtem a quantidade de bits dos operandos e do resultado da soma binaria. */

        if (bits <= 0 || bits > MAXBITS) /* Verifica se o usuario digitou a quantidade de bits correta em cada operando. */
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


void erro_absoluto_arq(FILE *arq, long int resultado, long int referencia) /* Calcula a diferenca entre os dois valores obtidos. */
{
    long int erro_absoluto = resultado-referencia;

    if (erro_absoluto >= 0)
        fprintf(arq,"%ld\n\n", erro_absoluto); /* Imprime o modulo do erro calculado. */

    else
    {
        erro_absoluto *= -1;
        fprintf(arq,"%ld\n\n", erro_absoluto); /* Imprime o modulo do erro calculado. */
    }
}

void erro_absoluto(long int resultado, long int referencia) /* Calcula o erro absoluto entre os dois resultados obtidos, com e sem interferencias de CARRY IN. */
{
    long int erro_absoluto = resultado-referencia;

    if (erro_absoluto >= 0)
        printf("%ld\n", erro_absoluto); /* Imprime o modulo do erro calculado. */

    else
    {
        erro_absoluto *= -1;
        printf("%ld\n", erro_absoluto); /* Imprime o modulo do erro calculado. */
    }
}

void erro_relativo(long int resultado, long int referencia) /* Calcula o erro relativo entre os dois resultados obtidos, com e sem interferencias de CARRY IN. */
{
    long int erro_absoluto = resultado-referencia;
    double erro_relativo;

    if (erro_absoluto < 0) /* Obtem o modulo da diferenca entre o resultado e a referencia. */
        erro_absoluto *= -1;

    if (referencia != 0)
        erro_relativo = (double)erro_absoluto/(double)referencia;

    else
        erro_relativo = (double)erro_absoluto/1;

    erro_relativo *= 100;

    printf("%0.4lf\n\n", erro_relativo);
}

void erro_relativo_arq(FILE *arq, long int resultado, long int referencia) /* Calcula a diferenca entre os dois valores obtidos. */
{
    long int erro_absoluto = resultado-referencia;
    double erro_relativo;

    if (erro_absoluto < 0) /* Obtem o modulo da diferenca entre o resultado e a referencia. */
        erro_absoluto *= -1;

    if (referencia != 0)
        erro_relativo = (double)erro_absoluto/(double)referencia;

    else
        erro_relativo = (double)erro_absoluto/1;

    erro_relativo *= 100;

    fprintf(arq,"%lf\n\n", erro_relativo);
}

long int calcula_erroabsoluto(long int resultado, long int referencia) /* Calcula e retorna a diferenca entre os dois valores obtidos. */
{
    long int erro_absoluto = resultado-referencia;

    if (erro_absoluto >= 0)
        return erro_absoluto;

    else
        erro_absoluto *= -1;

    return erro_absoluto;
}

int contem_vetor(int v[], int n, int dimensao) /* Verifica se um vetor possui um determinado numero inteiro. */
{
    for (int i = 0; i < dimensao; i++)
        if (n == v[i])
            return 1;

    return 0;
}

void copia_vetor(int entrada[], int saida[],int dimensao) /* Copia o conteudo de um vetor de entrada para um vetor de saida. */
{
    for (int i = 0; i < dimensao; i++)
        saida[i] = entrada[i];
}

void inicializa_menosum(int vetor[], int dimensao) /* Inicializa um vetor de entrada com os valores -1. */
{
    for (int i = 0; i < dimensao; i++)
        vetor[i] = -1;
}

void inicializa_vetor(int vetor[], int dimensao) /* Inicializa um vetor de entrada com valores iguais a zero. */
{
    for (int i = 0; i < dimensao; i++)
        vetor[i] = 0;
}

void imprime_vetor(int v[], int dimensao) /* Realiza a impressao completa do vetor na tela. */
{
    for (int i = 0; i < dimensao; i++)
        printf("%d", v[i]);
}

int calcula_decimal(int n[], int bits) /* Converte um numero binario para decimal. */
{
    int decimal = 0;
    int posicao = 0;

    for (int i = bits-1; i >= 0; i--) /* Conversao para decimal usando o metodo polinomial. */
    {
        decimal += n[i]*pow(2,posicao);
        posicao++;
    }

    return decimal;
}

void calcula_carry (Operacao parametros, int vetor_carry[]) /* Determina o vetor de carry entre os dois operandos, retornando no vetor de saida. */
{
    int carry = 0;

    for (int i = parametros.bits-1; i >= 0; i--) /* Iteracao para preencher o vetor contendo os CARRY IN. */
    {
        if (parametros.n1[i] == 1 && parametros.n2[i] == 1) /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            carry = 1;  /* Atualiza o valor de CARRY OUT propagado. */
            if (i != 0)
                vetor_carry[i-1] = 1;
        }

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
            carry = 0; /* Atualiza o valor de CARRY OUT propagado. */

        else
        {
            if (carry == 1) /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
                if (i != 0)
                    vetor_carry[i-1] = 1;
            }
        }
    }
}

void saida_comum (Operacao parametros, int saida[]) /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */
{
    int carry = 0;

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        saida[i] = (parametros.n1[i] ^ parametros.n2[i]) ^ carry;
        carry = (parametros.n1[i] & parametros.n2[i]) | (parametros.n1[i] & carry) | (parametros.n2[i] & carry);
    }
}

void novocircuitoOR (Operacao parametros, int saida[]) /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */
{
    int carry = 0;

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        saida[i] = (parametros.n1[i] | parametros.n2[i]) | carry;
        carry = (parametros.n1[i] & parametros.n2[i]) | (parametros.n1[i] & carry) | (parametros.n2[i] & carry);
    }
}

void novocircuitoAND (Operacao parametros, int saida[]) /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */
{
    int carry = 0;

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        saida[i] = (parametros.n1[i] & parametros.n2[i]) & carry;
        carry = (parametros.n1[i] & parametros.n2[i]) | (parametros.n1[i] & carry) | (parametros.n2[i] & carry);
    }
}

long int soma_decimal(Operacao parametros) /* Retorna o valor decimal da soma entre os operandos. */
{
    int carry = 0;
    int saida[parametros.bits];

    inicializa_vetor(saida,parametros.bits);

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        if (parametros.n1[i] == 1 && parametros.n2[i] == 1) /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            saida[i] = carry;   /* Atualiza o digito do numero binario resultante da soma. */
            carry = 1;           /* Atualiza o valor de CARRY OUT propagado. */
        }

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
        {
            saida[i] = carry;    /* Atualiza o digito do numero binario resultante da soma. */
            carry = 0;           /* Atualiza o valor de CARRY OUT propagado. */
        }

        else
        {
            if (carry == 1) /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida[i] = 0;   /* Atualiza o digito do numero binario resultante da soma. */
                carry = 1;      /* Atualiza o valor de CARRY OUT propagado. */
            }

            else
                saida[i] = 1; /* Atualiza o digito do numero binario resultante da soma. */
        }
    }

    return (calcula_decimal(saida,parametros.bits));
}

int recebe_bits_estatistica() /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria, para operacoes estatisticas. */
{
    int bits;
    int erro_entrada = 0;

    do
    {
        system("cls");
        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nQUANTIDADE DE BITS\n");

        printf("\nDigite a quantidade de bits dos operandos das somas (maximo %d bits):\n", MAXBITSESTATISTICA);
        scanf("%d", &bits); /* Obtem a quantidade de bits dos operandos e do resultado da soma binaria. */

        if (bits <= 0 || bits > MAXBITSESTATISTICA) /* Verifica se o usuario digitou a quantidade de bits correta em cada operando. */
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

void mostra_estatisticas_arq(FILE *arq, int diferentes, double media_erros, double desvio_padrao, double quantidade_somas) /* Imprime os resultados estatisticos das somas no arquivo de saida. */
{
    fprintf(arq,"\n\nQuantidade de somas realizadas:\n%.0lf\n", quantidade_somas);
    fprintf(arq,"\nQuantidade de vezes em que o resultado de uma soma com interferencia foi diferente do resultado exato:\n%d\n", diferentes);
    fprintf(arq,"\nMedia aritmetica dos erros absolutos durante as somas:\n%0.4lf\n", media_erros);
    fprintf(arq,"\nDesvio padrao dos erros absolutos durante as somas:\n%0.4lf\n", desvio_padrao);
    fprintf(arq,"\n--------------------------------------------------------------------------------\n");
}

void interferencias_estatisticas(int cortes[], int bits) /* Obtem do usuario os bits em que ocorrerao cortes de CARRY IN fixos. */
{
    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
    printf("\nPONTOS DE CORTES\n");

    char texto_bits[DIMENSAO];
    char *aux_numero;
    int  i = 0, bit_individual = 0;

    inicializa_menosum(cortes,bits);

    fflush(stdin);
    printf("\nDigite os bits em que ocorrerao os cortes de CARRY IN, separados por espacos ou virgulas:\n");
    gets(texto_bits);

    aux_numero = strtok(texto_bits,SEPARADORES);

    while (aux_numero != NULL)
    {
        bit_individual = atoi(aux_numero);

        if (bit_individual >= 0 && bit_individual < bits)
            cortes[i] = bit_individual;

        aux_numero = strtok(NULL,SEPARADORES);
        i++;

        if (i >= bits)
            break;
    }

    system("cls");
}

long int calcula_saidacortes (Operacao *parametros, int cortes[]) /* Determina a soma com interferencias de CARRY OUT entre os dois operandos, retornando no vetor de saida. */
{
    int indice_interf = 0;
    int saida_interf[parametros->bits];
    inicializa_vetor(saida_interf,parametros->bits);

    int carry = 0;

    for (int i = parametros->bits-1; i >= 0; i--)
    {
        if (contem_vetor(cortes,indice_interf,parametros->bits)) /* Realiza o corte de CARRY OUT, se necessario. */
            carry = 0; /* Muda o valor do CARRY IN escolhido de um para zero, desprezando o valor. */

        if (parametros->n1[i] == 1 && parametros->n2[i] == 1)  /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            saida_interf[i] = carry; /* Atualiza o digito do numero binario resultante da soma. */
            carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
        }

        else if (parametros->n1[i] == 0 && parametros->n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
        {
            saida_interf[i] = carry; /* Atualiza o digito do numero binario resultante da soma. */
            carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
        }

        else
        {
            if (carry == 1) /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida_interf[i] = 0; /* Atualiza o digito do numero binario resultante da soma. */
                carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
            }

            else /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida_interf[i] = 1; /* Atualiza o digito do numero binario resultante da soma. */
                carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
            }
        }

        indice_interf++;
    }


    long int decimal_saida = calcula_decimal(saida_interf,parametros->bits);
    return decimal_saida;
}

void soma_um (Operacao *parametros) /* Incrementa um vetor contendo um numero binario em uma unidade. */
{
    for (int i = parametros->bits-1; i >= 0; i--)
    {
        if (parametros->n1[i] == 0)
        {
            parametros->n1[i] = 1;
            break;
        }

        else if (parametros->n1[i] == 1)
            parametros->n1[i] = 0;
    }
}

long int calcula_saidainducoes(Operacao parametros, int inducoes[])
{
    int saida_inducao[parametros.bits];
    calcula_inducoes(parametros,inducoes,saida_inducao);
    long int valor_inducoes = calcula_decimal(saida_inducao,parametros.bits);
    return valor_inducoes;
}

void calcula_inducoes(Operacao parametros, int inducoes[], int saida_inducao[]) /* Realiza a soma entre dos operandos binarios, com inducoes de CARRY IN. */
{
    int carry = 0, indice_inducao = 0;

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        if (contem_vetor(inducoes,indice_inducao,parametros.bits)) /* Realiza a inducao de CARRY IN. */
            carry = 1; /* Muda o valor do CARRY IN escolhido zero para um, induzindo o valor. */

        if (parametros.n1[i] == 1 && parametros.n2[i] == 1)  /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            saida_inducao[i] = carry; /* Atualiza o digito do numero binario resultante da soma. */
            carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
        }

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
        {
            saida_inducao[i] = carry; /* Atualiza o digito do numero binario resultante da soma. */
            carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
        }

        else
        {
            if (carry == 1) /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida_inducao[i] = 0; /* Atualiza o digito do numero binario resultante da soma. */
                carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
            }

            else /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida_inducao[i] = 1; /* Atualiza o digito do numero binario resultante da soma. */
                carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
            }
        }

        indice_inducao++;
    }
}

void recebe_inducoes(int inducoes[]) /* Recebe do usuario os bits nos quais ocorrerao inducoes de CARRY IN. */
{
    int indice_inducao = 0, bit_individual = 0;
    char texto_bits[DIMENSAO];
    char *aux_numero;

    fflush(stdin);

    printf("\nDigite os bits em que ocorrerao as inducoes, separados por espacos ou virgulas:\n");
    gets(texto_bits);

    aux_numero = strtok(texto_bits,SEPARADORES);

    while (aux_numero != NULL)
    {
        bit_individual = atoi(aux_numero);
        inducoes[indice_inducao] = bit_individual;

        aux_numero = strtok(NULL,SEPARADORES);
        indice_inducao++;
    }
}

int recebe_total(long int valor_max) /* Recebe do usuario o numero total de operacoes a serem realizadas. */
{
    system("cls");

    int total_operacoes = 0;

    do
    {
        system("cls");

        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich");
        printf("\n\nDEFINICAO DO NUMERO DE OPERACOES:\n");

        printf("\nIntervalo de valores dos operandos:\n[0 , %ld)\n", valor_max);

        printf("\nDigite o numero total de operacoes a serem realizadas (maximo %d operacoes):\n", MAXOPERACOES);
        scanf("%d", &total_operacoes);

        if (total_operacoes > MAXOPERACOES || total_operacoes < 1)
        {
            printf("\nNumero de operacoes invalido. Pressione qualquer tecla para inserir novamente.\n");
            getch();
        }

    } while (total_operacoes > MAXOPERACOES || total_operacoes < 1);

    system("cls");
    return total_operacoes;
}

void preenche_maxmin(Operacao parametros) /* Preenche os vetores com os valores maximos e minimos das somas, para analise estatistica. */
{
    int digito, erro_insercao;

    do
    {
        erro_insercao = 0;
        system("cls");

        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nOPERANDOS BINARIOS\n");

        printf("\nDigite o numero binario minimo de %d bits:\n", parametros.bits);

        for (int i = 0; i < parametros.bits; i++) /* Obtem o primeiro operando binario. */
        {
            digito = (int)getch() - '0';
            fflush(stdin);
            printf("%d", digito);
            parametros.n1[i] = digito;
        }

        printf("\n\nDigite o segundo numero binario maximo de %d bits:\n", parametros.bits);
        fflush(stdin);

        for (int i = 0; i < parametros.bits; i++) /* Obtem o segundo operando binario. */
        {
            digito = (int)getch() - '0';
            fflush(stdin);
            printf("%d", digito);
            parametros.n2[i] = digito;
        }

        if (controla_binario(parametros.n1,parametros.bits) || controla_binario(parametros.n2,parametros.bits)) /* Verifica operandos invalidos. */
        {
            erro_insercao = 1;
            system("cls");
            printf("Um ou mais operandos foram mal inseridos.\n");
            printf("\nPressione qualquer tecla para inserir os operandos novamente.\n");
            getch();
        }

        else if (calcula_decimal(parametros.n1,parametros.bits) >= calcula_decimal(parametros.n2,parametros.bits)) /* Verifica se o primeiro operando de fato e menor do que o segundo. */
        {
            erro_insercao = 1;
            system("cls");
            printf("Erro. O primeiro operando possui magnitude maior ou igual ao segundo operando.\n");
            printf("\nPressione qualquer tecla para inserir os operandos novamente.\n");
            getch();
        }

    } while (erro_insercao != 0);

    system("cls");
}

void mostra_umporum(Operacao parametros, long int erro_parcial) /* Mostra cada par de operandos binarios e o erro absoluto obtido (funcao para fins de depuracao). */
{
    imprime_vetor(parametros.n1,parametros.bits);
    printf("+");
    imprime_vetor(parametros.n2,parametros.bits);
    printf(" ERRO = ");

    printf("%ld\n", erro_parcial); /* Impressao dos erros absolutos a cada iteracao (opcional). */
}

void mostra_estatisticas(int diferentes, double media_erros, double desvio_padrao, int quantidade_somas) /* Exibe os resultados estatisticos das somas na tela. */
{
    printf("\n\nQuantidade de somas realizadas:\n%d\n", quantidade_somas);
    printf("\nQuantidade de vezes em que o resultado de uma soma com interferencia foi diferente do resultado exato:\n%d\n", diferentes);
    printf("\nMedia aritmetica dos erros absolutos durante as somas:\n%0.4lf\n", media_erros);
    printf("\nDesvio padrao dos erros absolutos durante as somas:\n%0.4lf\n", desvio_padrao);

    printf("\nPressione a tecla para escolher a proxima acao:\n");
    printf("\nX - Anexar os resultados em um arquivo texto de saida.");
    printf("\n\nESC - Encerrar programa.");
    printf("\n\nPressione qualquer outra tecla para realizar uma nova operacao.\n");
}

void interferencias_metricas(int interferencias[], int bits, int codigo_interferencia) /* Obtem do usuario os bits em que ocorrerao interferencias de CARRY IN fixos. */
{
    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");

    if (codigo_interferencia == CODIGO_CORTES)
    {
        printf("\nPONTOS DE CORTES\n");
        printf("\nDigite os bits em que ocorrerao os cortes de CARRY IN, separados por espacos ou virgulas:\n");
    }

    else if (codigo_interferencia == CODIGO_INDUCOES)
    {
        printf("\nPONTOS DE INDUCOES\n");
        printf("\nDigite os bits em que ocorrerao as inducoes de CARRY IN, separados por espacos ou virgulas:\n");
    }

    char texto_bits[DIMENSAO];
    char *aux_numero;
    int  i = 0, bit_individual = 0;

    inicializa_menosum(interferencias,bits);

    fflush(stdin);

    gets(texto_bits);

    aux_numero = strtok(texto_bits,SEPARADORES);

    while (aux_numero != NULL)
    {
        bit_individual = atoi(aux_numero);

        if (bit_individual >= 0 && bit_individual < bits)
            interferencias[i] = bit_individual;

        aux_numero = strtok(NULL,SEPARADORES);
        i++;

        if (i >= bits)
            break;
    }

    system("cls");
}
