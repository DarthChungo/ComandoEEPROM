#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "debug.h"
#include "bits.h"

#define TIMEOUT_READ    150
#define TIMEOUT_WRITE   150
#define TIMEOUT_ENABLE  150
#define TIMEOUT_ACTION  5
#define TIMEOUT_READY   250

class EEPROM {
public:
    EEPROM(const uint8_t clk_pin, const uint8_t next_pin, const uint8_t lin_pin, const uint8_t lout_pin, const uint8_t len_pin, 
           const uint8_t hin_pin, const uint8_t hout_pin, const uint8_t hen_pin, const uint8_t data0_pin, const uint8_t data1_pin,
           const uint8_t data2_pin, const uint8_t data3_pin, const uint8_t data4_pin, const uint8_t data5_pin,
           const uint8_t data6_pin, const uint8_t data7_pin);

    void init();

    void read(word_t* dest);
    void write(word_t* data);

    uint8_t addr = 0;

private:
    void next();

    word_t read_word();
    void write_word(word_t data);

    byte_t read_low();
    byte_t read_high();

    void write_low(byte_t data);
    void write_high(byte_t data);

    uint8_t _addr_clk = 0;
    uint8_t _addr_next = 0;

    uint8_t _low_in = 0;
    uint8_t _low_out = 0;
    uint8_t _low_enable = 0;

    uint8_t _high_in = 0;
    uint8_t _high_out = 0;
    uint8_t _high_enable = 0;

    uint8_t _data0 = 0;
    uint8_t _data1 = 0;
    uint8_t _data2 = 0;
    uint8_t _data3 = 0;
    uint8_t _data4 = 0;
    uint8_t _data5 = 0;
    uint8_t _data6 = 0;
    uint8_t _data7 = 0;
};

EEPROM::EEPROM(const uint8_t clk_pin, const uint8_t next_pin, const uint8_t lin_pin, const uint8_t lout_pin, const uint8_t len_pin,
               const uint8_t hin_pin, const uint8_t hout_pin, const uint8_t hen_pin, const uint8_t data0_pin, const uint8_t data1_pin,
               const uint8_t data2_pin, const uint8_t data3_pin, const uint8_t data4_pin, const uint8_t data5_pin,
               const uint8_t data6_pin, const uint8_t data7_pin) :

               _addr_clk(clk_pin), _addr_next(next_pin), _low_in(lin_pin), _low_out(lout_pin), _low_enable(len_pin),
               _high_in(hin_pin), _high_out(hout_pin), _high_enable(hen_pin), _data0(data0_pin), _data1(data1_pin),
               _data2(data2_pin), _data3(data3_pin), _data4(data4_pin), _data5(data5_pin),
               _data6(data6_pin), _data7(data7_pin)
    {
        
}

void EEPROM::init() {
    pinMode(_addr_clk, OUTPUT);
    pinMode(_addr_next, OUTPUT);

    pinMode(_low_in, OUTPUT);
    pinMode(_low_out, OUTPUT);
    pinMode(_low_enable, OUTPUT);

    pinMode(_high_in, OUTPUT);
    pinMode(_high_out, OUTPUT);
    pinMode(_high_enable, OUTPUT);

    pinMode(_data0, INPUT_PULLUP);
    pinMode(_data1, INPUT_PULLUP);
    pinMode(_data2, INPUT_PULLUP);
    pinMode(_data3, INPUT_PULLUP);
    pinMode(_data4, INPUT_PULLUP);
    pinMode(_data5, INPUT_PULLUP);
    pinMode(_data6, INPUT_PULLUP);
    pinMode(_data7, INPUT_PULLUP);

    digitalWrite(_addr_clk, LOW);
    digitalWrite(_addr_next, LOW);

    digitalWrite(_low_in, HIGH);
    digitalWrite(_low_out, HIGH);
    digitalWrite(_low_enable, HIGH);

    digitalWrite(_high_in, HIGH);
    digitalWrite(_high_out, HIGH);
    digitalWrite(_high_enable, HIGH);

    delay(TIMEOUT_READY);
}

void EEPROM::read(word_t* dest) {
    uint8_t i = 0x00;

    do {
        *(dest + i) = read_word();
        next();
    } while (i++ < 0xFF);
}

void EEPROM::write(word_t* data) {
    uint8_t i = 0x00;

    do {
        write_word(data[i]);
        next();
    } while (i++ < 0xFF);
}

void EEPROM::next() {
    digitalWrite(_addr_next, HIGH);
    digitalWrite(_addr_clk, HIGH);
    delay(1);

    digitalWrite(_addr_clk, LOW);
    digitalWrite(_addr_next, LOW);
    delay(1);

    addr++;
}

word_t EEPROM::read_word() {
    byte_t high = read_low();
    delay(TIMEOUT_ACTION);

    byte_t low = read_high();
    delay(TIMEOUT_ACTION);

#ifdef EEPROM_VERBOSE_OP
    Serial.print("RD ");
    print_byte(addr);
    Serial.print(" ");
    print_byte(high);
    Serial.print(" ");
    print_byte(low);
    Serial.println("");
#endif

    return (low << 8) | high;
}

void EEPROM::write_word(word_t data) {
#ifdef EEPROM_VERBOSE_OP
    Serial.print("WT ");
    print_byte(addr);
    Serial.print(" ");
    print_word(data);
    Serial.println("");
#endif

    write_low(data);
    delay(TIMEOUT_ACTION);

    write_high(data>>8);
    delay(TIMEOUT_ACTION);
}

byte_t EEPROM::read_low() {
    byte low = 0;

    pinMode(_data0, INPUT_PULLUP);
    pinMode(_data1, INPUT_PULLUP);
    pinMode(_data2, INPUT_PULLUP);
    pinMode(_data3, INPUT_PULLUP);
    pinMode(_data4, INPUT_PULLUP);
    pinMode(_data5, INPUT_PULLUP);
    pinMode(_data6, INPUT_PULLUP);
    pinMode(_data7, INPUT_PULLUP);
    delay(TIMEOUT_ACTION);

    digitalWrite(_low_enable, LOW);
    delayMicroseconds(TIMEOUT_ENABLE);

    digitalWrite(_low_out, LOW);
    delayMicroseconds(TIMEOUT_READ);

    low |= digitalRead(_data0) << 0;
    low |= digitalRead(_data1) << 1;
    low |= digitalRead(_data2) << 2;
    low |= digitalRead(_data3) << 3;
    low |= digitalRead(_data4) << 4;
    low |= digitalRead(_data5) << 5;
    low |= digitalRead(_data6) << 6;
    low |= digitalRead(_data7) << 7;

    digitalWrite(_low_out, HIGH);
    delayMicroseconds(TIMEOUT_WRITE);

    digitalWrite(_low_enable, HIGH);
    delayMicroseconds(TIMEOUT_ENABLE);

    return low;
}

byte_t EEPROM::read_high() {
    byte high = 0;

    pinMode(_data0, INPUT_PULLUP);
    pinMode(_data1, INPUT_PULLUP);
    pinMode(_data2, INPUT_PULLUP);
    pinMode(_data3, INPUT_PULLUP);
    pinMode(_data4, INPUT_PULLUP);
    pinMode(_data5, INPUT_PULLUP);
    pinMode(_data6, INPUT_PULLUP);
    pinMode(_data7, INPUT_PULLUP);
    delay(TIMEOUT_ACTION);

    digitalWrite(_high_enable, LOW);
    delayMicroseconds(TIMEOUT_ENABLE);

    digitalWrite(_high_out, LOW);
    delayMicroseconds(TIMEOUT_READ);

    high |= digitalRead(_data0) << 0;
    high |= digitalRead(_data1) << 1;
    high |= digitalRead(_data2) << 2;
    high |= digitalRead(_data3) << 3;
    high |= digitalRead(_data4) << 4;
    high |= digitalRead(_data5) << 5;
    high |= digitalRead(_data6) << 6;
    high |= digitalRead(_data7) << 7;

    digitalWrite(_high_out, HIGH);
    delayMicroseconds(TIMEOUT_WRITE);

    digitalWrite(_high_enable, HIGH);
    delayMicroseconds(TIMEOUT_ENABLE);

    return high;
}

void EEPROM::write_low(byte_t data) {
    debugbreak("Low DATA -> WRITE");

    pinMode(_data0, OUTPUT);
    pinMode(_data1, OUTPUT);
    pinMode(_data2, OUTPUT);
    pinMode(_data3, OUTPUT);
    pinMode(_data4, OUTPUT);
    pinMode(_data5, OUTPUT);
    pinMode(_data6, OUTPUT);
    pinMode(_data7, OUTPUT);

    digitalWrite(_data0, BIN0(data) ? HIGH : LOW);
    digitalWrite(_data1, BIN1(data) ? HIGH : LOW);
    digitalWrite(_data2, BIN2(data) ? HIGH : LOW);
    digitalWrite(_data3, BIN3(data) ? HIGH : LOW);
    digitalWrite(_data4, BIN4(data) ? HIGH : LOW);
    digitalWrite(_data5, BIN5(data) ? HIGH : LOW);
    digitalWrite(_data6, BIN6(data) ? HIGH : LOW);
    digitalWrite(_data7, BIN7(data) ? HIGH : LOW);
    delayMicroseconds(TIMEOUT_WRITE);

    debugbreak("Low Enable -> LOW");

    digitalWrite(_low_enable, LOW);
    delayMicroseconds(TIMEOUT_ENABLE);

    debugbreak("Low In -> LOW");

    digitalWrite(_low_in, LOW);
    delayMicroseconds(TIMEOUT_WRITE);

    debugbreak("Low In -> HIGH");

    digitalWrite(_low_in, HIGH);
    delayMicroseconds(TIMEOUT_WRITE);

    debugbreak("Low Enable -> HIGH");

    digitalWrite(_low_enable, HIGH);
    delayMicroseconds(TIMEOUT_ENABLE);

    pinMode(_data0, INPUT_PULLUP);
    pinMode(_data1, INPUT_PULLUP);
    pinMode(_data2, INPUT_PULLUP);
    pinMode(_data3, INPUT_PULLUP);
    pinMode(_data4, INPUT_PULLUP);
    pinMode(_data5, INPUT_PULLUP);
    pinMode(_data6, INPUT_PULLUP);
    pinMode(_data7, INPUT_PULLUP);
}

void EEPROM::write_high(byte_t data) {
    debugbreak("High DATA -> WRITE");

    pinMode(_data0, OUTPUT);
    pinMode(_data1, OUTPUT);
    pinMode(_data2, OUTPUT);
    pinMode(_data3, OUTPUT);
    pinMode(_data4, OUTPUT);
    pinMode(_data5, OUTPUT);
    pinMode(_data6, OUTPUT);
    pinMode(_data7, OUTPUT);

    digitalWrite(_data0, BIN0(data) ? HIGH : LOW);
    digitalWrite(_data1, BIN1(data) ? HIGH : LOW);
    digitalWrite(_data2, BIN2(data) ? HIGH : LOW);
    digitalWrite(_data3, BIN3(data) ? HIGH : LOW);
    digitalWrite(_data4, BIN4(data) ? HIGH : LOW);
    digitalWrite(_data5, BIN5(data) ? HIGH : LOW);
    digitalWrite(_data6, BIN6(data) ? HIGH : LOW);
    digitalWrite(_data7, BIN7(data) ? HIGH : LOW);
    delayMicroseconds(TIMEOUT_WRITE);

    debugbreak("High Enable -> LOW");

    digitalWrite(_high_enable, LOW);
    delayMicroseconds(TIMEOUT_ENABLE);

    debugbreak("High In -> LOW");

    digitalWrite(_high_in, LOW);
    delayMicroseconds(TIMEOUT_WRITE);

    debugbreak("High In -> HIGH");

    digitalWrite(_high_in, HIGH);
    delayMicroseconds(TIMEOUT_WRITE);

    debugbreak("High Enable -> HIGH");

    digitalWrite(_low_enable, HIGH);
    delayMicroseconds(TIMEOUT_ENABLE);

    pinMode(_data0, INPUT_PULLUP);
    pinMode(_data1, INPUT_PULLUP);
    pinMode(_data2, INPUT_PULLUP);
    pinMode(_data3, INPUT_PULLUP);
    pinMode(_data4, INPUT_PULLUP);
    pinMode(_data5, INPUT_PULLUP);
    pinMode(_data6, INPUT_PULLUP);
    pinMode(_data7, INPUT_PULLUP);
}

#endif
