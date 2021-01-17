#include "eeprom.h"

const uint8_t version = 0x01;
bool receiving = false;

EEPROM eeprom(14, 13, 9, 10, 12, 2, 3, 11, 17, 16, 15, 8, 7, 6, 5, 4);

void setup() {
    eeprom.init();

    Serial.begin(115200);
    Serial.write(0x01);
    Serial.write(version);
}

word_t recv = 0x0000;
byte_t temp[2];

word_t buff[256];
uint8_t buffpos = 0;

word_t send[256];
uint8_t i = 0x00;

void loop() {
    if (Serial.available() > 0) {
        Serial.readBytes(temp, 2);
        recv = (temp[0] << 8) | temp[1];

        if (receiving) {
            buff[buffpos] = recv;

            if (buffpos == 255) {
                receiving = false;

                eeprom.write(buff);

                Serial.write(0x03);
                Serial.write((uint8_t)0x00);
            }

            buffpos++;

        } else {
            if (recv == 0x1000) {
                Serial.end();
            
                pinMode(LED_BUILTIN, OUTPUT);
            
                digitalWrite(LED_BUILTIN, HIGH);
                delay(500);
                digitalWrite(LED_BUILTIN, LOW);
                delay(500);

            } else if (recv == 0x1100) {
                Serial.write(0x02);
                Serial.write((uint8_t)0x00);

                eeprom.read(send);

                do {
                    Serial.write(send[i] >> 8);
                    Serial.write(send[i] & 0x00FF);
                } while (i++ < 0xFF);

            } else if (recv == 0x1200) {
                receiving = true;
            }
        }
    }
}
