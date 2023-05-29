#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef DFPLAYER_CORE_H
#define DFPLAYER_CORE_H

extern uint8_t result[16];

void sendCommand(uint8_t command, uint16_t parameters, bool feedback);

enum EQ {
    Normal = 0,
    Pop = 1,
    Rock = 2,
    Jazz = 3,
    Classic = 4,
    Bass = 5
};

enum Device {
    USB = 1,
    SD = 2
};

void dfp_update(uint8_t resp, uint8_t* message_buffer, void (*respHand) (uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param));                                     // Check responses
uint8_t* getResult();

void playNext();                                                // 01
void playPrev();                                                // 02
void specifyTrack(uint16_t num);                                // 03
void increaseVolume();                                          // 04
void decreaseVolume();                                          // 05
void specifyVolume(uint16_t vol);                               // 06
void specifyEQ(uint8_t eq);                                     // 07
void specifySingleRepeat(uint16_t num);                         // 08
void specifyDevice(uint8_t device);                             // 09
void sleep();                                                   // 0A
// Reserved                                                     // 0B
void reset();                                                   // 0C
void play();                                                    // 0D
void pause();                                                   // 0E
void specifyTackInFolder(uint8_t folder, uint8_t num);          // 0F
void setAudioAmp(bool on, uint8_t gain);                        // 10
void setAllRepeatPlayback(bool on);                             // 11
void specifyTrackInMP3(uint16_t num);                           // 12
void insetAd(uint16_t num);                                     // 13
void specifyTackInBigFolder(uint8_t folder, uint16_t num);      // 14
void stopAd();                                                  // 15
void stop();                                                    // 16
void repeatSpecificFolder(uint8_t folder);                      // 17
void setRandom();                                               // 18
void repeatCurrent(bool on);                                    // 19
void setDAC(bool on);                                           // 1A

#endif