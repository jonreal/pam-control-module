#include "Pam.h"

// Create global PAM (pneumatic artificial muscle) controller
Pam Pam(0x77); // argument is i2c address

// Other globals
long tnow = 0;  // loop counter
long tprev = 0;
long tprev_cnt = 0;
float p[4];  // array of pressures (one for each channel)

float p0 = 25.0;
float dpmax = 5;
float dp = 0.0;

void setup() {

  Serial.begin(9600);

  // Init PAM
  Pam.begin();

  // Set valve pins for each PAM channel
  Pam.setupChannel(0, 2, 3); // arguments: ch, a_pin, b_pin
  Pam.setupChannel(1, 4, 5);
  Pam.setupChannel(2, 6, 7);
  Pam.setupChannel(3, 8, 9);

  Serial.println("\n\n\n----------------------------------------\n");

  tnow = 0;
  while ((tnow = millis()) < 2000) {
    p[0] = Pam.getPressure(0);
    Pam.thresholdControl(0, p0, p[0], 1.0);
    p[1] = Pam.getPressure(1);
    Pam.thresholdControl(1, p0, p[1], 1.0);
  }
  Pam.setValves(0,0);
}

void loop() {
  if ( ((tnow = millis()) - tprev) < 1 )
    return;

  p[0] = Pam.getPressure(0);
  p[1] = Pam.getPressure(1);

  //e = p0 - p[0];

  char u1, u2;
  if (p[0] > ((p0 + dp) + dpmax)) {
    u1 = -1;
    dp = dp - 1.0;
  } else if (p[0] < (p0 + dp)) {
    u1 = 1;
  } else {
    u1 = 0;
  }

  if (p[1] > ((p0 - dp) + dpmax)) {
    u2 = -1;
    dp = dp + 1.0;
  } else if (p[1] < (p0 - dp)) {
    u2 = 1;
  } else {
    u2 = 0;
  }

  if (dp > p0)
    dp = p0;
  else if (dp < -p0)
    dp = -p0;

  Pam.setValves(0,u1);
  Pam.setValves(1,u2);


  Serial.print(p[0]);
  Serial.print("\t");
  Serial.print(p0 + dp);
  Serial.print("\t");
  Serial.print((p0 + dp) + dpmax);
  Serial.print("\t");
  Serial.print(p[1]);
  Serial.print("\t");
  Serial.print(p0 - dp);
  Serial.print("\t");
  Serial.print((p0 - dp) + dpmax);
  Serial.print("\n");

  tprev = tnow;
}

