/* 
 *   yang2.c - code for the second MCS assignment
 *   Created:     23/02/2021
 *   Last edited: 15/03/2021
 *   Author: Yang Li
 *   ID: 1962731
 */
#include <18F27K40.h>
#fuses CLKOUT NOMCLR
#use delay(internal=32MHZ,clock_out)
#PIN_SELECT PWM4 = PIN_A4
#PIN_SELECT SCK2 = PIN_B4
#PIN_SELECT SDO2 = PIN_B5
#use SPI(SPI2, MASTER, MODE=0, BITS=8)


struct IO_def1
// Although this is a separate project
// Let's leave used pins in Assignment 1 unocupied
{
    int8 RA01234_Unused:4;
    int8 RA4_PWM4:1;
    int8 RA567_Unused:3;
    int8 RB01_Waveform_Selection:2;
    int8 RB23_Unused:2;
    int8 RB4_MSSP2_Serial_CLOCK:1;
    int8 RB5_MSSP2_DATA:1;
    int8 RB67_Unused:2;
    int8 RC012345_Unused:6;
    int8 RC6_MSSP2_CHIP_SELECT:1;
    int8 RC7_Port_Debug:1;
};

struct IO_def1 IO_Latch_1, IO_Direction_1, IO_Port_1;

#byte IO_Latch_1 = 0xF83
#byte IO_Direction_1 = 0xF88
#byte IO_Port_1 = 0xF8D

int16 LUT_sin_1khz[] = {0x1800,0x198F,0x1B0F,0x1C71,0x1DA7,0x1EA6,0x1F63,0x1FD8,
                        0x1FFF,0x1FD8,0x1F63,0x1EA6,0x1DA7,0x1C71,0x1B0F,0x198F,
                        0x1800,0x1670,0x14F0,0x138E,0x1258,0x1159,0x109C,0x1027,
                        0x1000,0x1027,0x109C,0x1159,0x1258,0x138E,0x14F0,0x1670};
int16 LUT_sin_2khz[] = {0x1800,0x1B0F,0x1DA7,0x1F63,0x1FFF,0x1F63,0x1DA7,0x1B0F,
                        0x1800,0x14F0,0x1258,0x109C,0x1000,0x109C,0x1258,0x14F0,
                        0x1800,0x1B0F,0x1DA7,0x1F63,0x1FFF,0x1F63,0x1DA7,0x1B0F,
                        0x1800,0x14F0,0x1258,0x109C,0x1000,0x109C,0x1258,0x14F0};
int16 LUT_sin_1khz_2khz[] = {0x1800,0x1AA0,0x1CF3,0x1EB8,0x1FC1,0x1FF9,0x1F69,0x1E32,
                             0x1C8B,0x1AB7,0x18FC,0x1794,0x16AB,0x1653,0x1686,0x1725,
                             0x1800,0x18DA,0x1979,0x19AC,0x1954,0x186B,0x1703,0x1548,
                             0x1374,0x11CD,0x1096,0x1006,0x103E,0x1147,0x130C,0x155F};
int index_a;

#int_timer2
void Timer2_Service_Routine(void)
{
    switch(IO_Port_1.RB01_Waveform_Selection)
    {
        case 0b00:
            IO_Port_1.RC7_Port_Debug = 0b1;
            IO_Port_1.RC6_MSSP2_CHIP_SELECT = 0b0;
            // SPI Chip select signal low
            spi_write2(LUT_sin_1khz[index_a]>>8);
            // Shifts bits to the right and take the left 8 bits (1 byte)
            spi_write2(LUT_sin_1khz[index_a] & 0x00FF);
            // Bitwise operation to get the least significant 8 bits (1 byte)
            IO_Port_1.RC6_MSSP2_CHIP_SELECT = 0b1;
            index_a = ++index_a % 32;
            IO_Port_1.RC7_Port_Debug = 0b0;
            break;
            
        case 0b01:
            IO_Port_1.RC7_Port_Debug = 0b1;
            IO_Port_1.RC6_MSSP2_CHIP_SELECT = 0b0;
            // SPI Chip select signal low
            spi_write2(LUT_sin_2khz[index_a]>>8);
            // Shifts bits to the right and take the left 8 bits (1 byte)
            spi_write2(LUT_sin_2khz[index_a] & 0x00FF);
            // Bitwise operation to get the least significant 8 bits (1 byte)
            IO_Port_1.RC6_MSSP2_CHIP_SELECT = 0b1;
            index_a = ++index_a % 32;
            IO_Port_1.RC7_Port_Debug = 0b0;
            break;
            
        case 0b10:
            IO_Port_1.RC7_Port_Debug = 0b1;
            IO_Port_1.RC6_MSSP2_CHIP_SELECT = 0b0;
            // SPI Chip select signal low
            spi_write2(LUT_sin_1khz_2khz[index_a]>>8);
            // Shifts bits to the right and take the left 8 bits (1 byte)
            spi_write2(LUT_sin_1khz_2khz[index_a] & 0x00FF);
            // Bitwise operation to get the least significant 8 bits (1 byte)
            IO_Port_1.RC6_MSSP2_CHIP_SELECT = 0b1;
            index_a = ++index_a % 32;
            IO_Port_1.RC7_Port_Debug = 0b0;
            break;
    }
}

void main(void)
{
    // Define all user variables

    // Define direction of all 24 I/0 pins
    IO_Direction_1.RA4_PWM4 = 0b0;
    IO_Direction_1.RB01_Waveform_Selection = 0b11;
    IO_Direction_1.RB4_MSSP2_Serial_CLOCK = 0b0;
    IO_Direction_1.RB5_MSSP2_DATA = 0b0;
    IO_Direction_1.RC6_MSSP2_CHIP_SELECT = 0b0;
    IO_Direction_1.RC7_Port_Debug = 0b0;
    
    port_b_pullups(0x03);

    // Set up peripheral hardware
    setup_oscillator(OSC_HFINTRC_32MHZ);
    

    setup_timer_2(T2_DIV_BY_1 | T2_CLK_INTERNAL, 249, 1);
    // For best resolution, make the 'reset' value of the counter
    // as large as possible (<=255) and the prescalar value
    // as small as possible. Note that the division ration is
    // one more than the value passed to the period register
    setup_ccp2(CCP_PWM|CCP_USE_TIMER1_AND_TIMER2);
    // Configure CCP2 as a PWM,
    // CCP2 is paired with Timer 2
    setup_pwm4(PWM_ENABLED | PWM_ACTIVE_HIGH | PWM_TIMER2);
    set_pwm4_duty(968);
    
    enable_interrupts(INT_TIMER2);
    enable_interrupts(GLOBAL);
    
    while(TRUE)
    {
        
    }
}
