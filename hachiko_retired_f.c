#include "wanda_reference.h"

void trata_sensores() //Função que cuida de todo o tratamento dos sensores.
{
/*
   if ( bit_test(resposta_sensores,0) || bit_test(resposta_sensores,1) ) //Verifica isoladamente os sensores de linha
   {
      motor_1(70,'b');
      motor_2(70,'b');
      
      motor_1(0,'b');
      motor_2(0,'b');
      
      motor_1(70,'b');
      motor_2(70,'f');
      
      bit_clear(resposta_sensores,0);
      bit_clear(resposta_sensores,1);
      printf("cheguei aqui");
   }
*/
   switch(resposta_sensores)
   {
     
     /*
     Diagramação dos bits da palavra resposta_sensores
     
     bit 0 - Sensor Linha Direita
     bit 1 - Sensor Linha Esquerda
     bit 2 - Sensor Lateral Direita
     bit 3 - Sensor Frente Esquerda
     bit 4 - Sensor Frente Direita
     bit 5 - Sensor Lateral Esquerda
     
     */
    
     
      case 0b00000000:
      
      if(!primeira_busca)
      {
      //Caso ele já tenha achado pelo menos uma vez o adversário, ele gira em torno do próprio eixo
         motor_1(0,'b'); 
         motor_2(0,'f');
         mover = 0;
         
      }
      else{
      
      enable_interrupts(INT_TIMER1);
      motor_1(0,'f');
      motor_2(0,'f');
      
      }
      
      
      break;
      
      case 0b00000100: 
      motor_1(50,'f'); 
      motor_2(50,'b');
  
      primeira_busca = 0;
      disable_interrupts(INT_TIMER1);
      break;
      
      case 0b00001000: 
      motor_1(50,'f'); 
      motor_2(50,'f'); 
      primeira_busca = 0;
      disable_interrupts(INT_TIMER1);
      break;
      
      case 0b00010000: 
      motor_1(50,'f'); 
      motor_2(50,'f'); 
      primeira_busca = 0;
      disable_interrupts(INT_TIMER1);
      break;
      
      case 0b00100000: 
      motor_1(50,'b'); 
      motor_2(50,'f'); 
      primeira_busca = 0;
      disable_interrupts(INT_TIMER1);
      break;
      
      case 0b00011000: 
      motor_1(50,'f');
      motor_2(50,'f'); 
      primeira_busca = 0;
      disable_interrupts(INT_TIMER1);
      break;
      
      case 0b00001100: 
      motor_1(50,'f'); 
      motor_2(50,'b');

      primeira_busca = 0;
      disable_interrupts(INT_TIMER1);
      break;
      
      case 0b00110000: 
      motor_1(50,'b'); 
      motor_2(50,'f');
      primeira_busca = 0;
      disable_interrupts(INT_TIMER1);
      break;
      
      default:  motor_1(0,'b'); motor_2(0,'f'); break;
   }
   
   delay_us(10); //Delay para uma nova leitura
   resposta_sensores = 0b00000000; //Zera a variável de resposta para garantir que um resultado passado não fique gravado
}
void leitura_hc04() //Funcao para fazer a leitura do sensor ultrassonico
{
   enable_interrupts(INT_TIMER5); //Habilita interrupções do TIMER5
   
 
   if(cont_timer5 < 8)
   {
      output_low(TRIGGER);
   }
   else if(cont_timer5 >= 8 && cont_timer5 < 10)
   {
      output_high(TRIGGER);
   }
   else if(cont_timer5 == 10)
   {
      cont_timer5 = 0;
   }
   
   if( (input(ECHO)) && (!borda_subida) )
   {
      bs_t = get_timer5();
      borda_subida = 1;
      enable_overflow_count = 1;
   }
   
   else if( (!input(ECHO)) && (!borda_descida) && (borda_subida) )
   {
      bd_t = get_timer5();
      borda_descida = 1;
   }
   
   if( (borda_descida)  )
   {
      if(bd_t>bs_t)
      {
         if(contador_overflows_timer5==0){ trigger_t = bd_t - bs_t; }
         if(contador_overflows_timer5>0) { trigger_t = (bd_t - bs_t) + (65535*(contador_overflows_timer5));  }
      }
      else
      {
      trigger_t = (65535*contador_overflows_timer5) + (bd_t - bs_t);
     // printf(" %Lu \r",trigger_t);
      }
      if(trigger_t <= 30000)  output_high(LED_0); // bit_set(resposta_sensores,3); bit_set(resposta_sensores,4); 
      else output_low(LED_0);
      
      //Reset das variaveis para uma proxima leitura
      borda_descida = 0;
      borda_subida = 0;
      enable_overflow_count = 0;
      contador_overflows_timer5 = 0;
      
   }
   
}
