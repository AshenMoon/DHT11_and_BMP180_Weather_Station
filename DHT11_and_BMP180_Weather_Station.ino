// DHT11 and BMP180 Temperature, Humidity and Barometric Pressure Sensors Weather Station
////////////////////////////////
#include <Wire.h>
#include <LiquidCrystal_I2C.h>       // Include library
//#include <LiquidCrystal_PCF8574.h> // Include alternative library if necessary
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_BMP085.h>

#define seaLevelPressure_hPa 1013.25

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

//LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set I2C address (common value), and screen parameters (for LCD 1602 - 2 lines 16 symbol each
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Set I2C address (common value), and screen parameters (for LCD 2004 - 4 lines 20 symbol each
//LiquidCrystal_PCF8574 lcd(0x27);   // Variant for the PCF8574 library

#define DHT11_PIN 2

DHT_Unified dht(DHT11_PIN, DHTTYPE);

Adafruit_BMP085 bmp;

uint32_t delayMS;

bool bmpReady = false;

// Define your elevation here
const float elevation = 51.0f; // Elevation value for Petah Tikva, Israel

void setup()
{
  lcd.init();                        // Init display
  lcd.backlight();                   // Turn backlite on

  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  // Check Barometric Pressure Sensor
  Serial.println(F("Barometric Pressure Sensor"));
  bmpReady = bmp.begin();
  if (!bmpReady)
  {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  else
  {
     Serial.println("BMP085 sensor ready");
  }
  Serial.println(F("------------------------------------"));
}

void loop()
{
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) 
  {
    Serial.println(F("Error reading temperature!"));
  }
  else 
  {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    lcd.setCursor(0, 0);
    lcd.print("TEMPERATURE:");
    lcd.print(event.temperature, 1);
    lcd.print((char)223);
    lcd.print("C ");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
  {
    Serial.println(F("Error reading humidity!"));
  }
  else 
  {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    lcd.setCursor(0, 1);
    lcd.print("HUMIDITY:   ");
    lcd.print(event.relative_humidity, 1);
    lcd.print("%  ");
  }

  if(bmpReady)
  {
    //int32_t pressure = bmp.readPressure(); 
    float pressure = (float)bmp.readPressure() / 100.0f; 
    int32_t pressureMmHg = (int32_t)(pressure * 0.750061505 + 0.5);
    float pressureSeaLevel = (float)bmp.readSealevelPressure(elevation) / 100.0f; 
    int32_t pressureSeaLevelMmHg = (int32_t)(pressureSeaLevel * 0.750061505 + 0.5);
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" hPa");
    Serial.print("Pressure = ");
    Serial.print(pressureMmHg);
    Serial.println(" mmHg");
    Serial.print("Pressure at Sea Level = ");
    Serial.print(pressureSeaLevel);
    Serial.println(" hPa");
    Serial.print("Pressure at Sea Level = ");
    Serial.print(pressureSeaLevelMmHg);
    Serial.println(" mmHg");
    lcd.setCursor(0, 2);
    lcd.print("PRESSURE:   ");
    lcd.print(pressureMmHg);
    lcd.print(" mmHg");
    lcd.setCursor(0, 3);
    lcd.print("SEA LEVEL:  ");
    lcd.print(pressureSeaLevelMmHg);
    lcd.print(" mmHg");
  }
}
