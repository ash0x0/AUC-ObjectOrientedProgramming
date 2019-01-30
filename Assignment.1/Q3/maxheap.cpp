#include "maxheap.h"

bool maxheap::insert(int item) {
    int i = ++Nel;
    if (i == MaxSize) {
        printf("Heap size exceeded!\n");
        return false;
    }
    while ((i > 1) && (array[i/2] < item)) {
        array[i] = array[i/2];
        i = i / 2;
    }
    array[i] = item;
    return true;
}

bool maxheap::delMax (int& item) {
    if (!Nel) {
        printf("Heap is empty!\n");
        return false;
    }
    item = array[i];
    arrar[1] = array[Nel--];
    adjust(array, 1, Nel);
    return true;
}

void maxheap::adjust(int a[], int i, int n) {
    int j = 2 * i;
    int item = a[i];

    while (j <= n) {
        if ((j < n) && (a[j] < a[j+1])) j++;
        if (item >= a[j]) break;
        a[j/2] = a[j];
        j = j * 2;
    }
    a[j/2] = item;
}

ostream &operator<< (ostream &output, const maxheap &heap) {
    printf("\n");
    for (int i = 0; i <= MaxSize; i++) {
        printf("%i\t", array[i]);
    }
}
