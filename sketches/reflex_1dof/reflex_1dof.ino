#include "Pam.h"

// Create global PAM (pneumatic artificial muscle) controller
Pam Pam(0x77); // argument is i2c address

// Other globals
long tnow = 0;  // loop counter
long tprev = 0;
long tprev_cnt = 0;
float p[4];  // array of pressures (one for each channel)

float p0 = 50.0;
float dpmax = 5.0;
float error = 0.0;

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
  }
  Pam.setValves(0,0);
}

void loop() {
  if ( ((tnow = millis()) - tprev) < 1 )
    return;

  p[0] = Pam.getPressure(0);

  //e = p0 - p[0];

  char u;
  if (p[0] < p0) {
    u = 1;
  } else if (p[0] > (p0 + dpmax)) {
    u = -1;
    p0 = p0 + 1;
  } else if ((p[0] >= p0) && (p[0] <= (p[0] + dpmax))) {
    u = 0;
  }

  Pam.setValves(0,u);


  Serial.print(p[0]);
  Serial.print("\t");
  Serial.print(p0);
  Serial.print("\t");
  Serial.print(p0 + dpmax);
  Serial.print("\n");

  tprev = tnow;
}

