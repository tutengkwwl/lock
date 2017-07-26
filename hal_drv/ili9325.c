/*******************************************************************************

 **** Copyright (C), 2017, Transtek Co., Ltd.                ****

 *******************************************************************************
 * File Name     : ili9325.c
 * Author        : soul
 * Date          : 2017-07-26
 * Description   : .C file function description
 * Version       : 1.0
 * 
 * Record        :
 * Modification  : Created file // 2017-07-26

*******************************************************************************/



#include "ili9325.h"
#include "stm32f0xx_hal.h"





#define PORT_DB0    GPIOB
#define PORT_DB1    GPIOB
#define PORT_DB2    GPIOB
#define PORT_DB3    GPIOB
#define PORT_CS     GPIOB
#define PORT_RS     GPIOD
#define PORT_WR     GPIOC
#define PORT_RD     GPIOC
#define PORT_DB4    GPIOC
#define PORT_DB8    GPIOA
#define PORT_DB9    GPIOA
#define PORT_DB10   GPIOF
#define PORT_DB11   GPIOF
#define PORT_DB12   GPIOA
#define PORT_DB13   GPIOA
#define PORT_DB14   GPIOA
#define PORT_DB15   GPIOA
#define PORT_RST    GPIOA
#define PORT_DB5    GPIOA
#define PORT_DB6    GPIOC
#define PORT_DB7    GPIOC





#define PIN_DB0     GPIO_PIN_7
#define PIN_DB1     GPIO_PIN_6
#define PIN_DB2     GPIO_PIN_5
#define PIN_DB3     GPIO_PIN_4
#define PIN_CS      GPIO_PIN_3
#define PIN_RS      GPIO_PIN_2
#define PIN_WR      GPIO_PIN_12
#define PIN_RD      GPIO_PIN_11
#define PIN_DB4     GPIO_PIN_10
#define PIN_DB8     GPIO_PIN_15
#define PIN_DB9     GPIO_PIN_14
#define PIN_DB10    GPIO_PIN_7
#define PIN_DB11    GPIO_PIN_6
#define PIN_DB12    GPIO_PIN_13
#define PIN_DB13    GPIO_PIN_12
#define PIN_DB14    GPIO_PIN_11
#define PIN_DB15    GPIO_PIN_10
#define PIN_RST     GPIO_PIN_9
#define PIN_DB5     GPIO_PIN_8
#define PIN_DB6     GPIO_PIN_9
#define PIN_DB7     GPIO_PIN_8




err_t tft_init()
{
    /* GPIO Init */
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = PIN_DB0 | PIN_DB1 | PIN_DB2 | PIN_DB3 | PIN_CS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_RS;;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_Init();

    GPIO_InitStruct.Pin = PIN_WR | PIN_RD | PIN_DB4 | PIN_DB6 | PIN_DB7;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_DB8 | PIN_DB9 | PIN_DB4 | PIN_DB12 | PIN_DB13 |
                          PIN_DB14 | PIN_DB15;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    
    return ERR_SUCCESS;
}














#include <reg51.h>


#define Bus_16        //16位数据模式,如果使用8位模式，请注释此语句，如果使用16位模式，请打开此句
#define  LCD_DataPortH P2     //高8位数据口,8位模式下只使用高8位 
#define  LCD_DataPortL P0     //低8位数据口	,8位模式下低8位可以不接线,请确认P0口已经上拉10K电阻,不宜太小，最小4.7K,推荐10K.
sbit LCD_RS = P3^5;  		 //数据/命令切换
sbit LCD_WR = P3^6;		  //写控制
sbit LCD_RD =P3^7;		     //读控制
sbit LCD_CS=P1^0;		//片选	
sbit LCD_REST = P1^2;	      //复位   

void delayms(int count)  // /* X1ms */
{
        int i,j;
        for(i=0;i<count;i++)
                for(j=0;j<1000;j++);
}

#ifdef  Bus_16    //条件编译-16位数据模式   
void LCD_Writ_Bus(char VH,char VL)   //并行数据写入函数
{
    LCD_DataPortH=VH;	
	LCD_DataPortL=VL;		
	LCD_WR=0;
	LCD_WR=1; 
}
#else			//条件编译-8位数据模式 
void LCD_Writ_Bus(char VH,char VL)   //并行数据写入函数
{	
    LCD_DataPortH=VH;	
   	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=VL;		
	LCD_WR=0;
	LCD_WR=1;
}
#endif

void Lcd_Write_Com(char VH,char VL)	 //发送命令-8位参数
{	
    LCD_RS=0;
	LCD_Writ_Bus(VH,VL);
}
void Lcd_Write_Data(char VH,char VL) //发送数据-8位参数
{
    LCD_RS=1;
	LCD_Writ_Bus(VH,VL);
}
void Lcd_Write_Com_Data(int com,int val)		   //发送数据命令
{
	Lcd_Write_Com(com>>8,com);
    Lcd_Write_Data(val>>8,val);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	Lcd_Write_Com(0x00,0x20);Lcd_Write_Data(x1>>8,x1);	  //设置X坐标位置
    Lcd_Write_Com(0x00,0x21);Lcd_Write_Data(y1>>8,y1);   //设置Y坐标位置
    Lcd_Write_Com(0x00,0x50);Lcd_Write_Data(x1>>8,x1);   //开始X
	Lcd_Write_Com(0x00,0x52);Lcd_Write_Data(y1>>8,y1);	  //开始Y
    Lcd_Write_Com(0x00,0x51);Lcd_Write_Data(x2>>8,x2);   //结束X
	Lcd_Write_Com(0x00,0x53);Lcd_Write_Data(y2>>8,y2);   //结束Y
    Lcd_Write_Com(0x00,0x22);							 
}

void Lcd_Init(void)
{

    LCD_REST=1;
    delayms(5);	
	LCD_REST=0;
	delayms(5);
	LCD_REST=1;
	LCD_CS=1;
	LCD_RD=1;
	LCD_WR=1;
	delayms(5);
	

	LCD_CS =0;  //打开片选使能
	//************* Start Initial Sequence **********//

	
	Lcd_Write_Com_Data(0x0001, 0x0100); // set SS and SM bit
Lcd_Write_Com_Data(0x0002, 0x0700); // set 1 line inversion
Lcd_Write_Com_Data(0x0003, 0x1030); // set GRAM write direction and BGR=1.
Lcd_Write_Com_Data(0x0004, 0x0000); // Resize register
Lcd_Write_Com_Data(0x0008, 0x0202); // set the back porch and front porch
Lcd_Write_Com_Data(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
Lcd_Write_Com_Data(0x000A, 0x0000); // FMARK function
Lcd_Write_Com_Data(0x000C, 0x0000); // RGB interface setting
Lcd_Write_Com_Data(0x000D, 0x0000); // Frame marker Position
Lcd_Write_Com_Data(0x000F, 0x0000); // RGB interface polarity
//*************Power On sequence ****************//
Lcd_Write_Com_Data(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
Lcd_Write_Com_Data(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
Lcd_Write_Com_Data(0x0012, 0x0000); // VREG1OUT voltage
Lcd_Write_Com_Data(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
Lcd_Write_Com_Data(0x0007, 0x0001);
delayms(200); // Dis-charge capacitor power voltage
Lcd_Write_Com_Data(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
Lcd_Write_Com_Data(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
delayms(50); // Delay 50ms
Lcd_Write_Com_Data(0x0012, 0x009D); // Internal reference voltage= Vci;
delayms(50); // Delay 50ms
Lcd_Write_Com_Data(0x0013, 0x1900); // Set VDV[4:0] for VCOM amplitude
Lcd_Write_Com_Data(0x0029, 0x0025); // Set VCM[5:0] for VCOMH
Lcd_Write_Com_Data(0x002B, 0x000D); // Set Frame Rate
delayms(50); // Delay 50ms
Lcd_Write_Com_Data(0x0020, 0x0000); // GRAM horizontal Address
Lcd_Write_Com_Data(0x0021, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
Lcd_Write_Com_Data(0x0030, 0x0007);
Lcd_Write_Com_Data(0x0031, 0x0303);
Lcd_Write_Com_Data(0x0032, 0x0003);
Lcd_Write_Com_Data(0x0035, 0x0206);
Lcd_Write_Com_Data(0x0036, 0x0008);
Lcd_Write_Com_Data(0x0037, 0x0406);
Lcd_Write_Com_Data(0x0038, 0x0304);
Lcd_Write_Com_Data(0x0039, 0x0007);
Lcd_Write_Com_Data(0x003C, 0x0602);
Lcd_Write_Com_Data(0x003D, 0x0008);
//------------------ Set GRAM area ---------------//
Lcd_Write_Com_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
Lcd_Write_Com_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
Lcd_Write_Com_Data(0x0052, 0x0000); // Vertical GRAM Start Address
Lcd_Write_Com_Data(0x0053, 0x013F); // Vertical GRAM Start Address
Lcd_Write_Com_Data(0x0060, 0xA700); // Gate Scan Line
Lcd_Write_Com_Data(0x0061, 0x0001); // NDL,VLE, REV
Lcd_Write_Com_Data(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
Lcd_Write_Com_Data(0x0080, 0x0000);
Lcd_Write_Com_Data(0x0081, 0x0000);
Lcd_Write_Com_Data(0x0082, 0x0000);
Lcd_Write_Com_Data(0x0083, 0x0000);
Lcd_Write_Com_Data(0x0084, 0x0000);
Lcd_Write_Com_Data(0x0085, 0x0000);
//-------------- Panel Control -------------------//
Lcd_Write_Com_Data(0x0090, 0x0010);
Lcd_Write_Com_Data(0x0092, 0x0600);
Lcd_Write_Com_Data(0x0007, 0x0133); // 262K color and display ON

	LCD_CS =1;  //关闭片选使能

}

void Pant(char VH,char VL)
{
	int i,j;
	LCD_CS =0;  //打开片选使能
	Address_set(0,0,239,319);
    for(i=0;i<320;i++)
	 {
	  for (j=0;j<240;j++)
	   	{
        	 Lcd_Write_Data(VH,VL);
	    }

	  }
     LCD_CS =1;  //关闭片选使能
}	 
main()
{ 
	Lcd_Init();          //TFT初始化
 	Pant(0xff,0xff);	 //TFT清屏	
	while(1)
	{
	    Pant(0xf8,0x00); //红色
		delayms(2000);
	    Pant(0X07,0xE0); //绿色
		delayms(2000);
	    Pant(0x00,0x1f); //蓝色      		 
    	delayms(2000);
    }



}


























