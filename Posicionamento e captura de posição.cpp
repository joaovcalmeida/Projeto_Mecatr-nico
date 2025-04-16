#include "mbed.h"
#include "AcionamentoMotor.h" // Contém AcionamentoMotorZ()
#include "TextLCD.h"
#include "cmsis.h"

const int tamanho_array = 100;
int num_posicoes_salvas = 0;
//oi
//Início da função que salva a posição atual como posição de coleta
void SalvarPosicaoCOLETA() {
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
//inicio da funcao para salvar posicoes de liberacao
void salvarposicaoLIBERAÇÃO(){
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
// Início da função para zerar o eixo Z (por ex ao voltar para a posição segura)
void MovendoEixoZparaZero(){
    //enquanto posicao_Z for maior que zero, continua movendo o eixo z para baixo
    while (posicao_Z>0) moverEixoZ(0);
//desliga o motor do eixo Z apos alcancar o zero
    MotorZ =1;
    wait_ms(200);
}

//funcao que move os tres eixos para as coordenadas alvo fornecidas
void MoverParaPosicaoFornecida(int alvo_X, int alvo_Y, int alvo_Z){

    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Movendo para:");
    lcd.locate(0, 1);
    lcd.printf("X:%d Y:%d Z:%d", alvo_X, alvo_Y, alvo_Z);

    // Movimento do eixo X
    lcd.locate(0, 2);
    lcd.printf("Movendo eixo X...");

    while (abs(posicao_X - alvo_X) > 0) {
        if (posicao_X < alvo_X) {
            moverEixoX(1); //anda para frente
        } else {
            moverEixoX(0);//anda para tras
        }
    }
    MotorX = 1;
    wait_ms(200);

    // Movimento do eixo Y
    lcd.locate(0, 2);
    lcd.printf("Movendo eixo Y...");
    while (abs(posicao_Y - alvo_Y) > 0) {
        if (posicao_Y < alvo_Y) {
            moverEixoY(0);//anda para frente
        } else {
            moverEixoY(1);//anda para tras
        }
    }
    MotorY = 1;
    wait_ms(200);

    // Movimento do eixo Z
    lcd.locate(0, 2);
    lcd.printf("Movendo eixo Z...");
    while (posicao_Z != alvo_Z) {
        if (posicao_Z < alvo_Z) {
            moverEixoZ(1);//anda para frente
        } else {
            moverEixoZ(0);//anda para tras
        }
    }
    MotorZ = 1;
    wait_ms(200);

    lcd.locate(0, 2);
    lcd.printf("Posicionamento finalizado!");
    wait_ms(1000);
}
