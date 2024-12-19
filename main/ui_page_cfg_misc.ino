#include "lcd.h"

/*
  []  []   [ ]  []  []
  01234567890123456789
*/
void ui_page_cfg_misc()
{
  String line1 = F("CLK ZON LOC  JTR XCC");
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);

  if (SoftPanel.IsNewPage)
  {
    LCD_Clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
  }

  lcd.setCursor(0, 1);
  switch (systmClock)
  {
    case INTCLK:
      LCD_PrintCString(F("INT"));
      DOUT_PinSetOff(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
      break;

    case MIDCLK:
      lcd.print(F("Mid"));
      DOUT_PinSetOn(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
      break;

    case S24CLK:
      lcd.print(F("s24"));
      DOUT_PinSetOn(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
      break;

    case TRGCLK:
      lcd.print(F("Trg"));
      DOUT_PinSetOn(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
      break;

    case MTRGCLK:
      lcd.print(F("mTg"));
      DOUT_PinSetOn(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
      break;

    case SYSCLK:
      lcd.print(F("Sys")); // not possible normally
      DOUT_PinSetOn(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
      break;

    default:
      lcd.print(F("UKN"));
      DOUT_PinSetOn(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
      break;
  }

  lcd.setCursor(4, 1);

  if (zActive)
  {
    lcd.print(F(" * "));
    DOUT_PinSetOn(DIN_ConfigMap[DIN_FILTER].dout_pin);
  }
  else
  {
    lcd.print(F(" . "));
    DOUT_PinSetOff(DIN_ConfigMap[DIN_FILTER].dout_pin);
  }

  lcd.setCursor(8, 1);
  if (localControl)
  {
    lcd.print(F("on "));
    DOUT_PinSetOn(DIN_ConfigMap[DIN_ENVELOPES].dout_pin);
  }
  else
  {
    lcd.print(F("off"));
    DOUT_PinSetOff(DIN_ConfigMap[DIN_ENVELOPES].dout_pin);
  }

  lcd.setCursor(13, 1);
  lcd.print(filter_ratio, DEC);

  lcd.setCursor(17, 1);
  if (mThru_XCc)
  {
    lcd.print(F(" * "));
    DOUT_PinSetOn(DIN_ConfigMap[DIN_MATRIX].dout_pin);
  }
  else
  {
    lcd.print(F(" . "));
    DOUT_PinSetOff(DIN_ConfigMap[DIN_MATRIX].dout_pin);
  }
}

void ui_page_cfg_misc_format()
{
  String line1 = F("FORMAT EEPROM :  ENC");
  String line2 = F("Ext Int Init Sys ");
  LCD_Clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);

  lcd.setCursor(17, 1);
  if (encoder_inverted)
  {
    lcd.print(F("INV"));
    DOUT_PinSetOff(DIN_ConfigMap[DIN_MATRIX].dout_pin);
  }
  else
  {
    lcd.print(F("NOR"));
    DOUT_PinSetOn(DIN_ConfigMap[DIN_MATRIX].dout_pin);
  }
}


void handle_page_cfg_misc()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE:
      if (Shift)
        SoftPanel.Page = SOFT_PAGE31; // hidden feature FORMAT()
      else
      {
        EEPROM.update(EEPROM_SYS_CLK, systmClock);
        SoftPanel.Page = SOFT_PAGE1; // return to beginning of CFG
      }
      break;

    case SOFT_EDIT_1: // set midi clock for system
      if (++systmClock > MTRGCLK)
        systmClock = INTCLK; // can't be zero
      break;

    case SOFT_EDIT_2: // ext clock
      zActive = !zActive;
      EEPROM.update(EEPROM_Z_ACTIVE, zActive);
      break;

    case SOFT_EDIT_3: // midi local control of Ctrlr
      localControl ^= 1;
      EEPROM.update(EEPROM_LOCAL_CONTROL, localControl);
      break;

    case SOFT_EDIT_4: // edit potentiometers jitter "filter_ratio"
      if (++filter_ratio > 9)
        filter_ratio = 1;
      EEPROM.update(EEPROM_FILTER_RATIO, filter_ratio);
      break;

    case SOFT_EDIT_5: // Set A/B : deviceA is arp, device B is Seq
      mThru_XCc = !mThru_XCc;
      EEPROM.update(EEPROM_MTHRU_XCC, mThru_XCc);
      break;
  }
  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}

void handle_page_cfg_misc_format()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE: SoftPanel.Page = SOFT_PAGE1; break;
    case SOFT_EDIT_1:
      FORMAT_Memory(1);
      lcd.setCursor(0, 1);
      lcd.print(F("Ext.EEPROM formatted "));
      elapsedTime = 0;
      break;
    case SOFT_EDIT_2:
      FORMAT_Memory(2);
      lcd.setCursor(0, 1);
      lcd.print(F("Int.EEPROM formatted "));
      elapsedTime = 0;
      break;
    case SOFT_EDIT_3:
      FORMAT_Memory(3);
      lcd.setCursor(0, 1);
      lcd.print(F(" Patch 000-999 ready "));
      elapsedTime = 0;
      break;
    case SOFT_EDIT_4:
      FORMAT_Memory(4);
      lcd.setCursor(0, 1);
      lcd.print(F(" Sys & Masters ready "));
      elapsedTime = 0;
      break;
    case SOFT_EDIT_5:
      encoder_inverted = !encoder_inverted;
      EEPROM.update(EEPROM_ENCODER_INVERTED, encoder_inverted);
      break;
  }

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}
