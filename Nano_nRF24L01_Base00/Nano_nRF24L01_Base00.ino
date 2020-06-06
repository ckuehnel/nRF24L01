/*
 * File Nano_nRF24L01_Base00.ino
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

RF24 radio(7, 8);                // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network

const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;
const uint16_t node012 = 012;
const uint16_t node022 = 022;
const uint8_t  ch = 4;           // Used channel number

void setup() 
{
  SPI.begin();
  radio.begin();
  network.begin(ch, this_node);  //(channel, node address)
  radio.setDataRate(RF24_1MBPS);
  pinMode(LED_BUILTIN, HIGH);
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
    if (header.from_node == 01) {    // If data comes from Node 02
      //myservo.write(incomingData);  // tell servo to go to a particular angle
    }
    if (header.from_node == 02) {    // If data comes from Node 012
      //digitalWrite(led, !incomingData);  // Turn on or off the LED 02
    }
    //analogWrite(led, incomingData);    // PWM output to LED 01 (dimming)
  }
  //===== Sending =====//
  // Servo control at Node 01
  unsigned long potValue = analogRead(A0);
  unsigned long angleValue = map(potValue, 0, 1023, 0, 180); // Suitable for servo control
  RF24NetworkHeader header2(node01);     // (Address where the data is going)
  bool ok = network.write(header2, &angleValue, sizeof(angleValue)); // Send the data

  // LED Control at Node 012
  // unsigned long buttonState = digitalRead(button);
  RF24NetworkHeader header4(node012);    // (Address where the data is going)
  bool ok3 = network.write(header4, &buttonState, sizeof(buttonState)); // Send the data

  // LEDs control at Node 022
  unsigned long pot2Value = analogRead(A1);
  RF24NetworkHeader header3(node022);    // (Address where the data is going)
  bool ok2 = network.write(header3, &pot2Value, sizeof(pot2Value)); // Send the data
}
