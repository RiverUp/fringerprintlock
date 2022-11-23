#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.print("hello,He Ziang");
    lcd.setCursor(0, 1);
    lcd.print("Welcome!");
    //delay(10000);
    //lcd.clear();
    //delay(10000);
    //lcd.noDisplay();
}
void loop()
{
}