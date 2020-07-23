
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "HX711.h"                                            // подключаем библиотеку для работы с тензодатчиком

#define DT  3                                                // Указываем номер вывода, к которому подключен вывод DT  датчика
#define SCK 5                                                // Указываем номер вывода, к которому подключен вывод SCK датчика


LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
HX711 scale;  // создаём объект scale


float calibration_factor = -202;                            // вводим калибровочный коэффициент
int units;                                                  // задаём переменную для измерений в граммах
float N;                                                 // задаём переменную для измерений в N

void getdata()
{
  Serial.println ("********************************************");
  long w;
  w = 0;
  for (int q = 0; q < 200; q = q + 2)
  {
    EEPROM.get(q, units);
    Serial.print (w);
    Serial.print ("\t");
    Serial.println (units);
    w = w + 100;
  }
  Serial.println ("********************************************");
}
void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(2, 0);
  lcd.print("ROCKET MOTOR");
  lcd.setCursor(1, 1);
  lcd.print("by Kulich 2020");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  SENDIND DATA");

  lcd.setCursor(7, 1);
  lcd.print("1");
  getdata();
  delay(250);
  lcd.setCursor(7, 1);
  lcd.print("2");
  getdata();
  delay(250);
  lcd.setCursor(7, 1);
  lcd.print("3");
  getdata();
  delay(250);

  lcd.print("  SENDIND DATA");
  lcd.clear();
  lcd.print("    WARNING!");
  lcd.setCursor(0, 1);
  lcd.print("10 secs to START");
  delay(10000);



  scale.begin(DT, SCK);                                       // инициируем работу с датчиком
  scale.set_scale();                                          // выполняем измерение значения без калибровочного коэффициента
  scale.tare();                                               // сбрасываем значения веса на датчике в 0
  scale.set_scale(calibration_factor);                        // устанавливаем калибровочный коэффициент
  lcd.clear();

  Serial.println("Wait");

  lcd.print("Wait");
  units = scale.get_units();
  while (units < 30)
  {
    units = scale.get_units();
  }

  EEPROM.put(0, units);

  Serial.println ("Start");
  for (int q = 2; q < 200; q = q + 2)
  {
    units = scale.get_units();
    EEPROM.put(q, units);

    Serial.print(q);
    Serial.print ("\t");
    Serial.println (units);

    lcd.setCursor(0, 0);
    lcd.print(units);
    lcd.print("    ");
    delayMicroseconds (490);
  }
  Serial.println ("Finish");


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  SENDIND DATA");
  getdata();


}



void loop()
{

}
