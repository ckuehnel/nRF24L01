/*
 * File NodeMCU_nRF24L01_Base00.ino
 * 
 * based on Arduino Wireless Network - Multiple NRF24L01 Tutorial
 * by Dejan, www.HowToMechatronics.com
 * used Libraries
 * - nRF24/RF24, https://github.com/nRF24/RF24
 * - nRF24/RF24Network, https://github.com/nRF24/RF24Network
 * 
 * Claus Kühnel 2020-06-04 info@ckuehnel.ch
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(D1, D2);               // nRF24L01 (CE,CSN)
RF24Network network(radio);       // Include the radio in the network

const uint16_t this_node = 00;    // Address of this node in Octal format
const uint16_t node01    = 01;    // Address of the other nodes in Octal format
const uint16_t node02    = 02;
const uint16_t node03    = 03;
const uint16_t node04    = 04;    // for test purposes only
const uint16_t node013   = 013;
const uint16_t node023   = 023;

const uint8_t  ch = 4;            // Used channel number

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.print("\nInitializing Radio...");
  SPI.begin();
  radio.begin();
  radio.setChannel(ch); // set channel
  radio.setDataRate(RF24_250KBPS); // low data rate for higher range
  network.begin(ch, this_node);  //(channel, node address)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("done.");
  Serial.print("Ready to receive messages on channel "); 
  Serial.println(radio.getChannel());
}

void loop() 
{
  network.update();
  //===== Receiving =====//
  while ( network.available() )   // Is there any incoming data?
  {
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    //Serial.print("Message from Node "); Serial.println(header.from_node);
    if (header.from_node == node01) 
    {    // If data comes from Node 01
      Serial.print("Received from Node 01: "); Serial.println(incomingData);
    }
    if (header.from_node == node02) 
    {    // If data comes from Node 02
      Serial.print("Received from Node 02: "); Serial.println(incomingData);
    }
    if (header.from_node == node03) 
    {    // If data comes from Node 03
      Serial.print("Received from Node 03: "); Serial.println(incomingData);
    }
    if (header.from_node == node04) 
    {    // If data comes from Node 04
      Serial.print("Received from Node 04: "); Serial.println(incomingData);
    }
    if (header.from_node == node013) 
    {    // If data comes from Node 013
      Serial.print("Received from Node 013: "); Serial.println(incomingData);
    }
    if (header.from_node == node023) 
    {    // If data comes from Node 023
      Serial.print("Received from Node 023: "); Serial.println(incomingData);
    }
  }
}
