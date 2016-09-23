/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

//#include <stdint.h>        /* For uint8_t definition */
//#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#ifndef stdio.h
#include <stdio.h>
#endif
//#ifndef math.h
//#include <math.h>
//#endif
#ifndef stdlib.h
#include <stdlib.h>
#endif


/******************************************************************************/                                       */
int			twobyte;
float		volt = 0;
char		*buf1;
char		*buf2;
int			i, status;
unsigned char temp1, temp2, tempINT = 1;
float			BattOne;
float			BattTwo;
float			BattOnePercent;
float			BattTwoPercent;

unsigned char pattern[8] = {0,10,0,0,17,10,14,0};

/******************************************************************************/
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
	InitApp();
	LCDInit();
	SendCommand(CLEAR);

	tempINT = 1;
	Gate1 = 0;
	Gate2 = 0;
	Red   = 0;

	if(tempINT){
		SendCommand(CLEAR);
		SendCommand(Line2);
		wrstring("Standby");
		PORTBbits.RB4 = 0;
//		SLEEP();
		while(tempINT){}
	}
		SendCommand(CLEAR);
		wrstring("Batt 1");
		ShiftC(1);
		wrstring("Batt 2");
		SendCommand(Line2);
		ShiftC(1);

//******************************************************************************
		ADCON0bits.CHS = 0b0000;
		ADCON0bits.ADON = 1;
		Gate1 = 1;
		__delay_ms(1000); //Guess for Voltage time to go up.
		Gate2 = 0;

		ADCON0bits.GO = 1;
		__delay_ms(1);
		while(ADCON0bits.GO)continue;
			if (ADCON0bits.GO == 0){
				twobyte = ADRESL | (ADRESH << 8);
			}
		// Scale Batt1 Value
		BattOne = (float)(twobyte/((float)1023))*VREF / Ratio1;  /// /1023*VREF Ratio1
		//BattOnePercent = (unsigned char)(((twobyte/1023)*4.50) / VREF)*100;
		BattOnePercent = (float) BattOne/12.5*100;
		buf1 = ftoa(BattOnePercent, &status);

		SendCommand(Line2);
		if(BattOne > 4){
			wrstring(buf1);
			SendCommand(Line2);
			ShiftC(2);
			SendCharacter(0x25); // %
			SendCharacter(0x20); // blank
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
		}
		else{
			wrstring("Low Volt!");
			Red = 1;
		}
		Gate1  = 0;

		//temp2 = (unsigned char) ADC(Read,2);
//**************************************************

		ADCON0bits.CHS = 0b1010;
		Gate1 = 0;
		Gate2 = 1;
		__delay_ms(1000); //Guess time for Voltage time to go up.

		ADCON0bits.GO = 1;
		while(ADCON0bits.GO);
			if (ADCON0bits.GO == 0){
				twobyte = ADRESL | ADRESH << 8;
			}
		// Scale Batt2 Value
		BattTwo = ((twobyte/(float)1023)*VREF)/ Ratio2;
		BattTwo = BattTwo - BattOne;
		BattTwoPercent = (float) (BattTwo / 12.5) * 100;
		buf2 = ftoa(BattTwoPercent, &status);

		SendCommand(Line2);
		ShiftC(9);
		if(BattTwo > 4){
			wrstring( buf2 );
			SendCommand(Line2);
			ShiftC(11);
			SendCharacter(0x25); // %
			SendCharacter(0x20); // blank
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCharacter(0x20);
			SendCommand(SHIFTDRIGHT);
			SendCommand(SHIFTDRIGHT);
			SendCommand(SHIFTDRIGHT);
			SendCommand(SHIFTDLEFT);
			SendCommand(SHIFTDLEFT);
			SendCommand(SHIFTDLEFT);
			SendCommand(SHIFTDLEFT);
		}
		else{
			wrstring("Low Volt!");
			Red = 1;
			}

		__delay_ms(1000);
		Gate2 = 0;
		ADCON0bits.ADON = 0;
		TRISBbits.TRISB4 = 1;
		return;


	}




