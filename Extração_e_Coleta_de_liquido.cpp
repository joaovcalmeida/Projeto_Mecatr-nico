#include "mbed.h"
#include "AcionamentoMotor.h" // Contém AcionamentoMotorZ()
#include "TextLCD.h"
#include "cmsis.h"

extern BusOut MotorX;
extern BusOut MotorY;
extern BusOut MotorZ;


// Exemplo de pinos: rs, e, d4, d5, d6, d7
TextLCD lcd(D14, D15, D4, D5, D6, D7);


// Flags de controle
bool posicao_coleta_salva = false;
int num_posicoes_salvas = 0;

// Posição de coleta salva pelo usuário
int posicao_coletaX = 0;
int posicao_coletaY = 0;
int posicao_coletaZ = 0;

// Posições de liberação definidas pelo usuário (máximo de 10)
const int MAX_POSICOES = 10;
int posicoes_X[MAX_POSICOES];
int posicoes_Y[MAX_POSICOES];
int posicoes_Z[MAX_POSICOES];

// Posição atual (deve ser atualizada conforme movimentos reais)
int posicao_Z = 0;

// ------------------ FUNÇÕES AUXILIARES NECESSÁRIAS (EXTERNAS) ------------------

// Move o eixo Z fisicamente e atualiza a variável posicao_Z
void moverEixoZ(int sentido); // 0 = descer, 1 = subir

// Move para uma posição 3D definida
void moverParaPosicao(int x, int y, int z);


DigitalOut atuadorPipeta(PA_5);  // Pino conectado ao atuador do êmbolo

void pipetarVolumeTotal(int volume_total_mL) {
    // Validação inicial
    if (!posicao_coleta_salva || num_posicoes_salvas == 0) {
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("ERRO:");
        lcd.locate(0, 1);
        lcd.printf("Falta pos. coleta");
        lcd.locate(0, 2);
        lcd.printf("ou liberação!");
        wait_ms(2500);
        return;
    }

    int volume_restante = volume_total_mL;

    // Para cada 1 mL a ser pipetado
    for (int i = 0; i < volume_total_mL; i++) {
        // Etapa 1: coleta

        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Coletando 1 mL...");
        wait_ms(500);

        moverParaPosicao(posicao_coletaX, posicao_coletaY, posicao_coletaZ);

        // Desce eixo Z até o fundo
        while (posicao_Z > 0)
            moverEixoZ(0);
        MotorZ = 1;
        wait_ms(300);

        // Ativa o atuador para puxar 1 mL
        atuadorPipeta = 1;
        wait_ms(1000);  // tempo de sucção
        atuadorPipeta = 0;

        // Sobe até posição original
        while (posicao_Z < posicao_coletaZ)
            moverEixoZ(1);
        MotorZ = 1;
        wait_ms(300);

        // Etapa 2: liberação

        if (i < num_posicoes_salvas) {
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("Liberando 1 mL...");
            lcd.locate(0, 1);
            lcd.printf("Pos. %d de %d", i + 1, volume_total_mL);

            moverParaPosicao(posicoes_X[i], posicoes_Y[i], posicoes_Z[i]);

            // Desce para liberar
            while (posicao_Z > 0)
                moverEixoZ(0);
            MotorZ = 1;
            wait_ms(300);

            // Solta o líquido (empurra êmbolo)
            atuadorPipeta = 1;
            wait_ms(1000);
            atuadorPipeta = 0;

            // Sobe após liberação
            while (posicao_Z < posicoes_Z[i])
                moverEixoZ(1);
            MotorZ = 1;
            wait_ms(300);
        } else {
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("Erro: falta posicao");
            lcd.locate(0, 1);
            lcd.printf("de liberacao %d", i + 1);
            wait_ms(2000);
            break;
        }

        volume_restante--;
    }

    // Finaliza o ciclo
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Pipetagem finalizada");
    lcd.locate(0, 1);
    lcd.printf("Total: %d mL", volume_total_mL - volume_restante);
    wait_ms(3000);
    lcd.cls();
}
