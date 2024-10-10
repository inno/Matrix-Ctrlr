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
#endif
