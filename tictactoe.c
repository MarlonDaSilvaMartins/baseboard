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

} BoardCell;

typedef struct _boardGame{
    int isBlocked;
    int cellValue; // 0 = vazio; 1 = player; 2 = IA
} BoardGame;

BoardCell Board[3][3];
BoardGame boardGame[3][3];
int playsRemaining = 9;

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
        }
    }
    MasterReset();
}

void initGameBoard(){
    for(int line=0; line<3; line++){
        for(int column=0; column<3; column++){
            boardGame[line][column].isBlocked = 0;
            boardGame[line][column].cellValue = 0;
        }
    }
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
    for(k=buttonList[0]; k < buttonList[8]; k++){
        buttonState = digitalRead(buttonList[k]);
        if(buttonState == HIGH){
            button = k;
            break;
        }
    }

    return button;

}

void playerMove(int line, int column){
    if(boardGame[line][column].isBlocked == 0){
        LCD.clear();
        LCD.print("faca sua jogada");
        playsRemaining--;
        boardGame[line][column].isBlocked = 1;
        boardGame[line][column].cellValue = 1;
        Board[line][column].Blue=1;
    }else{
        LCD.clear();
        LCD.print("Selecione um campo vazio");
    }
}

void playerTurn(int keyPressed){
    switch(keyPressed){
        case 7:
            playerMove(1, 1);
            break;
        case 6:
            playerMove(1, 2);
            break;
        case 5:
            playerMove(1, 3);
            break;
        case 10:
            playerMove(2, 1);
            break;
        case 9:
            playerMove(2, 2);
            break;
        case 8:
            playerMove(2, 3);
            break;
        case 13:
            playerMove(3, 1);
            break;
        case 12:
            playerMove(3, 2);
            break;
        case 11:
            playerMove(3, 3);
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

    for(int k=buttonList[0]; k <= buttonList[8]; k++)
    {
        pinMode(buttonList[k], INPUT);
    }


    // resetting the Board
    ResetBoard(0, 0, 0);
    initGameBoard();

    LCD.print("faca sua jogada");
}

void loop(){
    int keyPressed = -1;
    int isPlayerTurn = 0;

//    ResetBoard(0, 0, 0);
    LightBoard();


    keyPressed = ButtonRead();
    if(keyPressed!= -1){
        playerTurn(keyPressed);
    }
//    IATurn();


    LightBoard();
}
