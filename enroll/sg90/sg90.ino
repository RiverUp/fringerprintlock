#include <Servo.h>

Servo servo;

const int pulse_pin = 10;

void setup()
{
    Serial.begin(9600);
    servo.attach(pulse_pin);
    servo.write(0);
}

void loop()
{
    while (!Serial.available())
        ;
    char direction = Serial.read();
    int angle = servo.read();
    switch (direction)
    {
    case 'r':
        if (angle == 0)
        {
            servo.write(180);
        }
        break;
    case 'l':
        if (angle == 180)
        {
            servo.write(0);
        }
        break;
    case 'c':
        Serial.print(angle);
        break;
    default:
        break;
    }
}
