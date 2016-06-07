/*******************************************************************************
* Copyright 2011 Broadcom Corporation.	All rights reserved.
*
* @file drivers/video/broadcom/lcd/ILI9806.h
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/

#ifndef __ILI9806_H__
#define __ILI9806_H__

#include "display_drv.h"
#include "lcd.h"

#define ILI9806_CMD_SLPIN	0x10
#define ILI9806_CMD_SLPOUT	0x11
#define ILI9806_CMD_DISPOFF	0x28
#define ILI9806_CMD_DISPON	0x29
#define ILI9806_CMD_DSTBON	0x4F

#define ILI9806_UPDT_WIN_SEQ_LEN 13 /* (6 + 6 + 1) */

__initdata struct DSI_COUNTER ili9806_timing[] = {
	/* LP Data Symbol Rate Calc - MUST BE FIRST RECORD */
	{"ESC2LP_RATIO", DSI_C_TIME_ESC2LPDT, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0x0000003F, 1, 1, 0},
	/* SPEC:  min =  100[us] + 0[UI] */
	/* SET:   min = 1000[us] + 0[UI]                             <= */
	{"HS_INIT", DSI_C_TIME_HS, 0,
		0, 100000, 0, 0, 0, 0, 0, 0, 0x00FFFFFF, 0, 0, 0},
	/* SPEC:  min = 1[ms] + 0[UI] */
	/* SET:   min = 1[ms] + 0[UI] */
	{"HS_WAKEUP", DSI_C_TIME_HS, 0,
		0, 1000000, 0, 0, 0, 0, 0, 0, 0x00FFFFFF, 0, 0, 0},
	/* SPEC:  min = 1[ms] + 0[UI] */
	/* SET:   min = 1[ms] + 0[UI] */
	{"LP_WAKEUP", DSI_C_TIME_ESC, 0,
		0, 1000000, 0, 0, 0, 0, 0, 0, 0x00FFFFFF, 1, 1, 0},
	/* SPEC:  min = 0[ns] +  8[UI] */
	/* SET:   min = 0[ns] + 12[UI]                               <= */
	{"HS_CLK_PRE", DSI_C_TIME_HS, 0,
		0, 10, 8, 0, 0, 0, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min = 38[ns] + 0[UI]   max= 95[ns] + 0[UI] */
	/* SET:   min = 68[ns] + 0[UI]   max= 95[ns] + 0[UI]         <= */
	{"HS_CLK_PREPARE", DSI_C_TIME_HS, DSI_C_HAS_MAX,
		0, 75, 0, 0, 0, 95, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min = 262[ns] + 0[UI] */
	/* SET:   min = 314[ns] + 0[UI]                              <= */
	{"HS_CLK_ZERO", DSI_C_TIME_HS, 0,
		0, 300, 0, 0, 0, 0, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min =  60[ns] + 52[UI] */
	/* SET:   min =  72[ns] + 52[UI]                             <= */
	{"HS_CLK_POST", DSI_C_TIME_HS, 0,
		0, 70, 128, 0, 0, 0, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min =  60[ns] + 0[UI] */
	/* SET:   min =  72[ns] + 0[UI]                              <= */
	{"HS_CLK_TRAIL", DSI_C_TIME_HS, 0,
		0, 300, 0, 0, 0, 0, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min =  50[ns] + 0[UI] */
	/* SET:   min =  60[ns] + 0[UI]                              <= */
	{"HS_LPX", DSI_C_TIME_HS, 0,
		0, 70, 0, 0, 0, 75, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min = 40[ns] + 4[UI]      max= 85[ns] + 6[UI] */
	/* SET:   min = 60[ns] + 4[UI]      max= 85[ns] + 6[UI]      <= */
	{"HS_PRE", DSI_C_TIME_HS, DSI_C_HAS_MAX,
		0, 60, 4, 0, 0, 85, 6, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min = 105[ns] + 6[UI] */
	/* SET:   min = 125[ns] + 6[UI]                              <= */
	{"HS_ZERO", DSI_C_TIME_HS, 0,
		0, 280, 6, 0, 0, 0, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min = max(0[ns]+32[UI],60[ns]+16[UI])  n=4 */
	/* SET:   min = max(0[ns]+32[UI],60[ns]+16[UI])  n=4 */
	{"HS_TRAIL", DSI_C_TIME_HS, DSI_C_MIN_MAX_OF_2,
		0, 60, 32, 100, 16, 0, 0, 0, 0x000001FF, 0, 0, 0},
	/* SPEC:  min = 100[ns] + 0[UI] */
	/* SET:   min = 120[ns] + 0[UI]                              <= */
	{"HS_EXIT", DSI_C_TIME_HS, 0,
		0, 300, 0, 0, 0, 0, 0, 0, 0x000001FF, 0, 0, 0},
	/* min = 50[ns] + 0[UI] */
	/* LP esc counters are speced in LP LPX units.
	   LP_LPX is calculated by chal_dsi_set_timing
	   and equals LP data clock */
	{"LPX", DSI_C_TIME_ESC, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 0x000000FF, 1, 1, 0},
	/* min = 4*[Tlpx]  max = 4[Tlpx], set to 4 */
	{"LP-TA-GO", DSI_C_TIME_ESC, 0,
		4, 0, 0, 0, 0, 0, 0, 0, 0x000000FF, 1, 1, 0},
	/* min = 1*[Tlpx]  max = 2[Tlpx], set to 2 */
	{"LP-TA-SURE", DSI_C_TIME_ESC, 0,
		2, 0, 0, 0, 0, 0, 0, 0, 0x000000FF, 1, 1, 0},
	/* min = 5*[Tlpx]  max = 5[Tlpx], set to 5 */
	{"LP-TA-GET", DSI_C_TIME_ESC, 0,
		5, 0, 0, 0, 0, 0, 0, 0, 0x000000FF, 1, 1, 0},
};


__initdata DISPCTRL_REC_T ili9806_scrn_on[] = {
	{DISPCTRL_WR_CMND, 0x29},
	{DISPCTRL_LIST_END, 0}
};

__initdata DISPCTRL_REC_T ili9806_scrn_off[] = {
	{DISPCTRL_WR_CMND, 0x28},
	{DISPCTRL_LIST_END, 0}
};

__initdata DISPCTRL_REC_T ili9806_slp_in[] = {
	{DISPCTRL_WR_CMND, 0x28},
	{DISPCTRL_SLEEP_MS, 20},
	{DISPCTRL_WR_CMND, 0x10},
	{DISPCTRL_SLEEP_MS, 120},
	{DISPCTRL_LIST_END, 0}
};

__initdata DISPCTRL_REC_T ili9806_slp_out[] = {
	{DISPCTRL_WR_CMND, 0x11},
	{DISPCTRL_SLEEP_MS, 120},
	{DISPCTRL_WR_CMND, 0x29},
	{DISPCTRL_SLEEP_MS, 10},
	{DISPCTRL_LIST_END, 0}
};

__initdata DISPCTRL_REC_T ili9806_init_panel_cmd[] = {

	{DISPCTRL_WR_CMND, 0xFF},//Set extc
	{DISPCTRL_WR_DATA, 0xFF},
	{DISPCTRL_WR_DATA, 0x98},
	{DISPCTRL_WR_DATA, 0x06},	
	
	{DISPCTRL_WR_CMND, 0xBA},//Interface setting
	{DISPCTRL_WR_DATA, 0x60},	

	{DISPCTRL_WR_CMND, 0xBC},//GIP1
	{DISPCTRL_WR_DATA, 0x01},
	{DISPCTRL_WR_DATA, 0x12},
	{DISPCTRL_WR_DATA, 0x61},
	{DISPCTRL_WR_DATA, 0xFF},
	{DISPCTRL_WR_DATA, 0x10},
	{DISPCTRL_WR_DATA, 0x10},
	{DISPCTRL_WR_DATA, 0x0B},
	{DISPCTRL_WR_DATA, 0x13},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x73},
	
	{DISPCTRL_WR_DATA, 0xFF},
	{DISPCTRL_WR_DATA, 0xFF},
	{DISPCTRL_WR_DATA, 0x0E},
	{DISPCTRL_WR_DATA, 0x0E},
	{DISPCTRL_WR_DATA, 0x00},	
	{DISPCTRL_WR_DATA, 0x03},
	{DISPCTRL_WR_DATA, 0x66},
	{DISPCTRL_WR_DATA, 0x63},
	{DISPCTRL_WR_DATA, 0x01},
	{DISPCTRL_WR_DATA, 0x00},	
	
	{DISPCTRL_WR_DATA, 0x00},	
	
	{DISPCTRL_WR_CMND, 0xBD},//GIP2
	{DISPCTRL_WR_DATA, 0x01},
	{DISPCTRL_WR_DATA, 0x23},
	{DISPCTRL_WR_DATA, 0x45},
	{DISPCTRL_WR_DATA, 0x67},
	{DISPCTRL_WR_DATA, 0x01},
	{DISPCTRL_WR_DATA, 0x23},
	{DISPCTRL_WR_DATA, 0x45},
	{DISPCTRL_WR_DATA, 0x67},

	{DISPCTRL_WR_CMND, 0xBE},//GIP3
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x21},
	{DISPCTRL_WR_DATA, 0xAB},
	{DISPCTRL_WR_DATA, 0x60},
	{DISPCTRL_WR_DATA, 0x22},
	{DISPCTRL_WR_DATA, 0x22},
	{DISPCTRL_WR_DATA, 0x22},
	{DISPCTRL_WR_DATA, 0x22},	
	{DISPCTRL_WR_DATA, 0x22},		
	
	{DISPCTRL_SLEEP_MS, 10},	
	
	{DISPCTRL_WR_CMND, 0xED},//Voltage_EN
	{DISPCTRL_WR_DATA, 0x7F},
	{DISPCTRL_WR_DATA, 0x0F},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_CMND, 0xB6},//Display setting
	{DISPCTRL_WR_DATA, 0x02},	
	
	{DISPCTRL_WR_CMND, 0x3A},//Format setting
	{DISPCTRL_WR_DATA, 0x77},

	{DISPCTRL_WR_CMND, 0xB5},//Porch setting
	{DISPCTRL_WR_DATA, 0x3E},
	{DISPCTRL_WR_DATA, 0x18},

	{DISPCTRL_WR_CMND, 0xC0},//Power setting
	{DISPCTRL_WR_DATA, 0xAB},
	{DISPCTRL_WR_DATA, 0x0B},
	{DISPCTRL_WR_DATA, 0x0A},

	{DISPCTRL_WR_CMND, 0xFC},//VGL setting
	{DISPCTRL_WR_DATA, 0x09},	
	
	{DISPCTRL_WR_CMND, 0xDF},//MIPI setting
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},	
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x20},		
	
	{DISPCTRL_WR_CMND, 0xF3},//DVDD setting
	{DISPCTRL_WR_DATA, 0x74},

	{DISPCTRL_WR_CMND, 0xB4},//Column inversion
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_CMND, 0xF7},//480X800
	{DISPCTRL_WR_DATA, 0x82},

	{DISPCTRL_WR_CMND, 0xB1},//Frequence
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x12},
	{DISPCTRL_WR_DATA, 0x13},

	{DISPCTRL_WR_CMND, 0xF2},//Panel timing control
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x59},
	{DISPCTRL_WR_DATA, 0x40},
	{DISPCTRL_WR_DATA, 0x28},

	{DISPCTRL_WR_CMND, 0xC1},//Power control 2
	{DISPCTRL_WR_DATA, 0x07},
	{DISPCTRL_WR_DATA, 0x61},
	{DISPCTRL_WR_DATA, 0x61},
	{DISPCTRL_WR_DATA, 0x20},

	{DISPCTRL_WR_CMND, 0xEA},//3-Gamma
	{DISPCTRL_WR_DATA, 0x03},

	{DISPCTRL_WR_CMND, 0x35},//TE efect
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_CMND, 0x36},//Backward scan
	{DISPCTRL_WR_DATA, 0x03},//0x0B

	{DISPCTRL_WR_CMND, 0xE0},//Gamma 1
	{DISPCTRL_WR_DATA, 0x04},
	{DISPCTRL_WR_DATA, 0x04},
	{DISPCTRL_WR_DATA, 0x0A},
	{DISPCTRL_WR_DATA, 0x08},
	{DISPCTRL_WR_DATA, 0x01},
	{DISPCTRL_WR_DATA, 0x04},
	{DISPCTRL_WR_DATA, 0x05},
	{DISPCTRL_WR_DATA, 0x02},
	{DISPCTRL_WR_DATA, 0x09},
	{DISPCTRL_WR_DATA, 0x0C},
	
	{DISPCTRL_WR_DATA, 0x16},
	{DISPCTRL_WR_DATA, 0x05},
	{DISPCTRL_WR_DATA, 0x0A},
	{DISPCTRL_WR_DATA, 0x13},
	{DISPCTRL_WR_DATA, 0x13},	
	{DISPCTRL_WR_DATA, 0x00},
	
	{DISPCTRL_WR_CMND, 0xE1},//Gamma 2
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x04},
	{DISPCTRL_WR_DATA, 0x09},
	{DISPCTRL_WR_DATA, 0x09},
	{DISPCTRL_WR_DATA, 0x01},
	{DISPCTRL_WR_DATA, 0x03},
	{DISPCTRL_WR_DATA, 0x05},
	{DISPCTRL_WR_DATA, 0x03},
	{DISPCTRL_WR_DATA, 0x0A},
	{DISPCTRL_WR_DATA, 0x0D},
	
	{DISPCTRL_WR_DATA, 0x17},
	{DISPCTRL_WR_DATA, 0x05},
	{DISPCTRL_WR_DATA, 0x0A},
	{DISPCTRL_WR_DATA, 0x13},
	{DISPCTRL_WR_DATA, 0x11},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_CMND, 0xE2},//Gamma 3
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x99},
	{DISPCTRL_WR_DATA, 0x99},
	{DISPCTRL_WR_DATA, 0x99},
	{DISPCTRL_WR_DATA, 0x99},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x99},

	{DISPCTRL_WR_DATA, 0x99},
	{DISPCTRL_WR_DATA, 0x99},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},


	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_CMND, 0xF9},//Gamma 3
	{DISPCTRL_WR_DATA, 0x80},

	{DISPCTRL_WR_CMND, 0xE3},//Gamma 3
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x76},

	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x43},
	{DISPCTRL_WR_DATA, 0x43},
	{DISPCTRL_WR_DATA, 0x43},
	{DISPCTRL_WR_DATA, 0x43},
	{DISPCTRL_WR_DATA, 0x43},
	{DISPCTRL_WR_DATA, 0x43},

	{DISPCTRL_WR_CMND, 0xE3},//Gamma 3
	{DISPCTRL_WR_DATA, 0x43},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x21},
	{DISPCTRL_WR_DATA, 0x21},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},

	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x76},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x87},
	{DISPCTRL_WR_DATA, 0x21},
	{DISPCTRL_WR_DATA, 0x21},

	{DISPCTRL_WR_DATA, 0x21},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_CMND, 0xE3},//Gamma 3
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_CMND, 0xE3},//Gamma 3
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},

	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x32},
	{DISPCTRL_WR_DATA, 0x00},

	{DISPCTRL_WR_CMND, 0xF9},//Gamma 3
	{DISPCTRL_WR_DATA, 0x00},

	//{DISPCTRL_WR_CMND, 0xC8}, //Temporary BL control on lcd initialization
	//{DISPCTRL_WR_DATA, 0x0F},

	{DISPCTRL_WR_CMND, 0x11},//Sleep out

	{DISPCTRL_SLEEP_MS, 120},

	{DISPCTRL_WR_CMND, 0x29},//Display on

	{DISPCTRL_SLEEP_MS, 70},	
	
	{DISPCTRL_LIST_END, 0}

	};

void ili9806_winset(char *msgData, DISPDRV_WIN_t *p_win)
{
	int i = 0;
	msgData[i++] = 5; /* Length of the sequence below */
	msgData[i++] = MIPI_DCS_SET_COLUMN_ADDRESS;
	msgData[i++] = (p_win->l & 0xFF00) >> 8;
	msgData[i++] = (p_win->l & 0x00FF);
	msgData[i++] = (p_win->r & 0xFF00) >> 8;
	msgData[i++] = (p_win->r & 0x00FF);

	msgData[i++] = 5; /* Length of the sequence below */
	msgData[i++] = MIPI_DCS_SET_PAGE_ADDRESS;
	msgData[i++] = (p_win->t & 0xFF00) >> 8;
	msgData[i++] = (p_win->t & 0x00FF);
	msgData[i++] = (p_win->b & 0xFF00) >> 8;
	msgData[i++] = (p_win->b & 0x00FF);
	msgData[i++] = 0;

	if (i != ILI9806_UPDT_WIN_SEQ_LEN)
		pr_err("ILI9806_winset msg len incorrect!\n");
}

__initdata struct lcd_config ili9806_cfg = {
	.name = "ILI9806",
	.mode_supp = LCD_CMD_ONLY,
	.phy_timing = &ili9806_timing[0],
	.max_lanes = 2,
	.max_hs_bps = 550000000,
	.max_lp_bps = 5000000,
	.phys_width = 52,
	.phys_height = 87,
	.init_cmd_seq = &ili9806_init_panel_cmd[0],
	.init_vid_seq = NULL,
	.slp_in_seq = &ili9806_slp_in[0],
	.slp_out_seq = &ili9806_slp_out[0],
	.scrn_on_seq = &ili9806_scrn_on[0],
	.scrn_off_seq = &ili9806_scrn_off[0],
	.id_seq = NULL,
	.verify_id = false,
	.updt_win_fn = ili9806_winset,
	.updt_win_seq_len = ILI9806_UPDT_WIN_SEQ_LEN,
	.vid_cmnds = false,
	.vburst = false,
	.cont_clk = false,
	.hs = 0,
	.hbp = 0,
	.hfp = 0,
	.vs = 0,
	.vbp = 0,
	.vfp = 0,
};

#endif
