/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#ifndef		stdio.h
#include	<stdio.h>
#endif
#ifndef		stdlib.h
#include	<stdlib.h>
#endif
#include	"user.h"
#define		_XTAL_FREQ	8000000

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
unsigned char status [8];
unsigned char Me [8] = {0,1,0,1,0,1,0,1};
extern float	 twobyte;
extern float	volt;
extern unsigned char	BattOnePercent;
extern unsigned char	BattTwoPercent;
extern char		*buf1;
extern char		*buf2;
extern unsigned char BattOne;
extern unsigned char BattTwo;


void	InitApp(void){
    /* TODO Initialize User Ports/Peripherals/Project here */
    OSCCON  = 0b01110000;
    TRISA   = 0b00000111;
    TRISB   = 0b00010000;
	TRISBbits.TRISB4 = 1;
    TRISC   = 0x00;

	WPUB    |= 0b0001;
	PORTAbits.RA0 = 0;
	PORTBbits.RB4 = 0;
	PORTBbits.RB5 = 0;
	//OPTION_REG  = 0b00000000;
    // ADC CONFIGs
    ADCON0  = 0b11000000; //Right justified
    ADCON1  = 0xff;		// The ACD Clock Source
	ANSEL  = 0b00000001;
	ANSELH = 0b0100;
    /* Initialize peripherals */
    /* Enable interrupts */
	INTCON = 0b10010000; // Enabled=GIE,INTE
}
void    LCDInit(void){
	__delay_ms(60);
	Enable = 0;
	RS = 0;
	RW = 0;
	__delay_us(1);

#ifdef	_8BIT
	SendCommand(0b00111000);
	__delay_us(1);
	SendCommand(0b00111000);
	__delay_us(1);
	SendCommand(0b00111000);
	__delay_us(1);
#else
	SendCommand(0x2);	// 0b0010
	SendCommand(0x2);	// 0b0010
	__delay_ms(1);
	__delay_ms(1);
	SendCommand(0x2);	// 0b0010
	__delay_ms(1);
	SendCommand(0x28);	// 0b0010 1000
	__delay_ms(1);
	SendCommand(0x28);	// 0b0010 1000
	__delay_ms(1);
#endif
	SendCommand(0b00001011);	//Display Off, Hide Cursor, Underline Cursor
	while( BusyXLCD() );
	SendCommand(0b00001111);	//Display On, Show Cursor, Underline Cursor, Blink Cursor
	while( BusyXLCD() );
	SendCommand(0b00000110);	//Char Entry is Increment, Display shift off
	while( BusyXLCD() );
	SendCommand(0b00010100);	//Move cursor, Shift (cursor right)
	while( BusyXLCD() );
	SendCommand(CLEAR);			//Clear Display
	while( BusyXLCD() );
	wrstring("Powering On ...");
	__delay_ms(1000);
	SendCommand(Line2);
	wrstring("Getting Ready ...");
	__delay_ms(1000);
	return;
}
char	ADC(char ADCwrite, char BattNum){
	__delay_ms(1);
	if(BattNum==1){
	 Change ACD Read Pin to Batt1
		ANSEL = BattOneAnsel;
		ADCON0bits.CHS = 0b0000;
	}
	else if(BattNum==2){
	 Change ACD Read Pin to Batt2
		ANSEL = 0x0;
		ANSELH = BattTwoAnsel;
		ADCON0bits.CHS = 0b1010;
	}
		ADCON0bits.GO = 1;
		while(ADCON0bits.GO)continue;

		if (ADCON0bits.GO == 0){
			twobyte = ADRESL | ADRESH << 8;
			if(BattNum==1){
			 Scale Batt1 Value
				return BattOne = (unsigned char)(((twobyte/1023)*4.50)/ Ratio1);
			}
			else if(BattNum==2){
			 Scale Batt2 Value
				return BattTwo = (unsigned char)((twobyte/1023)*4.13)/ Ratio2;

			}
		}
}
void	ShiftC(char shiftNum){
	for(char i = 0; i < shiftNum; i++){
		SendCommand(SHIFTCRIGHT);
		__delay_ms(SHIFTCTIME);
	}
}
void	ShiftD(char shiftNumD){
	for(char i = 0; i < shiftNumD; i++){
		SendCommand(SHIFTDLEFT);
		__delay_ms(250);
	}
}
void	PWMInit(unsigned char pr2, unsigned char ccphigh, char ccplow){
	SendCommand(CLEAR);
	SendCommand(0x2);
	wrstring("PWM Ready ...");
	TRISCbits.TRISC5 = 1;
	PR2 = pr2;
	CCP1CONbits.CCP1M = 0b1100 ;// PWM Mode
	CCPR1L = ccphigh;			// The eight  MSB
	CCP1CONbits.DC1B = ccplow;  // The two LSB
	//width = (CCPR1L << 8) | CCP1CONbits.DC1B
	PIR1bits.TMR2IF = 0;	  // Clear TMR2 Int Flag
	T2CONbits.T2CKPS = 0x00;   // TMR2 PreScale Value
	T2CONbits.TMR2ON = 1;
	TRISCbits.TRISC5 = 0;
	__delay_ms(750);
}
void    SendCommand(unsigned char cmd){
#ifdef	_8BIT
	DATA_PORTS = 0x00;
	__delay_us(1);
	DATA_PORTS = cmd;
	__delay_us(1);
	RS = 0;
	RW = 0;
	PulseE();
	__delay_us(1);
#elif	defined(_LOWER)
		__delay_ms(1);
	DATA_PORTS &= 0xf0;			//clear ports  //*
	DATA_PORTS |= ((cmd & 0xf0) >>4 ); //*
	__delay_ms(1);
	RS = 0;
	RW = 0;
	__delay_ms(1);
	PulseE();
	__delay_ms(1);
	//DATA_PORTS &= 0x0f;
	__delay_ms(1);
	DATA_PORTS |= (((cmd<<4) & 0xf0) >>4);
	RS = 0;
	RW = 0;
	__delay_ms(1);
	PulseE();
	__delay_ms(1);
#else
	__delay_ms(1);
	DATA_PORTS &= 0x0f;			//clear ports
	DATA_PORTS |= (cmd & 0xf0);
	__delay_ms(1);
	RS = 0;
	RW = 0;
	__delay_ms(1);
	PulseE();
	__delay_ms(1);
	//DATA_PORTS &= 0x0f;
	__delay_ms(1);
	DATA_PORTS |= ((cmd<<4) & 0xf0);
	RS = 0;
	RW = 0;
	__delay_ms(1);
	PulseE();
	__delay_ms(1);
#endif
	}
void    SendCharacter(unsigned char val){
#ifdef	_8BIT
	DATA_PORTS = 0x00;
	__delay_us(1);
	DATA_PORTS = val;
	__delay_us(1);
	RS = 1;
	RW = 0;
	PulseE();
	__delay_us(1);
#elif	defined(_LOWER)
	while( BusyXLCD() )continue;
	__delay_ms(1);
	DATA_PORTS &= 0xf0;			//clear ports  //*
	__delay_ms(1);
	DATA_PORTS |= ((val & 0xf0) >>4);
	__delay_ms(1);
	RS = 1;
	RW = 0;
	PulseE();
	__delay_ms(1);
	DATA_PORTS &= 0xf0;			//clear ports
	__delay_ms(1);
	DATA_PORTS |= (((val<<4) & 0xf0) >>4);
	RS = 1;
	RW = 0;
	__delay_ms(1);
	PulseE();
#else
	while( BusyXLCD() )continue;
	__delay_ms(1);
	DATA_PORTS &= 0x0f;			//clear ports
	__delay_ms(1);
	DATA_PORTS |= (val & 0xf0);
	__delay_ms(1);
	RS = 1;
	RW = 0;
	PulseE();
	__delay_ms(1);
	DATA_PORTS &= 0x0f;			//clear ports
	__delay_ms(1);
	DATA_PORTS |= ((val<<4) & 0xf0);
	RS = 1;
	RW = 0;
	__delay_ms(1);
	PulseE();
#endif
	}
void    PulseE(void){
	__delay_ms(1);
	Enable = 1;
	__delay_us(1);
	Enable = 0;
	__delay_ms(1);
}
unsigned char BusyXLCD(void){
	__delay_ms(1);
#ifdef	_8BIT
	TRIS_DATA_PORTS = 0xff;
#elif  defined(_LOWER)
	TRIS_DATA_PORTS |= 0x0f;
#else
	TRIS_DATA_PORTS |= 0xf0;
#endif
#ifdef	_LOWER
		__delay_us(1);
	RW = 1;
	RS = 0;
	__delay_us(1);
	Enable = 0;
	__delay_us(1);
	Enable = 1;
	__delay_us(10);
	if((DATA_PORTS << 4) & 0x80){		//*
		__delay_us(1);
		Enable = 0;
		RW = 0;
#else //for High PORTC bits of 8 bits
	__delay_us(1);
	RW = 1;
	RS = 0;
	__delay_us(1);
	Enable = 0;
	__delay_us(1);
	Enable = 1;
	__delay_us(10);
	if(DATA_PORTS & 0x80){
		__delay_us(1);
		Enable = 0;
		RW = 0;
#endif
#ifdef _8BIT
		TRIS_DATA_PORTS = 0x00;
#elif	defined(_LOWER)
		TRIS_DATA_PORTS ^= 0x0f;		//*
#else
		TRIS_DATA_PORTS ^= 0xf0;
#endif
		__delay_us(1);
		return 1;
	}
	else{
		Enable = 0;
		RW = 0;
#ifdef _8BIT
		TRIS_DATA_PORTS = 0x00;
#elif  defined(_LOWER)
		TRIS_DATA_PORTS ^= 0x0f;		//*
#else
		TRIS_DATA_PORTS ^= 0xf0;
#endif
		__delay_us(1);
		return 0;
		}
}
void	wrstring(char *str){
	char *ptr = str;

	while (*ptr != '\0' && *ptr != '\n'){
		SendCharacter(*(ptr++));
	}
}
