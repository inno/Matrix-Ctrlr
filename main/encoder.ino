/* read a rotary encoder with/without interrupts
   Encoder hooked up with common to GROUND,
   encoder0PinA to pin 2, encoder0PinB to pin 4 (or pin 3 see below)
   it does matter which encoder pin you use for A or C, set accordingly mainEncoder( ; )

   uses Arduino pullups on A & C channel outputs
   turning on the pullups saves having to hook up resistors
   to the A & B channel outputs

  https://www.arduino.cc/en/Reference/AttachInterrupt
*/

#include "softpanel.h"
#include <Encoder.h>
#include "define.h"

Encoder mainEncoder(2, 3); // encoder pin A and C. B to ground

long previousPosition = -999;
signed char encSmooth = 0;
bool encoderClic;
bool previousEncoderClic;


int decrementEncoder()
{
    if (--encSmooth == SMOOTHING_ENCODER)
    {
        encSmooth = 0;
        return -1;
    }
    return 0;
}

int incrementEncoder()
{
    if (++encSmooth == SMOOTHING_ENCODER)
    {
        encSmooth = 0;
        return 1;
    }
    return 0;
}

void encoder() // Youhouhouuuu !!!! ça marcheeee :)
{
  signed char incrementer = 0;
  // several *clics* of the encoder are needed to increment/decrement (nicer touch feel)
  long currentPosition = mainEncoder.read();

  if (currentPosition == previousPosition)
      return;
  else if (previousPosition < currentPosition)
      incrementer = decrementEncoder() ? encoder_inverted : incrementEncoder();
  else if (previousPosition > currentPosition)
      incrementer = incrementEncoder() ? encoder_inverted : decrementEncoder();

  if (incrementer != 0)
  {
    // NB : using Shift button we could set a new value to encSmoothING_ENCODER to increment faster
    SoftPanel_Handler(-1, incrementer); // no pin
    app_flags.Display_ENC_Req = 1; // update display
  }
  previousPosition = currentPosition;

#if DEBUG_encoder
    Serial.println(F("encoder() "));
    Serial.print(incrementer);
    Serial.print(F(" Position = "));
    Serial.println(previousPosition);
    Serial.println();
#endif
}

/////////////////////////////////////////////////////////
// ENCODER CLIC management (use pull up)
/////////////////////////////////////////////////////////
void Encoder_Clic() // OK elle marche 15/12/2016
{
  encoderClic = digitalRead(encoderClicPin) != HIGH;

#if DEBUG_encoder
  if (encoderClic != previousEncoderClic)
  {
    Serial.print(F("encoderClic = "));
    Serial.println(encoderClic, DEC);
  }
#endif

  // behaviour
  if (encoderClic && encoderClic != previousEncoderClic) // On push and state has changed
    SoftPanel_Handler(SOFT_EDIT_ENC_CLIC, 0);
  previousEncoderClic = encoderClic;
}
