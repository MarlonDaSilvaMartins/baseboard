// C++ code
// http://www.comofazerascoisas.com.br/controlando-8-leds-com-arduino-usando-ci74HC595-fazendo-uma-contagem-binaria.html
// http://www.learningaboutelectronics.com/Articles/Cascade-shift-registers.php
#include <LiquidCrystal.h>

LiquidCrystal LCD(A4, A5, A0, A1, A2, A3);

int const clock = 2; //ligado ao clock do 74HC595
int const latch = 3; //ligado ao latch do 74HC595
int const data = 4; //ligado ao data do 74HC595

typedef struct _boardCell {
    int Red;
    int Green;
    int Blue;

} BoardCell;

typedef struct _boardGame {
    int isBlocked;
    int cellValue; // 0 = vazio; 1 = player; 2 = IA
} BoardGame;

BoardCell Board[3][3];
BoardGame boardGame[3][3];
int playsRemaining;
int IADifficulty;

int buttonList[] = {5, 6, 7, 8, 9, 10, 11, 12, 13};

void MasterReset() {
    int qtBits = 8 * 4;
    digitalWrite(latch, LOW);
    for (int i = 0; i < qtBits; i++) {
        digitalWrite(clock, LOW);
        digitalWrite(data, LOW);
        digitalWrite(clock, HIGH);
    }
    digitalWrite(latch, HIGH);

}

void ResetBoard(int Red, int Green, int Blue) {
    for (int line = 0; line < 3; line++) {
        for (int column = 0; column < 3; column++) {
            Board[line][column].Red = Red;
            Board[line][column].Green = Green;
            Board[line][column].Blue = Blue;
        }
    }
    MasterReset();
}

void initGameBoard() {
    ResetBoard(0, 0, 0);
    playsRemaining = 9;
    IADifficulty = 0;
    for (int line = 0; line < 3; line++) {
        for (int column = 0; column < 3; column++) {
            boardGame[line][column].isBlocked = 0;
            boardGame[line][column].cellValue = 0;
        }
    }

    LCD.clear();
    LCD.print("Selecione a dificuldade");
}

void LightBoard() {
    /*
    ** Red -> Blue -> Green
    */

    int pos = 0;
    int bit = 0;

    MasterReset();

    digitalWrite(latch, LOW); // enable writting

    for (int k = 2; k >= 0; k--)
        for (int line = 2; line >= 0; line--) {
            for (int column = 2; column >= 0; column--) {
                //pos = line * 3 + column;

                // sending the bit
                digitalWrite(clock, LOW);
                switch (k) {
                    case 0: // Red
                        bit = (Board[line][column].Red == 1 ? HIGH : LOW);
                        break;
                    case 1: // Blue
                        bit = (Board[line][column].Blue == 1 ? HIGH : LOW);
                        break;
                    case 2: // Green
                        bit = (Board[line][column].Green == 1 ? HIGH : LOW);
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

int ButtonRead() {
    int k = 0, buttonState = 0, button = -1;
    for (k = 0; k < 9; k++) {
        buttonState = digitalRead(buttonList[k]);
        if (buttonState == HIGH) {
            LCD.clear();
            LCD.print(buttonList[k]);
            button = buttonList[k];
            break;
        }
    }

    return button;

}

void setDifficulty(int keyPressed){
    switch (keyPressed) {
        case 13:
            IADifficulty = 3;
            break;
        case 12:
            IADifficulty = 2;
            break;
        case 11:
            IADifficulty = 1;
            break;
    }
}

void IATurn() {

}

void playerMove(int line, int column) {
    if (boardGame[line][column].isBlocked == 0) {
        LCD.clear();
        LCD.print("faca sua jogada");
        playsRemaining--;
        boardGame[line][column].isBlocked = 1;
        boardGame[line][column].cellValue = 1;
        Board[line][column].Blue = 1;
        IATurn();
    } else {
        LCD.clear();
        LCD.print("Selecione um campo vazio");
    }
}

void playerTurn(int keyPressed) {
    switch (keyPressed) {
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

int checkWin() {
    for (int line = 0; line < 3; line++) {
        if (boardGame[line][0].cellValue == boardGame[line][1].cellValue &&
            boardGame[line][0].cellValue == boardGame[line][2].cellValue &&
            boardGame[line][0].cellValue != 0) {
            if (boardGame[line][0].cellValue == 1) {
                LCD.clear();
                LCD.print("Voce venceu");
            }
            if (boardGame[line][0].cellValue == 2) {
                LCD.clear();
                LCD.print("Voce perdeu");
            }
            return 1;
        }
    }
    for (int column = 0; column < 3; column++) {
        if (boardGame[0][column].cellValue == boardGame[1][column].cellValue &&
            boardGame[0][column].cellValue == boardGame[2][column].cellValue &&
            boardGame[0][column].cellValue != 0) {
            if (boardGame[0][column].cellValue == 1) {
                LCD.clear();
                LCD.print("Voce venceu");
            }
            if (boardGame[0][column].cellValue == 2) {
                LCD.clear();
                LCD.print("Voce perdeu");
            }
            return 1;
        }
    }
    if (boardGame[0][0].cellValue == boardGame[1][1].cellValue &&
        boardGame[0][0].cellValue == boardGame[2][2].cellValue &&
        boardGame[0][0].cellValue != 0) {
        if (boardGame[0][0].cellValue == 1) {
            LCD.clear();
            LCD.print("Voce venceu");
        }
        if (boardGame[0][0].cellValue == 2) {
            LCD.clear();
            LCD.print("Voce perdeu");
        }
        return 1;
    }
    if (boardGame[0][2].cellValue == boardGame[1][1].cellValue &&
        boardGame[0][2].cellValue == boardGame[2][0].cellValue &&
        boardGame[0][2].cellValue != 0) {
        if (boardGame[0][2].cellValue == 1) {
            LCD.clear();
            LCD.print("Voce venceu");
        }
        if (boardGame[0][2].cellValue == 2) {
            LCD.clear();
            LCD.print("Voce perdeu");
        }
        return 1;
    }
    return 0;
}

void setup() {
    Serial.begin(9600);
    LCD.begin(16, 2);

    //definindo os pinos como de saída
    pinMode(clock, OUTPUT);
    pinMode(latch, OUTPUT);
    pinMode(data, OUTPUT);

    for (int k = 0; k < 9; k++) {
        pinMode(buttonList[k], INPUT);
    }

    initGameBoard();
}

void loop() {
    int keyPressed = -1;

    delay(500);
    keyPressed = ButtonRead();
    if (IADifficulty != 0) {
        if (checkWin() == 0) {
            if (playsRemaining > 0)
                if (keyPressed != -1)
                    playerTurn(keyPressed);
        } else {
            if (keyPressed != -1)
                initGameBoard();
        }
    } else {
        if (keyPressed != -1)
            setDifficulty(keyPressed);
    }
    LightBoard();
}
