/*
 * @Descripttion: 主函数
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 12:44:51
 * @LastEditors: adol
 * @LastEditTime: 2019-11-22 21:02:40
 */

#include "core.h"

/**
 * @name: main();
 * @function: 主程序入口
 * @param { void }
 * @return: none
 */
void main(void)
{
    uart_init();
    lcd12864_init();
    while(1)
    {
        get_position_data();
        lcd12864_display();
        if (flag_gps_data_ok == 0 && flag_lcd12864_display_ok == 1)
        {
            uart_send_string(lcd12864_display_buffer[0]);
            uart_send_string(lcd12864_display_buffer[1]);
            uart_send_string(lcd12864_display_buffer[2]);
            uart_send_string(lcd12864_display_buffer[3]);
        }
    }
}