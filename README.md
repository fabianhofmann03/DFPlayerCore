# DFPlayer Core
## Overview
This library is for the dfrobot mp3 player. [Datasheet used](http://www.robotsforfun.com/datasheets/DFPlayer.pdf)

The core library. 

## How to use

### Audio functions

The audio functions are used to tell the module what audio or how to play the audio. Because this is the core library, the functions don't actually tell the module anything. They generate the code that has to be sent to the module. 

In order to do that you have to execute one of the [audio functions](#usable-audio-functions).
```c
playNext();                 // Example
```
Then the data will be stored in a result array. You can get the pointer using the `getResult()` function.
```c
uint8_t result_len = *getResult();
uint8_t* result = getResult() + 1;
```
As you can see, the length of the result, that has to be sent is stored in the first entry of the array.
Now you can send the command to the module using any way you want.

### Responses

The module also sends responses, which include certain statuses. To deal with these responses, you can make and set a response handler. A response handler is a function, that gets all the data sent from the module. It will be executed every time a valid response has been recognized.  
A response handler must look like this:

```c
void responseHandler(uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param);
```
But the library still needs to get the data from the module. This is why the update function exists. The update function looks like this:

```c
void dfp_update(uint8_t resp, uint8_t* message_buffer, void (*respHand) (uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param));
```
The update function needs five parameters.
- resp - The response from the module. You need to feed the update-function the response data individually, so every for a 10 byte response you'd have call this function 10 times with each byte as the resp-parameter.
- message_buffer - In order to make objectification possible, you have to make your own message-buffer, where the function can store the message before it can analyze it. For the DFPlayer, a 10-element array should be enough.
- respHand - Add a pointer to your response handler function. If you don't have a response handler function, why even bother using the update-function?

## Usable audio functions

Here are the functions you can use to do stuff.
<br><br>
| Function | Description |
| --- | ----------- |
| `void playNext();` | Plays the next sound from the selected device. |
| `void playPrev();` | Plays the previous sound from the selected device. |
| `void specifyTrack(uint16_t num);` | Plays a specific sound from the selected device. Can range between 1 and 3000. |
| `void increaseVolume();` | Increases the volume. |
| `void decreaseVolume();` | We both know what it does. |
| `void specifyVolume(uint16_t vol);` | Sets a specific volume. I don't know the range. |
| `void specifyVolume(uint16_t vol);` | Sets a specific volume. I don't know the range. |
| `void specifyEQ(uint8_t eq);` | Changes the equalizer settings. Usable are Normal, Pop, Rock, Jazz, Classic and Bass. | `player.specifyEQ(player.Bass);`
| `void specifySingleRepeat(uint16_t num);` | Selects a singe track in the root directory that will be repeated indefinetly. Same range as `specifyTrack`. |
| `void specifyDevice(uint8_t device);` | Selects the device where the sounds are saved. Usable are USB and SD. | `player.specifyDevice(player.USB);` |
| `void sleep();` | Puts the device into sleep mode. |
| `void wakeup();` | Puts the device out of sleep mode. I think. |
| `void reset();` | Resets something?. |
| `void play();` | Continues playing. |
| `void pause();` | Pauses the track. |
| `void specifyTackInFolder(uint8_t folder, uint8_t num);` | Plays a certain track in a certain folder. Folders can range from 1 to 99 and tracks from 1 to 255. |
| `void setAudioAmp(bool on, uint8_t gain);` | Sets the audio amplification. You can turn it on and off and set the amplification gain. The gain ranges from 0 to 31. |
| `void setAudioAmp(bool on);` | Just if you want to turn it off. |
| `void setAllRepeatPlayback(bool on);` | The whole folder will repeat indefinetly. |
| `void specifyTrackInMP3(uint16_t num);` | Plays a specific track in the MP3 folder. The folder can hold up to 65536 files, but it is recommended to store a max of 3000 files. |
| `void insetAd(uint16_t num);` | Pauses the current audio file and plays a specific file from the ADVERT folder. After that it returns with the original audio. |
| `void specifyTackInBigFolder(uint8_t folder, uint16_t num);` | If you want more files in the folders use this function. The track number can range from 1 to 3000, but the folders only go from 1 to 15. |
| `void stopAd();` | Stops the inserted ADVERT audio and returns to the original audio file. |
| `void stop();` | Stops decoding the device. |
| `void repeatSpecificFolder(uint8_t folder);` | Repeats a specific folder indefinetly. |
| `void setRandom();` | I don't get the documentation, but here it is: This command is used to randomly play sound files in the storage device according to physical sequence and no matter if there is a folder or not in the device. The first sound file that is conducted to be played is the first one in the device. |
| `void repeatCurrent(bool on);` | Repeats the current song indefinetly. |
| `void setDAC(bool on);` | Turns the DAC (Digital-Analog-Converter) on or off. |