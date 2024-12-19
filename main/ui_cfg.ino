#include "chaosmatrix.h"
#include "softpanel.h"
#include "din.h"
#include "lcd.h"
#include "memo.h"
#include "midi.h"
#include "ui_matrix.h"
#include "device.h"
#include "define.h"
#include "ui_filter.h"
#include "oner.h"
#include "uclock.h"

void ui_page_cfg_main()
{
  String line1 = F("CONFIGURE:          ");
  String line2 = F("VIB MID MISC GRP DMP");
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSet_Keypanel(1, 1, 1, 1, 1, 0);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);

  LCD_Clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void handle_page_cfg_main()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE   : break;
    case SOFT_EDIT_1: SoftPanel.Page = SOFT_PAGE11; break;
    case SOFT_EDIT_2: SoftPanel.Page = SOFT_PAGE2; break;
    case SOFT_EDIT_3: SoftPanel.Page = SOFT_PAGE3; break;
    case SOFT_EDIT_4: SoftPanel.Page = SOFT_PAGE4; break;
    case SOFT_EDIT_5: SoftPanel.Page = SOFT_PAGE5; break;
  }

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}

void UI_Display_Cfg()
{
  switch (SoftPanel.Page)
  {
    case SOFT_PAGE1 : ui_page_cfg_main(); break;
    case SOFT_PAGE11: ui_page_cfg_vib_1(); break;
    case SOFT_PAGE12: ui_page_cfg_vib_2(); break;
    case SOFT_PAGE2 : ui_page_cfg_midi_main(); break;
    case SOFT_PAGE21: ui_page_cfg_midi_1(); break;
    case SOFT_PAGE22: ui_page_cfg_midi_2(); break;
    case SOFT_PAGE3 : ui_page_cfg_misc(); break;
    case SOFT_PAGE31: ui_page_cfg_misc_format(); break;
    case SOFT_PAGE4 : ui_page_cfg_group(); break;
    case SOFT_PAGE5 : ui_page_cfg_dump_main(); break;
  }
}

void UI_Handle_Cfg()
{
  switch (SoftPanel.Page)
  {
    case SOFT_PAGE1 : handle_page_cfg_main(); break;
    case SOFT_PAGE11: handle_page_cfg_vib_1(); break;
    case SOFT_PAGE12: handle_page_cfg_vib_2(); break;
    case SOFT_PAGE2 : handle_page_cfg_midi_main(); break;
    case SOFT_PAGE21: handle_page_cfg_midi_1(); break;
    case SOFT_PAGE22: handle_page_cfg_midi_2(); break;
    case SOFT_PAGE23: handle_page_cfg_midi_3(); break; // XXX Is this ever actually called?
    case SOFT_PAGE3 : handle_page_cfg_misc(); break;
    case SOFT_PAGE31: handle_page_cfg_misc_format(); break;
    case SOFT_PAGE4 : handle_page_cfg_group(); break;
    case SOFT_PAGE5 : handle_page_cfg_dump(); break;
  }
}
