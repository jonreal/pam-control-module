#ifndef _PAM_H_
#define _PAM_H_

#include "Arduino.h"

#define SENSOR_I2C_ADDY 0x28

typedef struct {
  int a_sol;
  int b_sol;
} pam_channel_t;


class Pam
{
  public:
    Pam(char i2c_address);
    void begin(void);
    void setupChannel(char chan, int a_valve, int b_valve);
    void setValves(char chan, int u);
    float getPressure(char chan);
    void debugBus(void);
    unsigned char readBus(void);
    void thresholdControl(char chan, float pd, float pm, float thrshld);

  private:
    void setI2cBusChannel(char chan);
    pam_channel_t _pams[4];
    char _address;
};

#endif
