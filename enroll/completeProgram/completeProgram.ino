#include <Adafruit_Fingerprint.h>
#include <avr/sleep.h>

volatile boolean printDetected;

SoftwareSerial mySerial(2, 3)
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
    finger.begin(57600);
    while (!finger.verifyPassword())
        ;
}

int verifyFinger()
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
void goToSleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    attachInterrupt(0, setPrintDetected, LOW);
    sleep_mode();
    sleep_disable();
    detachInterrupt(0);
}
void setPrintDetected()
{
    printDetected = true;
}