#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          53        // Configurable, see typical pin layout above
 
extern MFRC522 mfrc522;  // Create MFRC522 instance
/* Create an instance of MIFARE_Key */
extern MFRC522::MIFARE_Key key;          


void peripherals_setup();
void key_preparation();
void main_app_loop();