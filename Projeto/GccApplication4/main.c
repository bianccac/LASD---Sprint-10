/*
 * GccApplication2.c
 *
 * Created: 17/07/2021 13:34:27
 * Laboratório de Arquitetura e Sistemas Digitais
 Aluna: Biancca Cavalcante de Almeida Bezerra
 Matricula: 118210714
 Turma: 02

 SPRINT 10
 */
#define F_CPU 16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

//Definindo as variaveis que serão usadas
int out = 0b00000001;
uint8_t cont = 0, cont2 = 0, aux = 0, aux2 = 0, aux3 = 0, tempo = 0;
static uint8_t carmin = 0;
uint8_t flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0;
uint8_t pessoa = 0, luz = 0;
uint8_t verde = 1, vermelho = 1, amarelo = 1;
uint16_t sel;
uint32_t tempo_ms = 0;
// Botões para decremento e incremento do display e para indicar o tempo do sinal
ISR(INT0_vect)
{

	switch(cont) //Para indicar em qual sinal está tendo aquela variação no tempo
	{
	case 0:
		if (aux == 0)
		{
			aux++;
		}
		else
		{
			aux--;
		}
		break;
	case 1:
		if (tempo == 0)
		{
			tempo++;
		}
		else
		{
			tempo--;
		}
		break;
	case 2:
		verde++;
		if (verde == 10) // Caso o número seja maior que 9, atribui que o seu valor é o inicial
		{
			verde = 1;
		}
		break;
	case 3:
		amarelo++;
		if (amarelo == 10)
		{
			amarelo = 1;
		}
		break;
	case 4:
		vermelho++;
		if (vermelho == 10)
		{
			vermelho = 1;
		}
		break;
	}
	display();

}

ISR(INT1_vect)
{

	switch(cont)
	{
	case 0:
		if (aux == 1)
		{
			aux--;
		}
		else
		{
			aux++;
		}
		break;
	case 1:
		if (tempo == 1)
		{
			tempo--;
		}
		else
		{
			tempo++;
		}
		break;
	case 2:
		verde--;
		if (verde == 0) // Caso o número seja menor que 0, atribui que o seu valor é o inicial
		{
			verde = 1;
		}
		break;
	case 3:
		amarelo--;
		if (amarelo == 0)
		{
			amarelo = 1;
		}
		break;
	case 4:
		vermelho--;
		if (vermelho == 0)
		{
			vermelho = 1;
		}
		break;
	}
	display();
}

ISR(PCINT2_vect) //Botão de seleção do sinal
{

	if (!(PIND & (1 << 4))) //
	{
		cont++;
		if (cont == 5) //Para determinar que só existem esses casos
		{
			cont = 0;
		}
	}
	display();

	if (!(PIND & (1 << 1))) //
	{
		aux3++;
	}

}
ISR (PCINT1_vect)
{

	if (!(PINC & (1 << 6))) //
	{
		aux2++;
		if (flag4 == 1)
		{

			carmin = aux2 * 12;
			aux2 = 0;
			flag4 = 0;
			display();
		}
	}

}
ISR(TIMER0_COMPA_vect)// Interrupção do TC0 a cada 1ms
{
	tempo_ms++;
	
	if (aux3 > 0)
	{
		if (cont2 < 4)
		{
			cont2 = 4;
		}
		aux3 = 0;
	}
	
	if (aux == 1)
	{

		if ((tempo_ms % 5000) == 0)
		{
			flag4 = 1;

		}
		verde = ((carmin / 60) + 1);
		vermelho = 10 - verde;

	}
	if ((tempo_ms % 500) == 0)
	{
		flag5 = 1;
		if ((PIND & 0b01000000) == 0b00000000)//
		{
			pessoa = 1;
		}
		if ((1023000 / ADC - 1000) < 300) // Caso possua intensidade menor que 300 Lux
		{
			luz = 1;
		}
		else
		{
			OCR0B = 0;
		}
	}
	if (tempo == 1)
	{

		if (cont2 < 4) // De 0 a 3 para representar o sinal verde
		{
			if ((tempo_ms % (verde * 330)) == 0)
			{
				cont2++;
				flag1 = 1;
			}
		}
		else if (cont2 == 4)  // Quando o contador for igual a 4, representa o sinal amarelo
		{
			if ((tempo_ms % ((amarelo) * 1000)) == 0)
			{
				cont2++;
				flag2 = 1;
			}
		}
		else if (cont2 > 4)  // Quando o contador for maior que 4, representa o sinal vermelho
		{
			if ((tempo_ms % (vermelho * 165)) == 0)
			{
				cont2++;
				flag3 = 1;
			}
		}
	}
	else
	{


		if (cont2 < 4) // De 0 a 3 para representar o sinal verde
		{
			if ((tempo_ms % (verde * 250)) == 0)
			{
				cont2++;
				flag1 = 1;
			}
		}
		else if (cont2 == 4)  // Quando o contador for igual a 4, representa o sinal amarelo
		{
			if ((tempo_ms % ((amarelo) * 1000)) == 0)
			{
				cont2++;
				flag2 = 1;
			}
		}
		else if (cont2 > 4)  // Quando o contador for maior que 4, representa o sinal vermelho
		{
			if ((tempo_ms % (vermelho * 250)) == 0)
			{
				cont2++;
				flag3 = 1;
			}
		}

	}

}
void display ()
{
	unsigned char tempo1[3], tempo2[3], tempo3[3], tempo4[100];

	nokia_lcd_clear(); //Limpa o LCD

	if (aux == 0)
	{
		nokia_lcd_write_string("Modo M.", 1); //Escreve um texto do tamanho 1

	}
	else
	{
		nokia_lcd_write_string("Modo A.", 1); //Escreve um texto do tamanho 1

	}

	if (tempo == 0)
	{
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("Dia", 1); //Escreve um texto do tamanho 1

	}
	else
	{
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("Noite", 1); //Escreve um texto do tamanho 1

	}
	nokia_lcd_set_cursor(0, 20); //Muda o cursor para a posição desejada
	nokia_lcd_write_string("Verde", 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("Amare", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("Verme", 1);

	sprintf(tempo1, "%u", verde);//Converte o valor em uma string
	nokia_lcd_set_cursor(35, 20);
	nokia_lcd_write_string(tempo1, 1);//Mostra no display o valor do tempo do sinal

	sprintf(tempo2, "%u", amarelo);
	nokia_lcd_set_cursor(35, 30);
	nokia_lcd_write_string(tempo2, 1);

	sprintf(tempo3, "%u", vermelho);
	nokia_lcd_set_cursor(35, 40);
	nokia_lcd_write_string(tempo3, 1);


	sprintf(tempo4, "%u", carmin);
	nokia_lcd_set_cursor(53, 0);
	nokia_lcd_write_string(tempo4, 2);
	nokia_lcd_set_cursor(53, 15);
	nokia_lcd_write_string("C/Min", 1);

	switch(cont)  //Usado para indicar em qual sinal o usuário está incrementando ou decrementando
	{
	case 0:
		nokia_lcd_set_cursor(45, 0);
		nokia_lcd_write_string("<", 1);
		break;
	case 1:
		nokia_lcd_set_cursor(45, 10);
		nokia_lcd_write_string("<", 1);
		break;
	case 2:
		nokia_lcd_set_cursor(45, 20);
		nokia_lcd_write_string("<", 1);
		break;
	case 3:
		nokia_lcd_set_cursor(45, 30);
		nokia_lcd_write_string("<", 1);
		break;
	case 4:
		nokia_lcd_set_cursor(45, 40);
		nokia_lcd_write_string("<", 1);
		break;
	}
	nokia_lcd_render();

}
void luminaria()
{
	if (luz) // Caso possua intensidade menor que 300 Lux
	{
		if (pessoa == 1 || aux2 > 0) // Caso detecte pedestres ou veículos
		{
			OCR0B = 255;
			pessoa = 0;
			aux2 = 0;
		}
		else
		{
			OCR0B = 77;
		}

		PORTD = 0b01111110;// Acende a luminária
		if(cont2 == 4) //Acende o amarelo
		{
			PORTD = 0b11111110;
		}
		else if (cont2 == 5)
		{
			if (cont2 > 4)
			{
				PORTD = 0b01111111;
			}
			PORTB = 0b11110000;//Acende os vermelhos
		}
		luz = 0;
	}

}
void leitura_sensores (uint8_t *flagdisp)
{
	unsigned char lux[100];

	if (*flagdisp)
	{
		sprintf(lux, "%u", 1023000 / ADC - 1000);

		nokia_lcd_set_cursor(53, 25);
		nokia_lcd_write_string(lux, 2);
		nokia_lcd_set_cursor(55, 40);
		nokia_lcd_write_string("Lux", 1);
		nokia_lcd_render();
		*flagdisp = 0;

	}

}
void sinal ()
{
	if (verde != 0 || vermelho != 0 || amarelo != 0)
	{

		if(flag1)  //Caso seja verdadeira
		{
			PORTD = 0b01011110;
			PORTB = PORTB >> 0b01; //Decrementa o sinal verde
			flag1 = 0;
			if(cont2 == 4) //Acende o amarelo
			{
				PORTD = 0b11011110;
				flag2 = 0;//Zera a flag
			}
		}
		else if (flag2 == 1 && cont2 == 5)
		{
			PORTD = 0b01011111;//Apaga o amarelo
			PORTB = 0b11110000;//Acende os vermelhos
			flag2 = 0;
		}
		else if(flag3)
		{
			PORTB = PORTB << 0b01;//Decrementa o vermelho
			flag3 = 0;
			if (cont2 == 9)
			{
				PORTB = 0b00001111;//Acende o vermelho
				cont2 = 0;
			}
		}
		if (cont2 > 4 && luz != 1)
		{
			PORTD = 0b01011111;
		}
	}
	luminaria();
}

int main(void)
{
	DDRB = 0b11111111; // Habilitando os pinos PB0...7 como saídas;
	DDRD = 0b11100001; // Habilitando o pino PD7 como saída e os pinos PD0 e PD1 como entrada;
	DDRC = 0b10111110;
	PORTD = 0b00011110;// Habilita o resistor de pull-up do pino PD0 e PD1
	PORTC = 0b01000000;

	//Configuração das interrupções
	EICRA = 0b00001010; //Interrupção externa INT0 e INT1 na borda de descida
	EIMSK = 0b00000011; // Habilita a interrupção externa
	PCICR = 0b00000110;// Interrupção externa da porta D
	PCMSK2 = 0b00010010;
	PCMSK1 = 0b01000000;

	//Configuração do Timer 0
	TCCR0A = 0b10100011;
	//TCCR0A = 0b00000010;
	TCCR0B = 0b00000011;
	OCR0B = 77;
	OCR0A = 249;
	TIMSK0 = 0b00000010;

	//Configuração do ADC
	ADMUX = 0b01000000;
	ADCSRA = 0b11100111;
	ADCSRB = 0b00000000;
	DIDR0 = 0b00000000;

	sei();
	PORTB = 0b00001111;
	nokia_lcd_init(); //Inicia o LCD
	display();
	/* Replace with your application code */
	while (1)
	{
		sinal();// Chamando a função responsável pela saída do sinal;
		leitura_sensores(&flag5);

	}
}
