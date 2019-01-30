#include "octetbag.h"

octetbag::octetbag()
{
    next = 0;
    for (uint8_t i = 0; i < bagsize; i++) {
        octbag[i] = -1;
    }
}

octetbag::~octetbag()
{
}

uint8_t octetbag::belong(uint8_t number) {
    repcount = 0;
    for (uint8_t i = 0; i < bagsize; i++) {
        uint8_t current = octbag[i];
        if(current == number) {
            index = i;
            repcount++;
        }
    }
    return repcount;
}

bool octetbag::add(uint8_t number) {
    if(next <= bagsize){
        octbag[next] = number;
        next++;
        return true;
    } else {
        for (uint8_t i = 0; i < bagsize; i++) {
            if (octbag[i] == -1) {
                octbag[i] = number;
                return true;
            }
        }
    }
    return false;
}

bool octetbag::remove(uint8_t number) {
    if (belong(number)) {
        octbag[index] = -1;
        return true;
    } return false;
}
