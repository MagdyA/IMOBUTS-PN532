// Receive a NDEF message from a Peer
// Requires SPI. Tested with Seeed Studio NFC Shield v2

#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"


PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];
const int ticketPrice = 4;
const int ledYes = 2;
const int ledNo = 4;

void setup() {

    Serial.begin(9600);
    Serial.println("Receive ticket is ready ... p2p is ready");
      pinMode(ledYes, OUTPUT);
      pinMode(ledNo, OUTPUT);

}

void loop() {
    Serial.println("Waiting for message from Peer");
    int msgSize = nfc.read(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        NdefMessage msg  = NdefMessage(ndefBuf, msgSize);
        //msg.print();
        int payloadLength = msg.getRecord(0).getPayloadLength();
        //Serial.println(payloadLength);
        byte payload[payloadLength];
        msg.getRecord(0).getPayload(payload);
        String payloadAsString = "";
    for (int c = 0; c < payloadLength; c++) {
      payloadAsString += (char)payload[c];
    }
    int balance = payloadAsString.toInt();
    Serial.print("balance:  ");Serial.println(balance);
        if(balance >= ticketPrice){
          pass();
        }
        else{
          block();
        }
    } else {
        Serial.println("Failed");
    }
    delay(3000);
}

void pass(){
  digitalWrite(ledYes,HIGH);
  delay(1000);
  digitalWrite(ledYes,LOW);
}

void block(){
  digitalWrite(ledNo,HIGH);
  delay(1000);
  digitalWrite(ledNo,LOW);
}


