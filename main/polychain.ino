#include "Midi.h"

bool polyChain = true; // XXX When enabled show multiple devices selected..?

int polyDepth = 2;  // Number of channels to chain
// Manage max depth based on #if SOFTSERIAL_ENABLED


// First iter: simple poly chaining
// Second iter: toggleable chaining in UI
// Third iter: transmit program changes to all supported devices
// Fourth iter: transmit CC to all supported devices


void PolyChainNoteOn(byte pitch, byte velocity)
{
  MIDI1.sendNoteOn(pitch, velocity, MIDI_CHANNEL);
  MIDI2.sendNoteOn(pitch, velocity, MIDI_CHANNEL + 1);
  // softMIDITHRU handles MIDI3
  #if SOFTSERIAL_ENABLED
  if (polyDepth >= 3)
    MIDI4.sendNoteOn(pitch, velocity, MIDI_CHANNEL + 2);
    if (polyDepth == 4)
      MIDI5.sendNoteOn(pitch, velocity, MIDI_CHANNEL + 3);
  #endif
}

void PolyChainNoteOff(byte pitch, byte velocity)
{
  MIDI1.sendNoteOff(pitch, velocity, MIDI_CHANNEL);
  MIDI2.sendNoteOff(pitch, velocity, MIDI_CHANNEL + 1);
  // softMIDITHRU handles MIDI3
  #if SOFTSERIAL_ENABLED
  if (polyDepth >= 3)
    MIDI4.sendNoteOff(pitch, velocity, MIDI_CHANNEL + 2);
    if (polyDepth == 4)
      MIDI5.sendNoteOff(pitch, velocity, MIDI_CHANNEL + 3);
  #endif
}

void PolyControlChange(byte number, byte value)
{
  for (int i = 0; i < polyDepth; i++) {
    HandleControlChange(MIDI_CHANNEL + i, number, value);
  }
}

void PolyAfterTouch(byte value)
{
  for (int i = 0; i < polyDepth; i++) {
    HandleAfterTouchChannel(MIDI_CHANNEL + i, value);
  }
}

void PolyPitchBend(byte value)
{
  for (int i = 0; i < polyDepth; i++) {
    HandlePitchBend(MIDI_CHANNEL + i, value);
  }
}

void PolyProgramChange(byte value)
{
  for (int i = 0; i < polyDepth; i++) {
    HandleProgramChange(MIDI_CHANNEL + i, value);
  }
}

void PolySendVoiceParam(unsigned char interface, unsigned char param, unsigned char value, bool midiThru)
{
  // XXX Conditionally change what we're modifying based on the depth and `interface`
  MIDI_SendVoiceParamSingle(INTERFACE_SERIAL1, param, value, midiThru);
  MIDI_SendVoiceParamSingle(INTERFACE_SERIAL2, param, value, midiThru);
  #if SOFTSERIAL_ENABLED
  if (polyDepth >= 3)
    MIDI_SendVoiceParamSingle(INTERFACE_SERIAL4, param, value, midiThru);
    if (polyDepth == 4)
      MIDI_SendVoiceParamSingle(INTERFACE_SERIAL5, param, value, midiThru);
  #endif
}
