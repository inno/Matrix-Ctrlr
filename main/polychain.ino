#include "Midi.h"
#include "chaosmatrix.h"

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

void PolySendPatchProgram(unsigned char interface, unsigned char program)
{
  // XXX Conditionally change what we're modifying based on the depth and `interface`
  MIDI_SendPatchProgramSingle(INTERFACE_SERIAL1, program);
  MIDI_SendPatchProgramSingle(INTERFACE_SERIAL2, program);
  #if SOFTSERIAL_ENABLED
  if (polyDepth >= 3)
    MIDI_SendPatchProgramSingle(INTERFACE_SERIAL4, program);
    if (polyDepth == 4)
      MIDI_SendPatchProgramSingle(INTERFACE_SERIAL5, program);
  #endif
}


void PolySendPatchNumber(unsigned char interface, unsigned char bank, unsigned char program)
{
  // XXX Conditionally change what we're modifying based on the depth and `interface`
  MIDI_SendPatchNumberSingle(INTERFACE_SERIAL1, bank, program);
  MIDI_SendPatchNumberSingle(INTERFACE_SERIAL2, bank, program);
  #if SOFTSERIAL_ENABLED
  if (polyDepth >= 3)
    MIDI_SendPatchNumberSingle(INTERFACE_SERIAL4, bank, program);
    if (polyDepth == 4)
      MIDI_SendPatchNumberSingle(INTERFACE_SERIAL5, bank, program);
  #endif
}

void PolySendEditBuffer(unsigned char device, unsigned char interface)
{
  bool orig_matrix_modele = matrix_modele;
  matrix_modele = matrix_model_A;
  SendEditBufferSingle(device, INTERFACE_SERIAL1);
  matrix_modele = matrix_model_B;
  SendEditBufferSingle(device, INTERFACE_SERIAL2);
  #if SOFTSERIAL_ENABLED
  if (polyDepth >= 3)
    matrix_modele = matrix_model_C;
    SendEditBufferSingle(device, INTERFACE_SERIAL4);
    if (polyDepth == 4)
      matrix_modele = matrix_model_D;
      SendEditBufferSingle(device, INTERFACE_SERIAL5);
  #endif
  matrix_modele = orig_matrix_modele;
}

extern void PolySendUnisonDetune(unsigned char interface, unsigned char value)
{
   MIDI_Send_UNISONDETUNESingle(INTERFACE_SERIAL1, value);
   MIDI_Send_UNISONDETUNESingle(INTERFACE_SERIAL2, value);
  #if SOFTSERIAL_ENABLED
  if (polyDepth >= 3)
    MIDI_Send_UNISONDETUNESingle(INTERFACE_SERIAL4, value);
    if (polyDepth == 4)
      MIDI_Send_UNISONDETUNESingle(INTERFACE_SERIAL5, value);
  #endif
}
