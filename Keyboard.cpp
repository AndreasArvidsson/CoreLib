#include "Keyboard.h"
#include <conio.h> //kbhit

const char Keyboard::getDigit() {
    if (_kbhit()) {
        char c = '\0';
        //Get last char
        while (_kbhit()) {
            const char tmp = (char)_getch();
            if (tmp) {
                c = tmp;
            }
        }
        //Char must be a digit
        if (c >= '0' && c <= '9') {
            return c;
        }
    }
    return '\0';
}