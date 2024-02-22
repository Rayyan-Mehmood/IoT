#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 0
#define SERVO_PIN 25

Servo myservo;
MFRC522 rfid(SS_PIN, RST_PIN);

int collar_id[] = {44, 214, 228, 56};
bool locked = true;
int lock = 90;
int unlock = 0;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  myservo.attach(SERVO_PIN);

}
 
void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  bool valid_card = true;
  for (byte i = 0; i < 4; i++) {
    if (collar_id[i] != rfid.uid.uidByte[i]){
        valid_card = false;
    }
  }

  if (valid_card == true){
      Serial.println("IT'S A VALID CARD");
      if (locked == true){
        myservo.write(unlock);
        locked = false;
      }
      else{
        myservo.write(lock);
        locked = true;
      }
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}