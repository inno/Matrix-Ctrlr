#ifndef chaosmatrix_h
#define chaosmatrix_h
#import "define.h"

/*
    used for blinking LEDs like LFO speed on the
    LFO waveform buttons
*/
#define TIMER_PRESCALER 3
#define TIMER_TICKS 32000
#define LED_BLINK_SPEED 16
#define SYNC_BLINK_SPEED 7
#define F1F2_BLINK_SPEED 8

#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3

#define FILTER_ENV_MODE_BUGGED 0 // ignores env1 sustain pot
#define FILTER_ENV_MODE_HACKED 1 // transmits filter sustain by sending entire edit buffer
#define FILTER_FREQ_MODE_SYSEX 0 // values for FilterCutoffMode, pot tweaks send out Sysex
#define FILTER_FREQ_MODE_CCONTROL 1 // pot tweaks send out continuous controller

#define KBDMODE_REASSIGN 0
#define KBDMODE_ROTATE 1
#define KBDMODE_UNISON 2
#define KBDMODE_REAROB 3
#define DEFAULT_KBD_MODE KBDMODE_REASSIGN

// 3 mars 2021
// following definitions modified according to online chart :
// https://www.alpesmachines.net/wp-content/uploads/resources_files_matrix_ctrlr/MatrixCtrlr_sysex_charts.pdf
// (previously : EditBuffer BYTE instead of Control Chnge nbr value)
#define CC_PATCHNAME0 255
#define CC_PATCHNAME1 255
#define CC_PATCHNAME2 255
#define CC_PATCHNAME3 255
#define CC_PATCHNAME4 255
#define CC_PATCHNAME5 255
#define CC_PATCHNAME6 255
#define CC_PATCHNAME7 255
#define CC_KEYBOARD_MODE 95
#define CC_OSC1_FREQ 9
#define CC_OSC1_WAVESHAPE 10
#define CC_OSC1_PULSEWIDTH 11
#define CC_OSC1_LEVERMOD 12
#define CC_OSC1_WAVEFORM 13
#define CC_OSC2_FREQ 14
#define CC_OSC2_WAVESHAPE 15
#define CC_OSC2_PULSEWIDTH 16
#define CC_OSC2_LEVERMOD 17
#define CC_OSC2_WAVEFORM 18
#define CC_OSC2_DETUNE 19
#define CC_MIX 8
#define CC_OSC1_KEYBOARDMOD 102
#define CC_OSC1_CLICK 80
#define CC_OSC2_KEYBOARDMOD 103
#define CC_OSC2_CLICK 81
#define CC_OSC_SYNCMODE 104
#define CC_FILTER_FREQ 105
#define CC_FILTER_RESONANCE 106
#define CC_FILTER_LEVERMOD 107
#define CC_FILTER_KEYBOARDMOD 108
#define CC_FILTER_FM_AMOUNT 109
#define CC_VCA1_AMOUNT 110
#define CC_PORTAMENTO_RATE 5
#define CC_PORTAMENTO_MODE 37 //+++
#define CC_PORTAMENTO_LEGATOENABLE 65
#define CC_LFO1_SPEED 111
#define CC_LFO1_TRIGGERMODE 112
#define CC_LFO1_LAGENABLE 82
#define CC_LFO1_WAVESHAPE 113
#define CC_LFO1_RETRIGGERPOINT 114
#define CC_LFO1_SAMPLEDSOURCE 115
#define CC_LFO1_AMPLITUDE 116
#define CC_LFO2_SPEED 117
#define CC_LFO2_TRIGGERMODE 118
#define CC_LFO2_LAGENABLE 83
#define CC_LFO2_WAVESHAPE 33 //+++
#define CC_LFO2_RETRIGGERPOINT 34
#define CC_LFO2_SAMPLEDSOURCE 35
#define CC_LFO2_AMPLITUDE 36
#define CC_ENV1_TRIGGERMODE 20
#define CC_ENV1_DELAY 21
#define CC_ENV1_ATTACK 22
#define CC_ENV1_DECAY 23
#define CC_ENV1_SUSTAIN 24
#define CC_ENV1_RELEASE 25
#define CC_ENV1_AMPLITUDE 26
#define CC_ENV1_LFOTRIGGERMODE 27
#define CC_ENV1_RELEASEMODE 28
#define CC_ENV2_TRIGGERMODE 29
#define CC_ENV2_DELAY 30
#define CC_ENV2_ATTACK 41
#define CC_ENV2_DECAY 42
#define CC_ENV2_SUSTAIN 43
#define CC_ENV2_RELEASE 44
#define CC_ENV2_AMPLITUDE 45
#define CC_ENV2_LFOTRIGGERMODE 46
#define CC_ENV2_RELEASEMODE 47
#define CC_ENV3_TRIGGERMODE 48
#define CC_ENV3_DELAY 49
#define CC_ENV3_ATTACK 50
#define CC_ENV3_DECAY 51
#define CC_ENV3_SUSTAIN 52
#define CC_ENV3_RELEASE 53
#define CC_ENV3_AMPLITUDE 54
#define CC_ENV3_LFOTRIGGERMODE 55
#define CC_ENV3_RELEASEMODE 56
#define CC_TRACK_INPUT 57
#define CC_TRACK_POINT1 58
#define CC_TRACK_POINT2 59
#define CC_TRACK_POINT3 60
#define CC_TRACK_POINT4 61
#define CC_TRACK_POINT5 62
#define CC_RAMP1_RATE 63
#define CC_RAMP1_MODE 38
#define CC_RAMP2_RATE 39
#define CC_RAMP2_MODE 40
#define CC_OSC1_FREQBYLFO1AMOUNT 70
#define CC_OSC1_PWBYLFO2AMOUNT 71
#define CC_OSC2_FREQBYLFO1AMOUNT 72
#define CC_OSC2_PWBYLFO2AMOUNT 73
#define CC_FILTER_FREQBYENV1AMOUNT 74
#define CC_FILTER_FREQBYPRESSURE 75
#define CC_VCA1_AMPBYVELOCITY 76
#define CC_VCA2_AMPBYENV2 77
#define CC_ENV1_AMPBYVELO 78
#define CC_ENV2_AMPBYVELO 79
#define CC_ENV3_AMPBYVELO 84
#define CC_LFO1_AMPMODULATION 85
#define CC_LFO2_AMPMODULATION 86
#define CC_PORTAMENTO_SPEEDBYVELOCITY 87
#define CC_FILTER_FMBYENV3 88
#define CC_FILTER_FMBYPRESSURE 89
#define CC_LFO1_SPEEDMODULATION 90 // pressure
#define CC_LFO2_SPEEDMODULATION 91 // keyboard tracking

// Gligli OS116 additional parameter :
#define CC_UNISON_DETUNE 94

//extern const unsigned char DefaultEditBuffer[134];

extern unsigned char EditBuffer[4][134]; // was 134 bytes originally, now 4x134
extern unsigned char EditBufferOrig[134]; // for the compare function
extern unsigned char UnisonDetune[4];
extern unsigned char UnisonDetuneOrig;
extern const unsigned char singlePatchDataFormatCC[100][9];
extern const unsigned char singlePatchDataFormatX[128][9];
extern const unsigned char singlePatchDataFormatEB[97][9];

extern unsigned char ProgramNumber; // patches
extern unsigned char ProgramNumberEdit; // patches
extern unsigned char BankNumber;
extern unsigned char BankNumberEdit;
extern unsigned char Osc_Pulse_Modulate[NUM_OSCILLATORS];
extern unsigned char FilterCutoffMode; // 0=sysex, 1=lever 3 (continuous controller)
extern unsigned char FilterSustainMode; // use FILTER_ENV_MODE_BUGGED or FILTER_ENV_MODE_HACKED
extern unsigned char Osc2_ClickMode; // DCO2 click button toggles click and noise waveform. 0=both off, 1=click, 2=noise

//extern void PrintDescription(code char *pretext, code char *description);
void PrintDescription(char *pretext, char *description);
extern void SetPatchBank(void);
extern void SetPatchChange(void);

extern void InitEditBuffer(void);
extern void SendPatchInit(unsigned char interface);
extern void SendEditBuffer(unsigned char device, unsigned char interface);
extern void SendEditBufferSingle(unsigned char device, unsigned char interface);
extern void SendEditBufferOrig(unsigned char interface);
extern void SendSinglePatchData(unsigned char interface, unsigned char patch);

extern unsigned char Convert_7bit_Signed(unsigned char value);
extern unsigned char Convert_6bit_Signed(unsigned char value);

#endif
