#include "lcd.h"

/*
  []  []   [ ]  []  []
  01234567890123456789
  CH    A   B   C   D
  00    0   0   1   0
*/

void ui_page_cfg_midi_main()
{
  String line1 = F("CH  A:  B:   C:  D: ");
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSet_Keypanel(0, 0, 0, 0, 0, 1);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);

  if (SoftPanel.IsNewPage)
  {
    LCD_Clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
  }
  LCD_CursorSet(64);
  LCD_PrintBCD2(MIDI_CHANNEL);

  // deviceA = 1000 or 6
  LCD_CursorSet(64 + 4);
  if (matrix_model_A)
    LCD_PrintCString(F("M6 "));
  else
    LCD_PrintCString(F("M1K"));
  // device B = 1000 or 6
  LCD_CursorSet(64 + 8);
  if (matrix_model_B)
    LCD_PrintCString(F("M6 "));
  else
    LCD_PrintCString(F("M1K"));
  // device C = 1000 or 6
  LCD_CursorSet(64 + 13);
  if (matrix_model_C)
    LCD_PrintCString(F("M6 "));
  else
    LCD_PrintCString(F("M1K"));
  // device D = 1000 or 6
  LCD_CursorSet(64 + 17);
  if (matrix_model_D)
    LCD_PrintCString(F("M6 "));
  else
    LCD_PrintCString(F("M1K"));
}

void ui_page_cfg_midi_1()
{
  String line1 = F("Pd1 Pd2  Lv1 Lv2 Ena");
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
  LCD_PrintBCD2(GlobalParameters[17]); // Midi Pedal 1 Controller
  lcd.setCursor(5, 1);
  LCD_PrintBCD2(GlobalParameters[18]); // midi Pedal 2 Controller
  lcd.setCursor(10, 1);
  LCD_PrintBCD2(GlobalParameters[19]); // midi Lever 1 Controller
  lcd.setCursor(14, 1);
  LCD_PrintBCD2(GlobalParameters[20]); // midi Lever 2 Controller
  lcd.setCursor(17, 1);
  if (GlobalParameters[13])
    lcd.print(F(" * "));
  else
    lcd.print(F(" . "));
}

void ui_page_cfg_midi_2()
{
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);
  if (SoftPanel.IsNewPage)
    LCD_Clear();

  lcd.setCursor(0, 0); // 1st line
  switch (ui_Zone)
  {
    case 0: lcd.print(F("A:Range  rCH tCH Trp")); break;
    case 1: lcd.print(F("B:Range  rCH tCH Trp")); break;
    case 2: lcd.print(F("C:Range  rCH tCH Trp")); break;
    case 3: lcd.print(F("D:Range  rCH tCH Trp")); break;
  }

  //2nd line
  lcd.setCursor(0, 1);
  lcd.print(strcpy_P(bufferProgmem, (PGM_P)pgm_read_word(&(pitchNotation[ZONE[ui_Zone][1]]))));

  lcd.setCursor(4, 1);
  lcd.print(strcpy_P(bufferProgmem, (PGM_P)pgm_read_word(&(pitchNotation[ZONE[ui_Zone][3]]))));

  lcd.setCursor(9, 1);
  LCD_PrintBCD2(ZONE[ui_Zone][0]);

  lcd.setCursor(13, 1);
  LCD_PrintBCD2(ZONE[ui_Zone][4]);

  lcd.setCursor(17, 1);
  LCD_Display7BitSigned(ZONE[ui_Zone][2] - 5);
}

void handle_page_cfg_midi_main()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE: SoftPanel.Page = SOFT_PAGE21; break;
    case SOFT_EDIT_1:
      // click this button to save the midi channel to eeprom
      EEPROM.update(EEPROM_MIDI_CHANNEL, MIDI_CHANNEL);
      break;
    case SOFT_EDIT_2:
      // store in EEPROM device A model
      matrix_model_A = !matrix_model_A;
      EEPROM.update(EEPROM_MATRIX_MODELE_A, matrix_model_A);
      break;
    case SOFT_EDIT_3:
      // store in EEPROM device B model
      matrix_model_B = !matrix_model_B;
      EEPROM.update(EEPROM_MATRIX_MODELE_B, matrix_model_B);
      break;
    case SOFT_EDIT_4:
      // store in EEPROM device C model
      matrix_model_C = !matrix_model_C;
      EEPROM.update(EEPROM_MATRIX_MODELE_C, matrix_model_C);
      break;
    case SOFT_EDIT_5:
      // store in EEPROM device D model
      matrix_model_D = !matrix_model_D;
      EEPROM.update(EEPROM_MATRIX_MODELE_D, matrix_model_D);
      break;
  }
#if DEBUG_device
  Serial.print(F("midi channel = "));
  Serial.println(MIDI_CHANNEL, DEC);
  Serial.print(F("MATRIX_MODELE_A = "));
  Serial.println(matrix_model_A, DEC);
  Serial.print(F("MATRIX_MODELE_B = "));
  Serial.println(matrix_model_B, DEC);
  Serial.print(F("MATRIX_MODELE_C = "));
  Serial.println(matrix_model_C, DEC);
  Serial.print(F("MATRIX_MODELE_D = "));
  Serial.println(matrix_model_D, DEC);
#endif

  if (SoftPanel.EncoderValue != 0)
  {
    if (SoftPanel.EditButton == Edit1)
    {
      SetEncoderValue(&MIDI_CHANNEL, 16);
    }
  }
}

void handle_page_cfg_midi_1()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE:
      STORE_GlobalParameters(); // save Masters Parameters into internal eeprom before leaving
      READ_GlobalParameters();
      SendGlobalParameters(INTERFACE_SERIAL); // edited matrix
      SendGlobalParameters(INTERFACE_MIDI_CORE); // core OUT to DAW
      SoftPanel.Page = SOFT_PAGE22;
      break;
    case SOFT_EDIT_1: DOUT_PinSet_Keypanel(1, 0, 0, 0, 0, 1); break; // vibrato speed
    case SOFT_EDIT_2: DOUT_PinSet_Keypanel(0, 1, 0, 0, 0, 1); break;
    case SOFT_EDIT_3: DOUT_PinSet_Keypanel(0, 0, 1, 0, 0, 1); break;
    case SOFT_EDIT_4: DOUT_PinSet_Keypanel(0, 0, 0, 1, 0, 1); break;
    case SOFT_EDIT_5:
      DOUT_PinSet_Keypanel(0, 0, 0, 0, 1, 1);
      if (++GlobalParameters[13] > 1)
        GlobalParameters[13] = 0;
      break;
  }
  if (SoftPanel.EncoderValue != 0)
  {
    if (SoftPanel.EditButton == Edit1)
      SetEncoderValue(&GlobalParameters[17], 120);
    if (SoftPanel.EditButton == Edit2)
      SetEncoderValue(&GlobalParameters[18], 120);
    if (SoftPanel.EditButton == Edit3)
      SetEncoderValue(&GlobalParameters[19], 120);
    if (SoftPanel.EditButton == Edit4)
      SetEncoderValue(&GlobalParameters[20], 120);
    else
      return;
  }

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}

void handle_page_cfg_midi_2()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE:
      ZONE_Save();
      SoftPanel.Page = SOFT_PAGE1;
      break;
    case SOFT_EDIT_1: DOUT_PinSet_Keypanel(1, 0, 0, 0, 0, 1); break;
    case SOFT_EDIT_2: DOUT_PinSet_Keypanel(0, 1, 0, 0, 0, 1); break;
    case SOFT_EDIT_3: DOUT_PinSet_Keypanel(0, 0, 1, 0, 0, 1); break;
    case SOFT_EDIT_4: DOUT_PinSet_Keypanel(0, 0, 0, 1, 0, 1); break;
    case SOFT_EDIT_5: DOUT_PinSet_Keypanel(0, 0, 0, 0, 1, 1); break;
    case SOFT_EDIT_INC:
      if (++ui_Zone > 3)
        ui_Zone = 3;
      break;
    case SOFT_EDIT_DEC:
      if (--ui_Zone == 255)
        ui_Zone = 0;
      break;
  }
  if (SoftPanel.EncoderValue != 0)
  {
    if (SoftPanel.EditButton == Edit3)
    {
      SetEncoderValue(&ZONE[ui_Zone][0], 16);
      if (ZONE[ui_Zone][0] == 0)
        ZONE[ui_Zone][0] = 1;
    }

    if (SoftPanel.EditButton == Edit1)
    {
      SetEncoderValue(&ZONE[ui_Zone][1], 127);
      if (ZONE[ui_Zone][1] < 0)
        ZONE[ui_Zone][1] = 0;
    }

    if (SoftPanel.EditButton == Edit5)
      SetEncoderValue(&ZONE[ui_Zone][2], 10);

    if (SoftPanel.EditButton == Edit2)
    {
      SetEncoderValue(&ZONE[ui_Zone][3], 127);
      if (ZONE[ui_Zone][3] < 0)
        ZONE[ui_Zone][3] = 0;
    }
    if (SoftPanel.EditButton == Edit4)
    {
      SetEncoderValue(&ZONE[ui_Zone][4], 16);
      if (ZONE[ui_Zone][4] == 0)
        ZONE[ui_Zone][4] = 1;
    }
    else
      return;
  }

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}

void handle_page_cfg_midi_3()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE   : SoftPanel.Page = SOFT_PAGE1; break;
    case SOFT_EDIT_1: DOUT_PinSet_Keypanel(1, 0, 0, 0, 0, 1); break; // vibrato speed
    case SOFT_EDIT_2: DOUT_PinSet_Keypanel(0, 1, 0, 0, 0, 1); break;
    case SOFT_EDIT_3: DOUT_PinSet_Keypanel(0, 0, 1, 0, 0, 1); break;
    case SOFT_EDIT_4: DOUT_PinSet_Keypanel(0, 0, 0, 1, 0, 1); break;
    case SOFT_EDIT_5: DOUT_PinSet_Keypanel(0, 0, 0, 0, 1, 1); break;
  }

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}
