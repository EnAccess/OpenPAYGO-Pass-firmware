#include "application.hpp"
#include "sensors.hpp"
#include "misc.hpp"
#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>

/* Create an instance of MFRC522 */
MFRC522 mfrc522(SS_PIN, RST_PIN);
/* Create an instance of MIFARE_Key */
MFRC522::MIFARE_Key key;       


/* Set the block to which we want to write data */
/* Be aware of Sector Trailer Blocks */
int blockNum = 2;  
/* Create an array of 16 Bytes and fill it with data */
/* This is the actual data which is going to be written into the card */
byte blockData [16] = {"Electronics-Hub-"};

/* Create another array to read data from Block */
/* Legthn of buffer should be 2 Bytes more than the size of Block (16 Bytes) */
byte bufferLen = 18;
byte readBlockData[18];

MFRC522::StatusCode status;
uint32_t  timeelapsedtags = 0;
uint32_t  timeelapsedreadings = 0;

void WriteDataToBlock(int blockNum, byte blockData[]) 
{
  /* Authenticating the desired data block for write access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  
  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Data was written into Block successfully");
  }
  
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  /* Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
  
}

void peripherals_setup(){
  /* Initialize serial communications with the PC */
  Serial.begin(9600);
  /* Initialize SPI bus */
  SPI.begin();
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  key_preparation();
  Serial.println("Scan a MIFARE 1K Tag to write data...");
}

void key_preparation(){
for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
}


void main_app_loop(){

  if (millis() - timeelapsedtags > TIME_BETWEEN_TAGS){
    mfrc522.PCD_Init();
    key_preparation();
    Serial.println("Scan a MIFARE 1K Tag to write data...");
    _delay_ms(10);
    timeelapsedtags = millis();
  } 

 


    if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    if (millis() - timeelapsedreadings > TIME_BETWEEN_READINGS){
    Serial.println("Reading Sensors...");
    sensors_reading(&BatteryVoltage, &BatteryCurrent);
    timeelapsedreadings = millis();
  }
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return; 
    
  }

     
    
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
 /* if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }*/

  Serial.print("\n");
  Serial.println("**Card Detected**");
  /* Print UID of the Card */
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print("\n");
  /* Print type of card (for example, MIFARE 1K) */
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
         
   /* Call 'WriteDataToBlock' function, which will write data to the block */
   Serial.print("\n");
   Serial.println("Writing to Data Block...");
   WriteDataToBlock(blockNum, blockData);
   
   /* Read data from the same block */
   Serial.print("\n");
   Serial.println("Reading from Data Block...");
   ReadDataFromBlock(blockNum, readBlockData);
   /* If you want to print the full memory dump, uncomment the next line */
   //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
   
   /* Print the data read from block */
   Serial.print("\n");
   Serial.print("Data in Block:");
   Serial.print(blockNum);
   Serial.print(" --> ");
   for (int j=0 ; j<16 ; j++)
   {
     Serial.write(readBlockData[j]);
   }
   Serial.print("\n");
}


