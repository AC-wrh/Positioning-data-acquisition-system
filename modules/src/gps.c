/*
 * @Descripttion: GPS&BPS�����ݴ���
 * @version: 1.0
 * @Author: adol
 * @Date: 2019-11-19 12:45:39
 * @LastEditors: adol
 * @LastEditTime: 2019-12-07 22:42:55
 */

#include "intrins.h"
#include "core.h"

bit flag_date_ov = 0;    //���ڽ�λ��־λ
unsigned char hour = 0, hour_high = 0, hour_low = 0;
unsigned char year = 0,month = 0, day = 0, year_high = 0, year_low = 0, month_high = 0, month_low = 0, day_high = 0, day_low = 0;
unsigned char max_day = 0;
unsigned char xdata gprmc_buffer[70];   //GPS+BPS�Ķ�λ����

/**
 * @name: leap_year_detection();
 * @function: ��⴫������Ƿ�Ϊ����
 * @param { unsigned int } years -> �ֽ����
 * @return: 1 : ����; 0 : ƽ��
 */
bit leap_year_detection(unsigned int years)
{
    return (((years % 4) == 0) && ((years % 100) != 0)) || ((years % 400) == 0);
}

/**
 * @name: unsigned char get_max_day();
 * @function: ��ȡ�����������ֵ
 * @param { unsigned char } month_value -> �ֽ��·�
 *        { unsigned int  } year_value  -> �ֽ����
 * @return: ���ص����������ֵ(max_day)
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
            //2�·ݱȽ����⣬��Ҫ�����ǲ����������жϵ�����28�컹��29��
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
GNRMC���Ļ�����ʽ��
$GNRMC,(1),(2),(3),(4),(5),(6),(7),(8),(9),(10),(11),(12)*hh(CR)(LF)
(1) UTCʱ�䣬 hhmmsshhmmss ��ʱ���룩
(2) ��λ״̬�� A= ��Ч��λ�� V= ��Ч��λ
(3) γ�� ddmm.mmm ���ȷ֣�
(4) γ�Ȱ��� N�������򣩻� S���ϰ���
(5) ���� dddmm.mm ���ȷ֣�
(6) ���Ȱ��� E���������� W��������
(7) �������ʣ�000.0~999.9 �ڣ�
(8) ���溽��000.0~359.9 �ȣ����汱��Ϊ�ο���׼��
(9) UTC���ڣ�ddmmyy�������꣩
(10) ��ƫ�ǣ�000.0~180.0�ȣ�ǰ��λ��������0��
(11) ��ƫ�Ƿ���E�������� W������
(12) ģʽָʾ�� A= ������λ�� D= ��֣� E= ���㣬 N= ������Ч��
*/
/**
 * @name: get_position_data();
 * @function: ��GPS+BPS��GNRMC��λ���ݽ��д���
 * @param { void } 
 * @return: none
 */
void get_position_data(void)
{
    unsigned char i;

    if(flag_gps_data_ok) //ȷ���Ƿ��յ�"GPRMC"��һ֡����
    {
        for(i = 0; i < 70 ; i++)
        {
            gprmc_buffer[i] = RX_buffer[i];
        }
        
        /***** ʱ���� *****/
        hour = (gprmc_buffer[7] - 0x30) * 10 + (gprmc_buffer[8] - 0x30) + 8;  //��UTCʱ��ת��������ʱ�� ��UTC+8
        if( hour >= 24 )    
        {
            hour %= 24;         //��ȡ��ǰHour
            flag_date_ov = 1;   //���ڽ�λ
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
        
        /***** ��γ�� *****/
        lcd12864_display_buffer[1][0] = gprmc_buffer[20];   //γ��
        lcd12864_display_buffer[1][1] = gprmc_buffer[21];
        lcd12864_display_buffer[1][3] = gprmc_buffer[22];
        lcd12864_display_buffer[1][4] = gprmc_buffer[23];
        lcd12864_display_buffer[1][5] = gprmc_buffer[30];   // N || S

        lcd12864_display_buffer[1][7]  = gprmc_buffer[32];  //����
        lcd12864_display_buffer[1][8]  = gprmc_buffer[33];
        lcd12864_display_buffer[1][9]  = gprmc_buffer[34];
        lcd12864_display_buffer[1][11] = gprmc_buffer[35];
        lcd12864_display_buffer[1][12] = gprmc_buffer[36];
        lcd12864_display_buffer[1][13] = gprmc_buffer[43];  // E || W

        /***** ������ *****/
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
        
        max_day = get_max_day(month, (2000 + year)); //��ȡ�����������ֵ
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
