#ifndef OPERACOES_COMUNS_H_INCLUDED
#define OPERACOES_COMUNS_H_INCLUDED

typedef struct /* Estrutura com informacoes dos operandos de uma soma binaria. */
{
    int *n1;
    int *n2;
    int bits;

} Operacao;

void converte_binario(int entrada, int bits, int saida[]); /* Converte um numero inteiro decimal de entrada em um numero binario de saida, embutido em um vetor. */

int recebe_bits(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */

int controla_binario(int binario[], int bits); /* Verifica se o numero recebido e binario. */

void erro_absoluto_arq(FILE *arq, long int resultado, long int referencia); /* Calcula a diferenca entre os dois valores obtidos. */

void erro_absoluto(long int resultado, long int referencia); /* Calcula e mostra a diferenca entre os dois valores obtidos. */

long int calcula_erroabsoluto(long int resultado, long int referencia); /* Calcula e retorna a diferenca entre os dois valores obtidos. */

int contem_vetor(int v[], int n, int dimensao); /* Verifica se um vetor possui um determinado numero inteiro. */

void copia_vetor(int entrada[], int saida[],int dimensao);

void inicializa_menosum(int vetor[], int dimensao); /* Inicializa um vetor de entrada com os valores -1. */

void inicializa_vetor(int vetor[], int dimensao); /* Inicializa um vetor de entrada com valores iguais a zero. */

void imprime_vetor(int v[], int dimensao); /* Realiza a impressao completa do vetor na tela. */

long int calcula_decimal(int n[], int bits); /* Converte um numero binario para decimal. */

void calcula_carry (Operacao parametros, int vetor_carry[]); /* Determina o vetor de carry entre os dois operandos, retornando no vetor de saida. */

void calcula_saida (Operacao parametros, int saida[]); /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */

long int soma_decimal(Operacao parametros); /* Retorna o valor decimal da soma entre os operandos. */

void mostra_estatisticas_arq(FILE *arq, int diferentes, double media_erros, double desvio_padrao, double quantidade_somas); /* Imprime os resultados estatisticos das somas no arquivo de saida. */

void interferencias_estatisticas(int cortes[], int bits); /* Obtem do usuario os bits em que ocorrerao cortes de CARRY IN fixos. */

int recebe_bits_estatistica(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria, para operacoes estatisticas. */

long int calcula_saidainterf (Operacao *parametros, int cortes[]); /* Determina a soma com interferencias de CARRY OUT entre os dois operandos, retornando no vetor de saida. */

void soma_um (Operacao *parametros); /* Incrementa um vetor contendo um numero binario em uma unidade. */

void mostra_estatisticas(int diferentes, double media_erros, double desvio_padrao, int quantidade_somas); /* Exibe os resultados estatisticos das somas na tela. */

void preenche_maxmin(Operacao parametros); /* Preenche os vetores com os valores maximos e minimos das somas, para analise estatistica. */

#endif // OPERACOES_COMUNS_H_INCLUDED
