
#include "chaosmatrix.h"
#include "softpanel.h"
#include "lcd.h"
#include "din.h"
#include "midi.h"
#include "device.h"
#include <EEPROM.h>

/////////////////////////////////////////////////////////////////////////////
//  Display patch pages, page 1 for bank and program changes,
//  page 2 for MIDI channel setup, edit buffer init and patch compare//
// page3 for Bank dump from Matrix 1000
/////////////////////////////////////////////////////////////////////////////
void UI_Display_Patch(void)
{
  switch (SoftPanel.Page)
  {
    case SOFT_PAGE1: ui_page_patch_main(); break;
    case SOFT_PAGE2: ui_page_patch_get(); break;
    case SOFT_PAGE3: ui_page_patch_save(); break;
    case SOFT_PAGE4: ui_page_patch_copy_paste(); break;
  }
}

/////////////////////////////////////////////////////////////////////////////
//  Send MIDI for the patch pages, display patch name
/////////////////////////////////////////////////////////////////////////////
void UI_Handle_Patch(void)
{
switch (SoftPanel.Page) {
    case SOFT_PAGE1: handle_page_patch_main(); break;
    case SOFT_PAGE2: handle_page_patch_get(); break;
    case SOFT_PAGE3: handle_page_patch_save(); break;
    case SOFT_PAGE4: handle_page_patch_copy_paste(); break;
  }
}
