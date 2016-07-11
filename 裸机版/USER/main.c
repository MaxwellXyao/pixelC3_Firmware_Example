#include "project_include.h"



FATFS FatFs;   				//存放文件系统信息
FIL file,file1;	   				//存放文件信息
FRESULT f_res;			//FatFs通用结果码
unsigned int br,bw;				//读写变量
u8 SDdatabuff[100];	//SD卡数据缓存
u16 rlen;			//读取到数据长度
u8 test_buff[] ="SDCard FatFs Test OK!";	//测试字符串

u8 SDtatus;	//SD卡初始化状态


MENU_PAGE index,fftest;						//页面声明
#define SIZE 100
u8 TEXT_Buffer[SIZE]="input!";


///////////////////////////////////////////////////////////////////////////////////////

void _writeFlash(void)
{
	SPI_Flash_Write(TEXT_Buffer,2*1024*1024-100,SIZE);//从倒数第 100个地址处开始,写入 SIZE 长度的数据	
}

void _eraseFlash(void)
{
	SPI_Flash_Erase_Sector(511);	
}

__M_PAGE(index,"pixelC3",PAGE_NULL,{		//宏方式创建页面
	u8 k=0;
	static u8 k1=0;
	u8 datatemp[SIZE];
	SOLGUI_Cursor(6,2,7);
	SOLGUI_Widget_GotoPage(0,&fftest);	
	SOLGUI_Widget_Button(1,"Blink",pixelC_HW_flagLight_Twinkle);						//按键
	SOLGUI_Widget_Button(2,"Write",_writeFlash);
	SOLGUI_Widget_Button(3,"Erase",_eraseFlash);
	SOLGUI_Widget_Edit(4,"Edit",100,TEXT_Buffer);
	k=SOLGUI_GetCurrentKey();
	if(k!=0) k1=k;
	SOLGUI_Widget_Text(0,8,F6X8,"key=%X,switch=%X",k1,pixelC_HW_Switch_GetValue());	
	SPI_Flash_Read(datatemp,2*1024*1024-100,SIZE);//从倒数第 100 个地址处开始,读出 SIZE 个字节
	SOLGUI_Widget_Text(0,0,F6X8,"%s",datatemp);
});

///////////////////////////////////////////////////////////////////////////////////////




		
void _testf1(void)
{
	pixelC_HW_flagLight_Twinkle();
	f_res = f_mount(SD_CARD,&FatFs);	//挂载SD卡
}

void _testf2(void)
{
	pixelC_HW_flagLight_Twinkle();
	f_res = f_open(&file,"test.txt",FA_READ);	//打开文件
	f_res = f_read(&file,SDdatabuff,80,&br);	//读取文件内容
	f_res = f_close(&file);	//关闭文件
}

 __M_PAGE(fftest,"FATFS_test",&index,{		//宏方式创建页面
	SOLGUI_Cursor(6,2,7);
	SOLGUI_Widget_Button(0,"1.f_mount",_testf1);						//按键
	SOLGUI_Widget_Button(1,"2.f_open",_testf2);						//按键
	SOLGUI_Widget_Text(0,8,F6X8,"Status=%d,res=%d",SDtatus,f_res);
	SOLGUI_Widget_Text(0,0,F6X8,"%s",SDdatabuff);
});













int main(void)
{		
	u8 kv=0;
	delay_init();
	pixelC_HW_flagLight_Init();
	SPI_Flash_Init();
	pixelC_HW_Key_Init();						//按键扫描初始化
	SOLGUI_Init(&index);
	
	if(SD_Init() != SD_OK)	//初始化失败
	{
		SDtatus = SD_Init();
	}


	pixelC_HW_flagLight_Twinkle();
			
	while(1)
	{
		kv=pixelC_HW_Key_GetValue();	//键值获取
		SOLGUI_InputKey(kv);			//键值输入
		SOLGUI_Menu_PageStage();	 	//前台
		SOLGUI_Refresh();				//更新屏幕
	}
}


