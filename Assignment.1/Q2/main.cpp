#include <iostream>
#include "octetbag.h"

using namespace std;

int main()
{
    octetbag mybag;
    uint8_t number;
    uint8_t repeats;

    printf("Testing add() function.\nEnter a value:");
    cin >> number;
    if(mybag.add(number)){
        printf("Number successfully stored in the bag.\n");
    } else printf("Number wasn't added. bag is probably full");

    printf("\nTesting belong() function.\nEnter a value: ");
    cin >> number;
    repeats = mybag.belong(number);
    if(repeats > 0) {
        printf("Number exists in the bag %i times. Safe to remove but not add.\n", repeats);
    } else
        printf("Number not found in the bag. Safe to add but not remove.\n");

    printf("\nTesting remove() function.\nEnter a value: ");
    cin >> number;
    if(mybag.remove(number)) {
        printf("Number successfully removed from the bag.\n");
    } else printf("Number not removed. Number probably doesn't exist. Use belong() to check.\n");

    return 0;
}
