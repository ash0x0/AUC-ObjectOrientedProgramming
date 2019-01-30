#include "octetset.h"

octetset::octetset()
{
    next = 0;
    for (uint8_t i = 0; i < setsize; i++) {
        octset[i] = -1;
    }
}

octetset::~octetset()
{
}

bool octetset::belong(uint8_t number) {
    for (uint8_t i = 0; i < setsize; i++) {
        uint8_t current = octset[i];
        if(current == number) {
            index = i;
            return true;
        }
    }
    return false;
}

bool octetset::add(uint8_t number) {
    if(belong(number)) {
        return false;
    } else if(next <= setsize){
        octset[next] = number;
        next++;
    } else {
        for (uint8_t i = 0; i < setsize; i++) {
            if (octset[i] == -1) {

            }
        }
    }
    return true;
}

bool octetset::remove(uint8_t number) {
    if (belong(number)) {
        octset[index] = -1;
        return true;
    } return false;
}
