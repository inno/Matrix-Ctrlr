#include "ui_arp.h"
#include "arp.h"
#include "seq.h"

#define seqLengthMAX 32

// sequence pattern populated with zeros :
unsigned char sequence[32][2];
unsigned char sequenceOrig[32][2];
unsigned char seqNbr;
unsigned char seqLength;
unsigned char seqRecStep;
unsigned char seqPlayStep;
unsigned char seqOption; // false, rest or tie
unsigned char seqTick;
byte TrspB;
bool ui_seqRec; // true or false, used in router.c and in ui_arp.ino
bool ui_seqPlay; // true or false, used in router and ui_arp
bool seqTrig; // sequence trigger using note On/Off
bool seqMonoMode; // one note at a time on the keyboard while we play a sequence
unsigned char seqSpeed; // 0 - 255
unsigned char sGate;
unsigned char playSeqTrigger[4]; // pitch, velo, channel, seqOption provided by NoteOn in router.ino
signed char skipSeqStep;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NOTA recording a sequence :
//    1 : initialise the sequence when you press the 'rec mode' button
//    2 : fill the array by entering pitches with your keyboard on the right midi channel (need a routing in router while noteOn/Off}
//    3 : define the length by the quantity of received pitches
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// sequence Reset (or Init)
/////////////////////////////////////////////////////////////////////////////////////////
void Init_Seq(void)
{
  MIDI_A.sendControlChange(123, 0, MIDI_CHANNEL); // all notes off msg

  //  seqSpeed = 3;

  // reset to default array
  for (unsigned char i = 0; i < 32; ++i)
  {
    sequence[i][0] = pgm_read_byte_near(&Default_Sequence[1][i][0]);
    sequence[i][1] = pgm_read_byte_near(&Default_Sequence[1][i][1]);
  }

  // reset seq length to zero
  seqLength = 32;
  seqRecStep = 0;

#if DEBUG_SEQ
  Serial.println(F("sequence init"));
  SerialPrintSeq();
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
// Load default seq
/////////////////////////////////////////////////////////////////////////////////////////
void DefaultSeq_Load(unsigned char index)
{
  MIDI_A.sendControlChange(123, 0, MIDI_CHANNEL); // all notes off msg

  // set to default array depending of index
  for (unsigned char i = 0; i < 32; ++i)
  {
    sequence[i][0] = pgm_read_byte_near(&Default_Sequence[index][i][0]);
    sequence[i][1] = pgm_read_byte_near(&Default_Sequence[index][i][1]);
  }

  seqSpeed = ArpParametersOrig[18];

  // reset seq length to zero
  seqLength = 32;
  seqRecStep = 0;

#if DEBUG_SEQ
  Serial.println(F("DefaultSeq_Load()"));
  SerialPrintSeq();
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
// print sequence array on serial port (DEBUG)
/////////////////////////////////////////////////////////////////////////////////////////
void SerialPrintSeq(void)
{
#if DEBUG_SEQ
  Serial.println(F("SeqStack = {"));
  for (unsigned char i = 0; i < 32; ++i)
  {
    Serial.print(F("("));
    Serial.print(sequence[i][0], HEX);
    Serial.print(F(","));
    Serial.print(sequence[i][1], HEX);
    Serial.print(F("),"));
  }
  Serial.println(F("};"));
  Serial.print(F("SeqLength : "));
  Serial.println(seqLength, DEC);
  Serial.println();

#endif
}

/////////////////////////////////////////////////////////////////////////////////////
// Populate the sequnce array with pitch, velocity and seqOption (rest or tie)
//////////////////////////////////////////////////////////////////////////////////////
void Rec_Seq(byte pitch, byte velo, byte channel,
  unsigned char opt) // 0.97a c'est bon tout marche pour remplir le tableau
{
  if (ui_seqRec && (channel == MIDI_CHANNEL)) // if we record the sequence and receive on right channel
  {
    // we can't play sequence
    ui_seqPlay = false;

    // each time is called that function :
    // we receive notes to populate the sequence array :
    // notice that seqRecStep starts at 0
    sequence[seqRecStep][0] = pitch; // 1st element is note number
    sequence[seqRecStep][1] = velo; // 2nd element is velocity of that note,rest or tie

    // you added a note to the sequence, i.e that its sequence length increments for the next loop
    ++seqRecStep;

    // seqLength is (secReqStep - 1) as we just incremented
    seqLength = seqRecStep; // ou - 1  ?

    if (seqLength > 32)
    {
      // we have reach the end of the seq
      // do we initialise the array or keep for further modification ?
      Init_Seq();
      // i choose to init, but we could imagine navigate inside the steps with +/- keys in order to change pitch values on specific step
      // TO DO & IMPROVE
      seqLength = seqRecStep = 0; // rtz if higher than max
    }

#if DEBUG_SEQ
    SerialPrintSeq();
#endif

    // update display lcd :
    if (SoftPanel.Mode == Arp)
      UI_Display_Arp();
  }
  else if (ui_seqRec == 0) // if we don't record the sequence
  {
    seqLength = 0;
  }

  else
    return;
}
/////////////////////////////////////////////////////////////////////////////////////////
// insert fake element in Seq Array (silent note) or Tie (liaison)
/////////////////////////////////////////////////////////////////////////////////////////
void Insert_SeqRest(unsigned char param)
{
#if DEBUG_SEQ
  Serial.print(F("param = "));
  Serial.println(param, DEC);
#endif

  if (ui_seqRec) // if we record the sequence and receive on right channel
  {
    // we can't play sequence
    ui_seqPlay = false;

    sequence[seqRecStep][0] = 60; // 1st element is note number
    sequence[seqRecStep][1] = param; // 2nd element is velocity of that note, rest or tie i.e 0, 1 or 2

    // you added a note to the sequence, i.e that its sequence length increments for the next loop
    ++seqRecStep;

    // seqLength is (secReqStep - 1) as we just incremented
    seqLength = seqRecStep; // ou - 1  ?

    if (seqLength > 32)
    {
      // we have reach the end of the seq
      // do we initialise the array or keep for further modification ?
      Init_Seq();
      // i choose to init, but we could imagine navigate inside the steps with +/- keys in order to change pitch values on specific step
      // TO DO & IMPROVE
      seqLength = seqRecStep = 0; // rtz if higher than max
    }

#if DEBUG_SEQ
    SerialPrintSeq();
#endif

    // update display lcd :
    if (SoftPanel.Mode == Arp)
      UI_Display_Arp();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
// used to play the seq
/////////////////////////////////////////////////////////////////////////////////////////
void Play_Seq(byte pitch, byte velocity, byte channel, bool type)
{
  if (ui_seqPlay && (channel == MIDI_CHANNEL))
  {
    // we can't record :
    ui_seqRec = false;

    seqMonoMode = !seqMonoMode; // change state bool

#if DEBUG_SEQ
    Serial.print(F("seqMonoMode = "));
    Serial.println(seqMonoMode);
#endif

    seqTick = 0; // reset du sequencer tick

    playSeqTrigger[0] = pitch;
    playSeqTrigger[1] = velocity;
    playSeqTrigger[2] = channel;
    playSeqTrigger[3] = type; // = seqTrig (note on/off = true/false)
  }
  else
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// similar to ARP(), called on each tick of the clock 0xf9 (int or ext)
/////////////////////////////////////////////////////////////////////////////////////////////
void SEQ(void)
{
  //unsigned char lastPlayedSeqStep=0;
  // static unsigned char sGate;
  static unsigned char skipBack;

  if (seqTick == 0)
  {
#if DEBUG_SEQTIK
    Serial.print(F("seqTick = "));
    Serial.println(seqTick);
#endif

    // update parameters :
    sGate = seqSpeed;
    TrspB = ui_TrspB;
    seqPlayStep = seqPlayStep + skipSeqStep;

    // extract an elemnt E contained in sequence, and play it as NoteOn
    // apres une certaine temporisation (correspond à la resolution):
    if (ui_seqPlay && seqTrig)
    {
      // skip do nothing
      if (sequence[seqPlayStep][1] == 0) {}
      // if a silent note or a tie do nothing
      else if (sequence[seqPlayStep][1] > 127)
        skipBack++;
      else // else normal note
      {
        MIDI_A.sendNoteOn(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
          sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);
        if (arp_send_notes)
          MIDI_CORE.sendNoteOn(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
            sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);

        // update display lcd :
        if (SoftPanel.Mode == Arp && SoftPanel.Page == SOFT_PAGE2)
          app_flags.Display_ARP_Req = 1;
      }
#if DEBUG_SEQ
      Serial.print(F("seqPlayStep = "));
      Serial.println(seqPlayStep, DEC);
      Serial.println();
      Serial.print(F("play_Seq de seq.ino NteOn interface 1 :: pitch: $"));
      Serial.print(sequence[seqPlayStep][0], HEX);
      Serial.print(F(" velo: $"));
      Serial.print(sequence[seqPlayStep][1], HEX);
      Serial.print(F(" channel: $"));
      Serial.println(MIDI_CHANNEL, HEX);

#endif
    }
  }

  // before we reach the gate time, we remove our finger from the key, so we need to kill the last note played (or it will sustain) :
  // kill last resting note when latching Key
  if (ui_seqPlay && (playSeqTrigger[3] == false))
  {
    //if it was a tie note, get previous pitch and send note off
    if (sequence[seqPlayStep][1] > 127)
    {
      MIDI_A.sendNoteOff(
        sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
      if (arp_send_notes)
        MIDI_CORE.sendNoteOff(
          sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
    }
    else if (sequence[seqPlayStep][1] != 0)
    {
      MIDI_A.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
      if (arp_send_notes)
        MIDI_CORE.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
    }

    // chnage state of released trigger
    playSeqTrigger[3] = true;
    // reset seq step to beginning
    seqPlayStep = 0;
    skipSeqStep = 0; //reset

#if DEBUG_SEQ
    Serial.print(F("seqPlayStep = "));
    Serial.println(seqPlayStep, DEC);
    Serial.println();
    Serial.print(F("kill last resting note when latching Key, pitch = $"));
    Serial.print(sequence[seqPlayStep][0], HEX);
    Serial.print(F(" velo = $"));
    Serial.print(sequence[seqPlayStep][1], HEX);
    Serial.print(F(" channel = $"));
    Serial.println(MIDI_CHANNEL, HEX);
#endif
  }

  // we reach gate time :
  if (seqTick == sGate)
  {
#if DEBUG_SEQTIK
    Serial.print(F("seqTick = "));
    Serial.println(seqTick);
#endif
    // extract an element E contained in sequence array, and play it as NoteOff after a delay corresponding to sGate
    if (ui_seqPlay && seqTrig)
    {
      // if  a 'rest' or next note is a tie
      if ((sequence[seqPlayStep][1] == 0) || (sequence[seqPlayStep + 1][1] > 127)) {}
      else if (sequence[seqPlayStep][1] > 127) // if it's a 'tie'
      {
        // play pitch of previous note with a zero velocity
        MIDI_A.sendNoteOff(
          sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
        if (arp_send_notes)
          MIDI_CORE.sendNoteOff(
            sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
        // reset counter
        skipBack = 0;
      }
      else // play note off normally
      {
        MIDI_A.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
          sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);
        if (arp_send_notes)
          MIDI_CORE.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
            sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);
      }

      // chnage state of released trigger
      playSeqTrigger[3] = true;
      // increment next step to play :
      ++seqPlayStep;
      skipSeqStep = 0; // reset
      if (seqPlayStep > seqLength - 1) // ça MARCHE !!! 0.97b :) RTZ when reaching end
        seqPlayStep = 0;
#if DEBUG_SEQ
      Serial.print(F("seqPlayStep = "));
      Serial.println(seqPlayStep, DEC);
      Serial.println();
      Serial.print(F("play_Seq de seq.ino NteOff interface 1 :: pitch: $"));
      Serial.print(sequence[seqPlayStep][0], HEX);
      Serial.print(F(" velo: $"));
      Serial.print(sequence[seqPlayStep][1], HEX);
      Serial.print(F(" channel: $"));
      Serial.println(MIDI_CHANNEL, HEX);
#endif
    }
  }

  // increment the tick of seq
  ++seqTick;

  // loop reset :
  // non ! on fixe la durée de note et le reset se fera au double. travailler sur les gates plutot
  if (seqTick == (sGate + sGate))
    seqTick = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// similar to ARP(), called on each tick of the clock 0xf9 (int or ext)
/////////////////////////////////////////////////////////////////////////////////////////////
void SEQ2(bool trig)
{
  static unsigned char skipBack;

  if (trig == true) // play noteOn
  {
#if DEBUG_SEQTIK
    Serial.print(F("seqTick = "));
    Serial.println(seqTick);
#endif

    // update parameters :
    sGate = seqSpeed = 0;
    TrspB = ui_TrspB;

    seqPlayStep = seqPlayStep + skipSeqStep;

    // extract an elemnt E contained in sequence, and play it as NoteOn
    // apres une certaine temporisation (correspond à la resolution):
    if (ui_seqPlay && seqTrig)
    {
      // increment next step to play :
      ++seqPlayStep;
      skipSeqStep = 0; // reset
      if (seqPlayStep > seqLength - 1) // ça MARCHE !!! 0.99x :) RTZ when reaching end
        seqPlayStep = 0;
      // skip do nothing
      if (sequence[seqPlayStep][1] == 0) {}
      // if a silent note or a  tie
      else if (sequence[seqPlayStep][1] > 127)
        ++skipBack;
      else // else normal note
      {
        MIDI_A.sendNoteOn(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
          sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);
        // NOTA : limits 0 & 127 are automatically made by sendNoteOn function Edit : pas tout à fait, ça revient à zero si >127
        if (arp_send_notes)
          MIDI_CORE.sendNoteOn(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
            sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);

        // update display lcd :
        if (SoftPanel.Mode == Arp && SoftPanel.Page == SOFT_PAGE2)
          app_flags.Display_ARP_Req = 1;
      }
#if DEBUG_SEQ
      Serial.print(F("seqPlayStep = "));
      Serial.println(seqPlayStep, DEC);
      Serial.println();
      Serial.print(F("play_Seq de seq.ino NteOn interface 1 :: pitch: $"));
      Serial.print(sequence[seqPlayStep][0], HEX);
      Serial.print(F(" velo: $"));
      Serial.print(sequence[seqPlayStep][1], HEX);
      Serial.print(F(" channel: $"));
      Serial.println(MIDI_CHANNEL, HEX);

#endif
    }
  }

  // before we reach the gate time, we remove our finger from the key, so we need to kill the last note played (or it will sustain) :
  // kill last resting note when latching Key
  if (ui_seqPlay && (playSeqTrigger[3] == false))
  {
    //if it was a tie note, get previous pitch and send note off
    if (sequence[seqPlayStep][1] > 127)
    {
      MIDI_A.sendNoteOff(
        sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
      if (arp_send_notes)
        MIDI_CORE.sendNoteOff(
          sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
    }
    else if (sequence[seqPlayStep][1] != 0)
    {
      MIDI_A.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
      if (arp_send_notes)
        MIDI_CORE.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
    }

    // chnage state of released trigger
    playSeqTrigger[3] = true;
    // reset seq step to beginning
    seqPlayStep = 0;
    skipSeqStep = 0; //reset

#if DEBUG_SEQ
    Serial.print(F("seqPlayStep = "));
    Serial.println(seqPlayStep, DEC);
    Serial.println();
    Serial.print(F("kill last resting note when latching Key, pitch = $"));
    Serial.print(sequence[seqPlayStep][0], HEX);
    Serial.print(F(" velo = $"));
    Serial.print(sequence[seqPlayStep][1], HEX);
    Serial.print(F(" channel = $"));
    Serial.println(MIDI_CHANNEL, HEX);
#endif
  }

  // we reach gate time :
  if (trig == false) // play noteOff
  {
#if DEBUG_SEQTIK
    Serial.print(F("seqTick = "));
    Serial.println(seqTick);
#endif
    // extract an element E contained in sequence array, and play it as NoteOff after a delay corresponding to sGate
    if (ui_seqPlay && seqTrig)
    {
      // if  a 'rest' or next note is a tie
      if ((sequence[seqPlayStep][1] == 0) || (sequence[seqPlayStep + 1][1] > 127)) {}
      else if (sequence[seqPlayStep][1] > 127) // if it's a 'tie'
      {
        // play pitch of previous note with a zero velocity
        MIDI_A.sendNoteOff(
          sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
        if (arp_send_notes)
          MIDI_CORE.sendNoteOff(
            sequence[seqPlayStep - skipBack][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36), 0, MIDI_CHANNEL);
        // reset counter
        skipBack = 0;
      }
      else // play note off normally
      {
        MIDI_A.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
          sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);
        if (arp_send_notes)
          MIDI_CORE.sendNoteOff(sequence[seqPlayStep][0] - sequence[0][0] + playSeqTrigger[0] + ((TrspB << 2) + (TrspB << 3) - 36),
            sequence[seqPlayStep][1] - sequence[0][1] + playSeqTrigger[1], MIDI_CHANNEL);
      }

      // chnage state of released trigger
      playSeqTrigger[3] = true;

#if DEBUG_SEQ
      Serial.print(F("seqPlayStep = "));
      Serial.println(seqPlayStep, DEC);
      Serial.println();
      Serial.print(F("play_Seq de seq.ino NteOff interface 1 :: pitch: $"));
      Serial.print(sequence[seqPlayStep][0], HEX);
      Serial.print(F(" velo: $"));
      Serial.print(sequence[seqPlayStep][1], HEX);
      Serial.print(F(" channel: $"));
      Serial.println(MIDI_CHANNEL, HEX);
#endif
    }
  }
}
