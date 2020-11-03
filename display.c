/*
 * display.c
 *
 *  Created on: Nov 1, 2020
 *      Author: Andrei de Oliveira
 */

#include "display.h"
/*------ Variaveis Globais -------*/
//Variaveis de controle do display: dados | seleção | Chip select
//uint16_t D4=0, D5=1, D6=2, D7=3, E=4, RS=5;

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
 * @brief Função temporizadora
 *
 * Função de tempo necessaria para o funcionamento do lcd.
 */
void tempo(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);//E
	HAL_Delay(15);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
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
	caracteres_especiais(cg0, &cd);
	caracteres_especiais(cg1, &ac);
	caracteres_especiais(cg2, &ec);
	caracteres_especiais(cg3, &at);
	caracteres_especiais(cg4, &ot);

}
/**
 * @brief Inicialização
 *
 * Essa função aciona o modo de instução do lcd no pino RS e carrega os dados da instução.
 *
 * @param[in] hexa: char com o valor em hexadecimal da instução a ser executada.
 */
void instrucoes(char hexa){

	int8_t conv_b[8];
	int16_t x = hexa; //converte pra inteiro

	//converte para binario
	conversorB(hexa, &conv_b);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //Desabilita o chip select
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //Habilita a seleção de instrução


	// Manda para o barramento de dados os 4 primeiros bit.
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, conv_b[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, conv_b[1]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, conv_b[2]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, conv_b[3]);
	tempo();

	// Manda para o barramento de dados os 4 ultimos bit.
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, conv_b[4]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, conv_b[5]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, conv_b[6]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, conv_b[7]);
	tempo();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);//Habilita a seleção de dados
}

/**
 *@brief Conversor para binário
 *
 *Essa função converte um valor inteiro para um vetor de binarios.
 *
 *@param[in] inte: unsigned int de 16 bits, contendo o valor a ser convertido.
 *@param[in] *convB: ponteiro para um unsigned int de 8 bits, que será armazenado o valor convertido.
 */
void conversorB(uint16_t inte, uint8_t *convB){
	for (uint16_t i=7; i>= 0 ; i --){
		if((inte % 2)==0 )convB[i]=0;
		else convB[i]=1;
		inte=inte/2;
	}
}

/**
 *@brief  Criador das caracteres especiais
 *
 * Essa função ira iniciar o endereço do CGRAM para armazenar a caractere especial desejada.
 * Chama a função que para armazenar os 8 bytes do caractere.
 *
 * param[in]posicao: char contendo o endereço que deve ser gravado os dados.
 * param[in]caracter: char contendo o desenho da caractere
 */
void caracteres_especiais(char posicao, char *caracter){

	instrucoes(posicao); //manda o endereço para as instruções iniciarem a CGRAM


	for(int8_t i=0; i < 8 ;i++){
		escreve_char(&caracter[i]);
	}
}

/**
 *@brief Limpa o display
 *
 *Essa função limpa a tela do display.
 */
void limpar(void){
	instrucoes(0x01);
}
/**
 * @brief Escreve caracter no display
 *
 *	Essa função escreve o valor do texto enviado no display
 *
 *	@param[in] texto: char contendo o texto que devera ser escrito no display.
 */
void escreve_char(char *texto){


	int8_t bin[8];
	int16_t inte;
	inte = texto;
	conversorB(inte, *bin);


	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Habilita a seleção de DADOS

		// Manda para o barramento de dados os 4 primeiros bit.
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, bin[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, bin[1]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, bin[2]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, bin[3]);
	tempo();

			// Manda para o barramento de dados os 4 ultimos bit.
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, bin[4]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, bin[5]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, bin[6]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, bin[7]);
	tempo();


}
/**
 * @biref Escrever String
 *
 *	Essa função ira gerenciar a escria de uma string no display.
 *
 *	@param[in] posicao: char contendo a posição que deve ser escrita no display
 *	@param[in] *texto: ponteiro para um vetor de char contendo a string que deve ser escrita no display.
 */
void escreve_string(char posicao, char *texto){
	uint16_t i=0;

	instrucoes(posicao);
	while(i < 17 & texto[i] != '\0'){
		if(texto[i] == 'ç'){
			texto[i]=0;
		}
		if(texto[i] == 'á'){
			texto[i]=1;
		}
		if(texto[i] == 'é'){
			texto[i]=2;
		}
		if(texto[i] == 'ã'){
			texto[i]=3;
		}
		if(texto[i] == 'õ'){
			texto[i]=4;
		}
		escreve_char(texto[i]);
		i++;
	}
}





