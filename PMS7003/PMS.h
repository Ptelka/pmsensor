#ifndef PMS_H
#define PMS_H

#include <SoftwareSerial.h>

namespace pms {

struct Measures {
    unsigned int PM1_0;
    unsigned int PM2_5;
    unsigned int PM10_0;
    unsigned int is_ok = false;
};

class PMS7003 {
public:
    PMS7003(SoftwareSerial &serial);

    Measures read();
    void read(Measures& measures);

private:
    static constexpr int MAX_FRAME_LEN = 32;

    SoftwareSerial &serial;

    unsigned int bytes = 0;
    unsigned char currentByte = 0;
    unsigned char previousByte = 0;
    unsigned int currentFrameLength = MAX_FRAME_LEN;
    unsigned int checksum = 0;

    void drain();
    void readNext();
    bool sync();
    void fill(Measures &measures);
    void reset();
};

PMS7003::PMS7003(SoftwareSerial &serial)
: serial(serial) { }

Measures PMS7003::read() { 
    Measures measures;
    read(measures);
    return measures;
}

void PMS7003::read(Measures& measures) {
    bool synced = false;

    do {
        if (serial.available() > MAX_FRAME_LEN) {
            drain();
        }

        if (serial.available() < 1) {
            continue;
        }

        readNext();

        if (!synced) {
            synced = sync();
        } else {
            fill(measures);
        }
    } while (bytes < currentFrameLength);

    reset();
}

void PMS7003::drain() {
    Serial.print("PMS7003: draining: ");
    Serial.println(serial.available());
    while(serial.available()) {
        serial.read();
    }
}

void PMS7003::readNext() {
    previousByte = currentByte;
    currentByte = serial.read();
    checksum += currentByte;
    ++bytes;
    //Serial.print("PMS7003: got ");
    //Serial.println(currentByte, HEX);
}

bool PMS7003::sync() {
    if (bytes == 2 && currentByte == 0x4D && previousByte == 0x42) {
      Serial.println("PMS7003: Synced");
      return true;
    }
    
    if(bytes < 2) {
      return false;
    }
    
    bytes = 0;
    return false;
}

void PMS7003::fill(Measures &measures) {
    unsigned int val = (currentByte & 0xff) + (previousByte << 8);
    switch (bytes) {
        case 4:
            currentFrameLength = val + bytes;
            break;
        case 6:
            measures.PM1_0 = val;
            break;
        case 8:
            measures.PM2_5 = val;
            break;
        case 10:
            measures.PM10_0 = val;
            break;
        case 32:
            checksum -= ((val >> 8) + (val & 0xFF));
            measures.is_ok = val == checksum;
            break;
        default:
            break;
    }
}

void PMS7003::reset() {
    bytes = 0;
    currentFrameLength = MAX_FRAME_LEN;
    checksum = 0;
}

}   // namespace pms

#endif
