#include "Pam.h"

// Create global PAM (pneumatic artificial muscle) controller
Pam Pam(0x77); // argument is i2c address

// Other globals
long tnow = 0;  // loop counter
long tprev = 0;
long tprev_cnt = 0;
float p[4];  // array of pressures (one for each channel)

float p0 = 50.0;
float dp = 0.0;
float e1 = 0.0;
float e2 = 0.0;
float pd1 = p0 + dp;
float pd2 = p0 - dp;

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
    p[1] = Pam.getPressure(1);
    Pam.thresholdControl(0, pd1, p[0], 1.0);
    Pam.thresholdControl(1, pd2, p[1], 1.0);
  }
  Pam.setValves(0,0);
  Pam.setValves(1,0);
  dp = 0;
}

void loop() {
  if ( ((tnow = millis()) - tprev) < 1 )
    return;

  p[0] = Pam.getPressure(0);
  p[1] = Pam.getPressure(1);

  e1 = pd1 - p[0];
  e2 = pd2 - p[1];

  if ((e1 > e2) && (e1 > 0.01))
    dp = dp - e1;
  else if ((e2 > e1) && (e2 > 0.01))
    dp = dp + e2;

  if (dp > p0)
    dp = p0;
  else if (dp < -p0)
    dp = -p0;

  pd1 = p0 + dp;
  pd2 = p0 - dp;

  Pam.thresholdControl(0, pd1, p[0], 1);
  Pam.thresholdControl(1, pd2, p[1], 1);

 // dp_prev = dp;

  // Find average difference
 // pdiff = (p0 - p[0]) + (p0 - p[1])

//  Serial.print(tnow);
//  Serial.print("\t");
  Serial.print(p[0]);
  Serial.print("\t");
  Serial.print(p[1]);
  Serial.print("\t");
  Serial.print(e1);
  Serial.print("\t");
  Serial.print(e2);
  Serial.print("\t");
  Serial.print(pd1);
  Serial.print("\t");
  Serial.print(pd2);
  Serial.print("\t");
  Serial.print(dp);
  Serial.print("\n");

  tprev = tnow;
}

