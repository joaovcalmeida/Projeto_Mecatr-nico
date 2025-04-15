#include "mbed.h"
#include "AcionamentoMotor.h"

DigitalIn FdC_X_Max(PB_2);
DigitalIn FdC_X_Min(PB_1);
DigitalIn FdC_Y_Max(PB_14);
DigitalIn FdC_Y_Min(PB_15);
DigitalIn FdC_Z_Max(PA_11);
DigitalIn FdC_Z_Min(PA_12);

void AcionamentoMotorX();
void AcionamentoMotorY();
void AcionamentoMotorZ();

