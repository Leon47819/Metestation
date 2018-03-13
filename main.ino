/** 
 *  stable version
**/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h> // Библиотека для работы с барометром https://github.com/adafruit/Adafruit-BMP280-Library
#include <OLED_I2C.h>

#define DISPLAY_TIMEOUT 1000
#define DEBUG_BMP

Adafruit_BMP280 bmp; // I2C

float P; // В данную переменную мы будем пересчитывать давление в милиметры ртутного столба

OLED myOLED(SDA, SCL, 8); // Подключение дисплея hardware SDA , SCL
extern uint8_t RusFont[]; // Русский шрифт
extern uint8_t MegaNumbers[]; // Подключение больших шрифтов
extern uint8_t SmallFont[]; // Базовый шрифт без поддержки русскийх символов.

#define SHOW_VERSION      0
#define SHOW_TEMP         1
#define SHOW_PREASSURE    2
int state = SHOW_VERSION;
unsigned long nextSwitch = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("BMP280_OLED_I2C v0.0.1 28.01.2018");
  myOLED.begin();
  bmp.begin(0x76);
  myOLED.setFont(RusFont);
  nextSwitch = millis()+3000;
}

void readBMP(){
  float currentTemp = bmp.readTemperature();
  float currentPressure = bmp.readPressure();
  
  #ifdef DEBUG_BMP
    Serial.print("Temperature = ");
    Serial.print(currentTemp);
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(currentPressure);
    Serial.println(" Pa");
    Serial.println();
  #endif
}

void showTemp(){
  myOLED.clrScr(); // очищаем экран
  myOLED.setFont(RusFont);
  myOLED.print("Ntvgthfnehf *C", CENTER, 3); // вывод текста русскими буквами
  myOLED.setFont(MegaNumbers);
  myOLED.print(String(bmp.readTemperature() , 1), CENTER, 15); // Отображение значения температуры, с точностью до десятых
  myOLED.update();
  if (millis()>nextSwitch){
    state = SHOW_PREASSURE;
    nextSwitch = millis()+5000;
  }
}

void showPreassure(){
  myOLED.clrScr(); // очищаем экран
  myOLED.setFont(RusFont);
  myOLED.print("Lfdktybt vv/hn/cn", CENTER, 3); // вывод текста русскими буквами
  P=bmp.readPressure();
  P=(float)0.0075*P; // пересчитываем давления в привычные нам мм ртутного столба.
  myOLED.setFont(MegaNumbers);
  myOLED.print(String(P , 1), CENTER, 15); // Отображение значения давления, с точностью до десятых
  myOLED.update();
  if (millis()>nextSwitch){
    state = SHOW_TEMP;
    nextSwitch = millis()+5000;
  }

}

void showVersion(){
  myOLED.clrScr(); // очищаем экран
  myOLED.setFont(RusFont);
  myOLED.print("DTHCBZ 12/03/2018", CENTER, 3); // вывод текста русскими буквами
  myOLED.setFont(RusFont);
  myOLED.print("PFUHEPRF LFYYS{", CENTER, 25); // Отображение значения высоты
  myOLED.update();  
  if (millis()>nextSwitch){
    state = SHOW_TEMP;
    nextSwitch = millis()+5000;
  }
}

void loop(){
  switch(state){
    case SHOW_TEMP:
      showTemp();
      break;
    case SHOW_PREASSURE:
      showPreassure();
      break;
    case SHOW_VERSION:
      showVersion();
      break;
    default:
      Serial.println("Error current state");
  }
//   readBMP(); //debug
  delay(DISPLAY_TIMEOUT);
}


