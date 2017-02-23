/*********************************************************************

        API REST FOR DDS V.2.1

 *********************************************************************
   FileName:        api_rest_v1.ino
   Dependencies:    <Base64.h>
                    <ArduinoJson.h>
                    <ArduinoHttpServer.h>

   Processor:       TIVA TM4C1294C


 *********************************************************************
   Platform: Energia based in Arduino

   Description:
        This program shows the ethernet communication using json message
        for the commands: READ, WRITE, STATUS, START, STOP.

 ********************************************************************/
/*   Adaptation by Jose Avalos. January 2017
    Jicamarca Radio Observatory - JRO
 *******************************************************************/

// Energia Libraries
#include <SPI.h>
#include <Ethernet.h>

// Extern libraries
#include <Base64.h>
#include <ArduinoJson.h>
#include <ArduinoHttpServer.h>
#include <AD9854.h>
#include <string>
#include "Tiva.h"


#define RESET_TIVA PL_4
#define MRESET  PE_4
#define OSK PC_4
#define CS PC_5
#define UDCLK PB_2
#define IO_RESET PB_3

#define SPI_module 1
#define SYSCLOCK 60000000
#define DDS_NBITS  48
#define DEFAULT_FREQ 49920000
#define LED RED_LED



IPAddress IP(10, 10, 50, 185);
byte MAC[] = {0x00, 0x1A, 0xB6, 0x02, 0xEC, 0x3E };
EthernetServer server(80);

DDS DDS_JRO(SYSCLOCK, CS, UDCLK, IO_RESET, MRESET);
API API_JRO(&DDS_JRO, IP, MAC);


void setup()
{
  SPI.setModule(2);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  server.begin();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(115200);
  Serial.println("#####################################");
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("Configuration for DDS has already inicializated.");

  //**************************************************
  while (true)
  {
    if (DDS_JRO.init());
      break;

    digitalWrite(LED, LOW);
    delay(10);

    //        if (wasIPResetPressed())
    //        {
    //            eraseIpConfig();
    //            mbed_reset();
    //        }
  }
  //
  digitalWrite(LED, LOW);
  //    /*
  //    screen.putc(0x33);
  //    screen.putc(0x31);
  //    */
  digitalWrite(LED, HIGH);
  //
  DDS_JRO.io_reset();
  DDS_JRO.wrFrequency1(DDS_JRO.freq2binary(DEFAULT_FREQ / 4));

  //int c=0;
  while (true)
  {
    EthernetClient _client = server.available();
    API_JRO.readcommand(_client);
    //        if (dds_device.isRFEnabled()){
    //            LedR = 0;
    //        }
    //        else{
    //            LedR = 1;
    //        }
    //
    //        if (wasIPResetPressed()){
    //            eraseIpConfig();
    //            mbed_reset();
    //        }

    delay(400);
  }
}

void loop(){}

