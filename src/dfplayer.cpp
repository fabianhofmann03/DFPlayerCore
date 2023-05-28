#include <dfplayer.h>

dfplayer::dfplayer(HardwareSerial *new_hs) {
    this->hs = new_hs;
}

/**
 * Starts the Serial port of the dfplayer object.
 */
void dfplayer::setup() {
    hs->begin(9600);
}

/**
 * Set a function that will be called every time the module sends a response. The function will not be called if the checksum is incorrect.
 * 
 * @param ResponseHandler The function that will be called when a response is read. It must be a void and have following parameters: uint8_t version, uint8_t length, uint8_t command, uint8_t feedback, uint16_t parameters.
*/
void dfplayer::setResponseHandler(void (*respHand) (uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param)) {
    this->respHand = respHand;
}

/**
 * Remove the response handler.
*/
void dfplayer::removeResponseHandler() {
    this->respHand = NULL;
}

/**
 * Set the default response handler. The default response handler analyzes the command and parameters and prints a description in the Serial port.
*/
void dfplayer::setDefaultResponseHandler() {
    this->respHand = &(dfplayer::printResponse);
}

/**
 * Reads the responses from the module. If you don't add this function the response handler won't be called. If this function is used, DO NOT read the serial port you set for the module.
*/
void dfplayer::update() {
    static uint8_t* message_buffer = new uint8_t[16];
    static uint8_t buffer_pos = 0;
    static bool command_started = false;

    while (hs->available()) {
        uint8_t data = (*hs).read();
        if (respHand != NULL) {
            switch (data) {
                case 0x7E:
                    {
                        command_started = true;
                        buffer_pos = 0;
                        break;
                    }
                case 0xEF:
                    {
                        command_started = false;

                        uint8_t vers = *(message_buffer);
                        uint8_t len = *(message_buffer + 1);
                        uint8_t cmd = *(message_buffer + 2);
                        uint8_t feedb = *(message_buffer + 3);
                        uint8_t param_msb = *(message_buffer + 4);
                        uint8_t param_lsb = *(message_buffer + 5);
                        uint16_t param = (param_msb << 8) | param_lsb;

                        uint8_t check_sent_msb = *(message_buffer + 6);
                        uint8_t check_sent_lsb = *(message_buffer + 7);
                        uint16_t check_sent = (check_sent_msb << 8) | check_sent_lsb;

                        uint16_t check_calc = 0xFFFF - (vers + len + cmd + feedb + param_msb + param_lsb) + 1;

                        if (check_sent == check_calc) {
                            respHand (vers, len, cmd, feedb, param);
                        }
                        break;
                    }
                default:
                    if(command_started) {
                        *(message_buffer + buffer_pos) = data;
                        buffer_pos++;
                    }
                    break;
            }
        }
    }
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

    (*hs).write((char*)response, 10);
}

/**
 * Starts playing the next file.
 */
void dfplayer::playNext() {
    sendCommand(0x01, 0x0000, 0);
}

/**
 * Starts playing the previouse file.
 */
void dfplayer::playPrev() {
    sendCommand(0x02, 0x0000, 0);
}

/**
 * Starts playing the specified track from the root directory.
 * 
 * @param Number Number of the track that should be played. Must be between 1 and 3000.
 */
void dfplayer::specifyTrack(uint16_t num) {
    sendCommand(0x03, num, 0);
}

/**
 * Increases the volume.
 */
void dfplayer::increaseVolume() {
    sendCommand(0x04, 0x0000, 0);
}

/**
 * Decreases the volume.
 */
void dfplayer::decreaseVolume() {
    sendCommand(0x05, 0x0000, 0);
}

/**
 * Sets a specific volume.
 * 
 * @param Volume Volume with which the audio should be played back. 
 */
void dfplayer::specifyVolume(uint16_t vol) {
    sendCommand(0x06, vol, 0);
}

/**
 * Sets specific equalizer settings.
 * 
 * @param Equalizer Equalizer settings that should be used. Usable settings are Normal, Pop, Rock, Jazz, Classic, Bass.
 * @example player.specifyEQ(player.Bass);
 */
void dfplayer::specifyEQ(uint8_t eq) {
    sendCommand(0x07, eq, 0);
}

/**
 * Starts playing a specific song in loop.
 * 
 * @param Number Number of the song that should be played in loop.
 */
void dfplayer::specifySingleRepeat(uint16_t num) {
    sendCommand(0x08, num, 0);
}

/**
 * Selects a device to play music from.
 * 
 * @param Device Device that should be used. Usable settings are SD and USB.
 * @example player.specifyDevice(player.USB);
 */
void dfplayer::specifyDevice(uint8_t device) {
    sendCommand(0x09, device, 0);
}

/**
 * Puts module in sleep mode.
 */
void dfplayer::sleep() {
    sendCommand(0x0A, 0x0000, 0);
}

/**
 * Resets ... something?
 */
void dfplayer::reset() {
    sendCommand(0x0C, 0x0000, 0);
}

/**
 * Plays the current song.
 */
void dfplayer::play() {
    sendCommand(0x0D, 0x0000, 0);
}

/**
 * Pauses the current song.
 */
void dfplayer::pause() {
    sendCommand(0x0E, 0x0000, 0);
}

/**
 * Plays a song in a certain folder.
 * 
 * @param Folder The folder number where the song is contained. Can range from 1 to 99.
 * @param Number The number of the song in the selected folder. Can range from 1 to 255.
 */
void dfplayer::specifyTackInFolder(uint8_t folder, uint8_t num) {
    sendCommand(0x0F, (folder << 8) & num , 0);
}

/**
 * Sets the audio amplification settings.
 * 
 * @param On If the amplification should be turned on or not.
 * @param Gain For the gains 💪. Can range from 0 to 31.
 */
void dfplayer::setAudioAmp(bool on, uint8_t gain) {
    sendCommand(0x10, (on << 8) & gain , 0);
}

void dfplayer::setAudioAmp(bool on) {
    sendCommand(0x10, (on << 8), 0);
}

/**
 * Sets if all the songs in the root directory should repeat. Aka once the last song is finished the first starts again.
 * 
 * @param On If the root directory should repeat.
 */
void dfplayer::setAllRepeatPlayback(bool on) {
    sendCommand(0x11, on, 0);
}

/**
 * Plays specific audio file in the MP3 directory. The MP3 directory allows 65536 audios to be stored, but its reccommended to store a max of 3000 tracks.
 * 
 * @param Number Number of the sound in the MP3 directory that should be played. Can range from 1 to 65536.
 */
void dfplayer::specifyTrackInMP3(uint16_t num) {
    sendCommand(0x12, num, 0);
}

/**
 * Pauses the current track and starts playing the selected track from the ADVERT folder. Once it's finished, it continues with the paused song.
 * 
 * @param Number Number of the sound in the ADVERT directory that should be played. Can range from 1 to 65536 ... I think.
 */
void dfplayer::insetAd(uint16_t num) {
    sendCommand(0x13, num, 0);
}

/**
 * Plays a specific song from a bigger folder. Allows to have fewer folders with more tracks.
 *
 * @param Folder The big-folder number where the song is contained. Can range from 1 to 15.
 * @param Number The number of the song in the selected big-folder. Can range from 1 to 3000.
 */
void dfplayer::specifyTackInBigFolder(uint8_t folder, uint16_t num) {
    sendCommand(0x14, ((folder & 0xF) << 12) | (num & 0xFFF), 0);
}

/**
 * Stops the track from the ADVERT folder and continues with the paused track.
 */
void dfplayer::stopAd() {
    sendCommand(0x15, 0x0000, 0);
}

/**
 * Stops all playback and stops decoding the audio files.
 */
void dfplayer::stop() {
    sendCommand(0x16, 0x0000, 0);
}

/**
 * Repeats all songs in a certain folder. Aka once the last song is finished the first starts again.
 * 
 * @param Folder The folder number where the tracks are contained.
 */
void dfplayer::repeatSpecificFolder(uint8_t folder) {
    sendCommand(0x17, folder, 0);
}

/**
 * I don't get the explaination from the datasheet, but here it is: This command is used to randomly play sound files in the storage device according to physical sequence and no matter if there is a folder or not in the device. The first sound file that is conducted to be played is the first one in the device.
 */
void dfplayer::setRandom() {
    sendCommand(0x18, 0x0000, 0);
}

/**
 * Repeats the track thats currently playing.
 * 
 * @param On If the repeat-mode should be on.
 */
void dfplayer::repeatCurrent(bool on) {
    sendCommand(0x19, !on, 0);
}

/**
 * Turns the DAC (Digital-Analog-Converter) on or off.
 * 
 * @param On If the DAC should be on.
 */
void dfplayer::setDAC(bool on) {
    sendCommand(0x1A, !on, 0);
}

void dfplayer::printResponse(uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param) {
  switch (cmd) {
    case 0x3F:
      Serial.print("Module turned on. ");
      switch(param) {
        case 1:
          Serial.println("USB flash drive online.");
          break;
        case 2:
          Serial.println("SD card online");
          break;
        case 3:
          Serial.println("SD card and USB flash drive online.");
          break;
        case 4:
          Serial.println("PC online.");
          break;
      }
      break;
    
    case 0x3D:
      Serial.println("Track number " + String(param) + " finished playing from the SD card.");
      break;
    case 0x3C:
      Serial.println("Track number " + String(param) + " finished playing from the USB flash drive.");
      break;
    
    case 0x41:
      Serial.println("Acknowledge returned.");
      break;

    case 0x40:
      Serial.print("ERROR: ");
      switch(param) {
        case 1:
          Serial.println("Module is still initializing.");
          break;
        case 2:
          Serial.println("Module is in sleep mode.");
          break;
        case 3:
          Serial.println("A frame has not been received completly yet.");
          break;
        case 4:
          Serial.println("The checksum is incorrect.");
          break;
        case 5:
          Serial.println("The specified track is outside the current track scope.");
          break;
        case 6:
          Serial.println("The specified track was not found.");
          break;
        case 7:
          Serial.println("Insertion error.");
          break;
        case 8:
          Serial.println("SD card reading failed");
          break;
        case 10:
          Serial.println("Entered into sleepmode.");
      }
      break;

    case 0x3A:
      switch(param) {
        case 1:
          Serial.print("USB flash drive ");
          break;
        case 2:
          Serial.print("SD card ");
          break;
        case 3:
          Serial.print("USB cable connected to PC ");
      }
      Serial.println(" is plugged in.");
      break;
    case 0x3B:
      switch(param) {
        case 1:
          Serial.print("USB flash drive ");
          break;
        case 2:
          Serial.print("SD card ");
          break;
        case 3:
          Serial.print("USB cable connected to PC ");
      }
      Serial.println("is pulled out.");
  }
}