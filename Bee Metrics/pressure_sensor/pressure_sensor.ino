#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>


Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
int currentTime;
int previouseTime = 0;

void setup(void)
{
  Serial.begin(9600);

  /* Initialise the sensor */
  if (!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
}

void loop(void)
{
  currentTime = millis();

  /* Get a new sensor event */
  sensors_event_t event;
  bmp.getEvent(&event);

  if (currentTime - previouseTime >= 2000) {
    previouseTime = currentTime;
    /* Display the results (barometric pressure is measure in hPa) */
    if (event.pressure)
    {
      /* Display atmospheric pressue in hPa */
      Serial.print("Pressure:    ");
      Serial.print(event.pressure);
      Serial.println(" hPa");

      float temperature;
      bmp.getTemperature(&temperature);
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");
    }
    else
    {
      Serial.println("Sensor error");
    }
  }
}
