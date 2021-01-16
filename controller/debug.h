#ifndef _DEBUG_H_
#define _DEBUG_H_

uint8_t debug = 0;
char res;

void debugmsg(const char* msg) {
    Serial.print("DBG ");
    Serial.println(msg);
}

void debugbreak(const char* msg) {
    if (debug) debugmsg(msg);

    while (debug) {
        if (Serial.available() > 0) {
            res = (char) Serial.read();
            if (res == 'c') {
                break;
            } else if (res == 's') {
                debug = 0;
            }
        }
    }
}

#endif
