int photocellPin = A0;
int speaker = D2;
int photocellThreshold = 1700;
bool hasBeeped = false;

void ToggleSpeaker(String command, String data)
{
    tone(speaker, 2000, 3000);
}

void setup()
{
    Serial.begin(9600);
    pinMode(speaker, OUTPUT);
    Particle.subscribe("translate", ToggleSpeaker);
}

void loop()
{
    int photocellReading = analogRead(photocellPin);
    if (photocellReading > photocellThreshold)
    {
        Particle.publish("tileopen");
    }
    Serial.printf("Analog reading =%d,\n", photocellReading);

    delay(100);
}
