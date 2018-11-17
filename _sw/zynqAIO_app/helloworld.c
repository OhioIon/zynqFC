/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "dshot.h"
#include "dshotWaveform.h"

dshot_ts dshot_s;
dshotWaveform_ts dshotWaveform_s;

int main()
{
    // Init PS already done by FSBL
    //ps7_init();

    // Init PS peripherals
    init_platform();

    // Init PL (check if programmed, clock enabled, reset released)
    // TODO

    // Init PL peripherals
    dshotWaveform_s.prm_s.addrBas_pv     = (void*)XPAR_DSHOTWAVEFORM_0_0;
    dshotWaveform_s.prm_s.speed_kbps_u16 = 600; // DShot600
    dshotWaveform_init( &dshotWaveform_s );

    dshot_s.prm_s.addrBas_pv = (void*)XPAR_DSHOT_0_0;
    dshot_init( &dshot_s );

    dshot_s.inp_s.rawData_u16  = 0U;
    dshot_s.inp_s.flgReqTel_u8 = 0U;
    dshot( &dshot_s );

    print("Hello World\n\r");

    cleanup_platform();
    return 0;
}
