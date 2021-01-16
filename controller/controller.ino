#define EEPROM_DEBUG 0
#include "eeprom.h"

void setup() {
    EEPROM eeprom(14, 13, 9, 10, 12, 2, 3, 11, 17, 16, 15, 8, 7, 6, 5, 4);

    Serial.begin(2000000);
    Serial.println("MSG READY");

    uint8_t i = 0x00;
    uint8_t n = 0x00;

    // do {
    //     eeprom.write((~i << 8) | i);
    //     eeprom.next();
    // } while (i++ < 0xFF);

    do {
        if (eeprom.read() != ((~i << 8) | i)) {
            Serial.println("MSG MISMATCH");
            n++;
        }
        
        eeprom.next();
    } while (i++ < 0xFF);

    Serial.print("MSG MISMATCH TOTAL ");
    Serial.println(String(n));
}

void loop() {}
