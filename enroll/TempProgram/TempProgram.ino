#include <Adafruit_Fingerprint.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

volatile int finger_status = -1;
const int sg_pulse_pin = 10;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    delay(100);
    Serial.println("\n\n finger print detect test");
    // as608
    finger.begin(57600);
    while (!finger.verifyPassword())
    {
        Serial.println("Did not find fingerprint sensor :(");
        delay(1);
    }
    // sg90
    servo.attach(sg_pulse_pin);
    servo.write(0);
}
void loop()
{
    finger_status = verrifyFingerPrint();
    switch (finger_status)
    {
    case -1:
        Serial.println("finger not detected");
        break;
    case -2:
        Serial.println("communication wrong");
        break;
    case -3:
        Serial.println("finger not matched");
        break;
    default:
        Serial.println(finger_status);
        unlockByPrint(finger_status);
        break;
    }
}

int verrifyFingerPrint()
{
    uint8_t print = finger.getImage();
    if (print != FINGERPRINT_OK)
    {
        switch (print)
        {
        case FINGERPRINT_NOFINGER:
        case FINGERPRINT_IMAGEFAIL:
            return -1;
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            return -2;
            break;
        default:
            break;
        }
    }
    print = finger.image2Tz();
    if (print != FINGERPRINT_OK)
    {
        switch (print)
        {
        case FINGERPRINT_IMAGEMESS:
        case FINGERPRINT_FEATUREFAIL:
        case FINGERPRINT_INVALIDIMAGE:
            return -1;
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            return -2;
            break;
        default:
            break;
        }
    }
    print = finger.fingerFastSearch();
    if (print != FINGERPRINT_OK)
    {
        switch (print)
        {
        case FINGERPRINT_NOTFOUND:
            return -3;
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            return -2;
            break;
        default:
            break;
        }
    }
    if (finger.confidence < 100)
    {
        return -3;
    }
    else
    {
        return finger.fingerID;
    }
}
void unlockByPrint(int fingerID)
{
    servo.write(180);
    lcd.init();
    lcd.backlight();
    switch (fingerID)
    {
    case 1:
        lcd.print("He Ziang");
        break;

    default:
        break;
    }
    lcd.setCursor(0, 1);
    lcd.print("Welcome!");
    delay(5000);
    servo.write(0);
    lcd.clear();
}