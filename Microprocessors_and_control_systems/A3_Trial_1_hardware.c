/* 
 *   A3_Trial_1_hardware.c - code for the third MCS assignment
 *   Created:     16/03/2021
 *   Edited:      07/05/2021
 *   Author: Yang Li
 *   ID: 1962731
 */
 
 
 
/* Include approproriate header files */
#include<18F27K40.h>
#include<stdio.h>
#include<string.h>

/* Set fuses and a clock frequency */
#fuses NOWDT NOPUT NOBROWNOUT NOLVP NOCPD NOMCLR NOEXTOSC CLKOUT NOPROTECT
#use delay(internal=32MHZ, clock_out)

/* Pin select */
#pin_select U1RX=PIN_C7
#pin_select U1TX=PIN_C6
#PIN_SELECT PWM4=PIN_A4

/* Initialise peripherals */
#use rs232(uart1, BAUD=9600, ERRORS, PARITY=N, STOP=2, BITS=8, STREAM = Serial_Stream)

/* Define a globle scope FIFO buffer
   Define TRUE and FALSE as C does not have bool type  */
#define SERIAL_BUFFER_SIZE 64                       // Try change this to 128
char serial_buffer[SERIAL_BUFFER_SIZE];
int serial_buffer_next_in = 0;    // Input pointer index, from 0 to 63
int serial_buffer_next_out = 0;   // Output pointer index , from 0 to 63

short int CmdDetectedFlg = FALSE;
short int first_power_up = TRUE;
int g_index;

char subarray[9];  // For substring_extractor function and other functions to use



struct IO_def
{
    int8 port_a_pins_3210:4;
    // Port A pins 3210 are not used in this experiment
    int8 port_a_pin_4_PWM:1;
    // Port A pin 4 is used as PWM output pin
    int8 port_a_pin_765:3;
    // Port A pins 765 are not used in this experiment
    int8 port_b_pins:8;
    // Port B pins are not used in this experiment
    int8 port_c_0_LED:1;
    int8 port_c_1_LED:1;
    int8 port_c_2_LED:1;
    int8 port_c_3_LED:1;
    int8 port_c_54_LEDs:2;
    // Pins 543210 of port C are connected to LEDs in this experiment
    int8 port_c_6_TX:1;
    // Pin RC6 is selected as the TX output for the PIC18F27K40
    int8 port_c_7_RX:1;
    // Pin RC7 is selected as the RX input for the PIC18F27K40
};

struct IO_def IO_Latch, IO_Direction, IO_Port;

#byte IO_Latch = 0xF83
#byte IO_Direction = 0xF88
#byte IO_Port = 0xF8D



#INT_RDA    // RDA strands for Receive data available
void serial_isr(void)    /* RS232 Receive Data Available */
{
    CmdDetectedFlg = FALSE;
    
    serial_buffer[serial_buffer_next_in] = fgetc(Serial_Stream);  
    // Get the data stream and store in the FIFO buffer
    serial_buffer_next_in = (serial_buffer_next_in+1) % SERIAL_BUFFER_SIZE;
    // Increment serial_buffer_next_in pointer with the range of 0 and 63
    
    if(serial_buffer[serial_buffer_next_in] == 0x0D)  // Carriage return detection
    {
        CmdDetectedFlg = TRUE;
    }
} // END serial_isr



/* Function that ignores meaningless space character leading the command
   and receive meaningful command terminated by a space or a carriage return
   so that user commands can be ready for strcmp()  */
char GetCom(char *command_string)  // command_string is a pointer to an array of characters
{
    short int SerialCmdReceivedFlg;
    int length;
    char c;
    length = 0;
    serial_buffer_next_out = 0;
    
    SerialCmdReceivedFlg = FALSE;
    // Reset command received flag
    
    /* Ignore leading spaces
       Increment the pointer until a non space character is read
       32 is Space in the ASCII Character Map  */
    do
    {
        c = serial_buffer[serial_buffer_next_out];
        serial_buffer_next_out = (serial_buffer_next_out + 1) % SERIAL_BUFFER_SIZE;
    } while(c == ' ');
    
    /* A non-space character has been entered */
    command_string[length++] = c;
    
    /* Get the rest of the command string
       Until sCR terminated or input string is too long
       13 is Carriage Return in the ASCII Character Map
       I am not sure how long is too long, since Phil put 20 there
       and I am not expecting a input of more than 20 characters... */
    while ((c != ' ') && (c != 13) && (length < 30))
    {
        c = serial_buffer[serial_buffer_next_out];
        serial_buffer_next_out = (serial_buffer_next_out + 1) % SERIAL_BUFFER_SIZE;
        command_string[length++] = c;
    }
    
    command_string[--length] = '\0';
    // Null terminate string
    
    SerialCmdReceivedFlg = TRUE;
    // Set command received flag
    serial_buffer_next_out = 0;
    
    return command_string;
}//END GetCom



/* This function separates the percentage for the brightness control command 
   Verified in CodeBlocks that if the input is correct, this would take the
   numbered digits and convert them to int16*/
int16 brightness(char *user_percentage)
{
    int number_percentage[2];
    int index, index_1 = 0;
    int16 final_number;
    
    for(index=9; index<11; index++)
    {
        number_percentage[index_1++] = user_percentage[index] - '0';
    }
    final_number = number_percentage[0]*10 + number_percentage[1];

    return final_number;
}



/* This function extract the 0th to the 16th character of an string */
void substring_extractor(char *source_array)
{
    int index;
    
    for(index=0; index<9; index++)
    {
        subarray[index] = source_array[index];
    }

}



/* This function is in charge of comparing the user input command with the preset
   and output the corresponding result */
void ProcessCom(char *Pro_command_string, int16 how_bright)
{
    /* strcmp() returns 0 when two strings are equal */
    
    /* Pre-define all the commands */
    char LED_1_ON[] = "LED_1_onn";
    char LED_1_OFF[] = "LED_1_off";
    char LED_2_ON[] = "LED_2_onn";
    char LED_2_OFF[] = "LED_2_off";
    char LED_3_ON[] = "LED_3_onn";
    char LED_3_OFF[] = "LED_3_off";
    char LED_4_ON[] = "LED_4_onn";
    char LED_4_OFF[] = "LED_4_off";
    char LED_1_FLASH[] = "flh_LED_1";
    char LED_2_FLASH[] = "flh_LED_2";
    char LED_3_FLASH[] = "flh_LED_3";
    char LED_4_FLASH[] = "flh_LED_4";
    char BRIGHTNESS[] = "LED_5_bt_";

    if(strcmp(Pro_command_string, LED_1_ON) == 0)
    {
        puts("OK! LED 1 is now on!\n");
        IO_Port.port_c_0_LED = 0b1;
    } 
    else if(strcmp(Pro_command_string, LED_1_OFF) == 0)
    {
        puts("OK! LED 1 is now off!\n");
        IO_Port.port_c_0_LED = 0b0;
    }
    else if(strcmp(Pro_command_string, LED_2_ON) == 0)
    {
        puts("OK! LED 2 is now on!\n");
        IO_Port.port_c_1_LED = 0b1;
    }
    else if(strcmp(Pro_command_string, LED_2_OFF) == 0)
    {
        puts("OK! LED 2 is now off!\n");
        IO_Port.port_c_1_LED = 0b0;
    }
    else if(strcmp(Pro_command_string, LED_3_ON) == 0)
    {
        puts("OK! LED 3 is now on!\n");
        IO_Port.port_c_2_LED = 0b1;
    }
    else if(strcmp(Pro_command_string, LED_3_OFF) == 0)
    {
        puts("OK! LED 3 is now off!\n");
        IO_Port.port_c_2_LED = 0b0;
    }
    else if(strcmp(Pro_command_string, LED_4_ON) == 0)
    {
        puts("OK! LED 4 is now on!\n");
        IO_Port.port_c_3_LED = 0b1;
    }
    else if(strcmp(Pro_command_string, LED_4_OFF) == 0)
    {
        puts("OK! LED 4 is now off!\n");
        IO_Port.port_c_3_LED = 0b0;
    }
    else if(strcmp(Pro_command_string, LED_1_FLASH) == 0)
    {
        puts("OK! Flashing LED 1!\n");
        for(g_index=0; g_index<100; g_index++)
        {
            IO_Port.port_c_0_LED = 0b0;
            delay_ms(500);
            IO_Port.port_c_0_LED = 0b1;
            delay_ms(500);
        }
    }
    else if(strcmp(Pro_command_string, LED_2_FLASH) == 0)
    {
        puts("OK! Flashing LED 2!\n");
        for(g_index=0; g_index<100; g_index++)
        {
            IO_Port.port_c_1_LED = 0b0;
            delay_ms(500);
            IO_Port.port_c_1_LED = 0b1;
            delay_ms(500);
        }
    }
    else if(strcmp(Pro_command_string, LED_3_FLASH) == 0)
    {
        puts("OK! Flashing LED 3!\n");
        for(g_index=0; g_index<100; g_index++)
        {
            IO_Port.port_c_2_LED = 0b0;
            delay_ms(500);
            IO_Port.port_c_2_LED = 0b1;
            delay_ms(500);
        }
    }
    else if(strcmp(Pro_command_string, LED_4_FLASH) == 0)
    {
        puts("OK! Flashing LED 4!\n");
        for(g_index=0; g_index<100; g_index++)
        {
            IO_Port.port_c_3_LED = 0b0;
            delay_ms(500);
            IO_Port.port_c_3_LED = 0b1;
            delay_ms(500);
        }
    }
    else if(strcmp(Pro_command_string, BRIGHTNESS) == 0)
    {
        puts("OK! LED 5 is now at desired brightness!\n");
        set_pwm4_duty(how_bright * 10);
    }
    else
    {
        puts("No! Invalid Input!\n");
        IO_Port.port_c_0_LED = 0b0;
        IO_Port.port_c_1_LED = 0b0;
        IO_Port.port_c_2_LED = 0b0;
        IO_Port.port_c_3_LED = 0b0;
    }
}



void main(void)
{
    int index=0, percent_Com;
    char command[SERIAL_BUFFER_SIZE];

    IO_Direction.port_a_pins_3210 = 0x0;
    IO_Direction.port_a_pin_4_PWM = 0b0;
    IO_Direction.port_a_pin_765 = 0b000;
    IO_Direction.port_b_pins = 0x00;
    // Phil was saying we need to define directions for all pins
    // I did not do that and things seems fine for some reason
    // But this time I added them just to be safe
    IO_Direction.port_c_0_LED = 0b0;
    IO_Direction.port_c_1_LED = 0b0;
    IO_Direction.port_c_2_LED = 0b0;
    IO_DIrection.port_c_3_LED = 0b0;
    IO_Direction.port_c_54_LEDs = 0b00;
    // These pins are used to drive the LEDs
    IO_Direction.port_c_6_TX = 0b0;
    // Pin RC6 is the TX output for the PIC18F27K40
    IO_Direction.port_c_7_RX = 0b1;
    // Pin RC7 is the RX input for the PIC18F27K40
    
    IO_Port.port_c_0_LED = 0b0;
    IO_Port.port_c_1_LED = 0b0;
    IO_Port.port_c_2_LED = 0b0;
    IO_Port.port_c_3_LED = 0b0;
    IO_Port.port_c_54_LEDs = 0b00;
    
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

    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
    if(kbhit())
    // When the keyboard is pressed, Serial_Stream is read by the main function
    {
        getc(Serial_Stream);
    }

    while(TRUE)
    {
        while(first_power_up == TRUE)
        {
            if(serial_buffer_next_in != 0)
            // Enter some random character to show the instructions
            {
                puts("Available Commands:\n");
                for(index=1; index<5; index++)
                {
                    printf("LED_%d_onn\n", index);
                    printf("LED_%d_off\n", index);
                    printf("flh_LED_%d\n\n", index);
                }
                printf("LED_5_bt_XX\n");
                printf("XX is a two-digit number\n\n");
                first_power_up = FALSE;
                serial_buffer_next_in = 0;
                serial_buffer_next_out = 0;
                break;
            }
        }
            
        if (serial_buffer_next_in != serial_buffer_next_out)
        // Ring buffer cannot be full, otherwise there is dataloss
        {
            if(CmdDetectedFlg == TRUE)
            // A carriage return which will terminate the command has been detected
            {
                strcpy(command, GetCom(serial_buffer));
                puts(command);
                if(strlen(command) < 10)
                {
                    ProcessCom(command, 99);
                }
                else
                {
                    substring_extractor(command);
                    percent_Com = brightness(command);
                    puts(subarray);
                    printf("%d\n", percent_Com);
                    ProcessCom(subarray, percent_Com);
                }
                serial_buffer_next_in = 0;
                serial_buffer_next_out = 0;
            }
        }
    }
}
