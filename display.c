/*
 * display.c
 *
 *  Created on: Nov 1, 2020
 *      Author: Andrei de Oliveira
 */

#include "display.h"
/*------ Variaveis Globais -------*/
//Variaveis de controle do display: dados | seleção | Chip select
uint16_t D4, D5, D6, D7, E, RS;

//Endereços da CGRAM 0, 1, 2, 3, 4, 6, 7
char cg0 = 0x40;
char cg1 = 0x48;
char cg2 = 0x50;
char cg3 = 0x58;
char cg4 = 0x60;


//binarios das caracteres especiais
//ç
char cd[8]={0b00011110,
			0b00010000,
			0b00010000,
			0b00010000,
			0b00010000,
			0b00011110,
			0b00000100,
			0b00000110};
//á
char ac[8]={0b00000010,
			0b00000100,
			0b00011111,
			0b00000001,
			0b00011111,
			0b00010001,
			0b00011111,
			0b00000000};
//é
char ec[8]={0b00000010,
		    0b00000100,
		    0b00011111,
		    0b00010001,
		    0b00011111,
		    0b00010000,
		    0b00011111,
		    0b00000000
};
//ã
char at[8]={0b00001110,
			0b00000000,
			0b00011111,
			0b00000001,
			0b00011111,
			0b00010001,
			0b00011111,
			0b00000000};
//õ
char ot[8]={0b00001110,
		    0b00000000,
			0b00011111,
			0b00010001,
			0b00010001,
			0b00010001,
			0b00011111,
			0b00000000};


/*----- Inicio das funções-----*/

/**
 *
 */
void tempo(void){
	HAL_GPIO_WritePin(GPIOC, E, GPIO_PIN_SET);//E
	HAL_Delay(15);
	HAL_GPIO_WritePin(GPIOC, E, GPIO_PIN_RESET);
}
/**
 * @brief Inicializa o display.
 *
 * Essa função é responsavel por fazer a configuração inicial do display.
 * Ele é inicializado como 16x2, sem cursor e escreve deslocando o cursor para a direita.
 * Também irá carregar as carracteres especiais criadas para a CGRAM.
 */

void inicializa (void){
	//Intruções de inicialização
	instrucoes(0x33);
	instrucoes(0x32);
	instrucoes(0x28);
	instrucoes(0x0C);

	//instruções para escrever na CGRAM
	caracteres_especiais(cg0, cd);
	caracteres_especiais(cg1, ac);
	caracteres_especiais(cg2, ec);
	caracteres_especiais(cg3, at);
	caracteres_especiais(cg4, ot);

}
/**
 *
 */
void instrucoes(char hexa){

	int8_t conv_b[8];
	int16_t x = hexa; //converte pra inteiro

	//converte para binario
	conversorB(hexa, &conv_b);

	HAL_GPIO_WritePin(GPIOC, E, GPIO_PIN_RESET); //Desabilita o chip select
	HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_RESET); //Habilita a seleção de instrução


	// Manda para o barramento de dados os 4 primeiros bit.
	HAL_GPIO_WritePin(GPIOC, D7, conv_b[0]);
	HAL_GPIO_WritePin(GPIOC, D6, conv_b[1]);
	HAL_GPIO_WritePin(GPIOC, D5, conv_b[2]);
	HAL_GPIO_WritePin(GPIOC, D4, conv_b[3]);
	tempo();

	// Manda para o barramento de dados os 4 ultimos bit.
	HAL_GPIO_WritePin(GPIOC, D7, conv_b[4]);
	HAL_GPIO_WritePin(GPIOC, D6, conv_b[5]);
	HAL_GPIO_WritePin(GPIOC, D5, conv_b[6]);
	HAL_GPIO_WritePin(GPIOC, D4, conv_b[7]);
	tempo();

	HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_SET);//Habilita a seleção de dados
}

/**
 *
 */
void conversorB(uint16_t inte, uint8_t *convB){
	for (uint16_t i=7; i>= 0 ; i --){
		if((inte % 2)==0 )convB[i]=0;
		else convB[i]=1;
		inte=inte/2;
	}
}

/**
 *
 */
void caracteres_especiais(char posicao, char caracter){

	instrucoes(posicao); //manda o endereço para as instruções iniciarem a CGRAM

	int8_t bin[8];
	int16_t inte;

	for(int8_t i=0; i < 8 ;i++){
		inte = caracter[i];
		conversorB(inte, *bin);
		HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_SET); //Habilita a seleção de DADOS

		HAL_GPIO_WritePin(GPIOC, E, GPIO_PIN_RESET); //Desabilita o chip select
		HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_RESET); //Habilita a seleção de instrução

		// Manda para o barramento de dados os 4 primeiros bit.
		HAL_GPIO_WritePin(GPIOC, D7, conv_b[0]);
		HAL_GPIO_WritePin(GPIOC, D6, conv_b[1]);
		HAL_GPIO_WritePin(GPIOC, D5, conv_b[2]);
		HAL_GPIO_WritePin(GPIOC, D4, conv_b[3]);
		tempo();

		// Manda para o barramento de dados os 4 ultimos bit.
		HAL_GPIO_WritePin(GPIOC, D7, conv_b[4]);
		HAL_GPIO_WritePin(GPIOC, D6, conv_b[5]);
		HAL_GPIO_WritePin(GPIOC, D5, conv_b[6]);
		HAL_GPIO_WritePin(GPIOC, D4, conv_b[7]);
		tempo();
	}
}

/**
 *
 */
void limpar(void){
	instrucoes(0x01);
}

void escreve_char(char posicao, char *texto){
	uint16_ i=0;

	instrucoes(posicao);
	while(i < 17 & texto[i] != '\0'){

	}
}





