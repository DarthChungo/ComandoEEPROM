#ifndef _BITS_H_
#define _BITS_H_

typedef unsigned char byte_t;
typedef unsigned short word_t; 

#define BIN0(val) (((val) & 0x01ll) ? 1 : 0)
#define BIN1(val) (((val) & 0x02ll) ? 1 : 0)
#define BIN2(val) (((val) & 0x04ll) ? 1 : 0)
#define BIN3(val) (((val) & 0x08ll) ? 1 : 0)
#define BIN4(val) (((val) & 0x10ll) ? 1 : 0)
#define BIN5(val) (((val) & 0x20ll) ? 1 : 0)
#define BIN6(val) (((val) & 0x40ll) ? 1 : 0)
#define BIN7(val) (((val) & 0x80ll) ? 1 : 0)

void print_byte(byte_t byte) {
    Serial.print(BIN7(byte) ? "1" : "0");
    Serial.print(BIN6(byte) ? "1" : "0");
    Serial.print(BIN5(byte) ? "1" : "0");
    Serial.print(BIN4(byte) ? "1" : "0");
    Serial.print(BIN3(byte) ? "1" : "0");
    Serial.print(BIN2(byte) ? "1" : "0");
    Serial.print(BIN1(byte) ? "1" : "0");
    Serial.print(BIN0(byte) ? "1" : "0");
}

void print_word(word_t word) {
    Serial.print(BIN7(word>>8) ? "1" : "0");
    Serial.print(BIN6(word>>8) ? "1" : "0");
    Serial.print(BIN5(word>>8) ? "1" : "0");
    Serial.print(BIN4(word>>8) ? "1" : "0");
    Serial.print(BIN3(word>>8) ? "1" : "0");
    Serial.print(BIN2(word>>8) ? "1" : "0");
    Serial.print(BIN1(word>>8) ? "1" : "0");
    Serial.print(BIN0(word>>8) ? "1" : "0");
    Serial.print(" ");
    Serial.print(BIN7(word) ? "1" : "0");
    Serial.print(BIN6(word) ? "1" : "0");
    Serial.print(BIN5(word) ? "1" : "0");
    Serial.print(BIN4(word) ? "1" : "0");
    Serial.print(BIN3(word) ? "1" : "0");
    Serial.print(BIN2(word) ? "1" : "0");
    Serial.print(BIN1(word) ? "1" : "0");
    Serial.print(BIN0(word) ? "1" : "0");
}

#endif
