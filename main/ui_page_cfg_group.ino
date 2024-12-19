#include "lcd.h"

/*
  []  []   [ ]  []  []
  01234567890123456789
*/
void ui_page_cfg_group()
{
  String line1 = F("CFG : GRouP (page 4)");
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_ARP].dout_pin);
  DOUT_PinSet_Keypanel(0, 0, 0, 0, 0, 1);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);

  LCD_Clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  switch (ui_groupMode)
  {
    case GROUPSOLO     : lcd.print(F("Solo")); break;
    case GROUPDUO      : lcd.print(F("Duo ")); break;
    case GROUPTRIO     : lcd.print(F("Trio")); break;
    case GROUPQUARTET  : lcd.print(F("Quar")); break;
    case GROUPDOUBLEDUO: lcd.print(F("2Duo")); break;
    default            : lcd.print(F("none")); break;
  }
}

void handle_page_cfg_group()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE: SoftPanel.Page = SOFT_PAGE1; break;
    case SOFT_EDIT_1:
      if (++ui_groupMode > 5)
        ui_groupMode = 0;

      // update system groupMode value
      groupMode = ui_groupMode;
      break;
  }
  // encoder : set groupMode
  ui_groupMode += SoftPanel.EncoderValue;
  if (ui_groupMode > 5)
    ui_groupMode = 0;

  // update system groupMode value
  groupMode = ui_groupMode;

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}
