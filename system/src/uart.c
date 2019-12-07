/*
 * @Descripttion: 串口初始化及配置
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 12:46:01
 * @LastEditors: adol
 * @LastEditTime: 2019-11-29 20:18:52
 */

#include "intrins.h"
#include "core.h"

bit flag_gps_data_ok = 0;
bit uart_busy;
unsigned char RX_count = 0;
unsigned char RX_buffer[70];

/**
 * @name: uart_init();
 * @function: 串口1初始化
 * @param { void } 
 * @return: none
 */
void uart_init(void)
{
#if (paritybit == none_parity)
    SCON = 0x50;            //8位可变UART
#elif (paritybit == odd_parity) || (paritybit == even_parity) || (paritybit == mark_parity)
    SCON = 0xda;            //9位可变UART, 奇偶校验位初始为1
#elif (paritybit == space_parity)
    SCON = 0xd2;            //9位可变UART, 奇偶校验位初始为0
#endif

    PCON &= 0x7F;                           //波特率不倍速
    TMOD = 0x20;                            //将Timer1设置为8位自动重载模式
    TH1 = TL1 = -(fosc / 12 / 32 / baud);   //设置自动重装值
    ET1 = 0;                                //禁止定时器1中断
    TR1 = 1;                                //Timer1开始运行
    REN = 1;                                //允许串行口接收数据
    ES = 1;                                 //启用UART中断
    EA = 1;                                 //开启总中断
}

/**
 * @name: uart_isr()
 * @function: 串口中断服务函数
 * @param { void } 
 * @return: none
 */
void uart_isr(void) interrupt 4 using 1
{
    unsigned char temp = 0;
    if (RI)
    {
        // RI = 0;             //清除接收中断标志
        // P0 = SBUF;          //接收UART数据

        /***** code *****/
        ES=0;
        temp = SBUF;
        RI = 0;
        
        if (temp == '$')
        {
            RX_count = 0;
            flag_gps_data_ok = 0;
        }
        
        RX_buffer[RX_count++] = temp;
        
        if(RX_buffer[RX_count - 1] == '*')
        {
            RX_count = 69;
            flag_gps_data_ok = 1;
        }

        ES=1; 
        /***** code *****/
    }
    if (TI)
    {
        TI = 0;         //清除发送中断标志
        uart_busy = 0;  //清除发送uart_busy标志
    }
}

/**
 * @name: uart_send_data();
 * @function: 发送一个字节到串口
 * @param {unsigned char} dat -> 字符
 * @return: none
 */
void uart_send_data(unsigned char dat)
{
    while (uart_busy);  //等待之前的数据发送完成
    ACC = dat;          
    uart_busy = 1;
    SBUF = ACC;         //发送数据到UART缓冲区
}

/**
 * @name: uart_send_string(char *s)
 * @function: 发送一个字符串到串口
 * @param {unsigned char} s -> 字符串地址
 * @return: none
 */
void uart_send_string(unsigned char *s)
{
    while (*s)      //检查字符串的结尾
    {
        uart_send_data(*s++);   //发送当前字符
    }
    uart_send_data('\r');
    uart_send_data('\n');
}

