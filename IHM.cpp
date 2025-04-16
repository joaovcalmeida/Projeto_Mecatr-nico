#include "mbed.h"
#include "TextLCD.h"

// ===== LCD 20x4 via I2C =====
I2C i2c_lcd(I2C_SDA, I2C_SCL);
TextLCD_I2C lcd(&i2c_lcd, 0x7E, TextLCD::LCD20x4); // Endereço do LCD pode variar (0x7E ou 0x4E)

// ===== Botões =====
DigitalIn botaoMenu(PC_0);
DigitalIn botaoCima(PC_1);
DigitalIn botaoBaixo(PC_2);
DigitalIn botaoOK(PC_3);


//Cria uma enumeração (enum) com os possíveis estados da interface.
enum EstadoIHM {
    INICIO,
    MENU_PRINCIPAL,
    MENU_VOLUMES,
    EXECUTANDO_ACAO
};
//Inicializa a variável de controle da IHM com o estado Inicio
EstadoIHM estadoAtual = INICIO;
int opcaoSelecionada = 0;//Qual item esta sendo selecionado no menu principal
int volumeSelecionado = 1;//Volume atual escolhido pelo usuário (1 a 5 mL).
const int TOTAL_OPCOES = 4;//Quantidade de opcoes no menu

//Um array de strings contendo as opções do menu que serão exibidas no LCD.
const char* opcoesMenu[] = {
    "Salvar pos. coleta",
    "Salvar pos. extracao",
    "Selecionar volume",
    "Executar pipetagem"
};

// ===== Funções auxiliares =====
void atualizarLCD() {
    lcd.cls();
    switch (estadoAtual) {
        //Mostra uma mensagem de início, aguardando que o usuário pressione o botão Menu
        case INICIO:
            lcd.locate(0, 0);
            lcd.printf("PIPETA AUTOMATIZADA");
            lcd.locate(0, 2);
            lcd.printf("Press. Menu p/ iniciar");
            break;
        case MENU_PRINCIPAL:
            //Escreve o título "MENU:" na primeira linha.
            lcd.locate(0, 0);
            lcd.printf("MENU:");
            //Percorre as opções do menu. Escreve > ao lado da opção atualmente selecionada.
            for (int i = 0; i < TOTAL_OPCOES; i++) {
                lcd.locate(0, i+1);
                if (i == opcaoSelecionada) lcd.putc('>');
                else lcd.putc(' ');
                lcd.printf(" %s", opcoesMenu[i]);
            }
            break;
        //Mostra a opção de escolher o volume, e o valor atual. Informa que o botão OK confirma
        case MENU_VOLUMES:
            lcd.locate(0, 0);
            lcd.printf("Selecionar Volume:");
            lcd.locate(0, 1);
            lcd.printf(">> %d mL", volumeSelecionado);
            lcd.locate(0, 3);
            lcd.printf("OK = Confirmar");
            break;
        //Mostra qual ação está sendo executada no momento (por ex., "Salvar pos. coleta")
        case EXECUTANDO_ACAO:
            lcd.locate(0, 0);
            lcd.printf("Executando:");
            lcd.locate(0, 1);
            lcd.printf("%s", opcoesMenu[opcaoSelecionada]);
            break;
    }
}

void executarLogica() {
    switch (estadoAtual) {
        case INICIO: //Se o botão Menu for pressionado, vai para o menu principal. wait_us() serve como debounce
            if (botaoMenu == 0) {
                estadoAtual = MENU_PRINCIPAL;
                opcaoSelecionada = 0;
                wait_us(300000);
            }
            break;

        case MENU_PRINCIPAL: //Ao pressionar Cima ou Baixo, muda a opção selecionada
            if (botaoCima == 0) {
                opcaoSelecionada = (opcaoSelecionada - 1 + TOTAL_OPCOES) % TOTAL_OPCOES;
                wait_us(300000);
            }
            if (botaoBaixo == 0) {
                opcaoSelecionada = (opcaoSelecionada + 1) % TOTAL_OPCOES;
                wait_us(300000);
            }
            if (botaoOK == 0) {
                if (opcaoSelecionada == 2) { // Selecionar volume
                    estadoAtual = MENU_VOLUMES;
                } else {
                    estadoAtual = EXECUTANDO_ACAO;
                }
                wait_us(300000);
            }
            break;

        case MENU_VOLUMES: //Permite aumentar ou diminuir o volume entre 1 mL e 5 mL com os botões. Botão OK confirma e retorna ao menu
            if (botaoCima == 0 && volumeSelecionado < 5) {
                volumeSelecionado++;
                wait_us(300000);
            }
            if (botaoBaixo == 0 && volumeSelecionado > 1) {
                volumeSelecionado--;
                wait_us(300000);
            }
            if (botaoOK == 0) {
                estadoAtual = MENU_PRINCIPAL;
                wait_us(300000);
            }
            break;

        case EXECUTANDO_ACAO:
            // Aqui você chamaria suas funções reais, como acionar motor, etc.
            wait_us(1500000); // Simula tempo de execução(1,5 segundos)
            estadoAtual = MENU_PRINCIPAL;// Depois volta ao menu principal
            break;
    }

    atualizarLCD();
}

// ===== Main loop embutido =====
//Inicializa o display. Entra em loop infinito, atualizando a lógica da IHM a cada 100 ms
int main() {
    atualizarLCD();
    while (true) {
        executarLogica();
        wait_us(100000);
    }
}


