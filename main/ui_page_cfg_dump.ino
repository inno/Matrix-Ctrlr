#include "lcd.h"

/*
  []  []   [ ]  []  []
  01234567890123456789
*/

void ui_page_cfg_dump_main()
{
  String line1 = F("DUMP CTRLR MEMORY : ");
  DOUT_PinSetOff(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_ARP].dout_pin);
  DOUT_PinSet_Keypanel(1, 0, 1, 1, 1, 1);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_CFG].dout_pin);

  LCD_Clear();
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(F("Bank"));
  LCD_PrintBCD2(uBank[device]);
  lcd.setCursor(9, 1);
  lcd.print(F("Sys Mst All"));
}

void handle_page_cfg_dump()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE: SoftPanel.Page = SOFT_PAGE1; break;
    case SOFT_EDIT_1:
      // dump bank selected in Patch to CORE OUT & selected device
      DumpCtrlrBank(INTERFACE_MIDI_CORE, uBank[device]);
      DumpCtrlrBank(INTERFACE_SERIAL, uBank[device]);
      break;
    case SOFT_EDIT_2: break; // bank dump
    case SOFT_EDIT_3: SendCtrlrSystemCfg(INTERFACE_MIDI_CORE); break;
    case SOFT_EDIT_4:
      // dump Global Parameters to CORE OUT & selected device
      SendGlobalParameters(INTERFACE_MIDI_CORE);
      SendGlobalParameters(INTERFACE_SERIAL);
      break;
    case SOFT_EDIT_5:
      // dump ALL Banks and Global parameters to CORE OUT & selected device
      for (unsigned char i = 0; i <= uBANK_MAX; ++i)
      {
        // specify bank (in sysex) first:
        MIDI_SetBank(INTERFACE_MIDI_CORE, i);
        // then dump 100 patches on core out
        DumpCtrlrBank(INTERFACE_MIDI_CORE, i);
      }
      // ends with Globals
      SendGlobalParameters(INTERFACE_MIDI_CORE);
      SendCtrlrSystemCfg(INTERFACE_MIDI_CORE);
      break;
  }

  if (SoftPanel.EncoderValue != 0 && SoftPanel.EditButton == Edit2)
    SetEncoderValue(&uBank[device], uBANK_MAX);

  // realtime display feature
  app_flags.Display_DIN_Req = 1;
}
