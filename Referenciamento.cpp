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

void ReferenciarZ() {
    switch (estado_Z) {
        case 0: // Operação normal
            if (FdC_Z_Min == 0) {
                MotorZ = 0;
                wait(1);
                estado_Z = 1; // Entrar em ajuste para cima
            }
            else if (FdC_Z_Max == 0 ) {
                MotorZ = 0;
                wait(1);
                estado_Z = 2; // Entrar em ajuste para baixo
            } else {
                AcionamentoMotorZ(); // Continua operação normal
            }
            break;

        case 1: // Ajuste para cima (liberar sensor mínimo)
            AcionamentoMotorZ(1); // sobe devagar
            wait(0.03);
            if (FdC_Z_Min == 1) {
                estado_Z = 0; // Volta para modo normal
            }
            break;

        case 2: //Ajust e fino para baixo (liberar sensor máximo)
            AcionamentoMotorZ(2); // desce devagar
            wait(0.03);
            if (FdC_Z_Max == 1) {
                estado_Z = 0; // Volta para modo normal
            }
            break;
    }
}
