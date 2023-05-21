#include <dfplayer.h>

dfplayer::dfplayer(HardwareSerial *new_hs) {
    this->hs = new_hs;
}

void dfplayer::setup() {
    hs->begin(9600);
}

void dfplayer::sendCommand(uint8_t command, uint16_t parameters, bool feedback) {
    uint8_t response[10];
    response[0] = 0x7E;
    response[1] = 0xFF;
    response[2] = 0x06;
    response[3] = command;
    response[4] = feedback;
    response[5] = (0xFF00 & parameters) >> 8;
    response[6] = 0x00FF & parameters;
    uint16_t checksum = 0xFFFF - (response[1] + response[2] + response[3] + response[4] + response[5] + response[6]) + 1;
    response[7] = (0xFF00 & checksum) >> 8;
    response[8] = 0x00FF & checksum;
    response[9] = 0xEF;

    Serial.print("Sending: ");
    for(int x = 0; x < 10; x++) {
        Serial.print(response[x], HEX);
        Serial.print(" ");
    }
    Serial.println();

    Serial2.write((char*)response, 10);
}

void dfplayer::playNextSound() {
    sendCommand(0x01, 0x0000, 0);
}

void dfplayer::playPrevSound() {
    sendCommand(0x02, 0x0000, 0);
}

void dfplayer::playSpecificSound(uint16_t num) {
    sendCommand(0x03, num, 0);
}

void dfplayer::reset() {
    sendCommand(0x0C, 0x0000, 0);
}

void dfplayer::pauseSound() {
    sendCommand(0x0E, 0x0000, 0);
}

void dfplayer::stopSound() {
    sendCommand(0x16, 0x0000, 0);
}