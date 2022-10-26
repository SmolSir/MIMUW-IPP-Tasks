#include "char_functions.h"

bool standard_char(char c) {
    
    if (32 < c && c < 127) {
        return true;
    }

    return false;
}

bool incorrect_char(char c) {
    
    if (c < 9 || (13 < c && c < 32) || 126 < c) {
        return true;
    }

    return false;
}

char standard_format(char c) {
    
    if (64 < c && c < 91) {
        return (char)(c + 32);
    }
    
    return c;
}