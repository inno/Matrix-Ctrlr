#include "lcd.h"

/*
  []  []   [ ]  []  []
  01234567890123456789
*/

////////// Vibrato waveform /////////////////////////////////////////////////////////
const char VIB_Waves_Name0[] PROGMEM = {"TRI"};
const char VIB_Waves_Name1[] PROGMEM = {"USW"};
const char VIB_Waves_Name2[] PROGMEM = {"DSW"};
const char VIB_Waves_Name3[] PROGMEM = {"SQR"};
const char VIB_Waves_Name4[] PROGMEM = {"RAN"};
const char VIB_Waves_Name5[] PROGMEM = {"NOI"};

// https://github.com/arduino/Arduino/wiki/1.6-Frequently-Asked-Questions  And, yes, it's really tricky even for experts!
const char *const VIB_Waves_Name[] PROGMEM = {
  VIB_Waves_Name0,
  VIB_Waves_Name1,
  VIB_Waves_Name2,
  VIB_Waves_Name3,
  VIB_Waves_Name4,
  VIB_Waves_Name5,
};

////////// Vibrato Speed modulated by /////////////////////////////////////////////////////////
const char VIB_Speed_By0[] PROGMEM = {"OFF"};
const char VIB_Speed_By1[] PROGMEM = {"Lv2"};
const char VIB_Speed_By2[] PROGMEM = {"Pd1"};

// https://github.com/arduino/Arduino/wiki/1.6-Frequently-Asked-Questions  And, yes, it's really tricky even for experts!
const char *const VIB_Speed_By[] PROGMEM = {
  VIB_Speed_By0,
  VIB_Speed_By1,
  VIB_Speed_By2,
};

////////// Vibrato Amplitude modulated by /////////////////////////////////////////////////////////
const char VIB_Amp_By0[] PROGMEM = {"OFF"};
const char VIB_Amp_By1[] PROGMEM = {"Lv2"};
const char VIB_Amp_By2[] PROGMEM = {"Pd1"};

// https://github.com/arduino/Arduino/wiki/1.6-Frequently-Asked-Questions  And, yes, it's really tricky even for experts!
const char *const VIB_Amp_By[] PROGMEM = {
  VIB_Amp_By0,
  VIB_Amp_By1,
  VIB_Amp_By2,
};


void ui_page_cfg_vib_1()
{
  String line1 = F("SPD MOD AMNT WAV GBL");
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);

  if (SoftPanel.IsNewPage)
  {
    LCD_Clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
  }
  lcd.setCursor(1, 1);
  LCD_PrintBCD2(GlobalParameters[1]);
  lcd.setCursor(4, 1);
  lcd.print(strcpy_P(bufferProgmem, (PGM_P)pgm_read_word(&(VIB_Speed_By[GlobalParameters[2]]))));
  lcd.setCursor(9, 1);
  LCD_PrintBCD2(GlobalParameters[3]);
  lcd.setCursor(13, 1);
  lcd.print(strcpy_P(bufferProgmem, (PGM_P)pgm_read_word(&(VIB_Waves_Name[GlobalParameters[4]]))));
  lcd.setCursor(17, 1);
  lcd.print(F("Get"));
}

void ui_page_cfg_vib_2()
{
  String line1 = F("AMP MOD AMNT BND GBL");
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);
  if (SoftPanel.IsNewPage)
  {
    LCD_Clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
  }
  lcd.setCursor(1, 1);
  LCD_PrintBCD2(GlobalParameters[5]);
  lcd.setCursor(4, 1);
  lcd.print(strcpy_P(bufferProgmem, (PGM_P)pgm_read_word(&(VIB_Amp_By[GlobalParameters[6]]))));
  lcd.setCursor(9, 1);
  LCD_PrintBCD2(GlobalParameters[7]);
  lcd.setCursor(14, 1);
  LCD_PrintBCD2(GlobalParameters[164]); // bend range 12 demi ton
  lcd.setCursor(17, 1);
  lcd.print(F("Snd"));
}

void handle_page_cfg_vib_1()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE   : SoftPanel.Page = SOFT_PAGE12; break;
    case SOFT_EDIT_1: DOUT_PinSet_Keypanel(1, 0, 0, 0, 0, 1); break; // vibrato speed
    case SOFT_EDIT_2: DOUT_PinSet_Keypanel(0, 1, 0, 0, 0, 1); break;
    case SOFT_EDIT_3: DOUT_PinSet_Keypanel(0, 0, 1, 0, 0, 1); break;
    case SOFT_EDIT_4: DOUT_PinSet_Keypanel(0, 0, 0, 1, 0, 1); break;
    case SOFT_EDIT_5: DOUT_PinSet_Keypanel(0, 0, 0, 0, 1, 1); MIDI_RequestMastersParameters(INTERFACE_SERIAL);
#if DEBUG_din
      Serial.print(F("MIDI_RequestMastersParameters "));
      Serial.println(INTERFACE_SERIAL, DEC);
#endif
      break;
  }

  if (SoftPanel.EncoderValue != 0)
  {
    if (SoftPanel.EditButton == Edit1)
      SetEncoderValue(&GlobalParameters[1], 63);
    if (SoftPanel.EditButton == Edit2)
      SetEncoderValue(&GlobalParameters[2], 2);
    if (SoftPanel.EditButton == Edit3)
      SetEncoderValue(&GlobalParameters[3], 63);
    if (SoftPanel.EditButton == Edit4)
      SetEncoderValue(&GlobalParameters[4], 5); // same as for LFO : Tri DnSaw, UpSaw, Sqr, Rnd, noise
    else
      return;
  }

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}

void handle_page_cfg_vib_2()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE   : SoftPanel.Page = SOFT_PAGE1; break;
    case SOFT_EDIT_1: DOUT_PinSet_Keypanel(1, 0, 0, 0, 0, 1); break; // vibrato speed
    case SOFT_EDIT_2: DOUT_PinSet_Keypanel(0, 1, 0, 0, 0, 1); break;
    case SOFT_EDIT_3: DOUT_PinSet_Keypanel(0, 0, 1, 0, 0, 1); break;
    case SOFT_EDIT_4: DOUT_PinSet_Keypanel(0, 0, 0, 1, 0, 1); break;
    case SOFT_EDIT_5:
      STORE_GlobalParameters();
      READ_GlobalParameters();
      SendGlobalParameters(INTERFACE_SERIAL); // edited matrix
      SendGlobalParameters(INTERFACE_MIDI_CORE); // core OUT to DAW
      break;
  }
  if (SoftPanel.EncoderValue != 0)
  {
    if (SoftPanel.EditButton == Edit1)
      SetEncoderValue(&GlobalParameters[5], 63);
    if (SoftPanel.EditButton == Edit2)
      SetEncoderValue(&GlobalParameters[6], 2);
    if (SoftPanel.EditButton == Edit3)
      SetEncoderValue(&GlobalParameters[7], 63);
    if (SoftPanel.EditButton == Edit4)
      SetEncoderValue(&GlobalParameters[164], 24);
    else
      return;
  }
  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}
