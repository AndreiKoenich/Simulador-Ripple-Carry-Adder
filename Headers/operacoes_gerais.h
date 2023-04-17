#ifndef OPERACOES_GERAIS_H_INCLUDED
#define OPERACOES_GERAIS_H_INCLUDED

typedef struct /* Estrutura com informacoes dos operandos de uma soma binaria. */
{
    int *n1;
    int *n2;
    int bits;

} Operacao;

void calcula_desviopadrao(long int amostra[], long double media, int total_operacoes);

void converte_binario(int entrada, int bits, int saida[]); /* Converte um numero inteiro decimal de entrada em um numero binario de saida, embutido em um vetor. */

int recebe_bits(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria. */

void inicializa_parametros(Operacao *parametros);

int controla_binario(int binario[], int bits); /* Verifica se o numero recebido e binario. */

void erro_absoluto_arq(FILE *arq, long int resultado, long int referencia); /* Calcula a diferenca entre os dois valores obtidos. */

void erro_absoluto(long int resultado, long int referencia); /* Calcula e mostra a diferenca entre os dois valores obtidos. */

void erro_relativo(long int resultado, long int referencia); /* Calcula o erro relativo entre os dois resultados obtidos, com e sem interferencias de CARRY IN. */

void erro_relativo_arq(FILE *arq, long int resultado, long int referencia); /* Calcula a diferenca entre os dois valores obtidos. */

long int calcula_erroabsoluto(long int resultado, long int referencia); /* Calcula e retorna a diferenca entre os dois valores obtidos. */

int contem_vetor(int v[], int n, int dimensao); /* Verifica se um vetor possui um determinado numero inteiro. */

void copia_vetor(int entrada[], int saida[],int dimensao); /* Copia o conteudo de um vetor de entrada para um vetor de saida. */

void inicializa_menosum(int vetor[], int dimensao); /* Inicializa um vetor de entrada com os valores -1. */

void inicializa_vetor(int vetor[], int dimensao); /* Inicializa um vetor de entrada com valores iguais a zero. */

void imprime_vetor(int v[], int dimensao); /* Realiza a impressao completa do vetor na tela. */

int calcula_decimal(int n[], int bits); /* Converte um numero binario para decimal. */

void calcula_carry (Operacao parametros, int vetor_carry[]); /* Determina o vetor de carry entre os dois operandos, retornando no vetor de saida. */

void saida_comum (Operacao parametros, int saida[]); /* Determina a soma entre os dois operandos sem cortes, retornando no vetor de saida. */

void novocircuitoOR (Operacao parametros, int saida[]); /* Determina a soma entre os dois operandos trocando as portas XOR por OR no RCA. */

void novocircuitoAND (Operacao parametros, int saida[]); /* Determina a soma entre os dois operandos trocando as portas XOR por AND no RCA. */

long int soma_decimal(Operacao parametros); /* Retorna o valor decimal da soma entre os operandos. */

void mostra_estatisticas_arq(FILE *arq, int diferentes, double media_erros, double desvio_padrao, double quantidade_somas); /* Imprime os resultados estatisticos das somas no arquivo de saida. */

void interferencias_estatisticas(int cortes[], int bits); /* Obtem do usuario os bits em que ocorrerao cortes de CARRY IN fixos. */

int recebe_bits_estatistica(); /* Obtem do usuario a quantidade de bits dos operandos e do resultado da soma binaria, para operacoes estatisticas. */

long int calcula_saidacortes (Operacao *parametros, int cortes[]); /* Determina a soma com interferencias de CARRY OUT entre os dois operandos, retornando no vetor de saida. */

long int calcula_saidainducoes(Operacao parametros, int inducoes[]);

void soma_um (Operacao *parametros); /* Incrementa um vetor contendo um numero binario em uma unidade. */

void recebe_inducoes(int inducoes[]); /* Recebe do usuario os bits nos quais ocorrerao inducoes de CARRY IN. */

void calcula_inducoes(Operacao parametros, int inducoes[], int saida_inducao[]); /* Realiza a soma entre dos operandos binarios, com inducoes de CARRY IN. */

void mostra_estatisticas(int diferentes, double media_erros, double desvio_padrao, int quantidade_somas); /* Exibe os resultados estatisticos das somas na tela. */

int recebe_total(long int valor_max); /* Recebe do usuario o numero total de operacoes a serem realizadas. */

void mostra_umporum(Operacao parametros, long int erro_parcial); /* Mostra cada par de operandos binarios e o erro absoluto obtido (funcao para fins de depuracao). */

void preenche_maxmin(Operacao parametros); /* Preenche os vetores com os valores maximos e minimos das somas, para analise estatistica. */

void interferencias_metricas(int interferencias[], int bits, int codigo_interferencia);

#endif // OPERACOES_GERAIS_H_INCLUDED
