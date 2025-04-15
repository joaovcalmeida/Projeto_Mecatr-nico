#include "mbed.h"

const int tamanho_array = 100;
int num_posicoes_salvas = 0;

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

void salvaposicaoLIBERAÇÃO(){
    //verifica se há espaços no array para salvar posições
    if (num_posicoes_salvas< tamanho_array){
        //armazena as posicoes atuais nos vetores correspondentes
        posicoes_X[num_posicoes_salvas] = posicao_X;
        posicoes_Y[num_posicoes_salvas] = posicao_Y;
        posicoes_Z[num_posicoes_salvas] = posicao_Z;

        lcd.cls();
        lcd.locate(0, 0);
        //confirma o salvamento da posicao
        lcd.printf("Posicao %d salva!", num_posicoes_salvas + 1);
        //mostra as coordenadas salvas na tela do lcd
        lcd.locate(0, 1);
        lcd.printf("X:%d", posicao_X);
        lcd.locate(0, 2);
        lcd.printf("Y:%d", posicao_Y);
        lcd.locate(0, 3);
        lcd.printf("Z:%d", posicao_Z);

        num_posicoes_salvas++;

         if (num_posicoes_salvas < tamanho_array) {
            // instrui o usuario a ir para a proxima posicao de liberação
            lcd.cls();
            lcd.locate(0, 2);
            lcd.printf("Va para posicao %d de liberacão", num_posicoes_salvas + 1);
        } else {
            //Se todas as posicoes ja foram salvs, instrui o usuario a iniciar a operacao da pipetagem
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("Iniciar pipetagem?");
            lcd.locate(0, 1);
            lcd.printf("Envie o comando");
        }
    //se o num de posicoes ja estiver no limite, entra nesse else
    } else {
        //informa por mensagem que ele foi atingido
        lcd.cls();
        lcd.printf("Limite de %d", tamanho_array);
        lcd.locate(0, 1);
        lcd.printf("posicoes atingido.");
        wait_ms(5000);
        lcd.cls();
    }
}