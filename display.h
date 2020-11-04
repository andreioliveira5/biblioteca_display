/*
 * display.h
 *
 *  Created on: Nov 1, 2020
 *      Author: Andrei de Oliveira
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


/*
 *função de tempo entre o envio de cada mensagem
 */
void tempo(void);

/*
 * Inicializa o display com as onfigurações inifiais
 */
void inicializa(void);

/*
 * Configura as instruções das outras funções
 */
void instrucoes(char hexa);

/*
 * converte valores hexa em binario e retorna um vetor de binario
 */
void conversorB(int16_t hexa, int8_t *convB);

/*
 * Escreve caracteres especiais
 */
void caracteres_especiais(char posicao, char *caracter);

/*
 *
 */
void escreve_string(char posicao, char *texto);
/*
 * Adiciona as variaveis padão escolhidas
 *  (usar portas C previamente escolhidas)
 */
void variaveis(char posicao, int32_t valor);
/*
 * Limpar display
 */
void limpar(void);

/*
 * Escreve variaveis numericas no display
 */
void escreve_numero(char posicao, int32_t valor);

void escreve_char(char texto);

#endif /* INC_DISPLAY_H_ */

