/*
 * @Descripttion: ��ʱ����
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 13:04:05
 * @LastEditors: adol
 * @LastEditTime: 2019-11-22 20:34:06
 */

#include "intrins.h"
#include "core.h"

/**
 * @name: lcd12864_delay_ms();
 * @function: lcd12864����ʱ���� ��λ����
 * @param { unsigned char x } x -> ��ʱʱ��
 * @return: none
 */
void lcd12864_delay_ms(unsigned char x)
{
    unsigned char i, j;

    while (x--)
    {
        _nop_();
        i = 11;
        j = 190;

        do
        {
            while (--j);
        } while (--i);
    }
}

