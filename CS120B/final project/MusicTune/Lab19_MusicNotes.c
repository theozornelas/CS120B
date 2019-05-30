/*
  Experiment No. 19: Playing music notes with a PIC micro
  MCU: PIC16F628A at 4.0 MHz, MCLR Enabled
  Description: Plays the Happy birthday tune
  Compile with MikroC Pro for PIC
*/

void pause(unsigned short i){
 unsigned short j;
 for (j = 0; j < i; j++){
  Delay_ms(10);
 }
}

// Happy birthday notes
/*                        Hap py  Birth Day  to  you,  Hap py  birth day  to
                         C4   C4   D4   C4   F4   E4   C4   C4   D4   C4   G4 */
unsigned int notes[] = {262, 262, 294, 262, 349, 330, 262, 262, 294, 262, 392,

/*                       you, Hap py  Birth Day  dear  xxxx      Hap  py   birth
                         F4   C4   C4   C5   A4   F4   E4   D4   B4b  B4b  A4 */
                        349, 262, 262, 523, 440, 349, 330, 294, 466, 466, 440,

/*                       day  to  you
                         F4   G4   F4   */
                        349, 392, 349
                        };

unsigned short interval[] = {4, 4, 8, 8, 8, 10, 4, 4, 8, 8, 8, 10, 4, 4, 8, 8, 8,
                             8, 8, 4, 4, 8, 8, 8, 12};



unsigned short k;
void main() {
  CMCON = 0x07;
  TRISB = 0b00001000;  // GP5, 5 I/P's, Rest O/P's
  Sound_Init(&PORTB,0); // Initialize sound o/p pin


do {
 for(k = 0; k<25; k++){
  Sound_Play(notes[k], 100*interval[k]);
  pause(6);
 }
 pause(100);
   }while(1);
}