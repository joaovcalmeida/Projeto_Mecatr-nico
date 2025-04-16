#include "mbed.h"
#include "AcionamentoMotor.h" // Contém AcionamentoMotorZ()
#include "TextLCD.h"
#include "cmsis.h"
// Exemplo de pinos: rs, e, d4, d5, d6, d7
TextLCD lcd(D14, D15, D4, D5, D6, D7);


DigitalOut led_emergencia_vermelho(PA_15);
DigitalOut buzzer_emergencia(PC_11);
InterruptIn BotaoEmergencia(PC_10);

extern BusOut MotorX;
extern BusOut MotorY;
extern BusOut MotorZ;

bool emergencia_ativada = false;
// --- Função para alterar estado de emergência ---
void setEmergencia(bool estado) {
    emergencia_ativada = estado;
}

void SinalizaEmergencia(){
    if (BotaoEmergencia ==0){
        setEmergencia(true); 

        led_emergencia_vermelho = 1;
        buzzer_emergencia = 1;
        lcd.cls();  // limpa a tela do nosso LCD
        lcd.locate(0, 0);  // faz com que o cursor vá para o canto superior esquerdo da tela do LCD
        lcd.printf("EMERGENCIA ACIONADA");


        // De imediato, interrompe os motores X,Y e Z
        MotorX = 1;
        MotorY = 1;
        MotorZ = 1;

        // Esperando o botao ser solto para poder resetar o sistema
        while (BotaoEmergencia==0) wait_ms(1000);

        // Se o botao nao for solto, surge mensagem de reset
        lcd.cls()
        lcd.locate(0,0);
        lcd.printf("Solte o botao para resetar");
        while (BotaoEmergencia == 1) wait_ms(1000);
        while (BotaoEmergencia == 0) wait_ms (1000);

        //Começa a resetar
        led_emergencia_vermelho = 0;
        buzzer_emergencia = 0;
        lcd.cls()
        lcd.locate(0,0);
        lcd.printf("Resetando");
        wait_ms(1000);
        NVIC_SystemReset();

    }
}
