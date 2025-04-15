#include "mbed.h"
void SalvaPosicaoCOLETA() {
    //armazenando as posicoes atuais dos eixos X,Y e Z
    posicao_coletaX = posicao_X;
    posicao_coletaY = posicao_Y;
    posicao_coletaZ = posicao_Z;

    //indica que a posicao atual foi salva
    posicao_de_coleta_salva = true;

    //limpa o display LCD e mensagem dizendo que foi salva a posicao atual
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("coleta salva:");

    //msg do eixo X
    lcd.locate(0,1);
    lcd.print("X = %d",posicao_coletaX);
    //msg do eixo Y
    lcd.locate(0,2);
    lcd.print("Y = %d",posicao_coletaY);
    //msg do eixo Z
    lcd.locate(0,3);
    lcd.print("Z = %d",posicao_coletaZ);

    wait_ms(5000);
    lcd.cls();
}

void salvaposicaoLIBERAÇÃO