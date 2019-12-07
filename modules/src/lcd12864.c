/*
 * @Descripttion: lcd12864��ʼ�����ü���ʾ����
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 12:45:50
 * @LastEditors: adol
 * @LastEditTime: 2019-11-22 20:32:19
 */

#include "intrins.h"
#include "core.h"

bit flag_lcd12864_display_ok = 0;
unsigned char lcd12864_display_buffer[4][16] = {"������λ", "xx.xxx xxx.xx", "--ʱ--��--��", "20--��--��--��"};

/**
 * @name: lcd12864_busy_test();
 * @function: ���Һ���Ƿ���æ״̬
 * @param { void } 
 * @return: 0 : Һ��δ����æ״̬,�ɽ��в���; 1 : Һ������æ״̬,���ɽ��в���
 */
unsigned char lcd12864_busy_test(void)
{
    unsigned char temp;
    lcd12864_rs(0);     //RS=0��RW=1, ��Һ����ȡ����
    lcd12864_rw(1);
    lcd12864_en(1);
    lcd12864_delay_ms(1);
    temp = lcd12864_data_port & (0x80);
    lcd12864_en(0);
    return temp;
}

/**
 * @name: send_cmd_to_lcd12864();
 * @function: ��Һ������ָ��
 * @param { unsigned char } cmd -> Ҫ���͵�ָ��
 * @return: none
 */
void send_cmd_to_lcd12864(unsigned char cmd)
{
    while(lcd12864_busy_test());
    lcd12864_rs(0);     //RS=0, RW=0, ��Һ����������
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
 * @function: ��Һ����������
 * @param { unsigned char } dat -> Ҫ���͵�����
 * @return: none
 */
void send_data_to_lcd12864(unsigned char dat)
{
    while(lcd12864_busy_test());
    lcd12864_rs(1);     //RS=1, RW=0, ��Һ����������
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
 * @function: "������ʾģʽ"����ʾ������ת��ָ���к���
 * @param { unsigned char } row    -> ������ʾģʽ��12864Һ�������е�һ��,�����趨��ΧΪ 1~4
 *                          column -> ������ʾģʽ��12864Һ�������е�һ��,�����趨��ΧΪ 1~8
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
 * @function: ������ʾģʽ�� "��Һ��ָ���к�������Ϊ���" ��ʾһ���ַ�,ֻ�ɽ��е�����ʾ
 * @param { unsigned char } row -> ������ʾģʽ��12864Һ�������е�һ��,�����趨��ΧΪ 1~4
 *                          column -> ������ʾģʽ��12864Һ�������е�һ��,�����趨��ΧΪ 1~8
 *                          add -> �ַ����׵�ַ
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
 * @function: ������ʾģʽ�� Һ������
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
�������÷��� -> 
    �������� -> 
         
      ����ֵ -> ��
ʹ�ñ�����ʱ��ο�12864Һ�� "ͼ����ʾ����"
*******************************************************************************/
/**
 * @name: lcd12864_picture_mode_jump_place();
 * @function: "��ͼģʽ"����ʾ������ת��ָ���к���
 * @param { unsigned char } row    -> "��ͼģʽ"��12864Һ�� 64 ���е�һ��,�����趨��ΧΪ 1~64
                            column -> "��ͼģʽ"��12864Һ�� 8  ���е�һ��,�����趨��ΧΪ 1~8
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
 * @function: "��ͼģʽ"�� "��ָ����(row)����(column)Ϊ���" ��ʾ�ֱ���Ϊ width(��) * height(��)�ĵ�ɫλͼ
 * @param { unsigned char } row -> "��ͼģʽ"��12864Һ�� 64 ���е�һ��,�����趨��ΧΪ 1~64
                            column -> "��ͼģʽ"��12864Һ�� 8  ���е�һ��,�����趨��ΧΪ 1~8
                            width -> ��ɫλͼ�Ŀ�ȣ�����Ϊ16������������ֵ��Χ 1~128
                            height -> ��ɫλͼ�ĸ߶ȣ���ֵ��Χ 1~64
                            reverse_flag -> ������ʾ��־λ��0:������ʾ;1:������ʾ
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
 * @function: ��ͼģʽ�� Һ������
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
 * @function: ���õ�Ƭ������Һ�����ƶ˿ڵ�IOΪ���ģʽ
 * @param { void } 
 * @return: none
 */
// void lcd12864_control_port_init(void)
// {
//   #ifdef lcd12864_res_pin
//     lcd12864_res(1);
//   #endif
//   #ifdef lcd12864_psb_pin
//     lcd12864_psb(1);         //ȷ��Һ�����Ʒ�ʽΪ����
//   #endif
// }

/**
 * @name: lcd12864_init();
 * @function: Һ����ʼ�����ã�ʹ����������ǰ���ȵ��ô˺���
 * @param { void } 
 * @return: none
 */
void lcd12864_init(void)
{
    send_cmd_to_lcd12864(0x30); /*ʹ��8λ����ͨѶ*/
    lcd12864_delay_ms(10);
    send_cmd_to_lcd12864(0x30); /*ʹ��8λ����ͨѶ*/
    lcd12864_delay_ms(10);
    send_cmd_to_lcd12864(0x0c); /*��ʾ�����������*/
    lcd12864_delay_ms(10);
    send_cmd_to_lcd12864(0x01); /*��ʾ����*/
    lcd12864_delay_ms(30);
    send_cmd_to_lcd12864(0x06); /*DDRAM�ĵ�ַ��������AC����1*/
    lcd12864_delay_ms(30);
}

/**
 * @name: lcd12864_display();
 * @function: lcd12864��ʾ����
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