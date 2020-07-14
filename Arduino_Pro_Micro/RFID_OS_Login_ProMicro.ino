#include <SPI.h>
#include <MFRC522.h>
#include "Keyboard.h"

const int pinRF_RST = 5;
const int pinRF_SS = 7;

MFRC522 rfId(pinRF_SS, pinRF_RST);

byte ID_One[4] = {150,161,215,19};

int cardReadCount = 0;

void setup() {  
  Serial.begin(19200);

  SPI.begin();
  rfId.PCD_Init();

  Keyboard.begin();
}

void loop() {
  
  if ( ! rfId.PICC_IsNewCardPresent())    
    return;

  if ( ! rfId.PICC_ReadCardSerial())     
    return;

  if (rfId.uid.uidByte[0] == ID_One[0] &&    
    rfId.uid.uidByte[1] == ID_One[1] && 
    rfId.uid.uidByte[2] == ID_One[2] && 
    rfId.uid.uidByte[3] == ID_One[3] ) {
      cardReadCount++;
      if(cardReadCount%2!=0) {
        Keyboard.press(KEY_RETURN);
        delay(200);
        Keyboard.releaseAll();
        delay(800);
      } else {
        cardReadCount = 0;
      }
      Keyboard.print("os sifreniz");
      Keyboard.press(KEY_RETURN);
      delay(50);
      Keyboard.releaseAll();
      Serial.println("Oturum Acildi...");
      delay(10);
  }
  else{
      Serial.println("Yetkisiz Kart");
      ekranaYazdir();
  }
    
  rfId.PICC_HaltA();
}
void ekranaYazdir(){
  Serial.print("ID Numarasi: ");
  for(int sayac = 0; sayac < 4; sayac++){
    Serial.print(rfId.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}
