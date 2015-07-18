/* 
 * File:   main.h
 * Author: robin
 *
 * Created on April 18, 2015, 1:59 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <p18f25k80.h>
#include "init.h"
#include "interrupts.h"
#include "converter.h"

void writeStringToUART (const char* msg);
unsigned int readADC(void);
void readLine(void);

#define CLK RA0
#define SI RA1

#endif	/* MAIN_H */

