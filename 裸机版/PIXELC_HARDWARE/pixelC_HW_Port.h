#ifndef PIXELC_HW_PORT_H
#define PIXELC_HW_PORT_H

#include<stdarg.h> 						//֧�ֱ䳤����

//#############################�����塿###############################
typedef enum _PIN_PXx{
	PA0=0x00,		
	PA1=0x01,
	PA2=0x02,
	PA3=0x03,									  
	PB12=0x1c,		
	PB13=0x1d,
	PB14=0x1e,
	PB15=0x1f	
}PIN_PXx; 	//���ű��:


/*
u32 ������Ϣ��¼��

GPIO:
[31:29]		:	GPIOX	(A~G)
[28:25]		��	.XX		(0~15)

	

*/

#endif
