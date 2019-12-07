/*
 * @Descripttion: ���ڳ�ʼ��������
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
 * @function: ����1��ʼ��
 * @param { void } 
 * @return: none
 */
void uart_init(void)
{
#if (paritybit == none_parity)
    SCON = 0x50;            //8λ�ɱ�UART
#elif (paritybit == odd_parity) || (paritybit == even_parity) || (paritybit == mark_parity)
    SCON = 0xda;            //9λ�ɱ�UART, ��żУ��λ��ʼΪ1
#elif (paritybit == space_parity)
    SCON = 0xd2;            //9λ�ɱ�UART, ��żУ��λ��ʼΪ0
#endif

    PCON &= 0x7F;                           //�����ʲ�����
    TMOD = 0x20;                            //��Timer1����Ϊ8λ�Զ�����ģʽ
    TH1 = TL1 = -(fosc / 12 / 32 / baud);   //�����Զ���װֵ
    ET1 = 0;                                //��ֹ��ʱ��1�ж�
    TR1 = 1;                                //Timer1��ʼ����
    REN = 1;                                //�����пڽ�������
    ES = 1;                                 //����UART�ж�
    EA = 1;                                 //�������ж�
}

/**
 * @name: uart_isr()
 * @function: �����жϷ�����
 * @param { void } 
 * @return: none
 */
void uart_isr(void) interrupt 4 using 1
{
    unsigned char temp = 0;
    if (RI)
    {
        // RI = 0;             //��������жϱ�־
        // P0 = SBUF;          //����UART����

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
        TI = 0;         //��������жϱ�־
        uart_busy = 0;  //�������uart_busy��־
    }
}

/**
 * @name: uart_send_data();
 * @function: ����һ���ֽڵ�����
 * @param {unsigned char} dat -> �ַ�
 * @return: none
 */
void uart_send_data(unsigned char dat)
{
    while (uart_busy);  //�ȴ�֮ǰ�����ݷ������
    ACC = dat;          
    uart_busy = 1;
    SBUF = ACC;         //�������ݵ�UART������
}

/**
 * @name: uart_send_string(char *s)
 * @function: ����һ���ַ���������
 * @param {unsigned char} s -> �ַ�����ַ
 * @return: none
 */
void uart_send_string(unsigned char *s)
{
    while (*s)      //����ַ����Ľ�β
    {
        uart_send_data(*s++);   //���͵�ǰ�ַ�
    }
    uart_send_data('\r');
    uart_send_data('\n');
}

