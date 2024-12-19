/*
  []  []   [ ]  []  []
  01234567890123456789
*/

unsigned char uBank[4];
unsigned char uPatch[4];
unsigned char tBank; // temporary - when scrolling w/ encoder
unsigned char tPatch;
unsigned char ProgramNumberReq;
unsigned char BankNumberReq;
unsigned char BankNumberDump;
static bool copy, paste;

void ui_page_patch_main()
{
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_FILTER].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_ENVELOPES].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_KEYBOARD].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_MATRIX].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PAGE].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_CFG].dout_pin);

  //1st line
  LCD_setAndPrint(0, 0, F("PATCH  "));
  switch (device)
  {
    case MATRIX_DEVICE_A: lcd.print(F("A")); break;
    case MATRIX_DEVICE_B: lcd.print(F("B")); break;
#if SOFTSERIAL_ENABLED
    case MATRIX_DEVICE_C: lcd.print(F("C")); break;
    case MATRIX_DEVICE_D: lcd.print(F("D")); break;
#endif
  }
  if (elapsedTime < 100)
  {
    LCD_PrintBCD1(tBank);
    LCD_PrintBCD2(tPatch);
    if (tPatch < 10)
      LCD_setAndPrint(9, 0, F("0"));
  }
  else
  {
    LCD_PrintBCD1(uBank[device]);
    LCD_PrintBCD2(uPatch[device]);
    if (uPatch[device] < 10)
      LCD_setAndPrint(9, 0, F("0"));

    // reset
    tBank = uBank[device];
    tPatch = uPatch[device];
  }

  // display patchname
  lcd.setCursor(11, 0);
  // temporary display of the name
  if (elapsedTime < 100)
  { // affiche temporaire 500 millisec
    LCD_PrintCString(F(" "));
    for (unsigned char j = 0; j < 8; j++)
      LCD_PrintChar(ToneName[j]);
  }
  else
  {
    LCD_PrintCString(F(" "));
    for (unsigned char j = 0; j < 8; j++)
    {
      if (EditBuffer[device][j] < 0x20) // cf ASCII tables
        EditBuffer[device][j] = EditBuffer[device][j] + 0x40; // +64 (0x40) compatible with Matrix 1000 patch names

      if (EditBuffer[device][j] < 0x20) // cf ASCII tables
        EditBuffer[device][j] = EditBuffer[device][j] + 0x40; // +64 (0x40) compatible with Matrix 1000 patch names
      LCD_PrintChar(EditBuffer[device][j]);
    }
  }

  // 2nd line
  LCD_setAndPrint(0, 1, F("GET INIT ORG EDT SAV"));
}

void ui_page_patch_get()
{
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_FILTER].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_ENVELOPES].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_KEYBOARD].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_MATRIX].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PAGE].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_CFG].dout_pin);

  //1st line
  LCD_Clear();
  LCD_setAndPrint(0, 0, F("mBank:"));
  LCD_PrintBCD1(BankNumberDump);
  lcd.setCursor(8, 0);
  lcd.write(255);
  lcd.write(255);
  lcd.write((byte)0);
  LCD_setAndPrint(12, 0, F(" Bank"));
  LCD_PrintBCD2(uBank[device]);
  //2nd line
  lcd.setCursor(0, 1);
  lcd.write((byte)6);
  lcd.setCursor(4, 1);
  lcd.write((byte)7);
  LCD_setAndPrint(10, 1, F("GET "));
  lcd.setCursor(16, 1);
  lcd.write((byte)6);
  lcd.setCursor(19, 1);
  lcd.write((byte)7);

  if (MIDI_ReceivingBank)
  {
    LCD_setAndPrint(0, 1, F("Getting MTRX Bank "));
    LCD_PrintBCD1(BankNumberDump);
    lcd.print(F(" "));
  }
}

void ui_page_patch_save()
{
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_FILTER].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_ENVELOPES].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_KEYBOARD].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_MATRIX].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PAGE].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_CFG].dout_pin);

  //1st line
  LCD_Clear();
  // patch number of M1000
  LCD_setAndPrint(1, 0, F("m"));
  LCD_PrintBCD1(BankNumber);
  LCD_PrintBCD2(ProgramNumber);
  if (ProgramNumber < 10)
    LCD_setAndPrint(3, 0, F("0"));

  // name of patch
  lcd.setCursor(6, 0);
  for (unsigned char j = 0; j < 8; j++)
  {
    // cf ASCII tables
    if (EditBufferOrig[j] < 0x20)
      EditBufferOrig[j] = EditBufferOrig[j] + 0x40; // +64 (0x40) compatible with Matrix 1000 patch names
    LCD_PrintChar(EditBufferOrig[j]);
  }
  // storage destination
  lcd.setCursor(15, 0);
  LCD_PrintBCD1(uBank[device]);
  LCD_PrintBCD2(uPatch[device]);
  if (uPatch[device] < 10)
    LCD_setAndPrint(16, 0, F("0"));

  //2nd line
  lcd.setCursor(1, 1);
  lcd.write((byte)6);
  lcd.setCursor(5, 1);
  lcd.write((byte)7);
  LCD_setAndPrint(8, 1, F("SAVE"));
  lcd.setCursor(14, 1);
  lcd.write((byte)6);
  lcd.setCursor(18, 1);
  lcd.write((byte)7);
}

void ui_page_patch_copy_paste()
{
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PATCH].dout_pin);
  DOUT_PinSetOn(DIN_ConfigMap[DIN_PAGE].dout_pin);
  DOUT_PinSetOff(DIN_ConfigMap[DIN_EDIT].dout_pin);

  LCD_Clear();
  LCD_setAndPrint(0, 0, F("COPY & PASTE :      "));
  LCD_setAndPrint(0, 1, F("Cpy|Pst> Edt Arp Seq"));
  DOUT_PinSet(DIN_ConfigMap[DIN_OSCILLATORS].dout_pin, copy);
  DOUT_PinSet(DIN_ConfigMap[DIN_FILTER].dout_pin, paste);
}

void handle_page_patch_main()
{
  if (Shift)
    tBank += SoftPanel.EncoderValue;
  else
    tPatch += SoftPanel.EncoderValue;

  if (tPatch > 200)
  {
    tPatch = uPATCH_MAX;
    tBank--;
  }

  if (tPatch > uPATCH_MAX)
  {
    tPatch = 0;
    tBank++;
  }

  if (tBank > 200)
    tBank = uBANK_MAX;

  if (tBank > uBANK_MAX)
    tBank = 0;

#if DEBUG_softpanel
  Serial.print(F("uPatch = "));
  Serial.println(uPatch[device]);
#endif

  // insérer ici la lecture du nom du tone en page B.PP (T.Heckmann)
  ToneName_Load(tBank, tPatch);
  elapsedTime = 0; // for temporary msg
  UI_Display_Patch(); // to update patch name on display

  // functions available on page 1
  switch (SoftPanel.Button)
  {
    case DIN_PAGE: SoftPanel.Page = SOFT_PAGE2; break;
    case SOFT_EDIT_F1:
      if (Shift)
      {
        if (ui_aHold)
          Release_aChordLatch(ui_aHold);
        ui_aHold = !ui_aHold;
      }
      else
      {
        router_arp_tag = !router_arp_tag;
        if (router_arp_tag == 0)
          active_arp = false;
      }
      break;

    case SOFT_EDIT_F2:
      if (Shift)
      {
        ui_toggleSeq = !ui_toggleSeq;
        if (seqPushedKey != 255)
          HandleNoteOff(seqPushedKey, 0x00, MIDI_CHANNEL);
      }
      else
      {
        ui_seqPlay = !ui_seqPlay;
        seqTick = 0;
      }
      break;

    case SOFT_EDIT_1: // set & request patch from the Matrix1000
      MIDI_RequestEditBuffer(device, INTERFACE_SERIAL, uPatch[device]);
      break;

    case SOFT_EDIT_2: // init patch in EditBuffer
      InitEditBuffer();
      ARP_GLOBAL_INIT(device);
      if (localControl)
      {
        SendEditBuffer(device, INTERFACE_SERIAL);
        MIDI_Send_UNISONDETUNE(INTERFACE_SERIAL, 0);
      }
      SendEditBuffer(device, INTERFACE_MIDI_CORE);
      MIDI_Send_UNISONDETUNE(INTERFACE_MIDI_CORE, 0);
      break;

    case SOFT_EDIT_3: // send Orig
      if (localControl)
      {
        SendEditBufferOrig(INTERFACE_SERIAL);
        MIDI_Send_UNISONDETUNE(INTERFACE_SERIAL, UnisonDetuneOrig);
      }
      SendEditBufferOrig(INTERFACE_MIDI_CORE);
      UpdateDinStates();
      MIDI_Send_UNISONDETUNE(INTERFACE_MIDI_CORE, UnisonDetuneOrig);
      break;

    case SOFT_EDIT_4: // send edit
      if (Shift)
        WizardEditBuffer(device, 6);

      if (localControl)
      {
        SendEditBuffer(device, INTERFACE_SERIAL);
        MIDI_Send_UNISONDETUNE(INTERFACE_SERIAL, UnisonDetune[device]);
      }
      // send EditBuffer[device] to core out for copy/past in MidiMonitor.app if necessary
      SendEditBuffer(device, INTERFACE_MIDI_CORE);
      UpdateDinStates();
      MIDI_Send_UNISONDETUNE(INTERFACE_MIDI_CORE, UnisonDetune[device]);
      break;

    case SOFT_EDIT_5: // SAV
      LCD_setAndPrint(0, 0, F("EditBuffer, Unison, "));
      LCD_setAndPrint(0, 1, F("Arp & Seq saved !   "));
      elapsedTime = 300; // temp msg

      ArpParameters_Store(device); // copy the ui_ in buffer[device] done in WritePatchtoBS() below
      Write_Patch_To_BS(device, uBank[device], uPatch[device]); // save patch buffer in BS
      Store_LastBankPatch_to_EEPROM(device, uBank[device], uPatch[device]); // save device & patch numbers in eeprom
      //Read_Patch_From_BS(device, uBank[device], uPatch[device]); // reload what's just been saved
      UpdateDinStates(); // mise à jour des Leds
      // send EditBuffer[device] to core out for copy/past in MidiMonitor.app if necessary
      SendEditBuffer(device, INTERFACE_MIDI_CORE);
      MIDI_Send_UNISONDETUNE(INTERFACE_SERIAL, UnisonDetune[device]);
      break;

    case SOFT_EDIT_INC: // increment 000-->001  099-->100 :: OK :)
      uPatch[device]++;
      uPatch[device] = uPatch[device] + Shift;
      if (uPatch[device] > uPATCH_MAX)
      {
        uPatch[device] = 0;
        if (++uBank[device] > uBANK_MAX)
          uBank[device] = 0;
      }
      tBank = uBank[device];
      tPatch = uPatch[device];

      if (localControl)
      {
        ARP_GLOBAL_INIT(device);
        Reset_UI_ARP(); // stop arp parameters

        Read_Patch_From_BS(device, uBank[device], uPatch[device]); // read into BS
        UpdateDinStates(); // mise à jour des Leds
        SendEditBuffer(device, INTERFACE_SERIAL); // send edit buffer :)
        // send Unison detune value :
        MIDI_Send_UNISONDETUNE(INTERFACE_SERIAL, UnisonDetune[device]);
        // load arp parameters
        ArpParameters_Load(device);

        // send EditBuffer[device] to core out
        SendEditBuffer(device, INTERFACE_MIDI_CORE);
        MIDI_Send_UNISONDETUNE(INTERFACE_MIDI_CORE, UnisonDetune[device]);
      }
      // send Bank et Patch number & EditBuffer[device] to Core  Midi out :
      MIDI_SendPatchNumber(INTERFACE_MIDI_CORE, uBank[device], uPatch[device]);

      break;

    case SOFT_EDIT_DEC: // decrement 000-->3199 100-->099 :: OK :)
      uPatch[device] = uPatch[device] - Shift - 1;
      if (uPatch[device] == 255)
      {
        uPatch[device] = uPATCH_MAX;
        if (--uBank[device] == 255)
          uBank[device] = uBANK_MAX;
      }
      tBank = uBank[device];
      tPatch = uPatch[device];
      if (localControl)
      {
        ARP_GLOBAL_INIT(device);
        Reset_UI_ARP(); // stop arp parameters
        Read_Patch_From_BS(device, uBank[device], uPatch[device]); // read into BS
        UpdateDinStates(); // mise à jour des Leds
        SendEditBuffer(device, INTERFACE_SERIAL);
        // send Unison detune value TO DO :
        MIDI_Send_UNISONDETUNE(INTERFACE_SERIAL, UnisonDetune[device]);
        // load arp parameters
        ArpParameters_Load(device);
        // send EditBuffer[device] to core out
        SendEditBuffer(device, INTERFACE_MIDI_CORE);
        MIDI_Send_UNISONDETUNE(INTERFACE_MIDI_CORE, UnisonDetune[device]);
      }
      // send Bank et Patch number to Core  Midi out
      MIDI_SendPatchNumber(INTERFACE_MIDI_CORE, uBank[device], uPatch[device]);

      break;

    case DIN_SHIFT:
      // nope
      break;

    case SOFT_EDIT_ENC_CLIC: // encoder clic : load patch
#if DEBUG_encoder
      Serial.println(F("SOFT_EDIT_ENC_CLIC load patch"));
#endif
      if (localControl)
      {
        ARP_GLOBAL_INIT(device);
        Reset_UI_ARP(); // stop arp parameters
        if (elapsedTime < 100)
        {
          uBank[device] = tBank;
          uPatch[device] = tPatch;
        }
        else
        {
          tBank = uBank[device];
          tPatch = uPatch[device];
        }
        Read_Patch_From_BS(device, uBank[device], uPatch[device]); // read into BS
        UpdateDinStates(); // mise à jour des Leds
        SendEditBuffer(device, INTERFACE_SERIAL);
        MIDI_Send_UNISONDETUNE(INTERFACE_SERIAL, UnisonDetune[device]); // send Unison detune value
        ArpParameters_Load(device); // load arp parameters
        SendEditBuffer(device, INTERFACE_MIDI_CORE); // send  to core out
        MIDI_Send_UNISONDETUNE(INTERFACE_MIDI_CORE, UnisonDetune[device]);
        UI_Display_Patch(); // to update patch name on display
      }
      // send Bank et Patch number & EditBuffer[device] to Core  Midi out :
      MIDI_SendPatchNumber(INTERFACE_MIDI_CORE, uBank[device], uPatch[device]);

      break;
  }
}

void handle_page_patch_get()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE:
      MIDI_ReceivingBank = false;
      SoftPanel.Page = SOFT_PAGE3; // goto page 3
      break;

    case SOFT_EDIT_1: // bank decrement
      if (--BankNumberDump == 255)
        BankNumberDump = 9;
      MIDI_SetBank(INTERFACE_SERIAL, BankNumberDump);
      break;

    case SOFT_EDIT_2: // bank increment
      if (++BankNumberDump > 9)
        BankNumberDump = 0;
      MIDI_SetBank(INTERFACE_SERIAL, BankNumberDump);
      break;

    case SOFT_EDIT_3: // click this button to store mBank into uBS
      // tell system we will receive a bank:
      MIDI_ReceivingBank = true; // toggle state
      MIDI_SetBank(INTERFACE_SERIAL, BankNumberDump);
      MIDI_BankRequest(INTERFACE_SERIAL, BankNumberDump);
      // then store is done automatically in router.ino Handle(sysex)
      break;

    case SOFT_EDIT_4: // decrement BS
      if (uBank[device] == 0)
        uBank[device] = uBANK_MAX;
      else
        uBank[device]--;
      break;

    case SOFT_EDIT_5: // increment BS
      if (++uBank[device] > uBANK_MAX)
        uBank[device] = 0;
      break;
  }
}

void handle_page_patch_save()
{
  switch (SoftPanel.Button)
  {
    case DIN_PAGE: SoftPanel.Page = SOFT_PAGE4; break;
    case SOFT_EDIT_1: // decrement patch & bank
      if (--ProgramNumber == 255)
      {
        ProgramNumber = uPATCH_MAX;
        BankNumber--;
      }
      if (BankNumber == 255)
        BankNumber = uBANK_MAX;
      if (matrix_modele == MATRIX_6)
      { // and ask corresponding Matrix patch using a SysEx msg
        MIDI_SendPatchProgram(INTERFACE_SERIAL, ProgramNumber);
        MIDI_RequestSinglePatch(INTERFACE_SERIAL, ProgramNumber);
      }
      else
      {
        // and ask corresponding Matrix patch using a SysEx msg
        MIDI_SetBank(INTERFACE_SERIAL, BankNumber);
        MIDI_RequestSinglePatch(INTERFACE_SERIAL, ProgramNumber);
      }
      break;

    case SOFT_EDIT_2: // increment patch & bank
      if (++ProgramNumber > uPATCH_MAX)
      {
        ProgramNumber = 0;
        BankNumber++;
      }
      if (BankNumber > uBANK_MAX)
        BankNumber = 0;
      if (matrix_modele == MATRIX_6)
      {
        MIDI_SendPatchProgram(INTERFACE_SERIAL, ProgramNumber);
        MIDI_RequestSinglePatch(INTERFACE_SERIAL, ProgramNumber);
      }
      else
      {
        MIDI_SetBank(INTERFACE_SERIAL, BankNumber);
        MIDI_RequestSinglePatch(INTERFACE_SERIAL, ProgramNumber); // 0.99 marche tres bien ne pas changer
      }
      break;

    case SOFT_EDIT_3: // click this button to store a patch sent by the Matrix1000 in the Ctrlr memory
      // set arp, uni & seq to default
      Set_OrigArpUniSeq_toDefault();
      // then save
      Write_Patch_To_BS(device, uBank[device], uPatch[device]);
      break;

    case SOFT_EDIT_4: // decrement BP
      if (--uPatch[device] == 255)
      {
        uPatch[device] = uPATCH_MAX;
        uBank[device]--;
      }
      if (uBank[device] == 255)
        uBank[device] = uBANK_MAX;

      if (uBank[device] > uBANK_MAX)
        uBank[device] = 0;
      break;

    case SOFT_EDIT_5: // increment BP
      if (++uPatch[device] > uPATCH_MAX)
      {
        uPatch[device] = 0;
        uBank[device]++;
      }
      if (uBank[device] == 255)
        uBank[device] = uBANK_MAX;

      if (uBank[device] > uBANK_MAX)
        uBank[device] = 0;
      break;

    case SOFT_EDIT_ENC_CLIC: // encoder clic : load patch
      if (matrix_modele == MATRIX_6)
      { // and ask corresponding Matrix patch using a SysEx msg
        MIDI_SendPatchProgram(INTERFACE_SERIAL, ProgramNumber);
        MIDI_RequestSinglePatch(INTERFACE_SERIAL, ProgramNumber);
      }
      else
      {
        // and ask corresponding Matrix patch using a SysEx msg
        MIDI_SetBank(INTERFACE_SERIAL, BankNumber);
        MIDI_RequestSinglePatch(INTERFACE_SERIAL, ProgramNumber);
      }
      break;
  }

  // encoder
  if (Shift)
  {
    uPatch[device] += SoftPanel.EncoderValue;
    if (uPatch[device] > uPATCH_MAX)
    {
      uPatch[device] = 0;
      uBank[device]++;
    }

    if (uPatch[device] == 255)
    {
      uPatch[device] = uPATCH_MAX;
      uBank[device]--;
    }

    if (uBank[device] == 255)
      uBank[device] = uBANK_MAX;

    if (uBank[device] > uBANK_MAX)
      uBank[device] = 0;
  }
  else
  {
    ProgramNumber += SoftPanel.EncoderValue;
    if (ProgramNumber > uPATCH_MAX)
    {
      ProgramNumber = 0;
      BankNumber++;
    }
    if (BankNumber > uBANK_MAX)
      BankNumber = 0;

    if (ProgramNumber == 255)
    {
      ProgramNumber = uPATCH_MAX;
      BankNumber--;
    }
    if (BankNumber == 255)
      BankNumber = uBANK_MAX;
  }
}

void handle_page_patch_copy_paste()
{
  switch (SoftPanel.Button)
  {
    // ...wut? This doesn't do anything...?
    case DIN_PAGE: SoftPanel.Page = SOFT_PAGE1; break;
  }
}
