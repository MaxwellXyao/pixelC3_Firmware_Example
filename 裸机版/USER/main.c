#include "project_include.h"



FATFS FatFs;   				//����ļ�ϵͳ��Ϣ
FIL file,file1;	   				//����ļ���Ϣ
FRESULT f_res;			//FatFsͨ�ý����
unsigned int br,bw;				//��д����
u8 SDdatabuff[100];	//SD�����ݻ���
u16 rlen;			//��ȡ�����ݳ���
u8 test_buff[] ="SDCard FatFs Test OK!";	//�����ַ���

u8 SDtatus;	//SD����ʼ��״̬


MENU_PAGE index,fftest;						//ҳ������
#define SIZE 100
u8 TEXT_Buffer[SIZE]="input!";


///////////////////////////////////////////////////////////////////////////////////////

void _writeFlash(void)
{
	SPI_Flash_Write(TEXT_Buffer,2*1024*1024-100,SIZE);//�ӵ����� 100����ַ����ʼ,д�� SIZE ���ȵ�����	
}

void _eraseFlash(void)
{
	SPI_Flash_Erase_Sector(511);	
}

__M_PAGE(index,"pixelC3",PAGE_NULL,{		//�귽ʽ����ҳ��
	u8 k=0;
	static u8 k1=0;
	u8 datatemp[SIZE];
	SOLGUI_Cursor(6,2,7);
	SOLGUI_Widget_GotoPage(0,&fftest);	
	SOLGUI_Widget_Button(1,"Blink",pixelC_HW_flagLight_Twinkle);						//����
	SOLGUI_Widget_Button(2,"Write",_writeFlash);
	SOLGUI_Widget_Button(3,"Erase",_eraseFlash);
	SOLGUI_Widget_Edit(4,"Edit",100,TEXT_Buffer);
	k=SOLGUI_GetCurrentKey();
	if(k!=0) k1=k;
	SOLGUI_Widget_Text(0,8,F6X8,"key=%X,switch=%X",k1,pixelC_HW_Switch_GetValue());	
	SPI_Flash_Read(datatemp,2*1024*1024-100,SIZE);//�ӵ����� 100 ����ַ����ʼ,���� SIZE ���ֽ�
	SOLGUI_Widget_Text(0,0,F6X8,"%s",datatemp);
});

///////////////////////////////////////////////////////////////////////////////////////




		
void _testf1(void)
{
	pixelC_HW_flagLight_Twinkle();
	f_res = f_mount(SD_CARD,&FatFs);	//����SD��
}

void _testf2(void)
{
	pixelC_HW_flagLight_Twinkle();
	f_res = f_open(&file,"test.txt",FA_READ);	//���ļ�
	f_res = f_read(&file,SDdatabuff,80,&br);	//��ȡ�ļ�����
	f_res = f_close(&file);	//�ر��ļ�
}

 __M_PAGE(fftest,"FATFS_test",&index,{		//�귽ʽ����ҳ��
	SOLGUI_Cursor(6,2,7);
	SOLGUI_Widget_Button(0,"1.f_mount",_testf1);						//����
	SOLGUI_Widget_Button(1,"2.f_open",_testf2);						//����
	SOLGUI_Widget_Text(0,8,F6X8,"Status=%d,res=%d",SDtatus,f_res);
	SOLGUI_Widget_Text(0,0,F6X8,"%s",SDdatabuff);
});













int main(void)
{		
	u8 kv=0;
	delay_init();
	pixelC_HW_flagLight_Init();
	SPI_Flash_Init();
	pixelC_HW_Key_Init();						//����ɨ���ʼ��
	SOLGUI_Init(&index);
	
	if(SD_Init() != SD_OK)	//��ʼ��ʧ��
	{
		SDtatus = SD_Init();
	}


	pixelC_HW_flagLight_Twinkle();
			
	while(1)
	{
		kv=pixelC_HW_Key_GetValue();	//��ֵ��ȡ
		SOLGUI_InputKey(kv);			//��ֵ����
		SOLGUI_Menu_PageStage();	 	//ǰ̨
		SOLGUI_Refresh();				//������Ļ
	}
}


