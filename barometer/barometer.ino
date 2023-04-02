#include <Adafruit_BMP280.h>
#include <SwitecX25.h>

// Error blink codes
#define ERR_BAROMETER 7

// Stepper steps for zero, 126 and 252 degrees, 
// corresponding to the minimum, middle and 
// max barometer readings (25.7mmHg, 29.5mmHg
// and 31.5mmHg.)
#define STEPPER_ZERO 0
#define STEPPER_CENTRE 378
#define STEPPER_FULL 756 

// In Pascals: 25.7mmHg, 29.5mmHg and 31.5mmHg. 
#define PRESSURE_MIN 93126
#define PRESSURE_CENTRE 99899
#define PRESSURE_FULL 106671

SwitecX25 stepper(315*3, 6,4,5,7);
Adafruit_BMP280 bmp;
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

int tapSensorPin = 1;
volatile unsigned int taps = 0;
volatile unsigned long lastTap = 0;

void setup() {
  attachInterrupt(digitalPinToInterrupt(tapSensorPin), tap, RISING);
  unsigned status = bmp.begin();
  if (!status) {
    error(ERR_BAROMETER);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                Adafruit_BMP280::SAMPLING_X16,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  delay(500);
  calibrate();
}

void tap() {
  lastTap = millis();
  taps++;
}

unsigned long lasttime = 0;
void loop() {
  if ((taps == 1) && (millis() - lastTap > 5000)) {
    taps = 0;
  }
  if (taps >= 2) {
    delay(250);
    calibrate();
    taps = 0;
  }
  stepper.update();

  if (millis() - lasttime > 5000) {
    sensors_event_t pressure_event;
    bmp_pressure->getEvent(&pressure_event);
    // pressure is in hPa, we convert to full Pascals to get
    // the integer math in map working with decent precision.
    long pressure = pressure_event.pressure * 100.0;

    long stepper_pos = map(
      pressure, 
      PRESSURE_MIN, PRESSURE_FULL, 
      STEPPER_ZERO, STEPPER_FULL
    );
    stepper.setPosition(stepper_pos);
    lasttime = millis();
  }
}

void calibrate() {
  stepper.zero(); // The only calibration we actually *need*; the rest is just for show, really.
  delay(500);
  stepper.setPosition(STEPPER_FULL);
  stepper.updateBlocking();
  delay(500);
  stepper.setPosition(STEPPER_ZERO);
  stepper.updateBlocking();
  delay(500);
  stepper.setPosition(STEPPER_CENTRE);
  stepper.updateBlocking();
  delay(500);
}

void error(unsigned int blinks) {
  for (int i=0; i<10; i++) {

    for (int j=0; j<blinks; j++) {
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(250);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      delay(250);                      // wait for a second
    }
    delay(1000);
  }
}
