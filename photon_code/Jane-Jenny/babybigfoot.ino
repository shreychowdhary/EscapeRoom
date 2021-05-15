// This #include statement was automatically added by the Particle IDE.
#include <speaker.h>

int photocellPin = A0;
int photocellReading;
int led = D7;

int red_light_pin = D0;
int green_light_pin = D2;
int blue_light_pin = D1;

uint16_t bufferSize = 128;
Speaker speaker(bufferSize);
uint16_t audioFrequency = 22050;

int count = 0;

void setup()
{
    // Hz
    speaker.begin(audioFrequency);

    pinMode(led, OUTPUT);

    pinMode(photocellPin, INPUT);

    pinMode(red_light_pin, OUTPUT);
    pinMode(green_light_pin, OUTPUT);
    pinMode(blue_light_pin, OUTPUT);

    Serial.begin(9600);
}

uint16_t audioSignal = 0;

void loop()
{

    photocellReading = analogRead(photocellPin);
    Serial.print("Analog read =");
    Serial.println(photocellReading);

    if (photocellReading > 2000)
    {
        if (count <= 7)
        {
            digitalWrite(led, HIGH);

            RGB_color(255, 0, 0); // Red
            delay(100);
            RGB_color(0, 0, 255); // Blue
            delay(100);
            count++;
            speaker.begin(audioFrequency);
            if (speaker.ready())
            {
                uint16_t *buffer = speaker.getBuffer();
                // Produces a 1 kHz sawtooth wave
                for (uint16_t i = 0; i < bufferSize; i++)
                {
                    buffer[i] = audioSignal;
                    audioSignal += 2267;
                    if (audioSignal > 50000)
                    {
                        audioSignal = 0;
                    }
                }
            }
        }
        else
        {
            digitalWrite(led, LOW);
            RGB_color(0, 0, 0);
            speaker.end();
        }
    }
    else
    {
        digitalWrite(led, LOW);
        RGB_color(0, 0, 0);
        speaker.end();
    }
    delay(100);
}
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
    Serial.print("Color Values, Red: ");
    Serial.print(red_light_pin);
    Serial.print(" Green: ");
    Serial.print(green_light_pin);
    Serial.print(" Blue: ");
    Serial.println(blue_light_pin);
    analogWrite(red_light_pin, 255 - red_light_value);
    analogWrite(blue_light_pin, 255 - blue_light_value);
    analogWrite(green_light_pin, 255 - green_light_value);
}