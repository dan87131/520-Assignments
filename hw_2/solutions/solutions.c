#include "solutions.h"
#include <stdlib.h>

int running_total(int x){
  static int total = 0;
  total += x;
  return total;
}




int* reverse(int* array, int length) {
    int* reversed_array = (int*) calloc(length, sizeof(int));
    for (int i = 0; i < length; i++) {
        reversed_array[i] = array[length - i - 1];
    }
    return reversed_array;
}




void reverse_in_place(int* array, int length) {
    int l = 0;
    int r = length - 1;
    
    while (l < r) {
        int temp = array[l];
        array[l] = array[r];
        array[r] = temp;
        l++;
        r--;
    }
}





int num_occurrences(int* array, int length, int val) {
    int c = 0;
    for (int i = 0; i < length; i++) {
        if (array[i] == val) {
            c++;
        }
    }
    return c;
}

