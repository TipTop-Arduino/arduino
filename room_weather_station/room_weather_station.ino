#include <SFE_BMP180.h>
#include <LCD_1602_RUS.h>
#include <DHT.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <Time.h>

LCD_1602_RUS lcd(0x27, 16, 2);
DHT dht(7,DHT11);
SFE_BMP180 sensor;
tmElements_t datetime;

#define DELAY 2500

double p0;
int count = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  if (sensor.begin())
  {
    Serial.println("Succesful start");
  }
  else {
  Serial.println("Something wrong");
  while(1);
  }
  p0 = pressure();
}

void loop() {
  float humid, p, temp180;
  
  p = pressure();
  temp180 = temperature();
  
  humid = dht.readHumidity();
  int ihumid = round(humid);
  
  p = p * 0.750062;
  lcd.setCursor(0,0);
  lcd.print(L"Температура: ");
  lcd.setCursor(9,1);
  lcd.print(temp180);
  lcd.print("C");
  delay(DELAY);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(L"Влажность: ");
  lcd.setCursor(11,1);
  lcd.print(ihumid);
  lcd.print("%");
  delay(DELAY);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(L"Давление: ");
  lcd.setCursor(7,1);
  lcd.print(p);
  lcd.print(L"мм");
  delay(DELAY);
  lcd.clear();

 for(int i = 0; i < 5; i++){
  count++;
  lcd.setCursor(4,0);
    if (RTC.read(datetime)) {
  print2(datetime.Hour, ":");
  print2(datetime.Minute, ":");
  print2(datetime.Second, " ");
  lcd.setCursor(3,1);
  print2(datetime.Day, "/");
  print2(datetime.Month, "/");
  print2(tmYearToCalendar(datetime.Year) ,"");
  if(count != 5){
  delay(1000);
  }
  else{
    count = 0;
    delay(500);
    }
  lcd.clear();
    }
  }  
}
double pressure()
{
  int status;
  double temp, pres;

  status = sensor.startTemperature();
  delay(status);
  sensor.getTemperature(temp);
  
  status = sensor.startPressure(3);
  delay(status);
  sensor.getPressure(pres,temp);
  
  return pres;
}

double temperature()
{
  int status;
  double temp;

  status = sensor.startTemperature();
  delay(status);
  sensor.getTemperature(temp);
  return temp;
}

void print2(int nn,String str) {
if (nn >= 0 && nn < 10)
{ 
  lcd.print("0");
}
  lcd.print(nn);
  lcd.print(str);
}
