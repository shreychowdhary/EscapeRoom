int arduinoPin = D0;
bool hasSentEnd = false;

void setup()
{
    pinMode(arduinoPin, INPUT);
}

void loop()
{
    Serial.printf("%d, %d\n", digitalRead(arduinoPin), hasSentEnd);
    if (digitalRead(arduinoPin) == HIGH && !hasSentEnd)
    {
        hasSentEnd = true;
        Particle.publish("end");
    }
}
