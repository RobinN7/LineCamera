/*
 * File:   main.c
 * Author: Robin
 *
 * Created on 15 april 2015, 20:06
 */

//****************************************************************************************
//                    CONFIGURATION BITS PIC18F25K80
//****************************************************************************************

// Configuration register

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG    // SOSC Power Selection and mode Configuration bits (Digital (SCLKI) mode)
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled) (!)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled) (!)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = OFF      // Master Clear Enable (MCLR Disabled, RG5 Enabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 0C000-0FFFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 0C000-0FFFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)


///////////////////////////////// INCLUDES /////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <p18f25k80.h>
#include <xc.h>
#include <timers.h>
#include <portb.h>
#include "main.h"
#include "converter.h"
#include "init.h"

//////////////////////////////// GLOBAL VARS ///////////////////////////////////

double line[128]= {0};

int main(int argc, char** argv) {

    initialisation();

    //char msg[15]="";
    //sprintf(msg, "%d\n\r", i);
    //writeStringToUART(msg);

    while (1) {
        // Lecture de l'ADC
        readLine();
    }

    return (EXIT_SUCCESS);
}


void writeStringToUART (const char *msg)
{
    while(*msg)    {
        while(PIR3bits.TX2IF == 0) {}
        TXREG2 = *msg++;
    }
}

// Lecture d'un canal de l'ADC
unsigned int readADC(void)
{
    char tempo=0;

    for(tempo=0;tempo<100;tempo++);  // Capa charging
    GO_nDONE = 1; //Initializes A/D Conversion
    while(GO_nDONE); //Wait for A/D Conversion to complete

    return ((ADRESH<<8)+ADRESL);
}

void readLine(void) // fonction de détection de la ligne
{
    CLK=0; // la clock est nulle au départ
    unsigned char compteur = 0, index_pixel = 0, capture_finie = 0;

    while (!capture_finie) {
        if (compteur & 1)    // si compteur impair => front descendant
            CLK = 0;
        else                // compteur pair => montant
            CLK = 1;
        if(compteur == 5)
            SI = 1;

        if (compteur == 7)
            SI = 0;
        if ( (compteur & 1)  && compteur >= 7) {     // mesure sur front descendant,
            //   pc.printf("lecture pixel\n");
            line[index_pixel]=readADC();
            index_pixel++;
        }

        unsigned int tempo=0;
        for(tempo=0;tempo<160;tempo++); // wait for 10 µs

        compteur++;

        if (index_pixel == 128)
            capture_finie = 1;
    }
}