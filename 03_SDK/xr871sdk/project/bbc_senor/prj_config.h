/*
 * Copyright (C) 2017 XRADIO TECHNOLOGY CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of XRADIO TECHNOLOGY CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PRJ_CONFIG_H_
#define _PRJ_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * project base config
 */
#define PRJCONF_MSP_STACK_SIZE          (1 * 1024)  /* stack size for IRQ service */

/* main thread */
#define PRJCONF_MAIN_THREAD_PRIO        OS_THREAD_PRIO_APP
#define PRJCONF_MAIN_THREAD_STACK_SIZE  (5 * 1024)

/* image */
#define PRJCONF_IMG_BOOT_OFFSET         (0x00000000)
#define PRJCONF_IMG_BOOT_CFG_OFFSET     (PRJCONF_IMG_BOOT_OFFSET + (1 << 20))
#define PRJCONF_IMG_OFFSET_1ST          PRJCONF_IMG_BOOT_OFFSET
#define PRJCONF_IMG_OFFSET_2ND          (PRJCONF_IMG_BOOT_OFFSET + (1 << 20))

/*
 * project hardware feature (enable/disable)
 */
#define PRJCONF_UART_EN                 1 /* uart */
#define PRJCONF_EFUSE_EN                0 /* efuse */
#define PRJCONF_CE_EN                   0 /* h/w crypto engine */
#define PRJCONF_MMC_EN                  0 /* mmc */
#define PRJCONF_MMC_DETECT_MODE         CARD_ALWAYS_PRESENT /* mmc detect mode */
#define PRJCONF_SOUNDCARD0_EN           0 /* sound card0, external audio codec */
#define PRJCONF_SOUNDCARD1_EN           0 /* sound card1, internal dmic */

/*
 * project service feature
 */
#define PRJCONF_CONSOLE_EN              1 /* console */
#define PRJCONF_PM_MODE                 (PM_SUPPORT_SLEEP | PM_SUPPORT_STANDBY | PM_SUPPORT_POWEROFF) /* app pm mode */
#define PRJCONF_XIP_INIT_EARLIEST       0 /* init xip earliest, before bring up net */

/* network */
#define PRJCONF_NET_EN                  1 /* network and wlan */
#define PRJCONF_NET_ONOFF_PM_MODE       (PM_SUPPORT_HIBERNATION | PM_SUPPORT_POWEROFF) /* net pm mode */

#ifdef __cplusplus
}
#endif

#endif /* _PRJ_CONFIG_H_ */
