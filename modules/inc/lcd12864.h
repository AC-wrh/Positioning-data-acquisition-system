/*
 * @Descripttion: 
 * @version: 
 * @Author: adol
 * @Date: 2019-11-19 12:45:55
 * @LastEditors: adol
 * @LastEditTime: 2019-11-22 20:59:46
 */

#ifndef __LCD12864_H__
#define __LCD12864_H__

/***** macro declaration *****/
#ifndef bit0
#define bit0 0x01
#endif
#ifndef bit1
#define bit1 0x02
#endif
#ifndef bit2
#define bit2 0x04
#endif
#ifndef bit3
#define bit3 0x08
#endif
#ifndef bit4
#define bit4 0x10
#endif
#ifndef bit5
#define bit5 0x20
#endif
#ifndef bit6
#define bit6 0x40
#endif
#ifndef bit7
#define bit7 0x80
#endif

#define lcd12864_control_port     P1
#define lcd12864_data_port        P0

#define lcd12864_rs_pin           bit0
#define lcd12864_rw_pin           bit1
#define lcd12864_en_pin           bit2
#define lcd12864_psb_pin          bit3
#define lcd12864_res_pin          bit4

#define lcd12864_rs(a) if(a) \
                        lcd12864_control_port |= lcd12864_rs_pin; \
                       else  \
                        lcd12864_control_port &= ~lcd12864_rs_pin;
#define lcd12864_rw(a) if(a) \
                        lcd12864_control_port |= lcd12864_rw_pin; \
                       else  \
                        lcd12864_control_port &= ~lcd12864_rw_pin;		
#define lcd12864_en(a) if(a) \
                        lcd12864_control_port |= lcd12864_en_pin; \
                       else  \
                        lcd12864_control_port &= ~lcd12864_en_pin;	
#ifdef lcd12864_psb_pin
#define lcd12864_psb(a) if(a) \
                          lcd12864_control_port |= lcd12864_psb_pin; \
                        else  \
                          lcd12864_control_port &= ~lcd12864_psb_pin;
#endif
#ifdef lcd12864_res_pin
#define lcd12864_res(a) if(a) \
                          lcd12864_control_port |= lcd12864_res_pin; \
                        else  \
                          lcd12864_control_port &= ~lcd12864_res_pin;
#endif

/***** variable declaration *****/
extern bit flag_lcd12864_display_ok;
extern unsigned char lcd12864_display_buffer[4][16];

/***** function declaration *****/
void lcd12864_init(void);
void lcd12864_display(void);

#endif