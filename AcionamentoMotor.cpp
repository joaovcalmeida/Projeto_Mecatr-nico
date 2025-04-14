#include "mbed.h"
#define velo 0.01 // tempo entre os passos

//BusOut MP_X(D3, D4, D5, D6); // Motor de Passo do Eixo X
//BusOut MP_Y(D7, D8, D9, D10); // Motor de Passo do Eixo Y
//BusOut MP_Z(D11, D12, D13, D14); // Motor de Passo do Eixo Z

BusOut MotorX(D2, D3, D4, D5); //4 pinos conectados ao driver do Motor Eixo X
BusOut MotorY(D6, D7, D8, D9); //4 pinos conectados ao driver do Motor Eixo Y


DigitalIn BotaoXP(A0);       //Botão X Positivo (Direita)
DigitalIn BotaoXN(PC_13);       //Botão X Negativo (Esquerda)
DigitalIn BotaoYP(A1);       //Botão Y Positivo (Sobe)
DigitalIn BotaoYN(A2);       //Botão Y Negativo (Desce)



void AcionamentoMotorX() {
    int AcionamentoFase[4] = {0x01, 0x02, 0x04, 0x08}; //Sequência de acionamento para motor unipolar (4 fases)
    int estado = 0; //Estado definido como 0 inicialmente

    // Lê os dois botões e transforma-o em um número, identificando o estado
    if (BotaoXP == 0) estado = 1;  //Pressionado (ativo baixo)
    if (BotaoXN == 0) estado = 2;

    switch (estado) {
        case 1: //BotaoXP pressionado -> incremento positivo
            for (int i = 0; i < 4; i++) {
                MotorX = AcionamentoFase[i];
                wait(velo);
            }
            break;

        case 2: //BotaoXN pressionado -> incremento negativo
            for (int i = 3; i >= 0; i--) {
                MotorX = AcionamentoFase[i];
                wait(velo);
            }
            break;

        default: //Nenhum botão pressionado
            MotorX = 0;
            break;
    }
}




void AcionamentoMotorY() {
    int AcionamentoFase[4] = {0x01, 0x02, 0x04, 0x08}; //Sequência de acionamento para motor unipolar (4 fases)
    int estado = 0; //Estado definido como 0 inicialmente

    // Lê os dois botões e transforma-o em um número, identificando o estado
    if (BotaoYP == 0) estado = 1;  //Pressionado (ativo baixo)
    if (BotaoYN == 0) estado = 2;

    switch (estado) {
        case 1: //BotaoYP pressionado -> incremento positivo
            for (int i = 0; i < 4; i++) {
                MotorY = AcionamentoFase[i];
                wait(velo);
            }
            break;

        case 2: //BotaoYN pressionado -> incremento negativo
            for (int i = 3; i >= 0; i--) {
                MotorY = AcionamentoFase[i];
                wait(velo);
            }
            break;

        default: //Nenhum botão pressionado
            MotorX = 0;
            break;
    }
}

int main(){
    while(1){
    AcionamentoMotorX();
    AcionamentoMotorY();
}
}