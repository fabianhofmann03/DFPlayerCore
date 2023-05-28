# dfplayer
## Overview
This library is for the dfrobot mp3 player. [Datasheet used](http://www.robotsforfun.com/datasheets/DFPlayer.pdf)

Arduino only right now.

## Setup

To use the dfplayer you have to initialize it first. There you have to give it the pointer to the Serial port that the module uses.
```
dfplayer player(&Serial3);
```

Next you have to call the setup-function inside the Arduino setup-function, which starts the Serial port.
```
player.setup();
```

Well done. You now have a working dfplayer ... thingi. You can now use it's functions to play different files.

## Play sound and change settings

Here are the functions you can use to do stuff. They're in order of the command code.
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

## Responses

The module returns responses to certain events.
To get access to the responses you can make a response handler. A response handler looks like this:
```cpp
void responseHandler(uint8_t vers, uint8_t len, uint8_t cmd, uint8_t feedb, uint16_t param);
```

To add the response handler to the dfplayer you can use the function `setResponseHandler`.
```cpp
player.setResponseHandler(&responseHandler);
```
You can also use the default response handler by using:
```
player.setDefaultResponseHandler();
```
The default response handler prints out an explanation of the response like "Track number 2 finished playing from the SD card." 
<br>

Now you have to call the `update` function, which reacts to incomming Serial data.
```cpp
player.update();
```
Now every time these's a response from the module, your function will be called.