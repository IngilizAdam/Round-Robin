#include "utils.h"

int strToInt(char *str){
    int value = 0;
    int digitValue = 1;
    for(int i = strlen(str)-1; i >= 0; i--){
        value += ((*(str+i)-48)*digitValue);
        digitValue *= 10;
    }
    
    return value;
}
