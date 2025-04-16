#include "mbed.h"

#define velo 0.01 // tempo entre os passos

BusOut MotorX(D10, D11, D12, D13); // Motor Eixo X
BusOut MotorY(D6, D7, D8, D9);     // Motor Eixo Y
BusOut MotorZ(D2, D3, D4, D5);     // Motor Eixo Z

// Botões de controle manual
DigitalIn BotaoXP(A3); //Botão X Positivo (Direita)
DigitalIn BotaoXN(A4); //Botão X Negativo (Esquerda)
DigitalIn BotaoYP(A1); //Botão Y Positivo (Frente)
DigitalIn BotaoYN(A2); //Botão Y Negativo (Atrás)
DigitalIn BotaoZP(A0); //Botão Z Positivo (Cima)
DigitalIn BotaoZN(PC_13); //Botão Z Negativo (Desce)

// Sensores de fim de curso
DigitalIn FdC_Z_Min(D10);
DigitalIn FdC_Z_Max(D11);
DigitalIn FdC_X_Max(PB_2);
DigitalIn FdC_X_Min(PB_1);
DigitalIn FdC_Y_Max(PB_14);
DigitalIn FdC_Y_Min(PB_15);

// ==================== REFERENCIAMENTO Z ====================
int estado_Z = 0;
bool referenciado_Z = false;

void AcionamentoMotorX(int estado);
void AcionamentoMotorY(int estado);
void AcionamentoMotorZ(int estado);

void ReferenciarZ() {
    if (referenciado_Z) return; // já foi referenciado

    switch (estado_Z) {
        case 0: // Descer até o sensor mínimo
            if (FdC_Z_Min == 1) {
                AcionamentoMotorZ(1); // Sobe
            } else {
                MotorZ = 0;
                estado_Z = 1;
            }
            break;

        case 1: // Subir lentamente até liberar sensor mínimo
            AcionamentoMotorZ(2); // desce devagar
            if (FdC_Z_Min == 1) {
                MotorZ = 0;
                referenciado_Z = true;
                printf("Eixo Z completamente referenciado!\n");
            }
            break;
    }
}

// ==================== REFERENCIAMENTO X ====================
int estado_X = 0;
bool referenciado_X = false;
void ReferenciarX() {
    if (referenciado_X) return; // já foi referenciado

    switch (estado_X) {
        case 0: // Descer até o sensor mínimo
            if (FdC_X_Min == 1) {
                AcionamentoMotorX(1); // Direita
            } else {
                MotorX = 0;
                estado_X = 1;
            }
            break;

        case 1: // Esquerda lentamente até liberar sensor mínimo
            AcionamentoMotorX(2); // Vai para esquerda devagar
            if (FdC_X_Min == 1) {
                MotorX = 0;
                referenciado_X = true;
                printf("Eixo X completamente referenciado!\n");
            }
            break;
    }
}


// ==================== REFERENCIAMENTO Y ====================
int estado_Y = 0;
bool referenciado_Y = false;
void ReferenciarY() {
    if (referenciado_Y) return; // já foi referenciado

    switch (estado_Y) {
        case 0: // Frente até o sensor mínimo
            if (FdC_Y_Min == 1) {
                AcionamentoMotorY(1); // Frente
            } else {
                MotorY = 0;
                estado_Y = 1;
            }
            break;

        case 1: // Para atrás lentamente até liberar sensor mínimo
            AcionamentoMotorX(2); // Vai para atrás devagar
            if (FdC_Y_Min == 1) {
                MotorY = 0;
                referenciado_Y = true;
                printf("Eixo Y completamente referenciado!\n");
            }
            break;
    }
}


// ==================== ACIONAMENTO DOS MOTORES ====================
void AcionamentoMotorX(int estado) {
    int AcionamentoFase[4] = {0x01, 0x02, 0x04, 0x08};
    switch (estado) {
        case 1:
            for (int i = 0; i < 4; i++) {
                MotorX = AcionamentoFase[i];
                wait(velo);
            }
            break;
        case 2:
            for (int i = 3; i >= 0; i--) {
                MotorX = AcionamentoFase[i];
                wait(velo);
            }
            break;
        default:
            MotorX = 0;
            break;
    }
}

void AcionamentoMotorY(int estado) {
    int AcionamentoFase[4] = {0x01, 0x02, 0x04, 0x08};
    switch (estado) {
        case 1:
            for (int i = 0; i < 4; i++) {
                MotorY = AcionamentoFase[i];
                wait(velo);
            }
            break;
        case 2:
            for (int i = 3; i >= 0; i--) {
                MotorY = AcionamentoFase[i];
                wait(velo);
            }
            break;
        default:
            MotorY = 0;
            break;
    }
}

void AcionamentoMotorZ(int estado) {
    int AcionamentoFase[4] = {0x01, 0x02, 0x04, 0x08};
    switch (estado) {
        case 1:
            for (int i = 0; i < 4; i++) {
                MotorZ = AcionamentoFase[i];
                wait(velo);
            }
            break;
        case 2:
            for (int i = 3; i >= 0; i--) {
                MotorZ = AcionamentoFase[i];
                wait(velo);
            }
            break;
        default:
            MotorZ = 0;
            break;
    }
}
// ============================================================


// ==================== FUNÇÃO PRINCIPAL ====================
int main() {
    while (!referenciado_Z) {
        ReferenciarZ(); // executa o referenciamento do eixo Z
    }
        while (!referenciado_X) {
        ReferenciarX(); // executa o referenciamento do eixo X
    }

    while (!referenciado_Y) {
        ReferenciarY(); // executa o referenciamento do eixo Y
    }


    while (true) {
        // controle manual dos motores, agora liberado
        if (BotaoXP == 0) AcionamentoMotorX(1);
        else if (BotaoXN == 0) AcionamentoMotorX(2);
        else AcionamentoMotorX(0);

        if (BotaoYP == 0) AcionamentoMotorY(1);
        else if (BotaoYN == 0) AcionamentoMotorY(2);
        else AcionamentoMotorY(0);

        if (BotaoZP == 0) AcionamentoMotorZ(1);
        else if (BotaoZN == 0) AcionamentoMotorZ(2);
        else AcionamentoMotorZ(0);
    }
}

