#include "init.h"
#define TASK3_3 0

typedef struct{
	int standardId;
	int data;
}txMsgPacket;
typedef char byte;
#define ENABLE 1
#define DISABLE 0

int speed=0,direction=0,speedMessageStatus=DISABLE;
txMsgPacket speedMsgPacket,dataPacket;

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
    PIT_ConfigureTimer(0,100);
    PIT_ConfigureTimer(1,200);
    PIT_StartTimer(0);
    PIT_StartTimer(1);
    speedMsgPacket.standardId=75; 
dataPacket.standardId=2; 
    speedMessageStatus=ENABLE;
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
void _delay_cycles(int numCycles){
	while(numCycles--);
}


void blinkTimes(int times,vuint8_t LEDx){
byte i;
	for(i=0;i<times;i++){
		LEDx=1;
		_delay_cycles(50000);
		LEDx=0;
		_delay_cycles(50000);
	}

}
void speedTick(){
    if(speed==200){dataPacket.data=1;CAN_Write(0,dataPacket);}
	if(speed>=300){direction=1;}
	else if (speed<=0){direction=0;}
	if(direction==1){
		speed-=5;
	} else {
		speed+=5;
	}
	speedMsgPacket.data=speed;
}


void PITCHANNEL0() {
	/* clear flag */
	PIT.CH[0].TFLG.B.TIF = 1;
	LED0=~LED0;
	if(speedMessageStatus!=DISABLE){
		CAN_Write(0,speedMsgPacket);	
	}
	
}

void PITCHANNEL1() {
	/* clear flag */
	PIT.CH[1].TFLG.B.TIF = 1;
	speedTick();

}

void FLEXCAN_BUF_00_03(){
	CAN_0.IFRL.B.BUF00I=1;
	CAN_0.IFRL.B.BUF01I=1;
	CAN_0.IFRL.B.BUF02I=1;
	CAN_0.IFRL.B.BUF03I=1;
}

void FLEXCAN_BUF_04_07(){
	if(CAN_0.IFRL.B.BUF05I==1){
    	 LED5=~LED5;
		//read the fifo from this interrupt	
		switch(CAN_0.RXFIFO.ID.B.STD_ID){
			case 0xFF:
				if(CAN_0.RXFIFO.DATA.B[0]!=0){
					//got valid data 
					blinkTimes(10,LED1);
				}
			break;
			case 0x01:
				speedMessageStatus=~speedMessageStatus;
                LED2=~LED2;
			break;
		}
		//read the incoming data in the  message buffer before clearing it
    }
    LED6=~LED6;
    CAN_0.IFRL.B.BUF04I=1;
	CAN_0.IFRL.B.BUF05I=1;
	CAN_0.IFRL.B.BUF06I=1;
	CAN_0.IFRL.B.BUF07I=1;
}

void FLEXCAN_BUF_08_11(){
		
	if(CAN_0.IFRL.B.BUF09I==1){
		//clearing the interrupt flag after working with it 
		CAN_0.IFRL.B.BUF09I=1;
	}
    CAN_0.IFRL.B.BUF09I=1;
    CAN_0.IFRL.B.BUF08I=1;
	CAN_0.IFRL.B.BUF10I=1;
	CAN_0.IFRL.B.BUF11I=1;
}

void FLEXCAN_BUF_12_15(){
	CAN_0.IFRL.B.BUF12I=1;
	CAN_0.IFRL.B.BUF13I=1;
	CAN_0.IFRL.B.BUF14I=1;
	CAN_0.IFRL.B.BUF15I=1;
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

