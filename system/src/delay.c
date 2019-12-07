/*
 * @Descripttion: 延时程序
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
 * @function: lcd12864的延时函数 单位毫秒
 * @param { unsigned char x } x -> 延时时间
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

