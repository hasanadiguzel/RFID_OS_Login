#include <SPI.h>
#include <MFRC522.h>
#include "Keyboard.h"

const int pinLedKirmizi = 9, pinLedYesil = 8;

const int pinRF_RST = 3;
const int pinRF_SS = 2;

MFRC522 rfId(pinRF_SS, pinRF_RST);

byte ID_One[4] = {110,111,111,11};

int cardReadCount = 0;

void setup() {  
  Serial.begin(19200);  //Seri port ile haberleşmemizi başlatıyoruz.
  Serial.println("setup() Calistiriliyor...");
 
  pinMode(pinLedKirmizi, OUTPUT);
  pinMode(pinLedYesil, OUTPUT);

  SPI.begin(); //SPI iletişimini başlatıyoruz
  rfId.PCD_Init(); //RC522 modülünü başlatıyoruz.

  Keyboard.begin();
}

void loop() {
  digitalWrite(pinLedYesil, LOW);
  digitalWrite(pinLedKirmizi, HIGH);
  
  if ( ! rfId.PICC_IsNewCardPresent())    //Yeni kartın okunmasını bekliyoruz.
    return;

  if ( ! rfId.PICC_ReadCardSerial())      //Kart okunmadığı zaman bekliyoruz.
    return;

  if (rfId.uid.uidByte[0] == ID_One[0] &&     //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
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
      Keyboard.print("programmer06");
      Keyboard.press(KEY_RETURN);
      delay(50);
      Keyboard.releaseAll();
      digitalWrite(pinLedYesil, HIGH);
      digitalWrite(pinLedKirmizi, LOW);
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