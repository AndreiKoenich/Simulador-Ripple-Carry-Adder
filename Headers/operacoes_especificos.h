#ifndef OPERACOES_ESPECIFICOS_H_INCLUDED
#define OPERACOES_ESPECIFICOS_H_INCLUDED

void mostra_opcoes(); /* Mostra o menu de opcoes ao usuario. */

void operacoes(); /* Realiza as operacoes comum e com interferencia de CARRY IN entre os operandos binarios. */

void preenche_vetores(Operacao *parametros);/* Preenche os vetores com os valores binarios dos operandos. */

int soma_comum(Operacao parametros); /* Faz a soma entre os operandos binarios, sem qualquer corte em CARRY IN. */

void resultados_comum (Operacao parametros, int saida[]); /* Mostra os resultados da soma sem nenhum corte. */

void desprezo_cumulativo(Operacao parametros, int contador_carry); /* Realiza as somas com cortes cumulativos de carry out. */

void despreza_tudo(Operacao parametros); /* Realiza as somas desprezando todos os CARRY IN. */

void despreza_maior(Operacao parametros); /* Realiza a soma desprezando o CARRY IN mais significativo. */

void despreza_especifico(Operacao parametros); /* Realiza a soma desprezando CARRY IN especificos. */

void recebe_especificos(int cortes[], int vetor_carry[],int contador_carry, int bits); /* Recebe do usuario os bits nos quais ocorrerao interferencia de CARRY IN. */

void gera_texto(Operacao parametros, int contador_carry); /* Gera o arquivo texto contendo somas com todas os cortes de CARRY IN possiveis. */

void imprime_comum_arq(FILE *arq, Operacao parametros, int saida[]); /* Imprime os dados da soma dos dois operandos, sem cortes, em um arquivo de saida. */

void inducao_carry(Operacao parametros, int contador_carry); /* Realiza a soma com inducoes especificas de CARRY IN. */

#endif // OPERACOES_ESPECIFICOS_H_INCLUDED
