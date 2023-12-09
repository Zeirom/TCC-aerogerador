#include <AVR_PWM.h>
#define _PWM_LOGLEVEL 4
#define pino 10

AVR_PWM* PWM_Instance;

int Tensao = 0;
float TensaoReal = 0.0;
float TensaoAnterior = 0.0;

int Corrente = 0;
float CorrenteReal = 0.0;

float PotenciaAtual = 0.0;
float PotenciaAnterior = 0.0;
int Frequencia=0;
int RCiclica=0;
int sensordecorrente = A0;   
int sensordetensao = A1;


void setup() {

//setar frequencia do PWM
Frequencia=25000;
RCiclica=50;
PWM_Instance= new AVR_PWM(pino, Frequencia, RCiclica);

}

void loop(){
  Tensao = analogRead(sensordetensao);  // 0 a 1023 => 0 a 5V
  TensaoReal = Tensao*0.0039100*3;    // 0,0039100 = 4v / 1023 (entrada analogica para digital)
                                        // 3 = 12v / 4v (saida do conversor para entrada analogica)

  Corrente = analogRead(sensordecorrente);  // 0 a 1023 => -30A a 30A (digital para analogico)  
                                            // 512 => 0A  (digital para analogico)       
  
  CorrenteReal = (Corrente-512)/17.033333; // Parte da equação da reta Y = A*X+B
                                            // Sendo 0 < y < 1023 (valor do leitor no codigo)
                                            // Sendo -30A < x < 30A
                                            // Monta o gráfico e equaciona o ponto inicial e ponto final
                                            // Corrente real = A*(Digital) + B
                                            // Corrente real = 17.06666666 * Digital + 512
                                            // A = 17.0666666
                                            // B = 512   
                                            // Isola o X da equação
                                            // Chega em X = (Y-B)/A        

PotenciaAtual = TensaoReal*CorrenteReal;
if(PotenciaAtual > PotenciaAnterior){ // Potencia atual ++

  if(TensaoReal > TensaoAnterior){ // Tensão atual ++
   
    PWM_Instance->setPWM(pino,Frequencia, RCiclica + 1); // ++

  }
  
  else{  // Tensão atual --

    PWM_Instance->setPWM(pino,Frequencia, RCiclica - 1); // --
  }
}

if(PotenciaAtual < PotenciaAnterior){// Potencia atual -- 

  if(TensaoReal > TensaoAnterior){ // Tensão atual ++

    PWM_Instance->setPWM(pino, Frequencia, RCiclica-1); // --
  
  }
  else{ // Tensão atual --

    PWM_Instance->setPWM(pino,Frequencia, RCiclica + 1); // ++

  }
}

TensaoAnterior = TensaoReal; //TensaoReal = tensão atual lida
PotenciaAnterior = PotenciaAtual;
delay(1000);
}