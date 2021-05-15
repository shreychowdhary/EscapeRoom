//variable that the programs checks
int correct = 0;

void setup()
{
    // declares the button as an input and sets it as an interrupt
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(3), BTN, LOW);
    //sets pin 8 as the control pin for the relay
    pinMode(8, OUTPUT);
    //sets pin 2 as the sense pin for the reed switch
    pinMode(2, INPUT);
    Serial.begin(9600);
    //sets the relay to open in case the arduino gets confused
    digitalWrite(8, LOW);
}

void loop()
{
    // The loop is not used for anything
}

//This establishes the action code for the interrupt
void BTN()
{
    //the check variable is set equal to the value of the reed switch
    correct = digitalRead(2);
    //small debounce
    delay(50);
    //if the magnet in the C tile is in the correct position, the relay trips and allows the phone to be powered
    if (correct == 1)
    {
        digitalWrite(8, HIGH);
    }
    if (correct == 0)
    {
        digitalWrite(8, LOW);
    }
    delay(500);
}
