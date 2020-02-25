/*******************************
 * Name: Deion Nwaefulu
 * Student ID#: 1001226334
 * Lab Day: 10/10/2018
 * CSE 3442/5442 - Embedded Systems 1
 * Lab x: Lab 3 - Simple I/O Circuit(GPIO)
 ********************************/

#include <xc.h> // For the XC8 Compiler (automatically finds/includes the specific PIC18F452.h header file)

#define _XTAL_FREQ  10000000     // Running at 10MHz (external oscillator/crystal), REQUIRED for delay functions

#define LED_LEFT    PORTAbits.RA3  // QwikFlash red LED (left) to toggle
#define LED_CENTER  PORTAbits.RA2  // QwikFlash red LED (center) to toggle
#define LED_RIGHT   PORTAbits.RA1  // QwikFlash red LED (right) to toggle

// PIC18F452 Configuration Bit Settings
#pragma config OSC     = HS     // Oscillator Selection bits (HS oscillator)
#pragma config OSCS    = OFF    // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))
#pragma config PWRT    = OFF    // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR     = OFF    // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT     = OFF    // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config CCP2MUX = ON     // CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)
#pragma config STVR    = ON     // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause RESET)
#pragma config LVP     = OFF    // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

/*******************************
 * Global Vars
 * 
 * Strings for LCD Initialization and general use
 * For stability reasons, each array must have 10 total elements (no less, no more)
 ********************************/
const char LCDstr[]  = {0x33,0x32,0x28,0x01,0x0c,0x06,0x00,0x00}; // LCD Initialization string (do not change)

//Never change element [0] or [9] of the following char arrays
//You may only change the middle 8 elements for displaying on the LCD
char Str_1[] = {0x80,' ',' ',' ',' ',' ',' ',' ',' ',0};    // First line of LCD
char Str_2[] = {0xC0,' ',' ',' ',' ',' ',' ',' ',' ',0};    // Second line of LCD

const char Clear1[] = {0x80,' ',' ',' ',' ',' ',' ',' ',' ',0};  // Clear first line of LCD
const char Clear2[] = {0xC0,' ',' ',' ',' ',' ',' ',' ',' ',0};  // Clear second line of LCD


/*******************************
 * Function prototypes
 ********************************/
void Initialize_PIC(void);
void Initialize_LCD(void);
void Print_To_LCD(const char *);
void Toggle_LEDs(void);

void main(void)
{
    Initialize_PIC();       //Initialize all settings required for general QwikFlash and LCD operation
	Print_To_LCD(Clear1);   //Clear the LCD line 1 to start your program
	Print_To_LCD(Clear2);   //Clear the LCD line 2 to start your program
    void Addition();        //B + C
    void Multiplication();  //B * C
    void Subtraction();     //B - C
    void AND();             //B AND C
    void NOT();             //B NOT
    
    TRISCbits.RC0 = 1;      //Input C0
    TRISCbits.RC1 = 1;      //Input C1
    TRISCbits.RC2 = 1;      //Input C2
    TRISCbits.RC3 = 1;      //Input C3

    TRISBbits.RB0 = 1;      //Input B0
    TRISBbits.RB1 = 1;      //Input B1
    TRISBbits.RB2 = 1;      //Input B2
    TRISBbits.RB3 = 1;      //Input B3

    TRISBbits.RB4 = 1;      //Input B4
    TRISBbits.RB5 = 1;      //Input B5

	// Your personal PORT/TRIS/ADCON/etc settings or configurations can go here 
	// Or make your own function and call it
            
    // You can use the two delay functions defined in pic18.h (automatically included from xc.h):
    //      __delay_us(x);  // to delay in microsecond increments
    //                      // x is an unsigned long (0 - 4294967295)
    //
    //      __delay_ms(x);  // to delay in millisecond increments
    //                      // x is an unsigned long (0 - 4294967295)
    
    //Main routine, forever in an infinite while(true) loop
    while(1)
    {
        if(PORTBbits.RB4 == 0 && PORTBbits.RB5 == 0)
        {
            Addition();             //Addition if B4 = 0 and B5 = 0
            Multiplication();       //Multiplication if B4 = 0 and B5 = 0
        }
        
        if(PORTBbits.RB4 == 0 && PORTBbits.RB5 == 1)
        {
            Subtraction();          //Subtraction if B4 = 0 and B5 = 1
        }
        
        if(PORTBbits.RB4 == 1 && PORTBbits.RB5 == 0)
        {
            AND();                  //AND if B4 = 1 and B5 = 0
        }
        
        if(PORTBbits.RB4 == 1 && PORTBbits.RB5 == 1)
        {
            NOT();                  //NOT if B4 = 1 and B5 = 1
        }
        
    }
}
/********************************
*Addition Function
*********************************/
void Addition()
{
    Str_1[1] = ' ';
    Str_1[2] = ' ';
    Str_1[3] = ' ';
    Str_1[4] = 'B';
    Str_1[5] = '+';         //B + C
    Str_1[6] = 'C';
    Str_1[7] = ' ';
    Str_1[8] = ' ';

    Print_To_LCD(Str_1);
        
        int rb0;        //Input for B0
        int rb1;        //Input for B1
        int rb2;        //Input for B2
        int rb3;        //Input for B3
        int equalB;     //Sum for all of PortB
        
        int rc0;        //Input for C0
        int rc1;        //Input for C1
        int rc2;        //Input for C2
        int rc3;        //Input for C3
        int equalC;     //Sum for all of PortC
        
        int Equal;      //Addition of equalB and equalC
        int E;          //Divides the equal by 10
        int E2;         //Finds the remainder of the equal by 10

        rb0 = PORTBbits.RB0;    //Binary to decimal of B0: 1
        rb0 = rb0 * 1;
        
        rb1 = PORTBbits.RB1;    //Binary to decimal of B1: 2
        rb1 = rb1 * 2;
        
        rb2 = PORTBbits.RB2;    //Binary to decimal of B2: 4
        rb2 = rb2 * 4;
        
        rb3 = PORTBbits.RB3;    //Binary to decimal of B3: 8
        rb3 = rb3 * 8;
        
        equalB = rb0 + rb1 + rb2 + rb3; //Sum of PortB numbers
        
        rc0 = PORTCbits.RC0;    //Binary to decimal of C0: 1
        rc0 = rc0 * 1;
        
        rc1 = PORTCbits.RC1;    //Binary to decimal of C1: 2
        rc1 = rc1 * 2;
        
        rc2 = PORTCbits.RC2;    //Binary to decimal of C2: 4
        rc2 = rc2 * 4;
        
        rc3 = PORTCbits.RC3;    //Binary to decimal of C3: 8
        rc3 = rc3 * 8;
        
        equalC = rc0 + rc1 + rc2 + rc3; //Sum of PortC numbers
       
        Equal = equalB * equalC;        //Result of equalB * equalC
        E = Equal / 10;                 //Divides the result by 10
        E2 = Equal % 10;                //Finds the remainder
        
    Str_2[1] = ' ';
    Str_2[2] = ' ';
    Str_2[3] = ' '; 
    Str_2[4] = '=';
    Str_2[5] = E +48;                   //=## Gives us the result of the addition
    Str_2[6] = E2 + 48;
    Str_2[7] = ' ';
    Str_2[8] = ' '; 
    Print_To_LCD(Str_2);
        
}
/********************************
*Multiplication Function
*********************************/
void Multiplication()
{
    Str_1[1] = ' ';
    Str_1[2] = ' ';
    Str_1[3] = ' ';
    Str_1[4] = 'B';
    Str_1[5] = '*';             //B * C
    Str_1[6] = 'C';
    Str_1[7] = ' ';
    Str_1[8] = ' ';

    Print_To_LCD(Str_1);
        
        int rb0;        //Input for B0
        int rb1;        //Input for B1
        int rb2;        //Input for B2
        int rb3;        //Input for B3
        int equalB;     //Sum for all of PortB
        
        int rc0;        //Input for C0
        int rc1;        //Input for C1
        int rc2;        //Input for C2
        int rc3;        //Input for C3
        int equalC;     //Sum for all of PortC
        
        int Equal;      //Multiplication of equalB and equalC

        rb0 = PORTBbits.RB0;    //Binary to decimal of B0: 1
        rb0 = rb0 * 1;
        
        rb1 = PORTBbits.RB1;    //Binary to decimal of B1: 2
        rb1 = rb1 * 2;
        
        rb2 = PORTBbits.RB2;    //Binary to decimal of B2: 4
        rb2 = rb2 * 4;
        
        rb3 = PORTBbits.RB3;    //Binary to decimal of B3: 8
        rb3 = rb3 * 8;
        
        equalB = rb0 + rb1 + rb2 + rb3; //Sum of PortB numbers
        
        rc0 = PORTCbits.RC0;    //Binary to decimal of C0: 1
        rc0 = rc0 * 1;
        
        rc1 = PORTCbits.RC1;    //Binary to decimal of C1: 2
        rc1 = rc1 * 2;
        
        rc2 = PORTCbits.RC2;    //Binary to decimal of C2: 4
        rc2 = rc2 * 4;
        
        rc3 = PORTCbits.RC3;    //Binary to decimal of C3: 8
        rc3 = rc3 * 8;
        
        equalC = rc0 + rc1 + rc2 + rc3; //Sum of PortC numbers
       
        Equal = equalB * equalC;        //Result of equalB * equalC
        if(Equal >= 0 && Equal < 10)
        {
            Str_2[1] = ' ';
            Str_2[2] = ' ';
            Str_2[3] = ' '; 
            Str_2[4] = '=';         //=00# Result by 1's decimal
            Str_2[5] = '0';
            Str_2[6] = '0';
            Str_2[7] = Equal + 48;
            Str_2[8] = ' '; 
            Print_To_LCD(Str_2);
        }
        
        else if(Equal >= 10 && Equal < 100)
        {
            Str_2[1] = ' ';
            Str_2[2] = ' ';
            Str_2[3] = ' '; 
            Str_2[4] = '=';         //=0## Result by 10's decimal
            Str_2[5] = '0';
            Str_2[6] = (Equal /10) + 48;
            Str_2[7] = (Equal % 10) + 48;
            Str_2[8] = ' '; 
            Print_To_LCD(Str_2);            
        }
        
        else if(Equal >= 100 && Equal < 1000)
        {
            Str_2[1] = ' ';
            Str_2[2] = ' ';
            Str_2[3] = ' '; 
            Str_2[4] = '=';        //=### Result by 100's decimal
            Str_2[5] = (Equal / 100) + 48;
            Equal = Equal % 100;
            Str_2[6] = (Equal / 10) + 48;
            Equal = Equal % 10;
            Str_2[7] = Equal + 48;
            Str_2[8] = ' '; 
            Print_To_LCD(Str_2);
        }       
}

/********************************
*Subtraction Function
*********************************/
void Subtraction()
{
    Str_1[1] = ' ';
    Str_1[2] = ' ';
    Str_1[3] = ' ';
    Str_1[4] = 'B';
    Str_1[5] = '-';         //B - C
    Str_1[6] = 'C';
    Str_1[7] = ' ';
    Str_1[8] = ' ';
    Print_To_LCD(Str_1);
        
        int rb0;        //Input for B0
        int rb1;        //Input for B1
        int rb2;        //Input for B2
        int rb3;        //Input for B3
        int equalB;     //Sum for all of PortB
        
        int rc0;        //Input for C0
        int rc1;        //Input for C1
        int rc2;        //Input for C2
        int rc3;        //Input for C3
        int equalC;     //Sum for all of PortC
        
        int Equal;      //Addition of equalB and equalC
        int E;          //Divides the equal by 10
        int E2;         //Finds the remainder of the equal by 10

        rb0 = PORTBbits.RB0;    //Binary to decimal of B0: 1
        rb0 = rb0 * 1;
        
        rb1 = PORTBbits.RB1;    //Binary to decimal of B1: 2
        rb1 = rb1 * 2;
        
        rb2 = PORTBbits.RB2;    //Binary to decimal of B2: 4
        rb2 = rb2 * 4;
        
        rb3 = PORTBbits.RB3;    //Binary to decimal of B3: 8
        rb3 = rb3 * 8;
        
        equalB = rb0 + rb1 + rb2 + rb3; //Sum of PortB numbers
        
        rc0 = PORTCbits.RC0;    //Binary to decimal of C0: 1
        rc0 = rc0 * 1;
        
        rc1 = PORTCbits.RC1;    //Binary to decimal of C1: 2
        rc1 = rc1 * 2;
        
        rc2 = PORTCbits.RC2;    //Binary to decimal of C2: 4
        rc2 = rc2 * 4;
        
        rc3 = PORTCbits.RC3;    //Binary to decimal of C3: 8
        rc3 = rc3 * 8;
        
        equalC = rc0 + rc1 + rc2 + rc3; //Sum of PortC numbers
        
        
        
        
    Str_2[4] = '=';
    
    if(equalB < equalC)     //If the result is negative, display '-' sign
    {
        Equal = (-1) * (equalB - equalC);
        E = Equal / 10;
        E2 = Equal % 10;
        Str_2[1] = ' '; 
        Str_2[2] = ' '; 
        Str_2[3] = ' '; 
        Str_2[4] = '-';         // =-##
        Str_2[5] = E + 48;
        Str_2[6] = E2 + 48;
        Str_2[7] = ' ';
        Str_2[8] = ' '; 
        Print_To_LCD(Str_2);
    }
    
    else                    //If the result is positive, display '+' sign
    {
        Equal = equalB - equalC;
        E = Equal / 10;
        E2 = Equal % 10;
        Str_2[1] = ' '; 
        Str_2[2] = ' '; 
        Str_2[3] = ' '; 
        Str_2[4] = '+';         // =+##
        Str_2[5] = E + 48;
        Str_2[6] = E2 + 48;
        Str_2[7] = ' ';
        Str_2[8] = ' '; 
        Print_To_LCD(Str_2);
    }
    
        
}

/********************************
*AND Function
*********************************/
void AND()
{
    Str_1[2] = 'B';
    Str_1[3] = ' ';
    Str_1[4] = 'A';
    Str_1[5] = 'N';     //B AND C
    Str_1[6] = 'D';
    Str_1[7] = ' ';
    Str_1[8] = 'C';
    Print_To_LCD(Str_1);

    TRISDbits.RD0 = 0;      //Output D0
    TRISDbits.RD1 = 0;      //Output D1
    TRISDbits.RD2 = 0;      //Output D2
    TRISDbits.RD3 = 0;      //Output D3
    
    PORTD = PORTC & PORTB;      //PortD contains input from PortC and PortB
    Str_2[1] = PORTDbits.RD7 + 48;      //Binary of D7
    Str_2[2] = PORTDbits.RD6 + 48;      //Binary of D6
    Str_2[3] = PORTDbits.RD5 + 48;      //Binary of D5
    Str_2[4] = PORTDbits.RD4 + 48;      //Binary of D4
    Str_2[5] = PORTDbits.RD3 + 48;      //Binary of D3
    Str_2[6] = PORTDbits.RD2 + 48;      //Binary of D2
    Str_2[7] = PORTDbits.RD1 + 48;      //Binary of D1
    Str_2[8] = PORTDbits.RD0 + 48;      //Binary of D0
    Print_To_LCD(Str_2);
    
}

/********************************
*NOT Function
*********************************/
void NOT()
{
    Str_1[2] = 'N';
    Str_1[3] = 'O';
    Str_1[4] = 'T';
    Str_1[5] = ' ';     //NOT B
    Str_1[6] = 'B';
    Str_1[7] = ' ';
    Str_1[8] = ' ';
    Print_To_LCD(Str_1);
    
    int rb0;        //Input for B0
    int rb1;        //Input for B1
    int rb2;        //Input for B2
    int rb3;        //Input for B3
    int rc0;        //Input for C0
    int rc1;        //Input for C1
    int rc2;        //Input for C2
    int rc3;        //Input for C3
    
    rb0 = !PORTBbits.RB0 + 48;      //Not RB0
    rb1 = !PORTBbits.RB1 + 48;      //Not RB1
    rb2 = !PORTBbits.RB2 + 48;      //Not RB2
    rb3 = !PORTBbits.RB3 + 48;      //Not RB3
    rc0 = !PORTCbits.RC0 + 48;      //Not RC0
    rc1 = !PORTCbits.RC1 + 48;      //Not RC1
    rc2 = !PORTCbits.RC2 + 48;      //Not RC2
    rc3 = !PORTCbits.RC3 + 48;      //Not RC3
    
    Str_2[1] = rb0;     //Binary of B0
    Str_2[2] = rb1;     //Binary of B1
    Str_2[3] = rb2;     //Binary of B2
    Str_2[4] = rb3;     //Binary of B3
    Str_2[5] = rc0;     //Binary of C0
    Str_2[6] = rc1;     //Binary of C1
    Str_2[7] = rc2;     //Binary of C2
    Str_2[8] = rc3;     //Binary of C3
    Print_To_LCD(Str_2);
    
}
/*******************************
 * Initialize_PIC(void)
 *
 * This function performs all initializations of variables and registers
 * for the PIC18F452 when specifically on the QwikFlash board.
 *
 * DO NOT CHANGE ANYTHING IN THIS FUNCTION
 ********************************/
void Initialize_PIC(void)
{
    // Reference the QwikFlash schematic (pdf) to understand analog/digital IO decisions
    
    ADCON1 = 0b10001110;    // Enable PORTA & PORTE digital I/O pins
    TRISA  = 0b11100001;    // Set I/O for PORTA
    TRISB  = 0b11011100;    // Set I/O for PORTB
    TRISC  = 0b11010000;    // Set I/0 for PORTC
    TRISD  = 0b00001111;    // Set I/O for PORTD
    TRISE  = 0b00000000;    // Set I/O for PORTE
    PORTA  = 0b00010000;    // Turn off all four LEDs driven from PORTA    
    LED_LEFT    = 0;        // All LEDs initially OFF
    LED_CENTER  = 0;        // All LEDs initially OFF
    LED_RIGHT   = 0;        // All LEDs initially OFF
    Initialize_LCD();       // Initialize LCD
}

/*******************************
 * Initialize_LCD(void)
 *
 * Initialize the Optrex 8x2 character LCD.
 * First wait for 0.1 second, to get past the displays power-on reset time.
 *
 * DO NOT CHANGE ANYTHING IN THIS FUNCTION
 *******************************/
void Initialize_LCD(void)
{
    char currentChar;
    char *tempPtr;

    __delay_ms(100);                // wait 0.1 sec (100 ms)

    PORTEbits.RE0 = 0;              // RS=0 for command
    tempPtr = LCDstr;

    while (*tempPtr)                // if the byte is not zero (end of string)
    {
        currentChar = *tempPtr;
        PORTEbits.RE1 = 1;          // Drive E pin high
        PORTD = currentChar;        // Send upper nibble
        PORTEbits.RE1 = 0;          // Drive E pin low so LCD will accept nibble          
        __delay_ms(10);             // wait 10 ms
        currentChar <<= 4;          // Shift lower nibble to upper nibble
        PORTEbits.RE1 = 1;          // Drive E pin high again
        PORTD = currentChar;        // Write lower nibble
        PORTEbits.RE1 = 0;          // Drive E pin low so LCD will process byte        
        __delay_ms(10);             // wait 10 ms	
        tempPtr++;                  // Increment pointer to next character
    }
}

/*******************************
 * Print_To_LCD(const char * tempPtr) 
 *
 * This function is called with the passing in of an array of a constant
 * display string.  It sends the bytes of the string to the LCD.  The first
 * byte sets the cursor position.  The remaining bytes are displayed, beginning
 * at that position.
 * This function expects a normal one-byte cursor-positioning code, 0xhh, or
 * an occasionally used two-byte cursor-positioning code of the form 0x00hh.
 *
 * DO NOT CHANGE ANYTHING IN THIS FUNCTION
 ********************************/
void Print_To_LCD(const char * tempPtr)
{
	char currentChar;
    PORTEbits.RE0 = 0;          // Drive RS pin low for cursor-positioning code

    while (*tempPtr)            // if the byte is not zero (end of string)
    {
        currentChar = *tempPtr;
        PORTEbits.RE1 = 1;      // Drive E pin high
        PORTD = currentChar;    // Send upper nibble
        PORTEbits.RE1 = 0;      // Drive E pin low so LCD will accept nibble
        currentChar <<= 4;      // Shift lower nibble to upper nibble
        PORTEbits.RE1 = 1;      // Drive E pin high again
        PORTD = currentChar;    // Write lower nibble
        PORTEbits.RE1 = 0;      // Drive E pin low so LCD will process byte
        __delay_ms(10);         // wait 10 ms	
        PORTEbits.RE0 = 1;      // Drive RS pin high for displayable characters
        tempPtr++;              // Increment pointer to next character
    }
}

/*******************************
 * Toggle_LEDs(void)
 *
 * This function simply toggles the QwikFlash's red LEDs in a simple sequence
 * The LED_X defines are at the top of this .c file
 * 
 * You may alter this function if you like or directly manipulate LEDs in other functions
 ********************************/
void Toggle_LEDs(void)
{
    LED_LEFT ^= 1;      // regardless of the bit's previous state, this flips it to 1 or 0
    __delay_ms(100);
    
    LED_CENTER ^= 1;
    __delay_ms(100);
    
    LED_RIGHT ^= 1;
    __delay_ms(100);
}

// Always have at least 1 blank line at the end of the .c file




