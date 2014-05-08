/******************************************************************************/
//* User Level #define Macros
#ifndef _user_h
#define _user_h

//__MPLAB_PICKIT3__
#define         _XTAL_FREQ	8000000
#define		OP		2.25
#define         BlinkCount      4
#define         write           1
#define         nowrite         0
#define     AnalogIN    PORTA,RA0
#define     AnalogIN2d  PORTA,RA2
#define     TestLED     PORTA,RA5
#define     TestLED2    PORTA,RA4
#define     TRIS_DATA_PORTS     TRISC
#define     DATA_PORTS          PORTC

//#define     _8BIT //Uncomment for use of 4bit with RC4-RC7
#define      _LOWER

#ifdef      _8BIT
#define     RS          PORTBbits.RB5
#define     RW          PORTBbits.RB6
#define     Enable      PORTBbits.RB7 
#else       _4BIT
#define     RS          PORTBbits.RB5
#define     RW          PORTBbits.RB6
#define     Enable      PORTBbits.RB7
#endif

/* Display ON/OFF Control defines */
#define CLEAR       0b00000001  // Clear Display
#define DON         0b00001000  /* Display on      */
#define DOFF        0b00001011  /* Display off     */
#define CURSOR_ON   0b00001111  /* Cursor on       */
#define CURSOR_OFF  0b00001101  /* Cursor off      */
#define BLINK_ON    0b00001111  /* Cursor Blink    */
#define BLINK_OFF   0b00001110  /* Cursor No Blink */
#define FUNC4        0b00101000 // Function Set with 4 bit and 5x7

/* Cursor or Display Shift defines */
#define SHIFT_CUR_LEFT    0b00000100  /* Cursor shifts to the left   */
#define SHIFT_CUR_RIGHT   0b00000101  /* Cursor shifts to the right  */
#define SHIFT_DISP_LEFT   0b00000110  /* Display shifts to the left  */
#define SHIFT_DISP_RIGHT  0b00000111  /* Display shifts to the right */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */
/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */
void            InitApp(void);         /* I/O and Peripheral Initialization */
void            LCDInit(void);
void            SendCommand(unsigned char cmd);     // For commands and shit
void            SendCharacter(unsigned char val);
void            LCDReady(void);        // Test if BF is busy
void            PulseE(void);
unsigned char   BusyXLCD(void);
void            putsXLCD(unsigned char *buffer);
void            wrstring(char *str);
float           ADC(char ADCwrite);
char            LED12(float LEDvolt);
void            dispVolt(char dispVal);
void            dispData(float tempV);
char            CG(unsigned char location, unsigned char *ptr);
void            testing(void);
void            RSSend(unsigned char rs);
void            PWMInit(unsigned char pr2, unsigned char ccphigh, char ccplow);

#endif


