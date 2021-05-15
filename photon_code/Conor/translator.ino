int potPin = A0; // select the input pin for the potentiometer
int ledPin = D0; // select the pin for the LED
int val = 0;     // variable to store the value coming from the sensor
int sense = 0;   // variable to detect which letter is selected
int count = 0;   // variable to count when all 5 letters have been input
int pressed = 0;
int translate = 0;

void setup()
{
    pinMode(D3, INPUT_PULLUP); // declare D3 as input for button
    pinMode(ledPin, OUTPUT);   // declare the ledPin as an OUTPUT
}

void loop()
{
    val = analogRead(A0);      //sets the variable to the value of the pot
    pressed = digitalRead(D3); //sets the variable to the state of the button

    if (pressed == 0)
    {               //if button pressed
        delay(500); //debounce
        count = count + 1;
        if (count == 5)
        { //begins logic if button has been pressed 5 times
            if ((val > 1708) and (val < 2380))
            {                                  //checks if the pot is pointing at the last letter
                Particle.publish("translate"); //tells website to advance
                Serial.print("PASS!");
            }
            else
            { //flashes error message on LED
                count = 0;
                Serial.print("FAIL!");
                digitalWrite(D0, HIGH);
                delay(300);
                digitalWrite(D0, LOW);
                delay(100);
                digitalWrite(D0, HIGH);
                delay(300);
                digitalWrite(D0, LOW);
                delay(100);
                digitalWrite(D0, HIGH);
                delay(300);
                digitalWrite(D0, LOW);
                delay(100);
                digitalWrite(D0, HIGH);
                delay(300);
                digitalWrite(D0, LOW);
                delay(100);
                digitalWrite(D0, HIGH);
                delay(300);
                digitalWrite(D0, LOW);
                delay(100);
                digitalWrite(D0, HIGH);
                delay(300);
                digitalWrite(D0, LOW);
            }
        }
        if (count > 5)
        {
            count = 0; // resets the counter to 0 after 5 presses
        }
    }
}
