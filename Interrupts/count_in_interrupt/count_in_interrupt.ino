volatile byte count;

ISR (TIMER1_OVF_vect)
  {
  count = 10;
  }  // end of TIMER1_OVF_vect

void setup ()
  {
  }  // end of setup

void loop () 
  {
  noInterrupts ();
  count++;
  interrupts ();
  } // end of loop
