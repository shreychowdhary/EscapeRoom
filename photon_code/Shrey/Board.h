#pragma once
#include <neopixel.h>
#include <vector>
#include <utility>
struct Slot
{
    uint8_t index;
    bool on;
    uint8_t tile;
    uint32_t highBit;
    uint32_t lowBit;
    int innerLedIndex;
    std::vector<int> outerLedIndex;
};

struct Puzzle
{
    std::vector<int> onTiles;
    std::vector<std::pair<int, int>> ranges;
};

class Board
{
public:
    Board();
    ~Board();
    void Setup();
    void Start();
    void Update(uint32_t);
    int pixelCount = 39;
    int pixelPin = D2;
    int dataPin = D5;
    int clockPin = D3;
    int latchPin = D4;
    int servoLeftPin = D0;
    int servoRightPin = D1;

private:
    void LoadPuzzle(int);
    void NextPuzzle();
    void EndPuzzle();
    void UpdateLights();
    void ResetPuzzle();
    int CorrectUpTo(int, int);
    Slot slots[12];
    Puzzle puzzles[5];
    int puzzleIndex = -1;
    int numPuzzles = 5;
    bool resetMode = false;
    Adafruit_NeoPixel *pixels;
    Servo leftServo;
    Servo rightServo;
};