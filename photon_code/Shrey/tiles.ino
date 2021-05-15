// This #include statement was automatically added by the Particle IDE.

#include "Board.h"

Board board;

void setup()
{
    // Listen to tileopen to start the puzzle
    Particle.subscribe("tileopen", boardStart);
    Serial.begin(9600);
    board.Setup();
    board.Start();
}

void boardStart(String event, String data)
{
    board.Start();
    Serial.println("Start");
}

void loop()
{
    digitalWrite(board.latchPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(board.latchPin, LOW);

    // Get tile input
    uint32_t slotData = uint32_t(shiftInData(board.dataPin, board.clockPin, MSBFIRST) | shiftInData(board.dataPin, board.clockPin, MSBFIRST) << 8 | shiftInData(board.dataPin, board.clockPin, MSBFIRST) << 16);
    board.Update(slotData);
    delay(500);
}

//Takes in the serial data from the shift in register
uint32_t shiftInData(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
    uint32_t value = 0;
    uint32_t i;
    for (i = 0; i < 8; ++i)
    {
        digitalWrite(clockPin, LOW);
        if (bitOrder == LSBFIRST)
        {
            value |= digitalRead(dataPin) << i;
        }
        else
        {
            value |= digitalRead(dataPin) << (7 - i);
        }
        digitalWrite(clockPin, HIGH);
    }
    return value;
}