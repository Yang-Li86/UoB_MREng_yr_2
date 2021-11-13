/* 
 *   MCS_A1_Frag_1.c - code for the first MCS assignment
 *   Created:     02/02/2021
 *   Last edited: 16/02/2021
 *   Author: Yang Li
 *   ID: 1962731
 */

#include <18F27K40.h> 
// Header file, include this library
#fuses RSTOSC_HFINTRC_1MHZ CLKOUT NOMCLR NOPUT NOWDT NOLVP NOPROTECT NOCPD
// Master clock frequency of 4 MHz -> CDIV -> CLKOUT of 250 kHz
#use delay(clock = 1MHZ,clock_out)
// Tell the compiler the clock speed after CDIV is 1 MHz using this directive

/* 
 * 6 LEDs on pins RC0..5
 * 8 Switches on pins RA0..3 and RB0..3
 */



struct IO_def1 
// Define a structure with the members listed in order of increasing memory address
{
    int8 Experiment_Switches_RA0123:4;
    // RA0..3 are experiment selector switches for selecting one/16 total experiments
    int8 UnusedPort_RA4567:4;
    // Pins RA4..7 are unused
    int8 Set_RB0:1;
    // Set for 3.1.11
    int8 Reset_RB1:1;
    // Reset for 3.1.11
    int8 Switch_RB234567:6;
    // Pins RB2..7 unused
    int8 LED_RC0:1;
    // Pin RC0 is used for single pin output 3.1.1 and 3.1.11
    int8 LED_RC1234567:7;
    // Pin RC1..7
};

struct IO_def2
// Define a structure with the members listed in order of increasing memory address
{
    int8 Experiment_Switches_RA0123:4;
    // RA0..3 are experiment selector switches for selecting one/16 total experiments
    int8 UnusedPort_RA4567:4;
    // Pins RA4..7 are unused
    int8 Switch_RB0123:4;
    // Pins RB0..1 bitwise logic
    int8 UnusedPort_RB4567:4;
    // Pins RB4..7 are unused
    int8 LED_RC0123:4;
    // Pin RC0..3 implements a 4-bit pure-binary counter
    int8 LED_RC4:1;
    // FSM
    int8 UnusedPortC:3;
    // Pins RC5..7 are unused
};

struct IO_def3
// Define a structure with the members listed in order of increasing memory address
{
    int8 Experiment_Switches_RA0123:4;
    // RA0..3 are experiment selector switches for selecting one/16 total experiments
    int8 UnusedPort_RA4567:4;
    // Pins RA4..7 are unused
    int8 UnusedPort_RB01234567:8;
    // Pins RB0..7 are unused
    int8 LED_RC012:3;
    // Pins RC0..3 implements a 3-bit pure-binary counter
    int8 LED_RC345:3;
    // Pins RC3..5 implements a 3-bit Gray-Code up-counter
    int8 LED_RC67:2;
    // Pins RC6..7 are unused
};

struct IO_def4
// Define a structure with the members listed in order of increasing memory address
{
    int8 Experiment_Switches_RA0123:4;
    // RA0..3 are experiment selector switches for selecting one/16 total experiments
    int8 UnusedPort_RA4567:4;
    // Pins RA4..7 are unused
    int8 Switch_RB01:2;
    // Pins RB0..1 bitwise logic
    int8 Switch_RB23:2;
    // Pins RB2..3 bitwise logic
    int8 UnusedPort_RB4567:4;
    // Pins RB4..7 are unused
    int8 LED_RC01:2;
    // Pins RC0..1 are used as the output of bit-wise operation
    int8 LED_RC234567:6;
    // Pins not used
};

struct IO_def5
{
    int8 Experiment_Switches_RA0123:4;
    // RA0..3 are experiment selector switches for selecting one/16 total experiments
    int8 UnusedPort_RA4567:4;
    // Pins RA4..7 are unused
    int8 Switch_RB0123:4;
    // Pins RB0..3 input look-up tables
    int8 UnusedPort_RB4567:4;
    // Pins RB4..7 are unused
    int8 LED_RC012345:6;
    // Pins RC0..5 are used for input look up tables
    int8 LED_RC67:2;
    // Pins not used
};

struct IO_def1 IO_Latch_1, IO_Direction_1, IO_Port_1;
struct IO_def2 IO_Latch_2, IO_Direction_2, IO_Port_2;
struct IO_def3 IO_Latch_3, IO_Direction_3, IO_Port_3;
struct IO_def4 IO_Latch_4, IO_Direction_4, IO_Port_4;
struct IO_def5 IO_Latch_5, IO_Direction_5, IO_Port_5;

#byte IO_Latch_1 = 0xF83
#byte IO_Direction_1 = 0xF88
#byte IO_Port_1 = 0xF8D

#byte IO_Latch_2 = 0xF83
#byte IO_Direction_2 = 0xF88
#byte IO_Port_2 = 0xF8D

#byte IO_Latch_3 = 0xF83
#byte IO_Direction_3 = 0xF88
#byte IO_Port_3 = 0xF8D

#byte IO_Latch_4 = 0xF83
#byte IO_Direction_4 = 0xF88
#byte IO_Port_4 = 0xF8D

#byte IO_Latch_5 = 0xF83
#byte IO_Direction_5 = 0xF88
#byte IO_Port_5 = 0xF8D

void main(void)
{
    int index_a=0;
    int four_bit_gray[]={0b0000, 0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b0101,
                         0b0111, 0b1111, 0b1110, 0b1010, 0b1011, 0b1001, 0b1101, 
                         0b1100, 0b1000};
    int three_bit_gray[]={0b000, 0b001, 0b011, 0b010, 0b110, 0b111, 0b101, 0b100};
    int look_up[]={0b000000, 0b100000, 0b010000, 0b001000, 0b000100, 0b000010, 
                   0b000001, 0b111111, 0b000000, 0b000001, 0b000010, 0b000100, 
                   0b001000, 0b010000, 0b100000, 0b111111};
    int FSM[]={0b0111, 0b1000, 0b1001, 0b1110, 0b0101, 0b1100, 0b1101, 0b0110, 
               0b0010, 0b0011, 0b0001, 0b1010, 0b1011, 0b0111, 0b1000, 0b1001, 0b1110};
    // Define the variables first
    
    port_a_pullups(0xFF);
    port_b_pullups(0xFF);
    port_c_pullups(0xFF);
    port_e_pullups(0xFF);
    
    IO_Direction_1.Experiment_Switches_RA0123=0xF;
    IO_Direction_1.LED_RC0=0b0;
    IO_Direction_2.Switch_RB0123=0xF;
    IO_Direction_2.LED_RC0123=0x0;
    IO_Direction_2.LED_RC4=0b0;
    IO_Direction_3.LED_RC012=0b000;
    IO_Direction_3.LED_RC345=0b000;
    IO_Direction_4.Switch_RB01=0b11;
    IO_Direction_4.Switch_RB23=0b11;
    IO_Direction_4.LED_RC01=0b00;
    IO_Direction_5.Switch_RB0123=0xF;
    IO_Direction_5.LED_RC012345=0b000000;
    // Define the direction of our I/O ports
    
    // In the future, here will be used to set up some peripherals

    // Most embedded processors would use endless loops
    switch (IO_Port_1.Experiment_Switches_RA0123)
    {
        case 0b0000: 
            setup_oscillator(OSC_HFINTRC_16MHZ);
#use delay(clock = 16MHZ,clock_out)
        // 3.1.0 Demonstrate that RA6 is running at 4 MHz
            for(index_a=0; index_a<10; index_a++)
            {
                IO_Port_1.LED_RC0 = ~IO_Port_1.LED_RC0;
                //delay_us(500);
            }
            break;
    
        case 0b0001:
        // 3.1.1 Single Pin Output with microprocessor clock running at 16 MHz
            setup_oscillator(OSC_HFINTRC_16MHZ);
#use delay(clock = 16MHZ,clock_out)
            for (index_a=0; index_a<17; index_a++)
            {
                IO_Port_1.LED_RC0 = ~IO_Port_1.LED_RC0;
                delay_us(500);
            }
            break;
        
        case 0b0010:
        // 3.1.2 Implicitly demonstrate the 'switch/case' construct
#use delay(internal = 1MHZ,clock_out)
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_5.LED_RC012345 = look_up[index_a];
                delay_us(100);
            }
            break;
        
        case 0b0011:
        // 3.1.3 Multiple Pin Output(i)
            IO_Port_2.LED_RC0123 = 0b0000;
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_2.LED_RC0123++;
                delay_us(100);
            }
            break;
            
        case 0b0100:
        // 3.1.4 Multiple Pin Output(ii)
            IO_Port_2.LED_RC0123 = 0b0000;
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_2.LED_RC0123--;
                delay_us(100);
            }
            break;
            
        case 0b0101:
        // 3.1.5 Multiple Pin Output with output logic
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_2.LED_RC0123 = four_bit_gray[index_a];
                delay_us(100);
            }
            break;
            
        case 0b0110:
        // 3.1.6 3-bit pure-binary up-counter and simultaneous 3-bit Gray-code up-counter
            IO_Port_3.LED_RC012 = 0b000;
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_3.LED_RC345 = three_bit_gray[IO_Port_3.LED_RC012++];
                delay_us(100);
            }
            break; 
        
        case 0b0111:
        // 3.1.7 Demonstrate bitwise AND operation
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_4.LED_RC01 = IO_Port_4.Switch_RB01 & IO_Port_4.Switch_RB23;
                delay_cycles(90);
            }
            break;
          
        case 0b1000:
        // 3.1.8 Demonstrate bitwise OR operation
            for(index_a=0; index_a<60; index_a++)
            {
                IO_Port_4.LED_RC01 = IO_Port_4.Switch_RB01 | IO_Port_4.Switch_RB23;
                delay_cycles(10);
            }
            
            break;
            
        case 0b1001:
        // 3.1.9 Demonstrate bitwise NOT operation
            for(index_a=0; index_a<65; index_a++)
            {
                IO_Port_2.LED_RC0123 = ~IO_Port_2.Switch_RB0123;
                delay_cycles(10);
            }
            break;
            
        case 0b1010:
        // 3.1.10 Demonstrate bitwise XOR operation
            for(index_a=0; index_a<65; index_a++)
            {
                IO_Port_4.LED_RC01 = IO_Port_4.Switch_RB01 ^ IO_Port_4.Switch_RB23;
                delay_cycles(10);
            }
            break;
            
        case 0b1011:
        // 3.1.11 Demonstrate set-reset flip-flop synthesis
            for(index_a=0; index_a<65; index_a++)
            {
                IO_Port_1.LED_RC0 = IO_Port_1.Set_RB0 | (~IO_Port_1.Reset_RB1 & IO_Latch_1.LED_RC0);
                delay_cycles(10);
            }
            break;
            
        case 0b1100:
        // 3.1.12 Demonstrate input look-up tables
            for(index_a=0; index_a<65; index_a++)
            {
                IO_Port_5.LED_RC012345 = look_up[IO_Port_5.Switch_RB0123];
                delay_cycles(10);
            }
            break;
           
        case 0b1101:
        // 3.1.13 Demonstrate your DEEM FSM specification
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_2.LED_RC0123 = FSM[index_a];
                IO_Port_2.LED_RC4 = ~(IO_Port_2.LED_RC0123 >> 3) & 
                                    ~(IO_Port_2.LED_RC0123 >> 2 & 0b1) & 
                                     (IO_Port_2.LED_RC0123 >> 1 & 0b1) & 
                                    ~(IO_Port_2.LED_RC0123 & 0b1);
                delay_us(100);
            }
            break;
           
        default:
            for(index_a=0; index_a<17; index_a++)
            {
                IO_Port_5.LED_RC012345 = look_up[index_a];
                delay_us(100);
            }
            break;
    }
}