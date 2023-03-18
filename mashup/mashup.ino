#include <Adafruit_BMP280.h>
#include <SwitecX25.h>

SwitecX25 motor1(315*3, 4,5,6,7);

Adafruit_BMP280 bmp;
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void setup() {
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  
  motor1.zero();   // this is a slow, blocking operation

  Serial.println(F("BMP280 Sensor event test"));

  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
  /* Default settings from datasheet. */
  // bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  /* As madly responsive as possible, please */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X16,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_125); /* Standby time. */

  bmp_temp->printSensorDetails();  
  bmp_pressure->printSensorDetails();
}

unsigned long lasttime = 0;

void loop() {
  motor1.update();

  if (millis() - lasttime > 0) {
    // put your main code here, to run repeatedly:
    sensors_event_t temp_event, pressure_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);
    
    Serial.print(F("Temperature = "));
    Serial.print(temp_event.temperature);
    Serial.println(" *C");

    // Serial.print(F("Pressure = "));
    // Serial.print(pressure_event.pressure);
    // Serial.println(" hPa");

    // Serial.println();
    
    int motor_pos = map(temp_event.temperature * 100, 2000, 2900, 5, 940);
    motor1.setPosition(motor_pos);
    // Serial.print("Position: ");
    // Serial.println(motor_pos);

    lasttime = millis();
  }
}
