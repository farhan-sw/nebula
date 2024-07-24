//----------------------------------------------------------------------------------------
//  Board Check
//----------------------------------------------------------------------------------------

#ifndef ARDUINO_ARCH_ESP32
  #error "Select an ESP32 board"
#endif

//----------------------------------------------------------------------------------------
//   Include files
//----------------------------------------------------------------------------------------

#include <ACAN_ESP32.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <core_version.h> // For ARDUINO_ESP32_RELEASE

//----------------------------------------------------------------------------------------
//  ESP32 Desired Bit Rate
//----------------------------------------------------------------------------------------

static const uint32_t DESIRED_BIT_RATE = 1000UL * 1000UL ; // 1 Mb/s

//----------------------------------------------------------------------------------------
//   SETUP
//----------------------------------------------------------------------------------------

void setup () {
//--- Switch on builtin led
  pinMode (LED_BUILTIN, OUTPUT) ;
  digitalWrite (LED_BUILTIN, HIGH) ;
//--- Start serial
  Serial.begin (115200) ;
  Serial2.begin (115200, SERIAL_8N1, GPIO_NUM_16, GPIO_NUM_17); // Set RX2 to GPIO 16 and TX2 to GPIO 17
  delay (100) ;
//--- Display ESP32 Chip Info
  esp_chip_info_t chip_info ;
  esp_chip_info (&chip_info) ;
  Serial2.print ("ESP32 Arduino Release: ") ;
  Serial2.println (ARDUINO_ESP32_RELEASE) ;
  Serial2.print ("ESP32 Chip Revision: ") ;
  Serial2.println (chip_info.revision) ;
  Serial2.print ("ESP32 SDK: ") ;
  Serial2.println (ESP.getSdkVersion ()) ;
  Serial2.print ("ESP32 Flash: ") ;
  uint32_t size_flash_chip ;
  esp_flash_get_size (NULL, &size_flash_chip) ;
  Serial2.print (size_flash_chip / (1024 * 1024)) ;
  Serial2.print (" MB ") ;
  Serial2.println (((chip_info.features & CHIP_FEATURE_EMB_FLASH) != 0) ? "(embeded)" : "(external)") ;
  Serial2.print ("APB CLOCK: ") ;
  Serial2.print (APB_CLK_FREQ) ;
  Serial2.println (" Hz") ;
//--- Configure ESP32 CAN
  Serial2.println ("Configure ESP32 CAN") ;
  ACAN_ESP32_Settings settings (DESIRED_BIT_RATE) ;
  settings.mRequestedCANMode = ACAN_ESP32_Settings::LoopBackMode ;
  settings.mRxPin = GPIO_NUM_21 ; // Optional, default Tx pin is GPIO_NUM_4
  settings.mTxPin = GPIO_NUM_22 ; // Optional, default Rx pin is GPIO_NUM_5
  const uint32_t errorCode = ACAN_ESP32::can.begin (settings) ;
  if (errorCode == 0) {
    Serial2.print ("Bit Rate prescaler: ") ;
    Serial2.println (settings.mBitRatePrescaler) ;
    Serial2.print ("Time Segment 1:     ") ;
    Serial2.println (settings.mTimeSegment1) ;
    Serial2.print ("Time Segment 2:     ") ;
    Serial2.println (settings.mTimeSegment2) ;
    Serial2.print ("RJW:                ") ;
    Serial2.println (settings.mRJW) ;
    Serial2.print ("Triple Sampling:    ") ;
    Serial2.println (settings.mTripleSampling ? "yes" : "no") ;
    Serial2.print ("Actual bit rate:    ") ;
    Serial2.print (settings.actualBitRate ()) ;
    Serial2.println (" bit/s") ;
    Serial2.print ("Exact bit rate ?    ") ;
    Serial2.println (settings.exactBitRate () ? "yes" : "no") ;
    Serial2.print ("Distance            ") ;
    Serial2.print (settings.ppmFromDesiredBitRate ()) ;
    Serial2.println (" ppm") ;
    Serial2.print ("Sample point:       ") ;
    Serial2.print (settings.samplePointFromBitStart ()) ;
    Serial2.println ("%") ;
    Serial2.println ("Configuration OK!");
  } else {
    Serial2.print ("Configuration error 0x") ;
    Serial2.println (errorCode, HEX) ;
  }
}

//----------------------------------------------------------------------------------------

static uint32_t gBlinkLedDate = 0 ;
static uint32_t gReceivedFrameCount = 0 ;
static uint32_t gSentFrameCount = 0 ;
static int16_t counter = 0 ; // 16-bit counter to be sent in CANMessage

//----------------------------------------------------------------------------------------
//   LOOP
//----------------------------------------------------------------------------------------

void loop () {
  CANMessage frame ;
  if (gBlinkLedDate < millis ()) {
    gBlinkLedDate += 500 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    Serial2.print ("Sent: ") ;
    Serial2.print (gSentFrameCount) ;
    Serial2.print (" ") ;
    Serial2.print ("Receive: ") ;
    Serial2.print (gReceivedFrameCount) ;
    Serial2.print (" ") ;
    Serial2.print (" STATUS 0x") ;
    Serial2.print (TWAI_STATUS_REG, HEX) ;
    Serial2.print (" RXERR ") ;
    Serial2.print (TWAI_RX_ERR_CNT_REG) ;
    Serial2.print (" TXERR ") ;
    Serial2.println (TWAI_TX_ERR_CNT_REG) ;
    
    // Fill CAN message
    frame.id = 0x123 ; // Example ID
    frame.len = 8 ; // Data length
    frame.data[6] = counter & 0xFF ; // LSB of counter
    frame.data[7] = (counter >> 8) & 0xFF ; // MSB of counter

    const bool ok = ACAN_ESP32::can.tryToSend (frame) ;
    if (ok) {
      gSentFrameCount += 1 ;
      counter++; // Increment counter for next message
    }
  }
  
  while (ACAN_ESP32::can.receive (frame)) {
    gReceivedFrameCount += 1 ;
    Serial2.println ("Received CAN message:");
    Serial2.print ("ID: "); Serial2.println (frame.id, HEX);
    Serial2.print ("Extended: "); Serial2.println (frame.ext);
    Serial2.print ("RTR: "); Serial2.println (frame.rtr);
    Serial2.print ("Index: "); Serial2.println (frame.idx);
    Serial2.print ("Length: "); Serial2.println (frame.len);
    for (uint8_t i = 0; i < frame.len; i++) {
      Serial2.print ("Data["); Serial2.print (i); Serial2.print ("]: ");
      Serial2.println (frame.data[i], HEX);
    }
  }
}

//----------------------------------------------------------------------------------------
