
// PIC24EP256GP202 Configuration Bit Settings

// 'C' source line config statements

// FICD
#pragma config ICS = NONE               // ICD Communication Channel Select bits (Reserved, do not use)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = FRCDIVN          // Oscillator Source Selection (Internal Fast RC (FRC) Oscillator with postscaler)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>

void delay(unsigned int delayVal);
unsigned int pot_selected = 0;

void selectPOT0(void) {
    pot_selected = 0;
    long position = 0b00000001000000010;
    LATBbits.LATB7 = 1;                     // pull RST of POT to 1
    LATBbits.LATB9 = 1;                     // set CLK of POT to 1
    LATBbits.LATB8 = position;              // set POT Shift reg to both pot wipers at 0% and select pot0
    LATBbits.LATB9 = 0;                     // set CLK of POT to 0
    LATBbits.LATB7 = 0;                     // pull RST of POT to 0
    // loop through 256 positions (0x0202 = 514)
    for (; position < 0b11111111111111110; position += 0x0202) {// increment position
        LATBbits.LATB7 = 1;                 // pull RST of POT to 1
        LATBbits.LATB9 = 1;                 // set CLK of POT to 1
        LATBbits.LATB8 = position;          // set POT Shift reg to both pot wipers at next position and select pot0
        LATBbits.LATB9 = 0;                 // set CLK of POT to 0
        LATBbits.LATB7 = 0;                 // pull RST of POT to 0
        //delay at this pos
        unsigned int j;
        for (j = 0; j < 160; j++);
    }
}

void selectPOT1(void) {
    pot_selected = 1;
    long position = 0b11111111111111111;
    LATBbits.LATB7 = 1;                     // pull RST of POT to 1
    LATBbits.LATB9 = 1;                     // set CLK of POT to 1
    LATBbits.LATB8 = position;              // set POT Shift reg to both pot wipers at 100% and select pot1
    LATBbits.LATB9 = 0;                     // set CLK of POT to 0
    LATBbits.LATB7 = 0;                     // pull RST of POT to 0
    // loop through 256 positions (0x0202 = 514)
    for (; position > 0b00000001000000011; position -= 0x0202) {// increment position
        LATBbits.LATB7 = 1;                 // pull RST of POT to 1
        LATBbits.LATB9 = 1;                 // set CLK of POT to 1
        LATBbits.LATB8 = position;          // set POT Shift reg to both pot wipers at next position and select pot1
        LATBbits.LATB9 = 0;                 // set CLK of POT to 0
        LATBbits.LATB7 = 0;                 // pull RST of POT to 0
        //delay at this pos
        unsigned int j;
        for (j = 0; j < 160; j++);
    }
}

void mux1A(void) {
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 0;
}

void mux1B(void) {
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 1;
}

void mux1C(void) {
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
}

void mux1D(void) {
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 1;
}

void mux2A(void) {
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
}

void mux2B(void) {
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
}

void mux2C(void) {
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
}

void mux2D(void) {
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
}

enum {
    mux1Apot0 = 0,
    mux2Apot1 = 1,
    mux1Bpot0 = 2,
    mux2Bpot1 = 3,
    mux1Cpot0 = 4,
    mux2Cpot1 = 5,
    mux1Dpot0 = 6,
    mux2Dpot1 = 7,
};

unsigned int state = 0;

int main(void)
{
    ANSELB = 0;
    LATB = 0xE000; 
    TRISB = 0xE000;
    
    selectPOT0();
    
    while(1)
    {
        if(PORTBbits.RB13 == 0)
        {
        switch(state) {
            case mux1Apot0:
                mux1A();
                selectPOT0();
                // delay(2048);
                break;
            case mux2Apot1:
                mux2A();
                selectPOT1();
                // delay(2048);
                break;
            case mux1Bpot0:
                mux1B();
                selectPOT0();
                // delay(2048);
                break;
            case mux2Bpot1:
                mux2B();
                selectPOT1();
                // delay(2048);
                break;
            case mux1Cpot0:
                mux1C();
                selectPOT0();
                // delay(2048);
                break;
            case mux2Cpot1:
                mux2C();
                selectPOT1();
                // delay(2048);
                break;
            case mux1Dpot0:
                mux1D();
                selectPOT0();
                // delay(2048);
                break;
            case mux2Dpot1:
                mux2D();
                selectPOT1();
                // delay(2048);
                break;
            default:
                state = mux1Apot0-1;
                break;
        }
        state++;
        }
    }
    return 0;
    
}

void delay(unsigned int delayVal)
{
    unsigned int i,j;

    for (i = 0; i < delayVal; i++)
    {
        for (j = 0; j < 20; j++);
    }
}



/**********************************************************************
* © 2013 Microchip Technology Inc.
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (Microchip) licenses this software to you
* solely for use with Microchip dsPIC digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED AS IS.  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
**********************************************************************/

