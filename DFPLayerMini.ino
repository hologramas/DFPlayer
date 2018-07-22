//
// Simple Arduino program to drive an MFPLayer module.
// Refer to the MFPL specification for details.
//

#include <SoftwareSerial.h>
#include <Wire.h>

#define SERIAL_RXPIN 10
#define SERIAL_TXPIN 11

#define COMMAND_HEADER 0x7E
#define COMMAND_ENDER 0xEF
#define COMMAND_VERSION 0xFF
#define COMMAND_SIZE 0x06
#define COMMAND_ACK 0x00
#define COMMAND_DELAY 500

enum CommandCode
{
  CommandCode_Next = 0x01,
  CommandCode_Previous = 0x02,
  CommandCode_PlayTrack = 0x03,
  CommandCode_IncreaseVolume = 0x04,
  CommandCode_DecreaseVolume = 0x05,
  CommandCode_SetVolume = 0x06,
  CommandCode_SetEqualizer = 0x07,
  CommandCode_SetPlaybackMode = 0x08,
  CommandCode_SetPlaybackSource = 0x09,
  CommandCode_EnterLowPowerMode = 0x0A,
  CommandCode_RepeatPlay = 0x11,
  CommandCode_Init = 0x3F
};

SoftwareSerial DFPLSerial(SERIAL_RXPIN, SERIAL_TXPIN);

void setup()
{
  DFPLSerial.begin(9600);
  ExecuteCommand(CommandCode_Init, 0, 0);

  // Set the initial volume to 26 (range goes from 0 to 48).
  ExecuteCommand(CommandCode_SetVolume, 0, 26);

  // Repeat play, starting in track 1.
  ExecuteCommand(CommandCode_RepeatPlay, 0, 1);
}

void loop()
{
}

void ExecuteCommand(byte cmdCode, byte param1, byte param2)
{
  const word checksum = -(COMMAND_VERSION + cmdCode + COMMAND_ACK + param1 + param2);
  const byte command[10] =
  {
    COMMAND_HEADER,
    COMMAND_VERSION,
    COMMAND_SIZE,
    cmdCode,
    COMMAND_ACK,
    param1,
    param2,
    highByte(checksum),
    lowByte(checksum),
    COMMAND_ENDER
  };

  for (byte i = 0; i < sizeof(command); i++)
  {
    DFPLSerial.write(command[i]);
  }

  delay(COMMAND_DELAY);
}

