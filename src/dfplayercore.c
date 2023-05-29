#include "dfplayercore.h"

uint8_t result[16];

void sendCommand(uint8_t command, uint16_t parameters, bool feedback) {                                               // Make the 
    result[1] = 0x7E;
    result[2] = 0xFF;
    result[3] = 0x06;
    result[4] = command;
    result[5] = feedback;
    result[6] = (0xFF00 & parameters) >> 8;
    result[7] = 0x00FF & parameters;
    uint16_t checksum = 0xFFFF - (result[2] + result[3] + result[4] + result[5] + result[6] + result[7]) + 1;
    result[8] = (0xFF00 & checksum) >> 8;
    result[9] = 0x00FF & checksum;
    result[10] = 0xEF;

    result[0] = 10;
}

/**
 * Updates responses
*/
/*
Message buffer:
    First byte: command_started
    Second byte: buffer_pos
*/
void dfp_update(uint8_t resp, uint8_t* message_buffer, void (*respHand) (uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param)) {
    if (respHand != NULL) {
        switch (resp) {
            case 0x7E:
                {
                    *message_buffer = true;                                         // command_started
                    *(message_buffer + 1) = 0;                                      // buffer_pos
                    break;
                }
            case 0xEF:
                {
                    *message_buffer = false;                                        // command_started

                    uint8_t vers = *(message_buffer + 2);
                    uint8_t len = *(message_buffer + 3);
                    uint8_t cmd = *(message_buffer + 4);
                    uint8_t feedb = *(message_buffer + 5);
                    uint8_t param_msb = *(message_buffer + 6);
                    uint8_t param_lsb = *(message_buffer + 7);
                    uint16_t param = (param_msb << 8) | param_lsb;

                    uint8_t check_sent_msb = *(message_buffer + 8);
                    uint8_t check_sent_lsb = *(message_buffer + 9);
                    uint16_t check_sent = (check_sent_msb << 8) | check_sent_lsb;

                    uint16_t check_calc = 0xFFFF - (vers + len + cmd + feedb + param_msb + param_lsb) + 1;

                    if (check_sent == check_calc) {
                        respHand (vers, len, cmd, feedb, param);
                    }
                    break;
                }
            default:
                if(*message_buffer) {                                               // command_started
                    *(message_buffer + *(message_buffer + 1) + 2) = resp;           // buffer_pos
                    (*(message_buffer + 1))++;                                      // buffer_pos
                }
                break;
        }
    }
}

uint8_t* getResult() {
    return result;
}

/**
 * Starts playing the next file.
 */
void playNext() {
    sendCommand(0x01, 0x0000, 0);
}

/**
 * Starts playing the previouse file.
 */
void playPrev() {
    sendCommand(0x02, 0x0000, 0);
}

/**
 * Starts playing the specified track from the root directory.
 * 
 * @param num Number of the track that should be played. Must be between 1 and 3000.
 */
void specifyTrack(uint16_t num) {
    sendCommand(0x03, num, 0);
}

/**
 * Increases the volume.
 */
void increaseVolume() {
    sendCommand(0x04, 0x0000, 0);
}

/**
 * Decreases the volume.
 */
void decreaseVolume() {
    sendCommand(0x05, 0x0000, 0);
}

/**
 * Sets a specific volume.
 * 
 * @param vol Volume with which the audio should be played back. 
 */
void specifyVolume(uint16_t vol) {
    sendCommand(0x06, vol, 0);
}

/**
 * Sets specific equalizer settings.
 * 
 * @param eq Equalizer settings that should be used. Usable settings are Normal, Pop, Rock, Jazz, Classic, Bass.
 * @example player.specifyEQ(player.Bass);
 */
void specifyEQ(uint8_t eq) {
    sendCommand(0x07, eq, 0);
}

/**
 * Starts playing a specific song in loop.
 * 
 * @param num Number of the song that should be played in loop.
 */
void specifySingleRepeat(uint16_t num) {
    sendCommand(0x08, num, 0);
}

/**
 * Selects a device to play music from.
 * 
 * @param device Device that should be used. Usable settings are SD and USB.
 * @example player.specifyDevice(player.USB);
 */
void specifyDevice(uint8_t device) {
    sendCommand(0x09, device, 0);
}

/**
 * Puts module in sleep mode.
 */
void sleep() {
    sendCommand(0x0A, 0x0000, 0);
}

/**
 * Resets ... something?
 */
void reset() {
    sendCommand(0x0C, 0x0000, 0);
}

/**
 * Plays the current song.
 */
void play() {
    sendCommand(0x0D, 0x0000, 0);
}

/**
 * Pauses the current song.
 */
void pause() {
    sendCommand(0x0E, 0x0000, 0);
}

/**
 * Plays a song in a certain folder.
 * 
 * @param folder The folder number where the song is contained. Can range from 1 to 99.
 * @param num The number of the song in the selected folder. Can range from 1 to 255.
 */
void specifyTackInFolder(uint8_t folder, uint8_t num) {
    sendCommand(0x0F, (folder << 8) & num , 0);
}

/**
 * Sets the audio amplification settings.
 * 
 * @param on If the amplification should be turned on or not.
 * @param gain For the gains 💪. Can range from 0 to 31.
 */
void setAudioAmp(bool on, uint8_t gain) {
    sendCommand(0x10, (on << 8) & gain , 0);
}

/**
 * Sets if all the songs in the root directory should repeat. Aka once the last song is finished the first starts again.
 * 
 * @param on If the root directory should repeat.
 */
void setAllRepeatPlayback(bool on) {
    sendCommand(0x11, on, 0);
}

/**
 * Plays specific audio file in the MP3 directory. The MP3 directory allows 65536 audios to be stored, but its reccommended to store a max of 3000 tracks.
 * 
 * @param num Number of the sound in the MP3 directory that should be played. Can range from 1 to 65536.
 */
void specifyTrackInMP3(uint16_t num) {
    sendCommand(0x12, num, 0);
}

/**
 * Pauses the current track and starts playing the selected track from the ADVERT folder. Once it's finished, it continues with the paused song.
 * 
 * @param num Number of the sound in the ADVERT directory that should be played. Can range from 1 to 65536 ... I think.
 */
void insetAd(uint16_t num) {
    sendCommand(0x13, num, 0);
}

/**
 * Plays a specific song from a bigger folder. Allows to have fewer folders with more tracks.
 *
 * @param folder The big-folder number where the song is contained. Can range from 1 to 15.
 * @param num The number of the song in the selected big-folder. Can range from 1 to 3000.
 */
void specifyTackInBigFolder(uint8_t folder, uint16_t num) {
    sendCommand(0x14, ((folder & 0xF) << 12) | (num & 0xFFF), 0);
}

/**
 * Stops the track from the ADVERT folder and continues with the paused track.
 */
void stopAd() {
    sendCommand(0x15, 0x0000, 0);
}

/**
 * Stops all playback and stops decoding the audio files.
 */
void stop() {
    sendCommand(0x16, 0x0000, 0);
}

/**
 * Repeats all songs in a certain folder. Aka once the last song is finished the first starts again.
 * 
 * @param folder The folder number where the tracks are contained.
 */
void repeatSpecificFolder(uint8_t folder) {
    sendCommand(0x17, folder, 0);
}

/**
 * I don't get the explaination from the datasheet, but here it is: This command is used to randomly play sound files in the storage device according to physical sequence and no matter if there is a folder or not in the device. The first sound file that is conducted to be played is the first one in the device.
 */
void setRandom() {
    sendCommand(0x18, 0x0000, 0);
}

/**
 * Repeats the track thats currently playing.
 * 
 * @param on If the repeat-mode should be on.
 */
void repeatCurrent(bool on) {
    sendCommand(0x19, !on, 0);
}

/**
 * Turns the DAC (Digital-Analog-Converter) on or off.
 * 
 * @param on If the DAC should be on.
 */
void setDAC(bool on) {
    sendCommand(0x1A, !on, 0);
}