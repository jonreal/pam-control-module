#include "Pam.h"

// Create global PAM (pneumatic artificial muscle) controller
Pam Pam(0x77); // argument is i2c address

// Other globals
long t = 0;  // loop counter
float p[4];  // array of pressures (one for each channel)

void setup() {

  Serial.begin(9600);

  // Init PAM
  Pam.begin();

  // Set valve pins for each PAM channel
  Pam.setupChannel(0, 2, 3); // arguments: ch, a_pin, b_pin
  Pam.setupChannel(1, 4, 5);
  Pam.setupChannel(2, 6, 7);
  Pam.setupChannel(3, 8, 9);
}

void loop() {

  // Possible valve commands (1 == high pressure, -1 == exhaust, 0 == closed)
  int u[3] = {1, -1, 0};

  // loop through all channels and commands
  for (int ch=0; ch<3; ch++) {
    for (int i=0; i<3; i++) {
      Pam.setValves(ch,u[i]);
      delay(100);
    }
    p[ch] = Pam.getPressure(ch); // sample each channels pressure
  }

  Serial.print("Loop ");
  Serial.println(t++);
  Serial.print("p0 = ");
  Serial.print(p[0],2);
  Serial.print("\tp1 = ");
  Serial.print(p[1],2);
  Serial.print("\tp2 = ");
  Serial.print(p[2],2);
  Serial.print("\tp3 = ");
  Serial.println(p[3],2);
  Serial.println();
}
