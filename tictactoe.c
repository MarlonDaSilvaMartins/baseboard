// C++ code
// http://www.comofazerascoisas.com.br/controlando-8-leds-com-arduino-usando-ci74HC595-fazendo-uma-contagem-binaria.html
// http://www.learningaboutelectronics.com/Articles/Cascade-shift-registers.php
#include <LiquidCrystal.h>
LiquidCrystal LCD(A4, A5, A0, A1, A2, A3);

int const clock = 2; //ligado ao clock do 74HC595
int const latch = 3; //ligado ao latch do 74HC595
int const data  = 4; //ligado ao data do 74HC595

typedef struct _boardCell{
    int Red;
    int Green;
    int Blue;

    int cellValue; // 0 = vazio; 1 = player; 2 = IA
} BoardCell;

BoardCell Board[3][3];
int playsRemaining;

int buttonList[] = {5, 6, 7, 8, 9, 10, 11, 12, 13};

void MasterReset()
{
    int qtBits=8*4;
    digitalWrite(latch, LOW);
    for(int i=0; i<qtBits; i++)
    {
        digitalWrite(clock, LOW);
        digitalWrite(data, LOW);
        digitalWrite(clock, HIGH);
    }
    digitalWrite(latch, HIGH);

}

void ResetBoard(int Red, int Green, int Blue)
{
    for(int line=0; line<3; line++)
    {
        for(int column=0; column<3; column++)
        {
            Board[line][column].Red   = Red;
            Board[line][column].Green = Green;
            Board[line][column].Blue  = Blue;

            Board[line][column].cellValue = 0;
        }
    }
    MasterReset();
}

void initGameBoard(){
    ResetBoard(0, 0, 0);
    playsRemaining = 9;

    LCD.clear();
    LCD.print("faca sua jogada");
}

void LightBoard()
{
    /*
    ** Red -> Blue -> Green
    */

    int pos=0;
    int bit=0;

    MasterReset();

    digitalWrite(latch, LOW); // enable writting

    for(int k=2; k>=0; k--)
        for(int line=2; line>=0; line--)
        {
            for(int column=2; column>=0; column--)
            {
                //pos = line * 3 + column;

                // sending the bit
                digitalWrite(clock, LOW);
                switch(k)
                {
                    case 0: // Red
                        bit = (Board[line][column].Red==1?HIGH:LOW);
                        break;
                    case 1: // Blue
                        bit = (Board[line][column].Blue==1?HIGH:LOW);
                        break;
                    case 2: // Green
                        bit = (Board[line][column].Green==1?HIGH:LOW);
                        break;
                    default:
                        bit = 0;
                        break;
                }


                digitalWrite(data, bit);
                digitalWrite(clock, HIGH);

                Serial.print(bit);
            }
        }
    digitalWrite(latch, HIGH);
    Serial.println();
}

int ButtonRead(){
    int k=0, buttonState=0, button=-1;
    for(k=0; k < 9; k++){
        buttonState = digitalRead(buttonList[k]);
        if(buttonState == HIGH){
            LCD.clear();
            LCD.print(buttonList[k]);
            button = buttonList[k];
            break;
        }
    }

    return button;

}

int checkWin(){
    int winner = 0;
    for(int line = 0; line < 3; line++){
        if(Board[line][0].cellValue == Board[line][1].cellValue &&
           Board[line][0].cellValue == Board[line][2].cellValue &&
           Board[line][0].cellValue != 0){
            if((winner = 1) == Board[line][0].cellValue){
                LCD.clear();
                LCD.print("Voce venceu");
                return winner;
            }
            if((winner = 1) == Board[line][0].cellValue){
                LCD.clear();
                LCD.print("Voce perdeu");
                return winner;
            }
        }
    }
    for(int column = 0; column < 3; column++){
        if(Board[0][column].cellValue == Board[1][column].cellValue &&
           Board[0][column].cellValue == Board[2][column].cellValue &&
           Board[0][column].cellValue != 0){
            if((winner = 1) == Board[0][column].cellValue){
                LCD.clear();
                LCD.print("Voce venceu");
                return winner;
            }
            if((winner = 1) == Board[0][column].cellValue){
                LCD.clear();
                LCD.print("Voce perdeu");
                return winner;
            }
        }
    }
    if(Board[0][0].cellValue == Board[1][1].cellValue &&
       Board[0][0].cellValue == Board[2][2].cellValue &&
       Board[0][0].cellValue != 0){
        if((winner = 1) == Board[0][0].cellValue){
            LCD.clear();
            LCD.print("Voce venceu");
            return winner;
        }
        if((winner = 2) == Board[0][0].cellValue){
            LCD.clear();
            LCD.print("Voce perdeu");
            return winner;
        }
    }
    if(Board[0][2].cellValue == Board[1][1].cellValue &&
       Board[0][2].cellValue == Board[2][0].cellValue &&
       Board[0][2].cellValue != 0){
        if((winner = 1) == Board[0][2].cellValue){
            LCD.clear();
            LCD.print("Voce venceu");
            return winner;
        }
        if((winner = 2) == Board[0][2].cellValue){
            LCD.clear();
            LCD.print("Voce perdeu");
            return winner;
        }
        return winner;
    }
    if(playsRemaining == 0 && winner == 0){
        LCD.clear();
        LCD.print("Empatou");
        return 2;
    }
    return winner;
}

int evaluate(int player) {
    int i, j, score = 0;

    for (i = 0; i < 3; i++) {
        if (Board[i][0].cellValue == player && Board[i][1].cellValue == player && Board[i][2].cellValue == player)
            return 100;
        else if (Board[i][0].cellValue != 0 && Board[i][0].cellValue != player && Board[i][1].cellValue != 0 && Board[i][1].cellValue != player && Board[i][2].cellValue != 0 && Board[i][2].cellValue != player)
            score -= 5;
        else if (Board[i][0].cellValue != 0 && Board[i][0].cellValue != player && Board[i][1].cellValue != 0 && Board[i][1].cellValue != player)
            score -= 2;
        else if (Board[i][1].cellValue != 0 && Board[i][1].cellValue != player && Board[i][2].cellValue != 0 && Board[i][2].cellValue != player)
            score -= 2;
        else if (Board[i][0].cellValue != 0 && Board[i][0].cellValue != player && Board[i][2].cellValue != 0 && Board[i][2].cellValue != player)
            score -= 2;
    }

    for (j = 0; j < 3; j++) {
        if (Board[0][j].cellValue == player && Board[1][j].cellValue == player && Board[2][j].cellValue == player)
            return 100;
        else if (Board[0][j].cellValue != 0 && Board[0][j].cellValue != player && Board[1][j].cellValue != 0 && Board[1][j].cellValue != player && Board[2][j].cellValue != 0 && Board[2][j].cellValue != player)
            score -= 5;
        else if (Board[0][j].cellValue != 0 && Board[0][j].cellValue != player && Board[1][j].cellValue != 0 && Board[1][j].cellValue != player)
            score -= 2;
        else if (Board[1][j].cellValue != 0 && Board[1][j].cellValue != player && Board[2][j].cellValue != 0 && Board[2][j].cellValue != player)
            score -= 2;
        else if (Board[0][j].cellValue != 0 && Board[0][j].cellValue != player && Board[2][j].cellValue != 0 && Board[2][j].cellValue != player)
            score -= 2;
    }

    if (Board[0][0].cellValue == player && Board[1][1].cellValue == player && Board[2][2].cellValue == player)
        return 100;
    else if (Board[0][0].cellValue != 0 && Board[0][0].cellValue != player && Board[1][1].cellValue != 0 && Board[1][1].cellValue != player && Board[2][2].cellValue != 0 && Board[2][2].cellValue != player)
        score -= 5;
    else if (Board[0][0].cellValue != 0 && Board[0][0].cellValue != player && Board[1][1].cellValue != 0 && Board[1][1].cellValue != player)
        score -= 2;
    else if (Board[1][1].cellValue != 0 && Board[1][1].cellValue != player && Board[2][2].cellValue != 0 && Board[2][2].cellValue != player)
        score -= 2;

    if (Board[0][2].cellValue == player && Board[1][1].cellValue == player && Board[2][0].cellValue == player)
        return 100;
    else if (Board[0][2].cellValue != 0 && Board[0][2].cellValue != player && Board[1][1].cellValue != 0 && Board[1][1].cellValue != player && Board[2][0].cellValue != 0 && Board[2][0].cellValue != player)
        score -= 5;
    else if (Board[0][2].cellValue != 0 && Board[0][2].cellValue != player && Board[1][1].cellValue != 0 && Board[1][1].cellValue != player)
        score -= 2;
    else if (Board[1][1].cellValue != 0 && Board[1][1].cellValue != player && Board[2][0].cellValue != 0 && Board[2][0].cellValue != player)
        score -= 2;

    return score;
}

int minimax(int player) {
    int score = evaluate(player);
    if (score != 0) {
        return score;
    }
    if (playsRemaining == 0) {
        return 0;
    }
    int bestScore = (player == 2) ? -10 : 10;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (Board[i][j].cellValue == 0) {
                Board[i][j].cellValue = player;
                int score = minimax((player == 2) ? 1 : 2);
                Board[i][j].cellValue = 0;
                if ((player == 2 && score > bestScore) || (player == 1 && score < bestScore)) {
                    bestScore = score;
                }
            }
        }
    }
    return bestScore;
}

void IATurn(){
    int bestScore = -10;
    int bestI = -1;
    int bestJ = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (Board[i][j].cellValue == 0) {
                Board[i][j].cellValue = 2;
                int score = minimax(1);
                Board[i][j].cellValue = 0;
                if (score > bestScore) {
                    bestScore = score;
                    bestI = i;
                    bestJ = j;
                }
            }
        }
    }
    playsRemaining--;
    Board[bestI][bestJ].Red = 1;
    Board[bestI][bestJ].cellValue = 2;
}

void playerMove(int line, int column){
    if(Board[line][column].cellValue == 0){
        LCD.clear();
        LCD.print("faca sua jogada");
        playsRemaining--;
        Board[line][column].cellValue = 1;
        Board[line][column].Blue=1;
        if(checkWin() == 0) {
            IATurn();
        }
    }else{
        LCD.clear();
        LCD.print("Selecione um campo vazio");
    }
}

void playerTurn(int keyPressed){
    switch(keyPressed){
        case 07:
            playerMove(0, 2);
            break;
        case 06:
            playerMove(0, 1);
            break;
        case 05:
            playerMove(0, 0);
            break;
        case 10:
            playerMove(1, 2);
            break;
        case 9:
            playerMove(1, 1);
            break;
        case 8:
            playerMove(1, 0);
            break;
        case 13:
            playerMove(2, 2);
            break;
        case 12:
            playerMove(2, 1);
            break;
        case 11:
            playerMove(2, 0);
            break;
    }
}

void setup()
{
    Serial.begin(9600);
    LCD.begin(16, 2);

    //definindo os pinos como de saída
    pinMode(clock,OUTPUT);
    pinMode(latch,OUTPUT);
    pinMode(data,OUTPUT);

    for(int k=0; k < 9; k++)
    {
        pinMode(buttonList[k], INPUT);
    }

    initGameBoard();
}

void loop(){
    int keyPressed = -1;

    keyPressed = ButtonRead();
    if(checkWin() == 0) {
        if (keyPressed != -1)
            playerTurn(keyPressed);
    }else {
        if (keyPressed != -1)
            initGameBoard();
    }
    LightBoard();
}
