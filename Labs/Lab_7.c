/*******************************
 * Name: Deion Nwaefulu
 * Student ID#: 1001226334
 * Lab Day: 12/4/2018
 * CSE 3442/5442 - Embedded Systems 1
 * Lab 7 (ABET): Building a PIC18F4520 Standalone Alarm System with EUSART Communication 
 ********************************/

 /**
 NOTE: 	
	*Your comments need to be extremely detailed and as professional as possible
	*Your code structure must be well-organized and efficient
	*Use meaningful naming conventions for variables, functions, etc.
	*Your code must be cleaned upon submission (no commented out sections of old instructions not used in the final system)
	*Your comments and structure need to be detailed enough so that someone with a basic 
            embedded programming background could take this file and know exactly what is going on
 **/
 
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_INPUT   4

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config OSC = HS    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//other includes

//Configuration bits settings code goes here

// Defining _XTAL_FREQ and selecting the XC8 compiler allows you to use the delay functions __delay_ms(x) and __delay_us(x)
#define _XTAL_FREQ 8000000 //If using the Internal Oscillator of 8 MHz
//#define _XTAL_FREQ 20000000 //If using the External Crystal of 20 MHz
/*
#define LED_GREEN   PORTAbits.RA3   //LED Green light
#define LED_YELLOW  PORTAbits.RA2   //LED Yellow light
#define LED_RED     PORTAbits.RA1   //LED Red light
*/

//Function prototypes
void putch(unsigned char data);
void Clear();
char getch(void);
char Keypad(void);
void pir_handler(void);
void Access(void);
int verify_password(void);
void menu(void);
void temperature(void);
void reset_password(char method);
void refresh_menu(char method);
void temperature_menu(char method);


//Global Variables
int temp1, temp2, temp3, temp4;
int tempEnable = 0;
double volt, celsius, fahrenheit;
char temp[MAX_INPUT];

unsigned char readEEPROM(unsigned char address)
{
  EEADR = address; //Address to be read
  EECON1bits.EEPGD = 0;//Selecting EEPROM Data Memory
  EECON1bits.RD = 1; //Initialize read cycle
  return EEDATA; //Returning data
}

void writeEEPROM(unsigned char address, unsigned char datas)
{
  unsigned char INTCON_SAVE;//To save INTCON register value
  EEADR = address; //Address to write
  EEDATA = datas; //Data to write
  EECON1bits.EEPGD = 0; //Selecting EEPROM Data Memory
  EECON1bits.WREN = 1; //Enable writing of EEPROM
  INTCON_SAVE=INTCON;//Backup INCON interrupt register
  INTCON=0; //Disables the interrupt
  EECON2=0x55; //Required sequence for write to internal EEPROM
  EECON2=0xAA; //Required sequence for write to internal EEPROM
  EECON1bits.WR = 1; //Initialize write cycle
  INTCON = INTCON_SAVE;//Enables Interrupt
  EECON1bits.WREN = 0; //To disable write
  while(PIR2bits.EEIF == 0)//Checking for complication of write operation
  {
   // __asm__("nop"); //do nothing
  }
  PIR2bits.EEIF = 0; //Clearing EEIF bit
}

/***************************************
 *ISR High - If PIR motion sensor is
 * activated then the system will enter
 * High Interrupt
 * *************************************/
//#pragma interrupt ISR_High
void interrupt ISR_High(void)
{
    if(INTCONbits.INT0IF == 1)
    {
        
          //Resets the High Interrupt Flag
        
        INTCONbits.INT0IF = 0;

       
           //Activates the Red LED light.
        
        PORTBbits.RB2 = 1;
        
        
        //  Calls pir_handler in order to prompt the user for a password
        //  in order to deactivate the alarm and return to the main menu.
         
         pir_handler();
    }
}

//#pragma interrupt ISR_Low
void low_priority interrupt ISR_Low(void)
{

}

/******************************
 *Putch - To print out results
 * ****************************/
void putch(unsigned char data)
{
    while(!PIR1bits.TXIF)
        continue;
    TXREG = data;
}

char getch(void)
{
    char myChar;

    while(PIR1bits.RCIF == 0)
        ;

    myChar = RCREG;

    return myChar;
}

/*************************************
 *Keypad - 4x4 Matrix keypad for input
 *************************************/
char Keypad(void)
{    
    /******************
     *Row 1 = 1 2 3 
     * ***************/
    PORTDbits.RD0 = 1; PORTDbits.RD1 = 0; PORTDbits.RD2 = 0; PORTDbits.RD3 = 0;
    if(PORTDbits.RD4 == 1) 
    {
        PORTDbits.RD0 = 0;  //Reset RD0
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '1';
    }
    
    if(PORTDbits.RD5 == 1)
    {
        PORTDbits.RD0 = 0;  //Reset RD0
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '2';
    }
    if(PORTDbits.RD6 == 1)
    {
        PORTDbits.RD0 = 0;  //Reset RD0
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '3';
    }
    
    /******************
     *Row 2 = 4 5 6 
     * ***************/
    PORTDbits.RD0 = 0; PORTDbits.RD1 = 1; PORTDbits.RD2 = 0; PORTDbits.RD3 = 0;
    if(PORTDbits.RD4 == 1) 
    {
        PORTDbits.RD1 = 0;  //Reset RD1
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '4';
    }
    
    if(PORTDbits.RD5 == 1)
    {
        PORTDbits.RD1 = 0;  //Reset RD1
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '5';
    }
    if(PORTDbits.RD6 == 1)
    {
        PORTDbits.RD1 = 0;  //Reset RD1
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '6';
    }
    
    /******************
     *Row 3 = 7 8 9 
     * ***************/
    PORTDbits.RD0 = 0; PORTDbits.RD1 = 0; PORTDbits.RD2 = 1; PORTDbits.RD3 = 0;
    if(PORTDbits.RD4 == 1) 
    {
        PORTDbits.RD2 = 0;  //Reset RD2
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '7';
    }
    
    if(PORTDbits.RD5 == 1)
    {
        PORTDbits.RD2 = 0;  //Reset RD2
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '8';
    }
    if(PORTDbits.RD6 == 1)
    {
        PORTDbits.RD2 = 0;  //Reset RD2
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '9';
    }

    /******************
     *Row 4 = 0 D
     * ***************/
    PORTDbits.RD0 = 0; PORTDbits.RD1 = 0; PORTDbits.RD2 = 0; PORTDbits.RD3 = 1;
    if(PORTDbits.RD5 == 1)
    {
        PORTDbits.RD3 = 0;  //Reset RD3
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return '0';
    }

    if(PORTDbits.RD7 == 1)
    {
        PORTDbits.RD3 = 0;  //Reset RD3
        PORTBbits.RB4 = 0;  //Turn off Blue led
        
        __delay_ms(200);
        
        PORTBbits.RB4 = 1;  //Turn on Blue led
        
        return 'D';     //For Enter button
    }
    
    return 'X';
}


/*********************************************
 *PIR handler - If the PIR sensor is triggered
 * then the user will be notified.
 *********************************************/
void pir_handler(void)
{
    int index, flag;
    char input = readEEPROM(0x0004);
    char passwordCmp, option;

    flag = 0;

    printf("\033[2J");
    printf("\033[2;1H");
    printf("!!!!!!Attention The Motion Sensor Has Detected Movement!!!!!!");

    do
    {
         printf("\033[4;1H");
         if(input == 0xFF)
            printf("Input Method: Keyboard");

         else
            printf("Input Method: Keypad");

        printf("\033[5;1H");
        printf("\033[K");
        printf("Enter a 4-Digit Password: ");

        /*
         * Reads Password (Keyboard)
         */
        if(input == 0xFF)
        {
            for(index = 0;index < MAX_INPUT;index++)
            {
                passwordCmp = getch();

                printf("*");

                writeEEPROM(0x0010 + index, passwordCmp);
            }//End of for loop (password input)

			//Waits for user to enter enter key
            while(getch() != 13)
                ;
        }//End of if statement (keyboard input)
		
		//Reads Password (Keypad)
        else
        {
            for(index = 0;index < MAX_INPUT;index++)
            {
                do
                {
                    passwordCmp = Keypad();
                }while(passwordCmp == 'n');

                printf("*");

                writeEEPROM(0x0010 + index, passwordCmp);
            }//End of for loop (password input)

			//Waits for user to press the enter key in the keypad
            while(Keypad() != 'D');
        }//End of else statement(Keypad input)

		//Checks if password is valid or not
        flag = verify_password();

		//Invalid password
        if(flag == 0)
        {
             printf("\033[7;1H");
             printf("Invalid Password!");

             for(index = 0;index < 5;index++)
                __delay_ms(200);
        }//End of if statement(invalid password)

		//Valid Password
        else
        {
            PORTBbits.RB2 = 0;

            writeEEPROM(0x0005, 0xFF);

            printf("\033[7;1H");
            printf("\033[K");
            printf("Valid Password!");

            for(index = 0;index < 5;index++)
                __delay_ms(20);

            printf("\033[2J");

			//Keyboard input
            if(input == 0xFF)
            {
				//Displays a menu of options to the user
                printf("\033[2;1H");
                printf("Input Method: Keyboard");
                printf("\033[4;1H");
                printf("1.Disable PIR Sensor");
                printf("\033[5;1H");
                printf("0.Return to Main Menu");
                printf("\033[7;1H");
                printf("Select an option from the menu above: ");

				//The loop will run until the user presses an option
				//The switch case will perform the task and return to the main menu.
                while(1)
                {
					//reads character from keyboard
                    option = getch();

                    putch(option);

					//Waits for enter key
                    while(getch() != 13)
                        ;

                    switch(option)
                    {
						//Deactivate the pir sensor
                        case '1':
                            writeEEPROM(0x0005, 0xFF);
                            INTCON = 0b11100000;
                            refresh_menu(input);
                            break;

						//Return to main menu
                        case '0':
                            writeEEPROM(0x0005, 'A');
                            INTCON = 0b11110000;
                            refresh_menu(input);
                            break;

						//Invalid input
                        default:
                            printf("\033[9;1H");
                            printf("Invalid Input!");
                            break;
                    }//end of switch case (Menu)
                }//Infinite while loop
            }//End of if statement(Keyboard input)

			//Keypad Input
            else
            {
				//Displays the menu to the user
                printf("\033[2;1H");
                printf("Input Method: Keyboard");
                printf("\033[4;1H");
                printf("1.Disable PIR Sensor");
                printf("\033[5;1H");
                printf("0.Return to Main Menu");
                printf("\033[7;1H");
                printf("Select an option from the menu above: ");

				//Reads input from the keypad
                while(1)
                {
                    option = Keypad();

                    if(option == '1' || option == '0')
                        break;
                }//End of infinite loop (Input)

                putch(option);

				//Waits for enter key to be pressed
                while(Keypad() != 'D')
                    ;
                
				//Perform a task based on the option selected
                switch(option)
                {
					//Deactivate pir sensor
                    case '1':
                        writeEEPROM(0x0005, 0xFF);
                        INTCON = 0b11100000;
                        refresh_menu(input);
                        break;

					//Return to main menu
                    case '0':
                        writeEEPROM(0x0005, 'A');
                        INTCON = 0b11110000;
                        refresh_menu(input);
                        break;
					
					//Return to main menu
                    default:
                        printf("\033[9;1H");
                        printf("Invalid Input!");
                        break;
                }//End of switch case(pir menu)
            }//end of else statement Keypad input

            refresh_menu(input);
        }//End of else statement (valid password)
    }while(flag == 0); //End of do while loop (Invalid input)
}

/******************************************
 *Access - The user enters their password
 * to gain access to the menu.
 * ****************************************/
void Access(void)
{
    int index, passwordFlag;
    char input, myChar, first;

    passwordFlag =  1;

    tempEnable = readEEPROM(0x0006);

    //Reads data from EEPROM to determine if this is a first time login or not.
    first = readEEPROM(0x0000);

    //Reads data from the EEPROM to determine the input method.
    input = readEEPROM(0x0004);

    //First time login
    if(first == 0xFF)
    {
        //Displays message to the user with instructions.
        //Prompts the user to enter a 4-Digit Password
        printf("\033[36m");
        printf("\nInput Method: Keyboard");
        printf("\033[2;1H");
        printf("\033[K");
        printf("Enter a 4-Digit Password: ");

        /*
         *  Reads the password entered by the user, checks if the
         *  password entered is an integer or not. If the digit is valid,
         *  then the password digit will be saved starting at address
         *  0x0000 of the EEPROM. Once the password has been entered, the
         *  user will be taken to the alarm system's menu.
         */
        for(index = 0;index < MAX_INPUT;index++)
        {
            //This part of the function will loop until the user enters
            //digits as the password
            do
            {
                //Reads a character pressed by the user in the keyboard
                myChar = getch();

                temp[index] = myChar;
                //If the character entered by the user is not an integer,
                //then the program will display a message to the user and
                //prompt the user to enter an integer.
                if(myChar < '0' || myChar > '9')
                {
                    printf("\033[4;1H");
                    printf("\033[K");
                    printf("Invalid Input! Please enter an integer!");

                    //Moves the cursor to the correct location in order to
                    //enter the password in the message that prompts for the
                    //password.
                    if(index == 0)
                        printf("\033[2;27H");
                     
                    else if(index == 1)
                        printf("\033[2;28H");

                    else if(index == 2)
                        printf("\033[2;29H");
                      
                    else
                        printf("\033[2;30H");
                }

                //If the user enter a valid digit for a password, then the
                //cursor will be moved to the next location in order to keep
                //entering the rest of the password.
                else
                {
                    printf("\033[4;1H");
                    printf("\033[K");
                    printf("Valid Input!");

                    if(index == 0)
                        printf("\033[2;27H");

                    else if(index == 1)
                        printf("\033[2;28H");

                    else if(index == 2)
                        printf("\033[2;29H");

                    else
                        printf("\033[2;30H");
                }

            }while(myChar < '0' || myChar > '9');

            //Displays the asterisk in order to let the user know that
            //the program read the digit entered via the keyboard.
            printf("*");
        }//End of for loop

        //Waits until the users presses the 'Enter' key in order to continue
        //running the program.
        while(getch() != 13)
            ;

        //Stores the password digit at the correct location in the EEPROM
        writeEEPROM(0x0000, temp[0]);
        writeEEPROM(0x0001, temp[1]);
        writeEEPROM(0x0002, temp[2]);
        writeEEPROM(0x0003, temp[3]);
        //Once the password has been entered, then the user will be taken to the
        //alarms main menu.
        menu();
    }

    /*
     * Login-in after reset.
     */
    else
    {
        //Keyboard input
        if(input == 0xFF)
        {
            //Lets the user know the input method
            printf("\033[2J");
            printf("\033[2;1H");
            printf("Input Method: Keyboard");

            //Prompts the user to enter a password
            printf("\033[3;1H");
            printf("\033[K");
            printf("Enter a 4-Digit Password: ");

            for(index = 0;index < MAX_INPUT;index++)
            {
                do
                {
                    //Reads a character pressed by the user in the keyboard
                    myChar = getch();

                    temp[index] = myChar;
                    //If the character entered by the user is not an integer,
                    //then the program will display a message to the user and
                    //prompt the user to enter an integer.
                    if(myChar < '0' || myChar > '9')
                    {
                        printf("\033[5;1H");
                        printf("\033[K");
                        printf("Invalid Input! Please enter an integer!");
                    }//End of if statement

                    //If the user enter a valid digit for a password, then the
                    //cursor will be moved to the next location in order to keep
                    //entering the rest of the password.
                    else
                    {
                        printf("\033[5;1H");
                        printf("\033[K");
                        printf("Valid Input!");
                    }//End of else statement

                    //Moves the cursor to the correct location in order to
                    //enter the password in the message that prompts for the
                    //password.
                    if(index == 0)
                        printf("\033[3;27H");

                    else if(index == 1)
                        printf("\033[3;28H");

                    else if(index == 2)
                        printf("\033[3;29H");

                    else
                        printf("\033[3;30H");
                }while(myChar < '0' || myChar > '9'); //End of Do-While loop

                //Stores the password digits entered starting at location
                //0x0010 of the EEPROM
                writeEEPROM(0x0010 + index, myChar);

                //Displays the asterisk to let the user know that the key
                //pressed has been recognized by the program.
                printf("*");
            }//End of for loop

            //Waits for the user to enter the 'Enter' key in order to continue
            while(getch() != 13);

            
        }//End of else statement (Keyboard input)

        //Keypad Input
        else
        {
            //Turns on the Blue LED Light
            PORTBbits.RB4 = 1;

            //Lets the user know the input type
            printf("\033[2J");
            printf("\033[2;1H");
            printf("Input Method: Keypad");

            //Prompts the user to input a password
            printf("\033[3;1H");
            printf("\033[K");
            printf("Enter a 4-Digit Password: ");

            //Loops in order to read the 4-digit password form the user
            //using the keypad
            index = 0;
            while(index < MAX_INPUT)
            {
                do
                {
                    //Reads input from the keypad.
                    myChar = Keypad();

                    //The input entered from the keypad is NOT valid
                    if(myChar == 'D')
                    {
                        printf("\033[5;1H");
                        printf("\033[K");
                        printf("Invalid Input! Please enter an integer!");
                    }

                    //The input entered from the keypad is valid
                    if(myChar != 'D' && myChar != 'X')
                    {
                        printf("\033[5;1H");
                        printf("\033[K");
                        printf("Valid Input!");
                    }

                    //Moves the cursor to the correct location in order to
                    //enter the password in the message that prompts for the
                    //password.
                    if(index == 0)
                        printf("\033[3;27H");

                    else if(index == 1)
                        printf("\033[3;28H");

                    else if(index == 2)
                        printf("\033[3;29H");

                    else
                        printf("\033[3;30H");
                }while(myChar == 'X' || myChar == 'D'); //End of do-while (Keypad Input)

                //Lets the user know that the key pressed has been recognized
                //by the program
                printf("*");

                //Stores the key pressed starting at address 0x0010 in the EEPROM
                writeEEPROM(0x0010 + index, myChar);

                //Increases the loop counter
                index++;
            }//End of while statement (Password input)

            //Waits until the user enters the equivalent of the 'Enter' key
            //in the keypad. 'D' = 'Enter' key.
            while(Keypad() != 'D')
                ;
        }//End of else statement (Keypad Input)
       
        //Checks if the password entered by the result is valid or not.
        passwordFlag = verify_password();

        //Invalid Password
        if(passwordFlag == 0)
        {
            printf("\033[4;1H");
            printf("\nInvalid Password!");

            //Waits a few seconds before recalling the function access.
            for(index = 0;index < 5;index++)
                __delay_ms(20);

            printf("\033[2J");

           // Access();
        } //End of if statement (Invalid password)

        //Valid Password!
        else
        {
            printf("\033[4;1H");
            printf("\nValid Password!");
            printf("\033[5;1H");
            printf("\nAccess Granted");

            //Waits a few seconds before the user is sent to the main menu
            //of the alarm system.
            printf("\033[2J");

            menu();
        }//End of else statement (Valid Password)
    }
}

/************************************************
 *Temperature - Takes the voltage output from the
 * from the voltage sensor and converts it to
 * celsius to fahrenheit.
 * **********************************************/
void temperature(void)
{
    int low, high, result;

	//Resets clock prescaler values
    TMR0H = 0x67;
    TMR0L = 0x6A;

    ADCON0bits.GO = 1;
    while(ADCON0bits.DONE == 1);

	//Sets the ADRESL and ADRESH
    low = ADRESL;
    high = ADRESH;

	//Perform the addition of the high bits and low bits
    result = (high<<8) + low;

	//Calculates the voltage of the TMP36
    volt = (((double)result) / 1023.0) * 5.0;

	//Converts the voltage to degree celsius
    celsius = (volt - 0.5) / 0.1;

	//Converts the celsius to fahrenheit
    fahrenheit = (celsius * 1.8) + 32;

	//Gets each digit of the temperature
    temp1 = ((int)fahrenheit / 10) % 10;
    temp2 = (int) fahrenheit % 10;
    temp3 = ((int) (fahrenheit * 10.0) % 10);
    temp4 = ((int) (fahrenheit * 100.0) % 10);
}

/******************************************
 *Verify password - Checks to see if the 
 * password input saved in the EEPROM match.
 * ****************************************/
int verify_password(void)
{
    int index, password, passwordCmp;
    
    for(index = 0;index < MAX_INPUT;index++)
    {
        password = readEEPROM(0x0000 + index);
        passwordCmp = readEEPROM(0x0010 + index);

        //Password does not match!
        if(password != passwordCmp)
            return 0;
    }

    //Password matches!
    return 1;
}

/******************************************
 *Menu - The user enters the menu with many
 *options for the alarm system.
 * ****************************************/
void menu(void)
{
    int index;
    char input, option, pirStatus;

    input = readEEPROM(0x0004);
    pirStatus = readEEPROM(0x0005);

    if(pirStatus != 0xFF)
        INTCONbits.INT0IE = 1;

    printf("\033[2J");

    //Displays the menu option and the header of the program
    refresh_menu(input);

    do
    {
        printf("\033[30;1H");
        //printf("\033[K");
        printf("Input: ");

        input = readEEPROM(0x0004);

        /*
         * Keyboard Input
         */
        if(input == 0xFF)
        {
            option = getch();

            putch(option);

            while(getch() != 13)
                ;
        }//End of if statement (Keyboard input)

        /*
         * Keypad Input
         */
        else
        {
            while(1)
            {
                option = Keypad();

                if(option != 'X')
                    break;
            }
            
            putch(option);

            while(Keypad() != 'D');
        }

		//Based on the option selected by the user
		//the switch statement will perform a certain task.
        switch(option)
        {
            //Reset Password
            case '1':
                reset_password(input);
                break;

            //Activate/Deactivate PIR Sensor
            case '2':
                pirStatus = readEEPROM(0x0005);

                //Activate PIR Sensor
                if(pirStatus == 0xFF)
                {
                    INTCONbits.INT0IE = 1;

                    writeEEPROM(0x0005, 'A');

                    printf("\033[12;1H");
                    printf("\033[K");
                    printf("PIR Sensor Alarm State:\t\tActive");
                }

                //Deactivate PIR Sensor
                else
                {
                    INTCONbits.INT0IE = 0;

                    writeEEPROM(0x0005, 0xFF);

                    printf("\033[12;1H");
                    printf("\033[K");
                    printf("PIR Sensor Alarm State:\t\tInactive");
                }
                break;

			//Temperature Sensor Menu
            case '3':
                printf("\033[2J");
                temperature_menu(input);
                break;
				
            //Keyboard Input
            case '4':
                printf("\033[16;1H");
                printf("Current Input Method:\t\tKeyboard (Terminal)");

                writeEEPROM(0x0004, 0xFF);

                PORTBbits.RB4 = 0;

                PORTDbits.RD0 = 1;
                PORTDbits.RD1 = 1;
                PORTDbits.RD2 = 1;
                PORTDbits.RD3 = 1;
                break;

			//Keypad Input
            case '5':
                printf("\033[16;1H");
                printf("\033[K");
                printf("Current Input Method:\t\tKeypad (Terminal)");

                writeEEPROM(0x0004, 'K');
                PORTBbits.RB4 = 1;

                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 0;
                PORTDbits.RD2 = 0;
                PORTDbits.RD3 = 0;
                break;

			//Refresh Menu
            case '0':
                refresh_menu(input);
                break;

            default:
                printf("\033[32;1H");
                printf("\033[K");
                printf("Invalid Option!");

                for(index = 0;index < 4;index++)
                    __delay_ms(200);

                printf("\033[32;1H");
                printf("\033[K");
                
                break;
        }//End of switch statement
    }while(option != '0');

    if(option == '0')
        refresh_menu(input);
}


/********************************************
 *Reset Password - The user has the option to
 * change their password.
 * ******************************************/
void reset_password(char method)
{
    char option, currentPassword, newPassword;
    int index;
    int passwordFlag = 0;

    printf("\033[2J");
    printf("\033[2;1H");

    if(method == 0xFF)
        printf("Input Method: Keyboard");

    else
        printf("Input Method: Keypad");

    printf("\033[4;1H");
    printf("1. Reset Password");
    printf("\033[5;1H");
    printf("0. Return to Main Menu");
    printf("\033[7;1H");
    printf("Please select an option from the menu above: ");

    //Keyboard Input
    if(method == 0xFF)
    {
        //Loops until the user enters a valid input
        do
        {
            //Gets input from keyboard
            option = getch();

            //Displays input to Putty
            putch(option);

            //Waits until the user presses the 'Enter' key
            while(getch() != 13)
                ;

            //Checks if the input is NOT valid
            if(option != '0' && option != '1')
            {
                printf("\033[8;1H");
                printf("\033[K");
                printf("Invalid Input!");
                printf("\033[7;46H");
            }//End of if statement (Invalid Input)

            //The input IS valid
            else
            {
                printf("\033[8;1H");
                printf("\033[K");
                printf("Valid Input!");
                printf("\033[7;46H");
                break;
            }//End of else statement (Valid Input)

        }while(option != '0' && option != '1');

		//Reset Password
		//Keyboard Method
        if(option == '1')
        {
			//Will check if the password entered is valid or not
            while(passwordFlag == 0)
            {
                printf("\033[2J");
                printf("\033[2;1H");

				//Input method
                if(method == 0xFF)
                    printf("Input Method: Keyboard");

                else
                    printf("Input Method: Keypad");

                printf("\033[3;1H");
                printf("Enter the current password: ");

				//The user enters the current password
				//Stored in address 0x0010 for comparison purposes
                for(index = 0;index < MAX_INPUT;index++)
                {
                    currentPassword = getch();

                    writeEEPROM(0x0010 + index, currentPassword);

                    printf("*");
                }

				//Waits for enter key to be pressed
                while(getch() != 13)
                    ;

				//Calls function to verify passowrd
                passwordFlag = verify_password();

				//Invalid Password
                if(passwordFlag == 0)
                {
                    printf("\033[5;1H");
                    printf("Invalid Password!");

                    for(index = 0;index < 5;index++)
                        __delay_ms(20);
                }

				//Valid password
                else
                {
                    printf("\033[5;1H");
                    printf("Valid Password!");

					//Prompts the user for a new password
                    printf("\033[2J");
                    printf("\033[2;1H");
                    printf("Input Method: Keyboard");
                    printf("\033[3;1H");
                    printf("Enter the new password: ");

                    for(index = 0;index < MAX_INPUT;index++)
                    {
                        do
                        {
                            //Reads a character pressed by the user in the keyboard
                            newPassword = getch();

                            //If the character entered by the user is not an integer,
                            //then the program will display a message to the user and
                            //prompt the user to enter an integer.
                            if(newPassword < '0' || newPassword > '9')
                            {
                                printf("\033[5;1H");
                                printf("\033[K");
                                printf("Invalid Input! Please enter an integer!");
                            }//End of if statement

                            //If the user enter a valid digit for a password, then the
                            //cursor will be moved to the next location in order to keep
                            //entering the rest of the password.
                            else
                            {
                                printf("\033[5;1H");
                                printf("\033[K");
                                printf("Valid Input!");
                            }//End of else statement

                            //Moves the cursor to the correct location in order to
                            //enter the password in the message that prompts for the
                            //password.
                            if(index == 0)
                                printf("\033[3;25H");

                            else if(index == 1)
                                printf("\033[3;26H");

                            else if(index == 2)
                                printf("\033[3;27H");

                            else
                                printf("\033[3;28H");

                        }while(newPassword < '0' || newPassword > '9'); //End of Do-While loop

                        //Displays the asterisk to let the user know that the key
                        //pressed has been recognized by the program.
                        printf("*");

                        //Stores the password digits entered starting at location
                        //0x0010 of the EEPROM
                        writeEEPROM(0x0000 + index, newPassword);
                    }//End of for loop

                    //Waits for the user to enter the 'Enter' key in order to continue
                    while(getch() != 13);

                    refresh_menu(option);
                    //menu();
                }
            }
        }
    }

    //Keypad Input
    else
    {
        //Loops until the user enters a valid input
        do
        {
            //Gets input from keyboard
            do
            {
                option = Keypad();
            }while(option == 'X');
            //Displays input on Putty
            putch(option);

            //Waits until the user presses the 'Enter' key
            while(Keypad() != 'D')
                ;

            //Checks if the input is NOT valid
            if(option != '0' && option != '1')
            {
                printf("\033[8;1H");
                printf("\033[K");
                printf("Invalid Input!");
                printf("\033[7;46H");
            }//End of if statement (Invalid Input)

            //The input IS valid
            else
            {
                printf("\033[8;1H");
                printf("\033[K");
                printf("Valid Input!");
                printf("\033[7;46H");

            }//End of else statement (Valid Input)

        }while(option != '0' && option != '1');

		//Reset Password
		//Keypad Method
        if(option == '1')
        {
            printf("\033[2J");
            printf("\033[2;1H");
            printf("Input Method: Keypad");
            printf("\033[3;1H");
            printf("Enter the current password: ");

            //Loops in order to read the 4-digit password form the user
            //using the keypad
            index = 0;
            while(index < MAX_INPUT)
            {
                do
                {
                    //Reads input from the keypad.
                    currentPassword = Keypad();

                    //The input entered from the keypad is NOT valid
                    if(currentPassword == 'D')
                    {
                        printf("\033[5;1H");
                        printf("\033[K");
                        printf("Invalid Input! Please enter an integer!");
                    }

                    //The input entered from the keypad is valid
                    if(currentPassword != 'D' && currentPassword != 'X')
                    {
                        printf("\033[5;1H");
                        printf("\033[K");
                        printf("Valid Input!");
                    }

                    //Moves the cursor to the correct location in order to
                    //enter the password in the message that prompts for the
                    //password.
                    if(index == 0)
                        printf("\033[3;29H");

                    else if(index == 1)
                        printf("\033[3;30H");

                    else if(index == 2)
                        printf("\033[3;31H");

                    else
                        printf("\033[3;32H");
                }while(currentPassword == 'X' || currentPassword == 'D'); //End of do-while (Keypad Input)

                //Lets the user know that the key pressed has been recognized
                //by the program
                printf("*");

                //Stores the key pressed starting at address 0x0010 in the EEPROM
                writeEEPROM(0x0010 + index, currentPassword);

                //Increases the loop counter
                index++;
            }//End of while statement (Password input)

            //Waits until the user enters the equivalent of the 'Enter' key
            //in the keypad. 'D' = 'Enter' key.
            while(Keypad() != 'D')
                ;
             //Checks if the password entered by the result is valid or not.
            passwordFlag = verify_password();

            //Invalid Password
            if(passwordFlag == 0)
            {
                printf("\033[4;1H");
                printf("\nInvalid Password!");

                //Waits a few seconds before recalling the function access.

                printf("\033[2J");

            //    reset_password(method);
            } //End of if statement (Invalid password)

            //Valid Password!
            else
            {
                printf("\033[4;1H");
                printf("\nValid Password!");
                printf("\033[5;1H");
                printf("\nAccess Granted");

                //Waits a few seconds before the user is sent to the main menu
                //of the alarm system.

                printf("\033[2J");
                //Lets the user know the input type
                printf("\033[2J");
                printf("\033[2;1H");
                printf("Input Method: Keypad");

                //Prompts the user to input a password
                printf("\033[3;1H");
                printf("\033[K");
                printf("Enter the new password: ");

                //Loops in order to read the 4-digit password form the user
                //using the keypad
                index = 0;
                while(index < MAX_INPUT)
                {
                    do
                    {
                        //Reads input from the keypad.
                        newPassword = Keypad();

                        //The input entered from the keypad is NOT valid
                        if(newPassword == 'D')
                        {
                            printf("\033[5;1H");
                            printf("\033[K");
                            printf("Invalid Input! Please enter an integer!");
                        }

                        //The input entered from the keypad is valid
                        if(newPassword != 'D' && newPassword != 'X')
                        {
                            printf("\033[5;1H");
                            printf("\033[K");
                            printf("Valid Input!");
                        }

                        //Moves the cursor to the correct location in order to
                        //enter the password in the message that prompts for the
                        //password.
                        if(index == 0)
                            printf("\033[3;25H");

                        else if(index == 1)
                            printf("\033[3;26H");

                        else if(index == 2)
                            printf("\033[3;27H");

                        else
                            printf("\033[3;28H");
                    }while(newPassword == 'X' || newPassword == 'D'); //End of do-while (Keypad Input)

                    //Lets the user know that the key pressed has been recognized
                    //by the program
                    printf("*");

                    //Stores the key pressed starting at address 0x0010 in the EEPROM
                    writeEEPROM(0x0000 + index, newPassword);

                    //Increases the loop counter
                    index++;
                }//End of while statement (Password input)

                //Waits until the user enters the equivalent of the 'Enter' key
                //in the keypad. 'D' = 'Enter' key.
                while(Keypad() != 'D');

             //   menu();
            }//End of else statement (Keypad Input)

        }//End of if statement (Option == 1)
    }//End of else statement (Keypad Input)

    refresh_menu(option);
    //menu();
}

/************************************
 *Refresh menu - Refreshes and re-opens
 *the user menu.
 * **********************************/
void refresh_menu(char method)
{
    char pirStatus = readEEPROM(0x0005);
    char tempStatus = readEEPROM(0x0006);

    int index;
    /*************************Displays Program Header**************************/
    printf("\033[2J");
    printf("\033[2;1H");

    for(index = 0;index < 114;index++)
        printf("*");

    printf("\033[3;1H");
    printf("*\t\t\t\t\t\tAlarm System is Connected\t\t\t\t\t *");

    printf("\033[4;1H");
    printf("*\t\t\t\t           CSE 3442/5442 - Embedded System 1\t\t\t\t\t *");

    printf("\033[5;1H");
    printf("*\t\t\t\t    Lab 7 (ABET) - Standalone PIC Alarm System with EUSART Communication\t\t\t\t *");

    printf("\033[6;1H");
    printf("*\t\t\t\t\t\t(Deion Nwaefulu)\t\t\t\t\t *\n");

    printf("\033[7;1H");
    for(index = 0;index < 114;index++)
        printf("*");
    /*************************End of Program Header****************************/

    /***************************Component Statuses*****************************/
    printf("\033[9;1H");
    printf("Component Statuses\n");

    printf("\033[10;1H");
    for(index = 0;index < 114;index++)
        printf("-");

    //Displays the status of the PIR Sensor
    printf("\033[12;1H");
    if(pirStatus == 0xFF)
        printf("PIR Sensor Alarm State:\t\tInactive");

    else
        printf("PIR Sensor Alarm State:\t\tActive");

    //If Statement
    printf("\033[13;1H");

    if(tempStatus == 0xFF)
        printf("Temperature Alarm State:\tInactive");

    else
        printf("Temperature Alarm State:\tActive");
    printf("\033[14;1H");
    printf("Current Temperature Reading:\t%d%d.%d%d", temp1, temp2, temp3, temp4);

    printf("\033[15;1H");
    printf("Temperature Alarm Threshold:\t");
    //
    printf("\033[16;1H");
    if(method == 0xFF)
        printf("Current Input Method:\t\tKeyboard (Terminal)");

    else
        printf("Current Input Method:\t\tKeypad");

    printf("\033[18;1H");
    for(index = 0;index < 114;index++)
        printf("-");
    /************************End of Component Statuses*************************/

    /******************************Main Menu***********************************/
    printf("\033[20;1H");
    printf("Select One of the Following:");

    printf("\033[22;1H");
    printf("\t1: Passcode Option");

    printf("\033[23;1H");
    printf("\t2: PIR Sensor Alarm Options");

    printf("\033[24;1H");
    printf("\t3: Temperature Sensor Alarm Options");

    printf("\033[25;1H");
    printf("\t4: Use Keyboard (Terminal} As The Only Input");

    printf("\033[26;1H");
    printf("\t5: Use Keypad As The Only Input ('D' = Enter Key)");

    printf("\033[27;1H");
    printf("\t0: Refresh Main Menu\n\n");
	/**************************End of Main Menu*****************************/
}
/******************************************
 *Temperature menu - The user has the option
 * to activate or deactivate the temperature
 * sensor.
 * ****************************************/
void temperature_menu(char method)
{
    char option;

    printf("\033[2J");
    printf("\033[2;1H");

	//Displays the input method
    if(method == 0xFF)
        printf("Input Method: Keyboard");

    else
        printf("Input Method: Keypad");

	//Displays the menu
    printf("\033[4;1H");
    printf("1.Enable Temperature Sensor");
    printf("\033[5;1H");
    printf("2.Disable Temperature Sensor");
    printf("\033[6;1H");
    printf("3.Set Temperature Threshold");
    printf("\033[8;1H");
    printf("0.Return to Main Menu");
    printf("\033[10;1H");
    printf("Input: ");

	//Keyboard Input
    if(method == 0xFF)
    {
        do
        {
            option = getch();

            putch(option);

            while(getch() != 13)
                ;

            switch(option)
            {
				//Activates the sensor & returns to the main menu
                case '1':
                    writeEEPROM(0x0006, 'T');
                    tempEnable = 1;
                    refresh_menu(option);
                    //menu();
                    break;

				//Deactivates the sensor & returns to the main menu
                case '2':
                    writeEEPROM(0x0006, 0xFF);
                    tempEnable = 0;
                    refresh_menu(option);
                    //menu();
                    break;
            }//End of switch case
        }while(option != '0');	//End of do while
    }//End of if statement (Keyboard input)

}

void main()
{
    //LED's Settings
    TRISBbits.RB1 = 0;      //TRISC register for A1
    TRISBbits.RB2 = 0;      //TRISC register for A2
    TRISBbits.RB3 = 0;      //TRISC register for A3
    TRISBbits.RB4 = 0;      //TRISC register for A4
    
    PORTBbits.RB1 = 0;  //LED Red light
    PORTBbits.RB2 = 0;  //LED Yellow light
    PORTBbits.RB3 = 1;  //LED Green light  

    
    //Keypad TRIS Output Rows
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD3 = 0;
    
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 1;
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    
    //Keypad TRIS Input Columns
    TRISDbits.RD4 = 1;
    TRISDbits.RD5 = 1;
    TRISDbits.RD6 = 1;
    TRISDbits.RD7 = 1;
    
    PORTDbits.RD4 = 0;
    PORTDbits.RD5 = 0;
    PORTDbits.RD6 = 0;
    PORTDbits.RD7 = 0;

    TRISAbits.RA0 = 1;      //Temperature voltage input
    TRISBbits.RB0 = 0;      //PIR Sensor Output External Output
    
    TXSTA = 0b00100100;     //Communication Input and Output
    RCSTA = 0b10010000;     //RCSTA Register Settings
    
    SPBRG = 129;
    
    //PIR Sensor Settings
    RCONbits.IPEN = 1;
    INTCON = 0b11100000;
    INTCON2bits.INTEDG0 = 1;
    
    //Timer and Temperature Sensor
    ADCON0 = 0b00000001;
    ADCON2 = 0b10101100;
    ADCON1 = 0b10001110;
    
	while(1)
	{
		//Main routine code
        Access();
	} //end of while(1)
    
	
} //end of void main()

//Be sure to have a blank line at the end of your program

