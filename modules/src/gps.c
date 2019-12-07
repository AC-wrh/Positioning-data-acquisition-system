/*
 * @Descripttion: GPS&BPS的数据处理
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 12:45:39
 * @LastEditors: adol
 * @LastEditTime: 2019-12-07 22:42:55
 */

#include "intrins.h"
#include "core.h"

bit flag_date_ov = 0;    //日期进位标志位
unsigned char hour = 0, hour_high = 0, hour_low = 0;
unsigned char year = 0,month = 0, day = 0, year_high = 0, year_low = 0, month_high = 0, month_low = 0, day_high = 0, day_low = 0;
unsigned char max_day = 0;
unsigned char xdata gprmc_buffer[70];   //GPS+BPS的定位数据

/**
 * @name: leap_year_detection();
 * @function: 检测传入年份是否为闰年
 * @param { unsigned int } years -> 现今年份
 * @return: 1 : 闰年; 0 : 平年
 */
bit leap_year_detection(unsigned int years)
{
    return (((years % 4) == 0) && ((years % 100) != 0)) || ((years % 400) == 0);
}

/**
 * @name: unsigned char get_max_day();
 * @function: 获取当月日期最大值
 * @param { unsigned char } month_value -> 现今月份
 *        { unsigned int  } year_value  -> 现今年份
 * @return: 返回当月日期最大值(max_day)
 */
unsigned char get_max_day(unsigned char month_value, unsigned int year_value)
{
    unsigned char days;
    switch(month_value)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        {
            days = 31;
        }
        break;

        case 2:
        {
            //2月份比较特殊，需要根据是不是闰年来判断当月是28天还是29天
            days = leap_year_detection(year_value)? 29 : 28;
        }
        break;

        case 4:
        case 6:
        case 9:
        case 11:
        {
            days = 30;
        }
        break;
        
        default : break;
    }
    return(days);
}

/*
GNRMC语句的基本格式：
$GNRMC,(1),(2),(3),(4),(5),(6),(7),(8),(9),(10),(11),(12)*hh(CR)(LF)
(1) UTC时间， hhmmsshhmmss （时分秒）
(2) 定位状态， A= 有效定位， V= 无效定位
(3) 纬度 ddmm.mmm （度分）
(4) 纬度半球 N（北半球）或 S（南半球）
(5) 经度 dddmm.mm （度分）
(6) 经度半球 E（东经）或 W（西经）
(7) 地面速率（000.0~999.9 节）
(8) 地面航向（000.0~359.9 度，以真北方为参考基准）
(9) UTC日期，ddmmyy（日月年）
(10) 磁偏角（000.0~180.0度，前导位数不足则补0）
(11) 磁偏角方向，E（东）或 W（西）
(12) 模式指示（ A= 自主定位， D= 差分， E= 估算， N= 数据无效）
*/
/**
 * @name: get_position_data();
 * @function: 对GPS+BPS的GNRMC定位数据进行处理
 * @param { void } 
 * @return: none
 */
void get_position_data(void)
{
    unsigned char i;

    if(flag_gps_data_ok) //确定是否收到"GPRMC"这一帧数据
    {
        for(i = 0; i < 70 ; i++)
        {
            gprmc_buffer[i] = RX_buffer[i];
        }
        
        /***** 时分秒 *****/
        hour = (gprmc_buffer[7] - 0x30) * 10 + (gprmc_buffer[8] - 0x30) + 8;  //将UTC时间转换到北京时间 即UTC+8
        if( hour >= 24 )    
        {
            hour %= 24;         //获取当前Hour
            flag_date_ov = 1;   //日期进位
        }
        else
        {
            flag_date_ov = 0;
        }

        hour_high = hour / 10 + 0x30;
        hour_low  = hour % 10 + 0x30;

        lcd12864_display_buffer[2][0] = hour_high;          //hour_high
        lcd12864_display_buffer[2][1] = hour_low;           //hour_low
        lcd12864_display_buffer[2][4] = gprmc_buffer[9];    //min_high
        lcd12864_display_buffer[2][5] = gprmc_buffer[10];   //min_low
        lcd12864_display_buffer[2][8] = gprmc_buffer[11];   //sec_high
        lcd12864_display_buffer[2][9] = gprmc_buffer[12];   //sec_low    
        
        /***** 经纬度 *****/
        lcd12864_display_buffer[1][0] = gprmc_buffer[20];   //纬度
        lcd12864_display_buffer[1][1] = gprmc_buffer[21];
        lcd12864_display_buffer[1][3] = gprmc_buffer[22];
        lcd12864_display_buffer[1][4] = gprmc_buffer[23];
        lcd12864_display_buffer[1][5] = gprmc_buffer[30];   // N || S

        lcd12864_display_buffer[1][7]  = gprmc_buffer[32];  //经度
        lcd12864_display_buffer[1][8]  = gprmc_buffer[33];
        lcd12864_display_buffer[1][9]  = gprmc_buffer[34];
        lcd12864_display_buffer[1][11] = gprmc_buffer[35];
        lcd12864_display_buffer[1][12] = gprmc_buffer[36];
        lcd12864_display_buffer[1][13] = gprmc_buffer[43];  // E || W

        /***** 年月日 *****/
        day_high   = gprmc_buffer[57];
        day_low    = gprmc_buffer[58];
        month_high = gprmc_buffer[59];
        month_low  = gprmc_buffer[60];
        year_high  = gprmc_buffer[61];
        year_low   = gprmc_buffer[62];

        if (flag_date_ov == 1)
        {
            day = (day_high - 0x30) * 10 + (day_low - 0x30) + 1;
        }
        else
        {
            day = (day_high - 0x30) * 10 + (day_low - 0x30);
        }

        month = (month_high - 0x30) * 10 + (month_low - 0x30);
        year = (year_high - 0x30) * 10 + (year_low - 0x30);
        
        max_day = get_max_day(month, (2000 + year)); //获取当月天数最大值
        if(day > max_day)
        {
            day %= max_day;
            month += 1;
            if(month > 12)
            {
                month %= 12;
                year += 1;
            }
        }
        
        year_high = year / 10 + 0x30;
        year_low = year % 10 + 0x30;
        month_high = month / 10 + 0x30;
        month_low = month % 10 + 0x30;
        day_high =  day / 10 + 0x30;
        day_low =  day % 10 + 0x30;

        lcd12864_display_buffer[3][2]  = year_high;
        lcd12864_display_buffer[3][3]  = year_low;
        lcd12864_display_buffer[3][6]  = month_high;
        lcd12864_display_buffer[3][7]  = month_low;
        lcd12864_display_buffer[3][10] = day_high;
        lcd12864_display_buffer[3][11] = day_low;

        /***** code *****/

        // lcd12864_display_buffer[3][2]  = gprmc_buffer[61];
        // lcd12864_display_buffer[3][3]  = gprmc_buffer[62];
        // lcd12864_display_buffer[3][6]  = gprmc_buffer[59];
        // lcd12864_display_buffer[3][7]  = gprmc_buffer[60];
        // lcd12864_display_buffer[3][10] = gprmc_buffer[57];
        // lcd12864_display_buffer[3][11] = gprmc_buffer[58];
        
        /***** code *****/
        flag_lcd12864_display_ok = 0;
    }
}
