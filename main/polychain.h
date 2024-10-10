#ifndef polychain_h
#define polychain_h

extern bool polyChain;

extern void PolyChainNoteOn(byte pitch, byte velocity);
extern void PolyChainNoteOff(byte pitch, byte velocity);

extern void PolyAfterTouch(byte value);
extern void PolyControlChange(byte number, byte value);
extern void PolyPitchBend(byte value);
extern void PolyProgramChange(byte value);
extern void PolySendVoiceParam(unsigned char interface, unsigned char param, unsigned char value, bool midiThru);
extern void PolySendPatchProgram(unsigned char interface, unsigned char program);
extern void PolySendPatchNumber(unsigned char interface, unsigned char bank, unsigned char program);
extern void PolySendEditBuffer(unsigned char device, unsigned char interface);
extern void PolySendUnisonDetune(unsigned char interface, unsigned char value);
#endif
