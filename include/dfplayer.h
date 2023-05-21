#include "Arduino.h"

#ifndef DFPLAYER_H
#define DFPLAYER_H

class dfplayer {
  private:
    HardwareSerial *hs;
    void sendCommand(uint8_t command, uint16_t parameters, bool feedback);

  public:
    dfplayer (HardwareSerial *new_hs);
    void setup();
    void playNextSound();
    void playPrevSound();
    void playSpecificSound(uint16_t num);
    void reset();
    void pauseSound();
    void stopSound();

};

#endif