// void AcionamentoMotorX();
// void AcionamentoMotorY();
void AcionamentoMotorZ();

#include "mbed.h"
#include "AcionamentoMotor.h" // Contém AcionamentoMotorZ()

DigitalIn FdC_Z_Min(PA_12);
DigitalIn FdC_Z_Max(PA_11);
DigitalIn FdC_X_Max(PB_2);
DigitalIn FdC_X_Min(PB_1);
DigitalIn FdC_Y_Max(PB_14);
DigitalIn FdC_Y_Min(PB_15);

extern BusOut MotorZ;
extern int direcao_Z; // 0 = desce, 1 = sobe

int estado_Z = 0; // 0 = operação normal, 1 = ajuste mínimo, 2 = ajuste máximo

void ReferenciarZ_RotinaSegura() {
    switch (estado_Z) {
        case 0: // Operação normal
            // Sensor mínimo atingido enquanto descendo
            if (FdC_Z_Min == 0 && direcao_Z == 0) {
                MotorZ = 0;
                wait(1);
                direcao_Z = 1;
                estado_Z = 1;
            }
            // Sensor máximo atingido enquanto subindo
            else if (FdC_Z_Max == 0 && direcao_Z == 1) {
                MotorZ = 0;
                wait(1);
                direcao_Z = 0;
                estado_Z = 2;
            } else {
                AcionamentoMotorZ(); // Move normalmente
            }
            break;

        case 1: // Ajuste fino para cima (liberar sensor mínimo)
            if (FdC_Z_Min == 1) { // Já liberou
                MotorZ = 0;
                estado_Z = 0;
            } else {
                AcionamentoMotorZ(1); // Sobe lentamente
                wait(0.03);
            }
            break;

        case 2: // Ajuste fino para baixo (liberar sensor máximo)
            if (FdC_Z_Max == 1) { // Já liberou
                MotorZ = 0;
                estado_Z = 0;
            } else {
                AcionamentoMotorZ(2); // Desce lentamente
                wait(0.03);
            }
            break;
    }
}