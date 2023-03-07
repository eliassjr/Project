/******************************************************************************* 
* File:   main_display_buffer.c      
* Developed by Nivaldo T. Schiefler Jr.  DSc.
* Build under XC8 compiler
* Created on 21 de Outubro de 2021, 19:36
* Updates: 
*******************************************************************************/

#include <avr/io.h>
#include "config.h"
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include "functions.h"
#include <stdio.h>
#include "PWM.h"
#include "ADC.h"
#include "LCD2L.h"

//******************************************************************************
char pino;
char escala;
char percentual;
int x;

char texto1[] = "Ventoinha ";
char texto2[] = "     ";
char texto3[] = "VL = ";
char texto4[] = "TA=";
char texto5[] = "TE=";

char valor_lido_txt[6], tensao_txt[10], tensaoescolhida_txt[10];
float tensao;
int status;
float valor=5;
unsigned int valor_lido, botao;
unsigned char cont=0;


//******************************************************************************
int main(void)
{
    configuracao();
    pino = 2;
    set_pin(PORTB,2);
    escala = 5;
    define_prescale(pino,escala);
    Init_ADC(1);
    x=0;
   
    while(1) 
    {
        botao = Read_ADC(1);
        if(x < 100 && cont == 0) //Comando para ligar a ventoinha 
        {
        x++;
        set_duty_PWM(x,pino); 
        _delay_ms(10);
        }
        
        if (x == 100 && cont == 0) //Comando para ligar LCD
        {
           init_lcd2L();
           clear_lcd2L();
           write_text_lcd2L(texto1, 0, 0);
           write_text_lcd2L(texto4, 1,9);
           write_text_lcd2L(texto5, 1,0);
           cont = 1;
        }
        
        if (cont == 1)
            
        {
          valor_lido = Read_ADC(0);
          //itoa(valor_lido, valor_lido_txt, 10);

          //ESCREVE A LEITURA EM VL
          sprintf(valor_lido_txt, "%d", valor_lido);
          write_text_lcd2L(texto2,0,12); 
          write_text_lcd2L(valor_lido_txt, 0, 12); 
        
          // ESCREVE A TENSÃO ATUAL
          tensao = (float)(valor_lido*5)/1023.0;
          dtostrf(tensao, 4,2,tensao_txt);
          write_text_lcd2L(texto2,1,12);
          write_text_lcd2L(tensao_txt, 1,12);
          
          // ESCREVE A TENSÃO ESCOLHIDA
          write_text_lcd2L(texto2,1,3);
          dtostrf(valor, 4,2,tensaoescolhida_txt);
          write_text_lcd2L(tensaoescolhida_txt,1,3);
          
          // REDUZ -0.1 A TENSÃO ESCOLHIDA INICIAL
          if((0.05 <= botao)  && (botao < 0.150) && (valor > 0))
           {
                valor = valor + 0.1;
           }
          
          if((0.150 <= botao)  && (botao < 0.300) && (valor > 0))
           {
                valor = valor + 0.1;
           }
          
          
          _delay_ms(100);
        }
    }
}
//*******************************************

