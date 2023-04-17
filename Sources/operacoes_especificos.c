#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>

#define BASE 10
#define MAXBITS 16
#define MAXBITSESTATISTICA 16
#define ESC 27
#define DIMENSAO 1000
#define SEPARADORES " ,.&*%\?!;/-'@\"$#=~><)(][}{:\n\t_"
#define MAXOPERACOES 10000

#include "operacoes_gerais.h"
#include "operacoes_especificos.h"

void mostra_opcoes() /* Mostra o menu de opcoes ao usuario. */
{
    printf("\nPressione a tecla correspondente ao resultado desejado:\n");
    printf("\n1 - Resultado com cortes cumulativos de CARRY IN.\n");
    printf("\n2 - Resultado com cortes em todos os CARRY IN.\n");
    printf("\n3 - Resultado com interferencia somente no CARRY IN mais significativo.\n");
    printf("\n4 - Resultado com cortes de CARRY IN especificos.\n");
    printf("\n5 - Resultado com inducoes de CARRY IN especificos.\n");
    printf("\n6 - Geracao de arquivo com todas os cortes de CARRY IN possiveis.\n");
    printf("\n7 - Realizar nova operacao.\n");
    printf("\nESC - Encerrar programa.\n");
}

void operacoes() /* Realiza as operacoes comum e com interferencia de CARRY IN entre os operandos binarios. */
{
    Operacao parametros;

    parametros.bits = recebe_bits(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */
    parametros.n1 = calloc(parametros.bits,sizeof(int));
    parametros.n2 = calloc(parametros.bits,sizeof(int));

    preenche_vetores(&parametros); /* Preenche os vetores com os valores binarios dos operandos. */

    int contador_carry = 0;

    while (1)
    {
        system("cls");

        contador_carry = soma_comum(parametros);
        mostra_opcoes();
        char tecla = getch();
        system("cls");

        switch(tecla)
        {
            case '1':/* Calcula resultados com cortes cumulativos de CARRY IN. */
            {
                printf("RESULTADOS COM CORTES CUMULATIVOS DE CARRY IN:\n\n");

                if (contador_carry == 0)
                    printf("Nenhum CARRY IN ocorreu durante a soma.\n");

                else
                    desprezo_cumulativo(parametros,contador_carry);

                printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
                getch();
            }
            break;

            case '2': /* Calcula resultado com cortes em todos os CARRY IN. */
            {
                printf("RESULTADOS DESPREZANDO TODOS OS CARRY IN:\n\n");

                despreza_tudo(parametros);

                printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
                getch();
            }
            break;

            case '3': /* Calcula resultado com interferencia somente no CARRY IN mais significativo. */
            {
                printf("RESULTADO DESPREZANDO SOMENTE O CARRY IN MAIS SIGNIFICATIVO:\n\n");

                if (contador_carry == 0)
                {
                    printf("Nenhum CARRY IN ocorreu durante a soma.\n");
                    printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
                    getch();
                    system("cls");
                }

                else
                    despreza_maior(parametros);
            }
            break;

            case '4': /* Calcula resultados com cortes de CARRY IN especificos. */
            {
                printf("RESULTADO COM CORTES DE CARRY IN ESPECIFICOS:\n\n");

                if (contador_carry == 0)
                {
                    printf("Nenhum CARRY IN ocorreu durante a soma.\n");
                    printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
                    getch();
                    system("cls");
                }

                else
                    despreza_especifico(parametros);
            }
            break;

            case '5': /* Calcula resultados com inducoes de CARRY IN especificas. */
            {
                inducao_carry(parametros,contador_carry);

                printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
                getch();
            }
            break;

            case '6': /* Gera arquivo contendo resultados todas os cortes de CARRY IN de CARRY IN possiveis. */
            {
                printf("GERACAO DE ARQUIVO TEXTO COM TODAS OS CORTES POSSIVEIS:\n\n");

                if (contador_carry == 0)
                {
                    printf("Nenhum CARRY IN ocorreu durante a soma.\n");
                    printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
                    getch();
                    system("cls");
                }

                else
                    gera_texto(parametros,contador_carry);
            }
            break;

            case '7': /* Permite que o usuario realize novos operandos. */
                return;
                break;

            case ESC: /* Encerra a execucao do programa. */
            {
                system("cls");
                exit(0);
                break;
            }

            default: /* Controle dos casos em que o usuario pressiona tecla invalida. */
            {
                printf("Tecla invalida.\n");
                printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
                getch();
            }
            break;
        }
    }

    free(parametros.n1);
    free(parametros.n2);
}

void preenche_vetores(Operacao *parametros) /* Preenche os vetores com os valores binarios dos operandos. */
{
    int digito, erro_insercao;

    do
    {
        erro_insercao = 0;
        system("cls");

        printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
        printf("\nOPERANDOS BINARIOS\n");

        printf("\nDigite o primeiro numero binario de %d bits:\n", parametros->bits);

        for (int i = 0; i < parametros->bits; i++) /* Obtem o primeiro operando binario. */
        {
            digito = (int)getch() - '0';
            fflush(stdin);
            printf("%d", digito);
            parametros->n1[i] = digito;
        }

        printf("\n\nDigite o segundo numero binario de %d bits:\n", parametros->bits);
        fflush(stdin);

        for (int i = 0; i < parametros->bits; i++) /* Obtem o segundo operando binario. */
        {
            digito = (int)getch() - '0';
            fflush(stdin);
            printf("%d", digito);
            parametros->n2[i] = digito;
        }

        if (controla_binario(parametros->n1,parametros->bits) || controla_binario(parametros->n2,parametros->bits)) /* Verifica operandos invalidos. */
        {
            erro_insercao = 1;
            system("cls");
            printf("Um ou mais operandos foram mal inseridos.\n");
            printf("\nPressione qualquer tecla para inserir os operandos novamente.\n");
            getch();
        }

    } while (erro_insercao != 0);
}

int soma_comum(Operacao parametros) /* Faz a soma entre os operandos binarios, sem qualquer corte em CARRY IN. */
{
    int flag_carry = 0, contador_carry = 0;
    int saida[parametros.bits];

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        if (parametros.n1[i] == 1 && parametros.n2[i] == 1) /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            saida[i] = flag_carry;   /* Atualiza o digito do numero binario resultante da soma. */
            flag_carry = 1;           /* Atualiza o valor de CARRY OUT propagado. */
            contador_carry++;
        }

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
        {
            saida[i] = flag_carry;    /* Atualiza o digito do numero binario resultante da soma. */
            flag_carry = 0;           /* Atualiza o valor de CARRY OUT propagado. */
        }

        else
        {
            if (flag_carry == 1) /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida[i] = 0;   /* Atualiza o digito do numero binario resultante da soma. */
                flag_carry = 1;      /* Atualiza o valor de CARRY OUT propagado. */
                contador_carry++; /* Incrementa o contador de CARRY OUT. */
            }

            else
                saida[i] = 1; /* Atualiza o digito do numero binario resultante da soma. */
        }
    }

    printf("SIMULADOR DE RIPPLE CARRY ADDER - Andrei Pochmann Koenich\n");
    printf("\nRESULTADO DA SOMA\n"); /* Impressao de todas as informacoes da soma. */
    resultados_comum(parametros,saida);

    printf("\n\nQuantos CARRY OUT ocorreram durante a soma?\n");
    printf("%d\n",contador_carry);

    printf("\nOcorreu CARRY OUT no bit mais significativo?");

    if (flag_carry == 1)
    {
        printf("\nSIM\n");
        contador_carry--;
    }

    else if (flag_carry != 1)
        printf("\nNAO\n");

    return contador_carry;
}

void resultados_comum (Operacao parametros, int saida[]) /* Mostra os resultados da soma sem nenhum corte. */
{
    printf("\nBINARIO:\n\n    "); /* Imprime a representacao da soma binaria dos operandos. */

    imprime_vetor(parametros.n1,parametros.bits);
    printf(" +\n    ");
    imprime_vetor(parametros.n2,parametros.bits);
    printf("\n-------------\n    ");
    imprime_vetor(saida,parametros.bits);
    printf("\n");

    printf("\nDECIMAL:\n\n    "); /* Imprime a representacao da soma decimal dos operandos. */

    printf("%ld", calcula_decimal(parametros.n1,parametros.bits));
    printf(" + ");
    printf("%ld", calcula_decimal(parametros.n2,parametros.bits));
    printf(" = ");
    printf("%ld", calcula_decimal(saida,parametros.bits));
}

void desprezo_cumulativo(Operacao parametros, int contador_carry) /* Realiza as somas com cortes cumulativos de carry out. */
{
    int flag_carry = 0, changes = 0;
    int saida[parametros.bits], vetor_carry[parametros.bits];

    long int aux, novo_decimal;
    long int decimal_somacomum = soma_decimal(parametros);

    inicializa_vetor(saida,parametros.bits);
    inicializa_vetor(vetor_carry,parametros.bits);

    for (int i = 1; i <= contador_carry; i++)
    {
        changes = 0;
        flag_carry = 0;

        for (int j = parametros.bits-1; j >= 0; j--)
        {
            if (flag_carry == 1 && changes < i) /* Realiza o corte de CARRY OUT. */
            {
                flag_carry = 0; /* Muda o valor do flag_carry out de um para zero, desprezando o valor. */
                changes++;
            }

            if (parametros.n1[j] == 1 && parametros.n2[j] == 1) /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
            {
                saida[j] = flag_carry; /* Atualiza o digito do numero binario resultante da soma. */
                flag_carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
            }

            else if (parametros.n1[j] == 0 && parametros.n2[j] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida[j] = flag_carry; /* Atualiza o digito do numero binario resultante da soma. */
                flag_carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
            }

            else /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                if (flag_carry == 1)
                {
                    saida[j] = 0; /* Atualiza o digito do numero binario resultante da soma. */
                    flag_carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
                }

                else
                {
                    saida[j] = 1; /* Atualiza o digito do numero binario resultante da soma. */
                    flag_carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
                }
            }
        }

        novo_decimal = calcula_decimal(saida,parametros.bits); /* Converte a saida da soma com corte para decimal, e armazena o valor. */

        if (i == 1) /* Comandos de selecao para verificar ate qual ponto e necessario calcular a soma. */
            aux = novo_decimal;

        else if (aux == novo_decimal)
            return;

        else
            aux = novo_decimal;

        printf("%d CORTES:\n", changes); /* Imprime o resultado de cada soma com corte. */
        printf("Valor binario:\t");
        imprime_vetor(saida,parametros.bits);
        printf("\n");
        printf("Valor decimal: \t");
        printf("%ld", novo_decimal);
        printf("\nErro absoluto:\t");
        erro_absoluto(novo_decimal,decimal_somacomum);
    }
}

void despreza_tudo(Operacao parametros) /* Realiza as somas desprezando todos os CARRY IN. */
{
    int saida_normal[parametros.bits], saida_nova[parametros.bits];

    inicializa_vetor(saida_normal,parametros.bits);
    inicializa_vetor(saida_nova,parametros.bits);

    calcula_saida(parametros,saida_normal); /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        if (parametros.n1[i] == 1 && parametros.n2[i] == 1) /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
            saida_nova[i] = 0;   /* Atualiza o digito do numero binario resultante da soma. */

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
            saida_nova[i] = 0;    /* Atualiza o digito do numero binario resultante da soma. */

        else
            saida_nova[i] = 1;
    }

    long int decimal_normal = calcula_decimal(saida_normal,parametros.bits);
    long int novo_decimal = calcula_decimal(saida_nova,parametros.bits); /* Converte a saida da soma com interferencia para decimal, e armazena o valor. */

    printf("Valor binario:\t");
    imprime_vetor(saida_nova,parametros.bits);
    printf("\n");
    printf("Valor decimal: \t");
    printf("%ld", novo_decimal);
    printf("\nErro absoluto:\t");
    erro_absoluto(novo_decimal, decimal_normal);
}

void despreza_maior(Operacao parametros) /* Realiza a soma desprezando o CARRY IN mais significativo. */
{
    int indice_carry = 0, carry = 0;
    int vetor_carry[parametros.bits], saida_normal[parametros.bits], saida_interf[parametros.bits];

    inicializa_vetor(vetor_carry,parametros.bits);
    inicializa_vetor(saida_normal,parametros.bits);
    inicializa_vetor(saida_interf,parametros.bits);

    calcula_saida(parametros,saida_normal); /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */
    calcula_carry(parametros,vetor_carry); /* Determina o vetor de carry entre os dois operandos, retornando no vetor de saida. */

    carry = 0;

    for (int i = 0; i < parametros.bits; i++) /* Acha o bit que corresponde ao CARRY OUT mais significativo. */
        if (vetor_carry[i] == 1)
        {
            indice_carry = i;
            break;
        }

    for (int i = parametros.bits-1; i >= 0; i--) /* Iteracao para realizar o corte mais significativa. */
    {
        if (i == indice_carry) /* Realiza o corte de CARRY OUT. */
            carry = 0; /* Muda o valor do CARRY IN mais significativo de um para zero, desprezando o valor. */

        if (parametros.n1[i] == 1 && parametros.n2[i] == 1)  /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            saida_interf[i] = carry; /* Atualiza o digito do numero binario resultante da soma. */
            carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
        }

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
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
    }

    long int decimal_normal = calcula_decimal(saida_normal,parametros.bits);
    long int novo_decimal = calcula_decimal(saida_interf,parametros.bits);

    printf("Valor binario:\t"); /* Imprime o resultado da soma com corte no CARRY IN mais significativo. */
    imprime_vetor(saida_interf,parametros.bits);
    printf("\n");
    printf("Valor decimal: \t");
    printf("%ld", novo_decimal);
    printf("\nErro absoluto:\t");
    erro_absoluto(novo_decimal, decimal_normal);

    printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
    getch();
}

void despreza_especifico(Operacao parametros) /* Realiza a soma desprezando CARRY IN especificos. */
{
    int carry = 0, contador_carry = 0, indice_interf = 0;

    int vetor_carry[parametros.bits];
    int saida_normal[parametros.bits];
    int saida_interf[parametros.bits];

    inicializa_vetor(vetor_carry,parametros.bits);
    inicializa_vetor(saida_normal,parametros.bits);
    inicializa_vetor(saida_interf,parametros.bits);

    for (int i = parametros.bits-1; i >= 0; i--) /* Iteracao para preencher o vetor contendo os CARRY IN. */
    {
        if (parametros.n1[i] == 1 && parametros.n2[i] == 1) /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            saida_normal[i] = carry; /* Atualiza o digito do numero binario resultante da soma. */
            carry = 1;  /* Atualiza o valor de CARRY OUT propagado. */
            if (i != 0)
                vetor_carry[i-1] = 1;
            contador_carry++; /* Incrementa o contador de CARRY OUT. */
        }

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
        {
            saida_normal[i] = carry;
            carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
        }

        else
        {
            if (carry == 1) /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida_normal[i] = 0;

                carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
                if (i != 0)
                    vetor_carry[i-1] = 1;

                contador_carry++; /* Incrementa o contador de CARRY OUT. */
            }

            else
                saida_normal[i] = 1; /* Atualiza o digito do numero binario resultante da soma. */
        }
    }

    if (carry == 1)
        contador_carry--;

    carry = 0;

    system("cls");

    int *cortes = calloc(contador_carry,sizeof(int));
    inicializa_vetor(cortes,contador_carry);

    recebe_especificos(cortes,vetor_carry,contador_carry,parametros.bits);

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        if (contem_vetor(cortes,indice_interf,contador_carry)) /* Realiza o corte de CARRY OUT, se necessario. */
            carry = 0; /* Muda o valor do CARRY IN escolhido de um para zero, desprezando o valor. */

        if (parametros.n1[i] == 1 && parametros.n2[i] == 1)  /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
        {
            saida_interf[i] = carry; /* Atualiza o digito do numero binario resultante da soma. */
            carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
        }

        else if (parametros.n1[i] == 0 && parametros.n2[i] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
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

    long int decimal_normal = calcula_decimal(saida_normal,parametros.bits);
    long int novo_decimal = calcula_decimal(saida_interf,parametros.bits); /* Converte a saida da soma com interferencia para decimal, e armazena o valor. */

    printf("Valor binario:\t");
    imprime_vetor(saida_interf,parametros.bits);
    printf("\n");
    printf("Valor decimal: \t");
    printf("%ld", novo_decimal);
    printf("\nErro absoluto:\t");
    erro_absoluto(novo_decimal, decimal_normal);

    printf("\nPressione qualquer tecla para voltar ao menu anterior.\n");
    getch();
}

void recebe_especificos(int cortes[], int vetor_carry[],int contador_carry, int bits) /* Recebe do usuario os bits nos quais ocorrerao interferencia de CARRY IN. */
{
    char texto_bits[DIMENSAO];
    char *aux_numero;
    int  i = 0, bit_individual = 0;

    printf("Ocorreu CARRY IN nos seguintes bits:\n");

    for (int j = 0; j < bits; j++)
        if (vetor_carry[j] == 1)
            printf("%d ",bits-j-1);

    inicializa_menosum(cortes,contador_carry);

    printf("\n\nDigite os bits em que ocorrerao os cortes de CARRY IN, separados por espacos ou virgulas:\n");
    gets(texto_bits);

    aux_numero = strtok(texto_bits,SEPARADORES);

    while (aux_numero != NULL)
    {
        bit_individual = atoi(aux_numero);
        cortes[i] = bit_individual;

        aux_numero = strtok(NULL,SEPARADORES);
        i++;

        if (i >= contador_carry)
            break;
    }

    printf("\n");
}

void imprime_comum_arq(FILE *arq, Operacao parametros, int saida[]) /* Imprime os dados da soma dos dois operandos, sem cortes, em um arquivo de saida. */
{
    fprintf(arq,"------------------------------\n");
    fprintf(arq,"OPERACAO ANALISADA:\n\n");

    for (int i = 0; i < parametros.bits; i++)
        fprintf(arq,"%d",parametros.n1[i]);
    fprintf(arq," +\n");
    for (int i = 0; i < parametros.bits; i++)
        fprintf(arq,"%d",parametros.n2[i]);
    fprintf(arq,"\n-------------\n");
    for (int i = 0; i < parametros.bits; i++)
        fprintf(arq,"%d",saida[i]);
    fprintf(arq,"\n");

    fprintf(arq,"\nDECIMAL:\n\n    "); /* Imprime a representacao da soma decimal dos operandos. */

    fprintf(arq,"%ld", calcula_decimal(parametros.n1,parametros.bits));
    fprintf(arq," + ");
    fprintf(arq,"%ld", calcula_decimal(parametros.n2,parametros.bits));
    fprintf(arq," = ");
    fprintf(arq,"%ld", calcula_decimal(saida,parametros.bits));

    fprintf(arq,"\n\n");
}

void gera_texto(Operacao parametros, int contador_carry) /* Gera o arquivo texto contendo somas com todas os cortes de CARRY IN possiveis. */
{
    FILE *arq;

    int saida_normal[parametros.bits], saida_interf[parametros.bits], vetor_carry[parametros.bits];
    int carry = 0, indice_interf = 0;

    inicializa_vetor(saida_normal,parametros.bits);
    inicializa_vetor(saida_interf,parametros.bits);
    inicializa_vetor(vetor_carry,parametros.bits);

    if (!(arq = fopen("somas.txt","a")))
    {
        system("cls");
        printf("Erro na criacao ou abertura do arquivo texto de saida. Fim do programa.\n");
        getch();
        exit(1);
    }

    calcula_saida(parametros,saida_normal); /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */
    calcula_carry(parametros,vetor_carry); /* Determina o vetor de carry entre os dois operandos, retornando no vetor de saida. */

    imprime_comum_arq(arq,parametros,saida_normal);

    int *cortes = calloc(contador_carry,sizeof(int));
    int *interf_aux = calloc(contador_carry,sizeof(int));

    int total_linhas = pow(2,contador_carry);
    int combinacoes[total_linhas][contador_carry];

    for (int i = 0; i < total_linhas; i++)
        for (int j = 0; j < contador_carry; j++)
            combinacoes[i][j] = 0;

    for (int i = 0; i < parametros.bits; i++)
        if (vetor_carry[i] == 1)
        {
            cortes[indice_interf] = parametros.bits-i-1;
            indice_interf++;
        }

    fprintf(arq,"\nBITS COM OCORRENCIA DE CARRY IN:\n");
    for (int i = 0; i < contador_carry; i++)
            fprintf(arq,"%d ", cortes[i]);

    fprintf(arq,"\n\n");

    long int decimal_normal = calcula_decimal(saida_normal,parametros.bits);
    long int novo_decimal;

    for (int linha = 0; linha < total_linhas; linha++) /* Iteracao para imprimir os dados no arquivo texto de saida. */
    {
        converte_binario(linha,contador_carry,combinacoes[linha]);

        for (int j = 0; j < contador_carry; j++)
        {
            if (combinacoes[linha][j] == 1)
                interf_aux[j] = cortes[j];

            else
                interf_aux[j] = -1;
        }

        indice_interf = carry = 0;

        for (int j = parametros.bits-1; j >= 0; j--)
        {
            if (contem_vetor(interf_aux,indice_interf,contador_carry)) /* Realiza o corte de CARRY OUT. */
                carry = 0; /* Muda o valor do CARRY IN escolhido de um para zero, desprezando o valor. */

            if (parametros.n1[j] == 1 && parametros.n2[j] == 1)  /* Faz a soma entre dois digitos 1, atualizando o valor de CARRY OUT propagado. */
            {
                saida_interf[j] = carry; /* Atualiza o digito do numero binario resultante da soma. */
                carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
            }

            else if (parametros.n1[j] == 0 && parametros.n2[j] == 0) /* Faz a soma entre dois digitos 0, atualizando o valor de CARRY OUT propagado. */
            {
                saida_interf[j] = carry; /* Atualiza o digito do numero binario resultante da soma. */
                carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
            }

            else
            {
                if (carry == 1) /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
                {
                    saida_interf[j] = 0; /* Atualiza o digito do numero binario resultante da soma. */
                    carry = 1; /* Atualiza o valor de CARRY OUT propagado. */
                }

                else /* Faz a soma entre um digito 1 e um digito 0, atualizando o valor de CARRY OUT propagado. */
                {
                    saida_interf[j] = 1; /* Atualiza o digito do numero binario resultante da soma. */
                    carry = 0; /* Atualiza o valor de CARRY OUT propagado. */
                }
            }

            indice_interf++;
        }

        novo_decimal = calcula_decimal(saida_interf,parametros.bits); /* Converte a saida da soma com interferencia para decimal, e armazena o valor. */

        fprintf(arq,"------------------------------\n");

        if (linha == 0)
            fprintf(arq,"\nOPERACAO 1: sem interferencia de CARRY IN");

        else
        {
            fprintf(arq,"\nOPERACAO %d: interferencia nos bits ", linha+1);

            for (int j = 0; j < contador_carry; j++)
                if (interf_aux[j] != -1)
                    fprintf(arq,"%d ", interf_aux[j]);
        }

        fprintf(arq,"\n\nValor binario:\t");
        for (int j = 0; j < parametros.bits; j++)
            fprintf(arq,"%d",saida_interf[j]);
        fprintf(arq,"\n");
        fprintf(arq,"Valor decimal: \t");
        fprintf(arq,"%ld\n", novo_decimal);
        fprintf(arq,"\nErro absoluto:\t");
        erro_absoluto_arq(arq,novo_decimal, decimal_normal);
        fprintf(arq,"------------------------------\n");
    }

    fclose(arq);

    printf("Arquivo texto de saida ""somas.txt"" atualizado com sucesso.\n");
    printf("\nPressione qualquer tecla para voltar ao menu anterior.");
    getch();
}

void inducao_carry(Operacao parametros, int contador_carry) /* Realiza a soma com inducoes especificas de CARRY IN. */
{
    int saida_normal[parametros.bits], saida_inducao[parametros.bits], vetor_carry[parametros.bits], inducoes[parametros.bits];
    int carry = 0, indice_inducao = 0, bit_individual = 0;

    char texto_bits[DIMENSAO];
    char *aux_numero;

    inicializa_vetor(saida_normal,parametros.bits);
    inicializa_vetor(saida_inducao,parametros.bits);
    inicializa_vetor(vetor_carry,parametros.bits);

    inicializa_menosum(inducoes,parametros.bits);

    calcula_saida(parametros,saida_normal); /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */
    calcula_carry(parametros,vetor_carry); /* Determina o vetor de carry entre os dois operandos, retornando no vetor de saida. */

    if (contador_carry == 0)
        printf("Nao ocorreu nenhum CARRY IN durante a soma.");

    else
        printf("Ocorreu CARRY IN nos seguintes bits:\n");

    for (int j = 0; j < parametros.bits; j++)
        if (vetor_carry[j] == 1)
            printf("%d ",parametros.bits-j-1);

    printf("\n\nDigite os bits em que ocorrerao as inducoes, separados por espacos ou virgulas:\n");
    gets(texto_bits);

    aux_numero = strtok(texto_bits,SEPARADORES);

    while (aux_numero != NULL)
    {
        bit_individual = atoi(aux_numero);
        inducoes[indice_inducao] = bit_individual;

        aux_numero = strtok(NULL,SEPARADORES);
        indice_inducao++;

        if (indice_inducao >= contador_carry)
            break;
    }

    indice_inducao = 0;

    for (int i = parametros.bits-1; i >= 0; i--)
    {
        if (contem_vetor(inducoes,indice_inducao,parametros.bits)) /* Realiza o corte de CARRY OUT. */
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

    long int decimal_normal = calcula_decimal(saida_normal,parametros.bits);
    long int novo_decimal = calcula_decimal(saida_inducao,parametros.bits); /* Converte a saida da soma com interferencia para decimal, e armazena o valor. */

    printf("\nValor binario:\t");
    imprime_vetor(saida_inducao,parametros.bits);
    printf("\n");
    printf("Valor decimal: \t");
    printf("%ld", novo_decimal);
    printf("\nErro absoluto:\t");
    erro_absoluto(novo_decimal, decimal_normal);
}
