/*
 * @Descripttion: ����ͷ�ļ�
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 12:46:09
 * @LastEditors: adol
 * @LastEditTime: 2019-11-29 20:23:23
 */

#ifndef __UART_H__
#define __UART_H__

/***** macro declaration *****/
#define fosc 11059200L      //ϵͳƵ��
#define baud 9600           //UART������

#define none_parity     0   //��У��
#define odd_parity      1   //��У��
#define even_parity     2   //żУ��
#define mark_parity     3   //1У��
#define space_parity    4   //0У��

#define paritybit   none_parity

/***** variable declaration *****/
extern unsigned char RX_buffer[70];
extern bit flag_gps_data_ok;

/***** function declaration *****/
void uart_init();
void uart_send_data(unsigned char dat);
void uart_send_string(unsigned char *s);

#endif