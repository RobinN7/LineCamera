#include "main.h"

// color = R V ou B, dutycycle entre 0 et 1023
void pwm(int dutycycle) {
    CCPR3L=dutycycle/4;
    DC3B1=(dutycycle/2)%1;
    DC3B0=0;
}
