#ifndef portas_metal_H_
#define portas_metal_H_
//Definicao de parametros do codigo

//Pinos dos Leds
#define LED_0 PIN_D0
#define LED_1 PIN_D1
#define TRIGGER PIN_E1
#define ECHO PIN_E2

//Pinos dos sensores de linha
#define pin_sensor_linha_1 PIN_A5
#define pin_sensor_linha_2 PIN_A6

//Pinos dos sensores de distância
#define pinSensorDistanciaDireita PIN_A0
#define pinSensorDistanciaFrente_direita PIN_A1
#define pinSensorDistanciaFrente_centro PIN_A2
#define pinSensorDistanciaFrente_esquerda PIN_A3
#define pinSensorDistanciaEsquerda PIN_A4
#define pinSensorDistanciaTras PIN_C0

//Canais dos sensores de linha
#define canal_sensor_linha_1 5
#define canal_sensor_linha_2 6
#define canal_sensor_linha_3 7
#define canal_sensor_linha_4 8

//Canais dos sensores de distância
#define canalSensorDistanciaDireita 0
#define canalSensorDistanciaFrenteDireita 1
#define canalSensorDistanciaFrenteCentro 2
#define canalSensorDistanciaFrenteEsquerda 3
#define canalSensorDistanciaEsquerda 4



//Habilitacao das pontes H
#define ENA_state 1
#define ENB_state 1
//Periodo do PWM
#define POWER_PWM_PERIOD 275 //Define o valor da constante período do PWM
//Modo de Habilita��o por BlueTooth
#define SAFE_BT 0
#define ULTRASSONICO 0

#define TEMPO_ATACANDO 4500 //overflow a cada 13ms
#define TEMPO_LINHA_TRAS_MS 45 //Tempo em que o rob� vai para tr�s ao achar a linha branca
#define TEMPO_GIRO_LINHA 100 //Tempo(em ciclos) que o rob� fica girando depois de ver  linha
#define TEMPO_ESTADO_GIRO 50 //Tempo (em ms) em que o estado de giro permanece (1 e 3)
#define TEMPO_ESTADO_GIRO_TRAS 35 //Tempo em que o estado de giro ao ver algo atrás permanece(estado 4)
#define TEMPO_ESTADO_GIRO_FRENTE 50 //Tempo (em ms) em que o estado de giro da frente permanece


//#define VELOCIDADE_ATAQUE 45
int VELOCIDADE_BUSCA_AGRESSIVA;
#define VELOCIDADE_EVASAO 0
#define VELOCIDADE_LINHA 0
#define VELOCIDADE_GIRO_LINHA 0

//Declaracao de variaveis globais
int velocidade_teste=20; //Velocidade padrao para testes
int16 leitura = 0; //variável que realiza a leitura instantanea
//int16 ultima_leitura = 0;//Variável usada para calcular a diferença entre uma leitura e sua última leitura e apresentar uma nova apenas se necessário
//signed int16 variacao = 0; //Vari�vel que indica o quanto o valor atual varia em rela��o ao valor anterior
int8 respostaSensores = 0b00000000;
int8 resposta_sensores_frente = 0b00000000;

/*Variavel que indica o estado atual dos sensores de lnha sem seus dois primeiros bits
podendo ter então os seguintes valores

00000000 - Nenhum sensor acionado
01000000 - Um sensor acionado
10000000 - Um sensor acionado
11000000 - Dois sensores acionados

Ela e inicializada em zero.
*/

//Inicializa contadores de overflow dos timers 0,1 e 5.
int16 overflowTimer1 = 0;
int16 overflowTimer0 = 0;
int16 overflowTimer5 = 0;


//char input_robo[5];


int primeiro_inicio = 0;
//Variavel que indica se eh primeira vez que o robo entra na sua rotina normal

int estado_teste = 0; //Variavel que controla o estado do teste quando necess�rio

int LIGA = 0;
/*Variavel que indica se o rob� esta ligado ou desligado.
0 - Desligado
1 - Ligado
*/

//int1 sair = 0;

int1 primeiraBusca = 1; //Indica se o robo est� na primeira tentativa de busca
int1 mover = 0; //Indica se o robo deve dar um pulso de movimento
int1 pre_estrategia_executada = 0; //verifica se a pre estrat�gia j� foi executada
long overflowsAcao = 0;

int8 estadoSensores = 0; //Variavel que controla o estado em que o robo se
//encontra em relacao a encontrar o adversario

//Variaveis de controle da borda de subida e descida
int1 borda_subida = 0;
int1 borda_descida = 0;

//Variaveis usadas no sensor ultrassonico
int16 bs_t = 0; //momento em que houve borda de subida
int16 bd_t = 0; //momento em que houve borda de descida
int16 trigger_t = 0; //Tempo em alta no trigger
int1 enable_overflow_count = 0; //Controla a partir de quando conta overflows do timer5
int contador_overflows_timer5 = 0; //Conta quantos overflows houveram no timer 5


int buscaInicial; //variavel que indica qual a busca incial do robo
int buscaPadrao; //variavel que indica qual a busca padrao do robo
int preBusca; //variavel que indica qual a pre busca do robo

char stringEstrategia[14];
char charBuscaInicial; //char recebido para indicar a busca inicial
char charBuscaPadrao; //char recebido para indicar a busca padrao
char charPreBusca; //char recebido para indicar qual a prebusca
char charVelocidadeAtaque[4];
char charVelocidadeTwister[4];
char charDebuggingLinha;
char charGiro[4];
void imprimeEstrategia(int buscaInicial, int buscaPadrao, int preBusca);
void config();

char leitura_bt; //char generico para a leitura do bluetooth


char lado; //variavel que indica qual lado esta o adversario


int i;//Variavel de iteracao com 'for'
int giro;
int1 sair = 0; //variável que controla o desligamento do robô

char acionaRobo; //Char que aciona o robô ou desliga antes mesmo que comece.

//VARIAVEIS ALTERADAS PELA STRING ESTRATEGIAS
int VELOCIDADE_ATAQUE = 0;
int VELOCIDADE_TWISTER;
int debuggingLinha = 0;

//Declaracoes das funcoes do Firmware
void enable_motors();
void config();
int8 leituraAdc(int16 threshold, int canal, int8 resposta_leitura, int bit);
void motor1(int duty_cycle, char sentido);
void motor2(int duty_cycle, char sentido);
void bob();
void disables();
void trataSensores2();
void trata_sensores();
void sensores();
void leitura_hc04();
void evasao();
void timer(long tempo_ms);
void resposta_sensores_frente2();

#endif
