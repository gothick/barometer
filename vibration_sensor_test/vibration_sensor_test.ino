// Vibration sensor has two outputs, one analogue
// and one digital. I've connected these to A3 and
// D3.

int analogPin = A3;
int digitalPin = 3;
int ledPin = 2;
volatile bool tapped = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("Starting test."));
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(digitalPin), tap, RISING);
}

int highest = 0;

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(analogPin);  // read the input pin
  if (val > highest) {
    highest = val;
  }
  Serial.print(val);
  Serial.print(F(" Highest: "));
  Serial.print(highest);
  Serial.print(F(" Tapped: "));
  Serial.println(tapped ? F("Yes") : F("No"));

  if (tapped) {
    onTap();
    tapped = false;
  }
}

void onTap() {
  for (int i=0; i<10; i++) {
      Serial.println("On");
      digitalWrite(ledPin, HIGH);
      delay(100);
      Serial.println("Off");
      digitalWrite(ledPin, LOW);
      delay(100);
  }  
}

void tap() {
  tapped = true;
}
