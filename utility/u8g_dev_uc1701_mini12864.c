/*

  u8g_dev_uc1701_mini12864.c (dealextreme)

  Universal 8bit Graphics Library
  
  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  
*/

#include "u8g.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

static const uint8_t u8g_dev_uc1701_mini12864_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip */

  0x040,		/* set display start line to 0 */
  0x0a0,		/* ADC set to reverse */
  0x0c0,		/* common output mode  c8 change to c0*/
  0x0a6,		/* display normal, bit val 0: LCD pixel off. */
  0x0a2,		/* LCD bias 1/9 */
  0x02f,		/* all power  control circuits on */
  0x0f8,		/* set booster ratio to */
  0x000,		/* 4x */
  0x023,		/* set V0 voltage resistor ratio to large */
  0x081,		/* set contrast */
  0x027,		/* contrast value */
  0x0ac,		/* indicator */
  0x000,		/* disable */
  0x0af,		/* display on */

  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0a5,		                /* display all points, ST7565 */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0a4,		                /* normal display */
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_uc1701_mini12864_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x010,		/* set upper 4 bit of the col adr to 0 */
  0x000,		/* set lower 4 bit of the col adr to 4  */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_uc1701_mini12864_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_data_start);    
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 2);
      u8g_SetChipSelect(u8g, dev, 0);      
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_uc1701_mini12864_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1701_mini12864_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_uc1701_mini12864_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1701_mini12864_fn, U8G_COM_HW_SPI);
