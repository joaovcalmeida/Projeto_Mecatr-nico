#include "mbed.h"
#include "AcionamentoMotor.h" // Contém AcionamentoMotorZ()
#include "Referenciamento.h" // 


BusOut MotorZ(D2, D3, D4, D5); //4 pinos conectados ao driver do Motor Eixo Z
DigitalIn BotaoZP(A0);       //Botão Z Positivo (Sobe) A0
DigitalIn BotaoZN(PC_13);       //Botão Z Positivo (Sobe) PC_13
DigitalIn FdC_Z_Min(D10);
DigitalIn FdC_Z_Max(D11);

void AcionamentoMotorZ(int estado);

int main(){
    
int estado = 0; //Estado definido como 0 inicialmente
    while(1){
            if (BotaoZP == 0) AcionamentoMotorZ(1);  //Pressionado (ativo baixo)
            if (BotaoZN == 0) AcionamentoMotorZ(2);
            ReferenciarZ();
}
}