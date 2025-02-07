/*
Készítette: Sóti Kristóf, 2025. 02. 07.

Források:
- https://www.youtube.com/watch?v=AvSCAxbqvvE&t=761s
- https://www.youtube.com/watch?v=vJct0vD6WWg
- https://github.com/adafruit/Adafruit_BMP280_Library
*/


//!!Személyi okok miatt néhány adatot üresen hagytam.!!
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial
#define uv_sensor A0

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;

//Elküldi a mért adatokat a Blynk applikációba 20 másodpercenként.
void send_data() {
  read_bmp280();
  read_uv();
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  bmp.begin(0x76);

  pinMode(uv_sensor, INPUT);

  timer.setInterval(20 * 1000L, send_data);

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  Blynk.run();
  timer.run();
}

//Ki olvassa a hőmérsékletet, légnyomást és a tengerszint feletti magasságot a bmp280-as szenzor segítségével.
void read_bmp280() {
  float temperature = bmp.readTemperature();
  int pressure = bmp.readPressure();
  int altitude = bmp.readAltitude(1028);  

  //Itt küldi el az adatokat a megfelelő virtuális portokra.
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, pressure);
  Blynk.virtualWrite(V2, altitude);

  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print(F("Pressure: "));
  Serial.print(pressure);
  Serial.println(" Pa");
  Serial.print(F("Approximate altitude: "));
  Serial.print(altitude);
  Serial.println(" m");
}

//Ki olvassa az ultraibolya sugárzás értékét a GUVA-S12SD szenzor segítségével.
void read_uv() {
  float uv_index = (analogRead(uv_sensor)) * (5.0 / 1023.0);

  Blynk.virtualWrite(V3, uv_index);

  Serial.print(F("UV index: "));
  Serial.println(uv_index) / .1;
  Serial.println();
}