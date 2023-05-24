#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#define ESC 27

#define NOME_ARQSAIDA_ESPECIFICAS "fontes_especificas.txt"
#define NOME_ARQSAIDA_ALEATORIAS "fontes_aleatorias.txt"

typedef struct
{
    int bits;
    int quantidade_operacoes;
    float tempo_operacoes;
    float tempo_transicao;

    int numero_fonte;
    float valor_tempo;
    int tensao_atual;

} Operacoes;

void converte_binario(int op_1, int op_2, int* vetor, int bits)
{
    for (int i = bits-1; i >= 0; i--)
    {
        vetor[i] = op_1 % 2;
        op_1 = op_1 / 2;
    }

    for (int i = 2*bits-1; i >= bits; i--)
    {
        vetor[i] = op_2 % 2;
        op_2 = op_2 / 2;
    }
}

void recebe_dados(Operacoes *dados)
{
    printf("\nDigite o numero de bits dos operandos:\n");
    scanf("%d", &dados->bits);
    printf("\nDigite quantas operacoes de soma serao representadas:\n");
    scanf("%d", &dados->quantidade_operacoes);
    printf("\nDigite o tempo existente (em nanosegundos) de duracao de cada operacao:\n");
    scanf("%f", &dados->tempo_operacoes);
    printf("\nDigite o tempo (em nanosegundos) de transicao entre cada nivel de tensao:\n");
    scanf("%f", &dados->tempo_transicao);
}

void gera_saida(char nome_arqsaida[], Operacoes dados, int **ondas, int op_1[], int op_2[])
{
    printf("\n\nPressione X para anexar tudo no arquivo %s.\n", nome_arqsaida);
    printf("\nPressione qualquer outra tecla para voltar ao menu inicial.\n");
    char tecla_escolha = getch();

    switch(tecla_escolha)
    {
        case 'x':
        case 'X':
        {
            FILE *arq_saida;
            if (!(arq_saida = fopen(nome_arqsaida,"w")))
            {
                system("cls");
                printf("Erro na abertura do arquivo texto de saida. Fim do programa.\n");
                exit(1);
            }

            fprintf(arq_saida,"SOMAS A SEREM REPRESENTADAS:\n\n");
            for (int i = 0; i < dados.quantidade_operacoes; i++)
                fprintf(arq_saida,"%d + %d\n", op_1[i],op_2[i]);

            fprintf(arq_saida,"\nFONTES NA SINTAXE DO NGSPICE:\n\n");
            for (int i = 0; i < 2*dados.bits; i++)
            {
                if (i < dados.bits)
                {
                    dados.numero_fonte = dados.bits-i-1;
                    fprintf(arq_saida,"V%d a%d 0 pwL( ", dados.numero_fonte, dados.numero_fonte);
                }
                else
                {
                    dados.numero_fonte = (3*dados.bits)-i-1;
                    fprintf(arq_saida,"V%d b%d 0 pwL( ", dados.numero_fonte, dados.numero_fonte-dados.bits);
                }

                dados.tensao_atual = ondas[0][i];
                for (int j = 0; j < dados.quantidade_operacoes; j++)
                {
                    if (ondas[j][i] != dados.tensao_atual)
                    {
                       dados.tensao_atual = ondas[j][i];
                       fprintf(arq_saida,"%0.2fns %dV ", (dados.valor_tempo-dados.tempo_operacoes)+dados.tempo_transicao, ondas[j][i]);
                    }

                    fprintf(arq_saida,"%0.2fns %dV ", dados.valor_tempo, ondas[j][i]);
                    dados.valor_tempo += dados.tempo_operacoes;
                }

                fprintf(arq_saida,")\n");
                dados.valor_tempo = dados.tempo_operacoes;
            }

            fprintf(arq_saida,"\nFULL ADDER NA SINTAXE DO NGSPICE:\n\n");
            for (int i = dados.bits-1; i > 0; i--)
                fprintf(arq_saida,"XFA%d\ta%d b%d C%d vdd gnd s%d C%d FA2\n",i,i,i,i-1,i,i);
            fprintf(arq_saida,"XFA0\ta0 b0 0 vdd gnd s0 C0 FA2");
            fclose(arq_saida);
        }
        break;

        default:
            return;
        break;
    }
}

void imprime_dados(Operacoes dados, int **ondas, int op_1[], int op_2[])
{
    system("cls");
    printf("SOMAS A SEREM REPRESENTADAS:\n\n");
    for (int i = 0; i < dados.quantidade_operacoes; i++)
        printf("%d + %d\n", op_1[i],op_2[i]);

    printf("\nFONTES NA SINTAXE DO NGSPICE:\n\n");
    for (int i = 0; i < 2*dados.bits; i++)
    {
        if (i < dados.bits)
        {
            dados.numero_fonte = dados.bits-i-1;
            printf("V%d a%d 0 pwL( ", dados.numero_fonte, dados.numero_fonte);
        }
        else
        {
            dados.numero_fonte = (3*dados.bits)-i-1;
            printf("V%d b%d 0 pwL( ", dados.numero_fonte, dados.numero_fonte-dados.bits);
        }

        dados.tensao_atual = ondas[0][i];
        for (int j = 0; j < dados.quantidade_operacoes; j++)
        {
            if (ondas[j][i] != dados.tensao_atual)
            {
               dados.tensao_atual = ondas[j][i];
               printf("%0.2fns %dV ", (dados.valor_tempo-dados.tempo_operacoes)+dados.tempo_transicao, ondas[j][i]);
            }

            printf("%0.2fns %dV ", dados.valor_tempo, ondas[j][i]);
            dados.valor_tempo += dados.tempo_operacoes;
        }

        printf(")\n");
        dados.valor_tempo = dados.tempo_operacoes;
    }

    printf("\nFULL ADDER NA SINTAXE DO NGSPICE:\n\n");
    for (int i = dados.bits-1; i > 0; i--)
        printf("XFA%d\ta%d b%d C%d vdd gnd s%d C%d FA2\n",i,i,i,i-1,i,i);
    printf("XFA0\ta0 b0 0 vdd gnd s0 C0 FA2");
}

void ondas_especificas()
{
    system("cls");
    Operacoes dados;
    printf("GERACAO DE ONDAS PWL ESPECIFICAS\n");
    recebe_dados(&dados);

    int op_1[dados.quantidade_operacoes],op_2[dados.quantidade_operacoes];

    int **ondas;
    ondas = malloc(dados.quantidade_operacoes*sizeof(int*));
    for(int i = 0; i < dados.quantidade_operacoes; i++)
            ondas[i] = malloc(2*dados.bits*sizeof(int));

    int valor_maximo = pow(2,dados.bits);
    for (int i = 0; i < dados.quantidade_operacoes; i++)
    {
        system("cls");
        printf("SOMA %d --\n", i+1);
        printf("\nDigite o primeiro operando:\n");
        scanf("%d", &op_1[i]);
        printf("\nDigite o segundo operando:\n");
        scanf("%d", &op_2[i]);

        if(op_1[i] > valor_maximo)
            op_1[i] = valor_maximo;
        if(op_2[i] > valor_maximo)
            op_2[i] = valor_maximo;
        converte_binario(op_1[i],op_2[i],ondas[i],dados.bits);
    }

    dados.numero_fonte = 0;
    dados.valor_tempo = dados.tempo_operacoes;

    imprime_dados(dados,ondas,op_1,op_2);
    gera_saida(NOME_ARQSAIDA_ESPECIFICAS, dados, ondas, op_1, op_2);

    for(int i = 0 ; i < dados.quantidade_operacoes; i++)
        free(ondas[i]);
    free(ondas);
}

void ondas_aleatorias()
{
    system("cls");
    Operacoes dados;
    printf("GERACAO DE ONDAS PWL ALEATORIAS\n");
    recebe_dados(&dados);

    int op_1[dados.quantidade_operacoes],op_2[dados.quantidade_operacoes];

    int **ondas;
    ondas = malloc(dados.quantidade_operacoes*sizeof(int*));
    for(int i = 0; i < dados.quantidade_operacoes; i++)
            ondas[i] = malloc(2*dados.bits*sizeof(int));

    int valor_maximo = pow(2,dados.bits);
    for (int i = 0; i < dados.quantidade_operacoes; i++)
    {
        op_1[i] = rand() % valor_maximo;
        op_2[i] = rand() % valor_maximo;
        converte_binario(op_1[i],op_2[i],ondas[i],dados.bits);
    }

    dados.numero_fonte = 0;
    dados.valor_tempo = dados.tempo_operacoes;

    imprime_dados(dados,ondas,op_1,op_2);
    gera_saida(NOME_ARQSAIDA_ALEATORIAS, dados, ondas, op_1, op_2);
    for(int i = 0 ; i < dados.quantidade_operacoes; i++)
        free(ondas[i]);
    free(ondas);
}

void inicia_programa()
{
    while (1)
    {
        system("cls");
        printf("Pressione a tecla para escolher a opcao desejada:\n");
        printf("\n1 - Geracao de ondas PWL especificas.\n");
        printf("\n2 - Geracao de ondas PWL aleatorias.\n");
        printf("\nESC - Encerrar programa.\n");
        char tecla_escolha = getch();

        switch (tecla_escolha)
        {
            case '1':
                ondas_especificas();
            break;

            case '2':
                ondas_aleatorias();
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
    system("PAUSE");
    return 0;
}
