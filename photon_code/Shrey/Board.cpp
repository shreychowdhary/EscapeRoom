#include "Board.h"
#include "Particle.h"

Board::Board()
{
    // Assign each slot its index, on state, current tile value, high bit index, low bit index, slot led index, border led indicies
    slots[0] = {0, false, 0, 23, 22, 4, {20, 21}};
    slots[1] = {1, false, 0, 19, 18, 5, {22, 23}};
    slots[2] = {2, false, 0, 17, 16, 7, {24, 25}};
    slots[3] = {3, false, 0, 11, 10, 8, {26, 27}};
    slots[4] = {4, false, 0, 9, 8, 9, {28, 29}};
    slots[5] = {5, false, 0, 3, 2, 11, {30, 31}};
    slots[6] = {6, false, 0, 1, 0, 12, {32, 33}};
    slots[7] = {7, false, 0, 5, 4, 13, {34, 35}};
    slots[8] = {8, false, 0, 7, 6, 14, {36, 37}};
    slots[9] = {9, false, 0, 13, 12, 0, {15, 16, 38}};
    slots[10] = {10, false, 0, 15, 14, 1, {17}};
    slots[11] = {11, false, 0, 21, 20, 3, {18, 19}};
    pixels = new Adafruit_NeoPixel(pixelCount, pixelPin);

    // Intialize Puzzle structs
    puzzles[0] = {{0, 1, 8}, {{0, 2}, {8, 9}}};
    puzzles[1] = {{1, 2, 3, 9}, {{1, 4}, {9, 12}}};
    puzzles[2] = {{3, 4, 5, 7}, {{3, 8}}};
    puzzles[3] = {{0, 1, 2, 4, 6, 10}, {{0, 5}, {6, 11}}};
    puzzles[4] = {{2, 7, 10}, {{2, 11}}};
}

void Board::Setup()
{
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);
    leftServo.attach(servoLeftPin);
    rightServo.attach(servoRightPin);
    // Clear leds
    pixels->begin();
    for (int i = 0; i < pixelCount; ++i)
    {
        pixels->setPixelColor(i, 0, 0, 0);
    }
    pixels->show();
    Serial.println("Board Setup");
}

void Board::Start()
{
    puzzleIndex = 0;
    LoadPuzzle(puzzleIndex);
}

void Board::LoadPuzzle(int index)
{
    Puzzle puzzle = puzzles[index];
    for (int i = 0; i < 12; ++i)
    {
        slots[i].on = false;
    }
    // Turn on slots
    for (int tile : puzzle.onTiles)
    {
        slots[tile].on = true;
    }
}

void Board::Update(uint32_t slotData)
{
    // Make sure its a valid puzzle, otherwise exit
    if (puzzleIndex < 0 || puzzleIndex > numPuzzles)
    {
        for (int i = 0; i < pixelCount; ++i)
        {
            pixels->setPixelColor(i, 0, 0, 0);
        }
        pixels->show();
        return;
    }

    // Update each slots tile value
    Serial.print("{");
    for (int i = 0; i < 12; ++i)
    {
        if ((1 << slots[i].lowBit) & slotData && (1 << slots[i].highBit) & slotData)
        {
            slots[i].tile = 5;
        }
        else if ((1 << slots[i].lowBit) & slotData)
        {
            slots[i].tile = 3;
        }
        else if ((1 << slots[i].highBit) & slotData)
        {
            slots[i].tile = 1;
        }
        else
        {
            slots[i].tile = 0;
        }
        Serial.printf("%d:%d,", i, slots[i].tile);
    }
    Serial.println("}");

    // If resetMode on wait till all tiles are removed from board
    if (resetMode)
    {
        ResetPuzzle();
        return;
    }

    UpdateLights();
    pixels->show();
    // Check to see if puzzle is solved
    for (std::pair<int, int> range : puzzles[puzzleIndex].ranges)
    {
        int lastCorrect = CorrectUpTo(range.first, range.second);
        if (lastCorrect != range.second - 1)
        {
            return;
        }
    }
    for (int i = 0; i < 12; ++i)
    {
        if (slots[i].on == false && slots[i].tile > 0)
        {
            return;
        }
    }
    Serial.println("Correct");
    delay(300);
    // Turn everything green to indicate solved puzzle
    for (int i = 0; i < pixelCount; i++)
    {
        pixels->setPixelColor(i, 0, 255, 0);
    }
    pixels->show();
    delay(1000);
    // Turn everything off for reset
    for (int i = 0; i < pixelCount; i++)
    {
        pixels->setPixelColor(i, 0, 0, 0);
    }
    pixels->show();
    delay(1000);
    resetMode = true;
}

void Board::UpdateLights()
{
    int doneRanges = 0;
    // Iterate along each range to turn on border leds in a wave manner, set 10 as an arbitrary upper limit
    for (int i = 0; i < 10; ++i)
    {
        for (std::pair<int, int> range : puzzles[puzzleIndex].ranges)
        {
            int lastCorrect = CorrectUpTo(range.first, range.second);
            int j = range.first + i;
            // Exit if this range no longer has any border leds
            if (j >= range.second)
            {
                continue;
            }
            if (j <= lastCorrect)
            {
                for (int index : slots[j].outerLedIndex)
                {
                    pixels->setPixelColor(index, 0, 0, 255);
                }
            }
            else
            {
                for (int index : slots[j].outerLedIndex)
                {
                    pixels->setPixelColor(index, 255, 255, 0);
                }
            }
            if (j + 1 == range.second)
            {
                doneRanges++;
            }
        }
        pixels->show();
        delay(100);
        if (doneRanges >= puzzles[puzzleIndex].ranges.size())
        {
            break;
        }
    }

    //Clear incorrect leds
    for (std::pair<int, int> range : puzzles[puzzleIndex].ranges)
    {
        int lastCorrect = CorrectUpTo(range.first, range.second);
        for (int i = range.first; i < range.second; ++i)
        {
            if (i > lastCorrect)
            {
                for (int index : slots[i].outerLedIndex)
                {
                    pixels->setPixelColor(index, 0, 0, 0);
                }
            }
        }
    }
    pixels->show();

    for (int i = 0; i < 12; i++)
    {
        // Set slot color to green if on, red if off but tile on it
        if (slots[i].on)
        {
            pixels->setPixelColor(slots[i].innerLedIndex, 0, 255, 0);
        }
        else if (!slots[i].on && slots[i].tile > 0)
        {
            pixels->setPixelColor(slots[i].innerLedIndex, 255, 0, 0);
            for (int index : slots[i].outerLedIndex)
            {
                pixels->setPixelColor(index, 255, 0, 0);
            }
        }
        else
        {
            pixels->setPixelColor(slots[i].innerLedIndex, 0, 0, 0);
            bool in = false;
            for (std::pair<int, int> range : puzzles[puzzleIndex].ranges)
            {
                if (i >= range.first && i < range.second)
                {
                    in = true;
                }
            }
            if (!in)
            {
                // Reset all border leds, unless its a part of a range
                for (int index : slots[i].outerLedIndex)
                {
                    pixels->setPixelColor(index, 0, 0, 0);
                }
            }
        }
    }
    pixels->show();
}

int Board::CorrectUpTo(int start, int end)
{
    int lastCorrect = start;
    int tileLeft = 0;
    for (int i = start; i < end; ++i)
    {
        if (tileLeft == 0 && slots[i % 12].tile > 0)
        {
            tileLeft = slots[i % 12].tile;
            lastCorrect = i - 1;
        }
        else if (tileLeft > 0 && slots[i % 12].tile > 0)
        {
            return lastCorrect;
        }
        else if (tileLeft == 0 && slots[i % 12].tile == 0)
        {
            return i - 1;
        }
        tileLeft--;
    }
    if (tileLeft == 0)
    {
        return end - 1;
    }
    return lastCorrect;
}

void Board::NextPuzzle()
{
    Particle.publish("location", String(puzzleIndex + 1));
    puzzleIndex++;
    Serial.printf("Start Puzzle:%d", puzzleIndex);
    if (puzzleIndex == 1)
    {
        for (int pos = 0; pos <= 180; pos += 1)
        {
            leftServo.write(pos);
            delay(15);
        }
    }
    if (puzzleIndex == 3)
    {
        for (int pos = 0; pos <= 180; pos += 1)
        {
            rightServo.write(pos);
            delay(15);
        }
    }
    if (puzzleIndex >= numPuzzles)
    {
        EndPuzzle();
    }
    else
    {
        LoadPuzzle(puzzleIndex);
    }
}

void Board::ResetPuzzle()
{
    bool correct = true;
    // Turn slot and border red if tile on it
    for (int i = 0; i < 12; i++)
    {
        if (slots[i].tile > 0)
        {
            correct = false;
            pixels->setPixelColor(slots[i].innerLedIndex, 255, 0, 0);
            for (int index : slots[i].outerLedIndex)
            {
                pixels->setPixelColor(index, 255, 0, 0);
            }
        }
        else
        {
            pixels->setPixelColor(slots[i].innerLedIndex, 0, 0, 0);
            for (int index : slots[i].outerLedIndex)
            {
                pixels->setPixelColor(index, 0, 0, 0);
            }
        }
    }
    pixels->show();
    if (correct)
    {
        resetMode = false;
        NextPuzzle();
    }
}

void Board::EndPuzzle()
{
    // Nothing here, but maybe eventually
}

Board::~Board()
{
    delete[] slots;
    delete pixels;
}
