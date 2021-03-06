#include <SPI.h>
#define interCodG  0 //pin2 (interrupt0)
#define interCodD  1 //pin3 (interrup1)
#define AV 1 //indexs pour les tableaux de retour de la fonction de conversion (normalement inutilisé mais on va les garder pour la norme)
#define ARR 0
#define GAUCHE 0
#define DROIT 1

#define PIN_ATT 5


#define CANAL_BD  8 //canal avant ou arrière pour les codeurs (droit)
#define CANAL_BG  7 // (gauche)

#define ADDR_ARDUINO  9

#define CMD_RESET_ACTIONNEURS -1
#define CMD_RESET_CODEURS -3
#define CMD_ACK -2

volatile int nbTicD; //compteur de ticks codeur droit
volatile int nbTicG; // compteur de ticks codeur gauche

bool debug = true;

bool firstAsk = true;

void setup()
{
  if(debug)
    Serial.begin (115200);   // debugging
  
  reset();
    
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

   // turn on interrupts
  SPI.attachInterrupt();
  
  attachInterrupt(interCodD, compterTicDroit, RISING );
  attachInterrupt(interCodG, compterTicGauche, RISING);
}

void reset()
{
  nbTicD = 0;
  nbTicG = 0;
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte cmd = SPDR;

  if(debug)
  {
    Serial.print ("cmd : ");
    Serial.println (cmd);

    /*Serial.print ("nbTicD : ");
    Serial.println (nbTicD);

    Serial.print ("nbTicG : ");
    Serial.println (nbTicG);*/
  }

  if(firstAsk == true)
  {
    SPDR = cmd + nbTicD;
    firstAsk = false;
    if(debug)
    {
      Serial.print ("nbTicD : ");
      Serial.println (nbTicD);
    }
  }
  else
  {
    SPDR = cmd + nbTicG;
    firstAsk = true;
    
    if(debug)
    {
      Serial.print ("nbTicG : ");
      Serial.println (nbTicG);
    }
  }
   
}  

void loop()
{
  
}

void compterTicDroit ()
{
  if (digitalRead(CANAL_BD) == HIGH)
  {
    nbTicD--;
  }
  else
    nbTicD++;
}

void compterTicGauche ()
{
  if (digitalRead(CANAL_BG) == HIGH)
  {
    nbTicG++;
  }
  else
    nbTicG--;
}
