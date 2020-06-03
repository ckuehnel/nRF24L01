/*
 * File: Nano_nRF24L01_Scan.ino
 * 
 * based on 
 * Channel scanner Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 * 
 * Adaptions: Claus Kühnel 2020-05-30 info@ckuehnel.ch
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 7 & 8
RF24 radio(7,8);

const uint8_t num_channels = 126;
uint8_t values[num_channels];
uint8_t best_channel = 0, max_value = 0;

const int num_reps = 100;

void setup(void)
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  printf_begin();
  Serial.println(F("\nnRF24L01 Default Initialization"));
  Serial.println(F("-------------------------------"));

  // Setup and configure rf radio
  radio.begin();
  if (!radio.isChipConnected()) Serial.println(F("No nRF24L01 connected!"));
  
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS); // low dara rate for higher range
  radio.startListening();
  radio.stopListening();

  Serial.println(F("\nnRF24L01 Initialization values"));
  radio.printDetails();

  Serial.println(F("\nnRF24L01 Channel"));
  // Print out header, high then low digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x",i>>4);
    ++i;
  }
  Serial.println("\tBest");
  i = 0;
  while ( i < num_channels )
  {
    printf("%x",i&0xf);
    ++i;
  }
  Serial.println("\tChannel");
}

void loop(void)
{
  // Clear measurement values
  memset(values,0,sizeof(values));
  best_channel = 127;
  max_value = 0;

  // Scan all channels num_reps times
  int rep_counter = num_reps;
  while (rep_counter--)
  {
    int i = num_channels;
    while (i--)
    {
      // Select this channel
      radio.setChannel(i);

      // Listen for a little
      radio.startListening();
      delayMicroseconds(128);
      radio.stopListening();

      // Did we get a carrier?
      if ( radio.testCarrier() ){
        ++values[i];
      }
    }
  }
  
  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x",min(0xf,values[i]));
    ++i;
  }
  //Serial.println();

  // Print out best channel
  i = 0;
  while ( i < num_channels )
  {
    if (values[i] > max_value)
    {
      max_value = values[i];
      best_channel = i;
    }
    ++i;
  }
  Serial.print("\t");
  Serial.print(best_channel, HEX);
  Serial.print(F(" hex ("));
  Serial.print(best_channel);
  Serial.println(F(" dec)"));
}
