#include "init.h"

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
    PIT_ConfigureTimer(0,100);
    PIT_ConfigureTimer(1,200);
    /* forever */
    for(;;)
    {
/********************************************************************
 *                           Main Loop                              *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                  Write down your logic here.                     *
 ********************************************************************/  
    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                Interrupts can be handled below.                  *
 ********************************************************************/  
int speed=0,direction=0;
void speedTick(){
	if(speed>=300){direction=1;}
	else if (speed<=0){direction=0;}
	if(direction==1){
		speed-=5;
	} else {
		speed+=5;
	}

}


void PITCHANNEL0() {
	/* clear flag */
	PIT.CH[0].TFLG.B.TIF = 1;
	LED0=~LED0;
	CAN_Write(0,speed);
}

void PITCHANNEL1() {
	/* clear flag */
	PIT.CH[1].TFLG.B.TIF = 1;
	speedTick();

}

void FLEXCAN_BUF_00_03(){
	CAN_0.IFRL.B.BUF00I=0;
	CAN_0.IFRL.B.BUF01I=0;
	CAN_0.IFRL.B.BUF02I=0;
	CAN_0.IFRL.B.BUF03I=0;
}

void FLEXCAN_BUF_04_07(){
	CAN_0.IFRL.B.BUF04I=0;
	CAN_0.IFRL.B.BUF05I=0;
	CAN_0.IFRL.B.BUF06I=0;
	CAN_0.IFRL.B.BUF07I=0;
}

void FLEXCAN_BUF_08_11(){
	CAN_0.IFRL.B.BUF08I=0;
	CAN_0.IFRL.B.BUF09I=0;
	CAN_0.IFRL.B.BUF10I=0;
	CAN_0.IFRL.B.BUF11I=0;
}

void FLEXCAN_BUF_12_15(){
	CAN_0.IFRL.B.BUF12I=0;
	CAN_0.IFRL.B.BUF13I=0;
	CAN_0.IFRL.B.BUF14I=0;
	CAN_0.IFRL.B.BUF15I=0;
}

/* ... ISRs ... */

/********************************************************************
 *                   Interrupt Vector Table                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC){
		/* ... ISR calls should be here ... */
		case 59:
			PITCHANNEL0();
			break;
		case 60:
			PITCHANNEL1();
			break;
		case 68:
			FLEXCAN_BUF_00_03();
			break;
		case 69:
			FLEXCAN_BUF_04_07();
			break;
		case 70:
			FLEXCAN_BUF_08_11();
			break;
		case 71:
			FLEXCAN_BUF_12_15();
			break;
		default:
			break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}

