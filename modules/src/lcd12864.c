/*
 * @Descripttion: lcd12864初始化配置及显示程序
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 12:45:50
 * @LastEditors: adol
 * @LastEditTime: 2019-11-22 20:32:19
 */

#include "intrins.h"
#include "core.h"

bit flag_lcd12864_display_ok = 0;
unsigned char lcd12864_display_buffer[4][16] = {"北斗定位", "xx.xxx xxx.xx", "--时--分--秒", "20--年--月--日"};

/**
 * @name: lcd12864_busy_test();
 * @function: 检测液晶是否处于忙状态
 * @param { void } 
 * @return: 0 : 液晶未处于忙状态,可进行操作; 1 : 液晶处于忙状态,不可进行操作
 */
unsigned char lcd12864_busy_test(void)
{
    unsigned char temp;
    lcd12864_rs(0);     //RS=0，RW=1, 从液晶读取数据
    lcd12864_rw(1);
    lcd12864_en(1);
    lcd12864_delay_ms(1);
    temp = lcd12864_data_port & (0x80);
    lcd12864_en(0);
    return temp;
}

/**
 * @name: send_cmd_to_lcd12864();
 * @function: 向液晶发送指令
 * @param { unsigned char } cmd -> 要发送的指令
 * @return: none
 */
void send_cmd_to_lcd12864(unsigned char cmd)
{
    while(lcd12864_busy_test());
    lcd12864_rs(0);     //RS=0, RW=0, 向液晶发送命令
    lcd12864_rw(0);
    lcd12864_data_port = cmd;
    lcd12864_en(1);
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    lcd12864_en(0);
}

/**
 * @name: send_data_to_lcd12864();
 * @function: 向液晶发送数据
 * @param { unsigned char } dat -> 要发送的数据
 * @return: none
 */
void send_data_to_lcd12864(unsigned char dat)
{
    while(lcd12864_busy_test());
    lcd12864_rs(1);     //RS=1, RW=0, 向液晶发送数据
    lcd12864_rw(0);
    lcd12864_data_port = dat;
    lcd12864_en(1);
    _nop_();
    _nop_();	
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    lcd12864_en(0);
}

/**
 * @name: lcd12864_jump_place(char row,char column);
 * @function: "常规显示模式"下显示坐标跳转至指定行和列
 * @param { unsigned char } row    -> 常规显示模式下12864液晶四行中的一行,参数设定范围为 1~4
 *                          column -> 常规显示模式下12864液晶八列中的一列,参数设定范围为 1~8
 * @return: none
 */
void lcd12864_jump_place(unsigned char row, unsigned char column)    
{
    send_cmd_to_lcd12864(0x30);
    switch(row)
    {
        case 1: send_cmd_to_lcd12864(0x80 + column - 1); break;
        case 2: send_cmd_to_lcd12864(0x90 + column - 1); break;
        case 3: send_cmd_to_lcd12864(0x88 + column - 1); break;
        case 4: send_cmd_to_lcd12864(0x98 + column - 1); break;
    }
}

/**
 * @name: lcd12864_printf_string();
 * @function: 常规显示模式下 "以液晶指定行和列坐标为起点" 显示一串字符,只可进行单行显示
 * @param { unsigned char } row -> 常规显示模式下12864液晶四行中的一行,参数设定范围为 1~4
 *                          column -> 常规显示模式下12864液晶八列中的一列,参数设定范围为 1~8
 *                          add -> 字符串首地址
 * @return: none
 */
void lcd12864_printf_string(unsigned char row, unsigned char column, unsigned char *add)  
{
    unsigned char *temp = add;
    // unsigned char* temp = add;
    lcd12864_jump_place(row, column);
    while((*temp != '\0'))
    {
        send_data_to_lcd12864(*temp++);
    }
}

/**
 * @name: lcd12864_clear();
 * @function: 常规显示模式下 液晶清屏
 * @param { void } 
 * @return: none
 */
// void lcd12864_clear(void)
// {
//   lcd12864_printf_string(1,1,"                ");
//   lcd12864_printf_string(2,1,"                ");
//   lcd12864_printf_string(3,1,"                ");
//   lcd12864_printf_string(4,1,"                ");
// }

/*******************************************************************************
函数调用方法 -> 
    函数功能 -> 
         
      返回值 -> 无
使用本函数时请参考12864液晶 "图形显示坐标"
*******************************************************************************/
/**
 * @name: lcd12864_picture_mode_jump_place();
 * @function: "绘图模式"下显示坐标跳转至指定行和列
 * @param { unsigned char } row    -> "绘图模式"下12864液晶 64 行中的一行,参数设定范围为 1~64
                            column -> "绘图模式"下12864液晶 8  列中的一列,参数设定范围为 1~8
 * @return: none
 */
// void lcd12864_picture_mode_jump_place(unsigned char row, unsigned char column)  
// {
//     char temp;
//     if(row >= 1 && row <= 64 && column >= 1 && column <= 8)
//     {
//         send_cmd_to_lcd12864(0x34);
//         if(row <= 32)
//         {
//             temp = row;
//         }
//         else
//         {
//             temp = row-32;
//         }
//         send_cmd_to_lcd12864(0x80 + temp -1);
        
//         if(row <= 32)
//         {
//             send_cmd_to_lcd12864(0x80 + column -1);
//         }
//         else
//         {
//             send_cmd_to_lcd12864(0x88 + column -1);
//         }
        
//     }
// }

/**
 * @name: lcd12864_picture_mode_print();
 * @function: "绘图模式"下 "以指定行(row)和列(column)为起点" 显示分辨率为 width(宽) * height(高)的单色位图
 * @param { unsigned char } row -> "绘图模式"下12864液晶 64 行中的一行,参数设定范围为 1~64
                            column -> "绘图模式"下12864液晶 8  列中的一列,参数设定范围为 1~8
                            width -> 单色位图的宽度，必须为16的整数倍，数值范围 1~128
                            height -> 单色位图的高度，数值范围 1~64
                            reverse_flag -> 反白显示标志位，0:常规显示;1:反白显示
 * @return: none
 */
// void lcd12864_picture_mode_print(unsigned char row, unsigned char column,
//                                  unsigned char width, unsigned char height,
//                                  unsigned char reverse_flag, unsigned char* add)
// {
//     unsigned char *temp = add;
//     unsigned char i = 0, j = 0, k = 0;
//     k = width / 16;
//     if(reverse_flag)
//     {
//         for(i = 0; i < height; i++)
//         {
//             lcd12864_picture_mode_jump_place(row+i, column);
//             for(j = 0 ;j < k; j++)
//             {
//                 send_data_to_lcd12864(~(*temp++));
//                 send_data_to_lcd12864(~(*temp++));
//             }
//         }
//     }
//     else
//     {
//         for(i = 0; i < height; i++)
//         {
//             lcd12864_picture_mode_jump_place(row + i, column);
//             for(j = 0; j < k; j++)
//             {
//                 send_data_to_lcd12864(*temp++);
//                 send_data_to_lcd12864(*temp++);
//             }
//         }
//     }
//     send_cmd_to_lcd12864(0x36);
//     send_cmd_to_lcd12864(0x30);
// }


/**
 * @name: lcd12864_picture_mode_clear();
 * @function: 绘图模式下 液晶清屏
 * @param { void } 
 * @return: none
 */
// void lcd12864_picture_mode_clear(void)
// {
//     unsigned char i = 0,j = 0;
//     for(i = 0;i < 64; i++)
//     {
//         lcd12864_picture_mode_jump_place(i + 1, 1);
//         for(j = 0; j < 8; j++)
//         {
//             send_data_to_lcd12864(0x00);
//             send_data_to_lcd12864(0x00);
//         }
//     }
// }

/**
 * @name: lcd12864_control_port_init();
 * @function: 设置单片机连接液晶控制端口的IO为输出模式
 * @param { void } 
 * @return: none
 */
// void lcd12864_control_port_init(void)
// {
//   #ifdef lcd12864_res_pin
//     lcd12864_res(1);
//   #endif
//   #ifdef lcd12864_psb_pin
//     lcd12864_psb(1);         //确定液晶控制方式为并行
//   #endif
// }

/**
 * @name: lcd12864_init();
 * @function: 液晶初始化设置，使用其他函数前请先调用此函数
 * @param { void } 
 * @return: none
 */
void lcd12864_init(void)
{
    send_cmd_to_lcd12864(0x30); /*使用8位并口通讯*/
    lcd12864_delay_ms(10);
    send_cmd_to_lcd12864(0x30); /*使用8位并口通讯*/
    lcd12864_delay_ms(10);
    send_cmd_to_lcd12864(0x0c); /*显示开及光标设置*/
    lcd12864_delay_ms(10);
    send_cmd_to_lcd12864(0x01); /*显示清屏*/
    lcd12864_delay_ms(30);
    send_cmd_to_lcd12864(0x06); /*DDRAM的地址计数器（AC）加1*/
    lcd12864_delay_ms(30);
}

/**
 * @name: lcd12864_display();
 * @function: lcd12864显示程序
 * @param { void } 
 * @return: none
 */
void lcd12864_display(void)
{
    if (flag_lcd12864_display_ok == 0)
    {
        lcd12864_printf_string(1, 1, lcd12864_display_buffer[0]);
        lcd12864_printf_string(2, 1, lcd12864_display_buffer[1]);
        lcd12864_printf_string(3, 1, lcd12864_display_buffer[2]);
        lcd12864_printf_string(4, 1, lcd12864_display_buffer[3]);
    }
    
    flag_lcd12864_display_ok = 1;
}