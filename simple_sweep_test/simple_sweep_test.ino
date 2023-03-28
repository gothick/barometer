/* #include <SwitecX12.h> */
#include <SwitecX25.h>

/*
*/


SwitecX25 motor1(315*3, 6,4,5,7);
int pot = A0;

#define NUM_READINGS 20

int readings[NUM_READINGS];
int read_pos = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Go!");

  motor1.zero();   // this is a slow, blocking operation
  motor1.setPosition(470);

}

// the loop function runs over and over again forever
void loop() {
  motor1.setPosition(940);
  motor1.updateBlocking();
  motor1.setPosition(5);
  motor1.updateBlocking();  
}
