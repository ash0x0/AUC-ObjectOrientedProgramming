#include <iostream>
#include "octetset.h"

using namespace std;

int main()
{
    octetset myset;
    uint8_t number;


    printf("Testing add() function.\nEnter a value:");
    cin >> number;
    if(myset.add(number)){
        printf("Number successfully stored in the set.\n");
    } else printf("Number wasn't added. set is probably full");

    printf("\nTesting belong() function.\nEnter a value: ");
    cin >> number;
    if(myset.belong(number)) {
        printf("Number exists in the set. Safe to remove but not add.\n");
    } else
        printf("Number not found in the set. Safe to add but not remove.\n");

    printf("\nTesting remove() function.\nEnter a value: ");
    cin >> number;
    if(myset.remove(number)) {
        printf("Number successfully removed from the set.\n");
    } else printf("Number not removed. Number probably doesn't exist. Use belong() to check.\n");

    return 0;
}
