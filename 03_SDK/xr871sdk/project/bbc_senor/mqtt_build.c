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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../net/mqtt/MQTTClient-C/MQTTClient.h"

#include "mqtt_build.h"
#include "cjson_analy.h"
#include "../include/net/mbedtls/sha1.h"
#include "../src/bbc/devguid_get.h"
#include "../include/driver/chip/hal_norflash.h"

mqt_dev MqttDevice = {

	.ProdectKey = "95",
	.Licesence  = "c1c1f7408965eb52f21219e8b6b56e16093ebf9d3f6d97e8b05dccdaedb91ef0cc27b1e28b5d8847",
	.SdkVersion = "0.8.3",
	//.address	= "112.74.160.43",
	//.port		= 1884,
	.address 	= "mqtt.bigbigon.com",
	.port		= 1883,
};

unsigned char BbcSubGet[mqtt_buff_size] = {0};
unsigned char BbcPubSet[mqtt_buff_size] = {0};
char devguid_get[40] = {0};

unsigned char MessageArriveFlag = 0;
mqt_cal cal_set;

void messageArrived(MessageData* data)
{
	printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data,
		data->message->payloadlen, (char *)data->message->payload);

	memcpy(BbcSubGet,(char *)data->message->payload,data->message->payloadlen);
	//printf("BbcSubGet =  %s\r\n",BbcSubGet);
	MessageArriveFlag = MES_ARVE;
}

char* get_devguid_from_flash()
{
	SF_Config flash_config;
	SF_Handler hdl;

	flash_config.spi = SPI0;
	flash_config.cs = SPI_TCTRL_SS_SEL_SS0;
	flash_config.dma_en = 1;
	flash_config.sclk = 12 * 1000 * 1000;

	HAL_SF_Init(&hdl, &flash_config);
	//HAL_SF_Erase(&hdl, SF_ERASE_SIZE_32KB, devguid_in_flash, 1);
	HAL_SF_Read(&hdl, devguid_in_flash, devguid_get, 40);

	return devguid_get;
}

#define MQTT_THREAD_STACK_SIZE	(1024 * 6)
OS_Thread_t MQTT_ctrl_task_thread;
char MQTT_ctrl = 0;

unsigned char sendbuf[350] = {0}, readbuf[350] = {0};
void mqtt_work_set()
{
	Client client;
	Network network;
	int rc = 0;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
	char str_pwd[160] = {0};
	char pwd_sha1_out[64] = {0};
	char pwd_to_hex[64] = {0};
	char str_clientID[126] = {0};
	int pwd_leng = 0;

	MqttDevice.DevGuid = get_devguid_from_flash();

	char sub_topic[50] = {0};
	char pub_topic[50] = {0};
	sprintf(sub_topic, "p2p/%s/cmd",MqttDevice.DevGuid);
	sprintf(pub_topic, "p2p/%s/ntf",MqttDevice.DevGuid);
	printf("sub_topic = %s\n",sub_topic);
	printf("pub_topic = %s\n",pub_topic);

	unsigned int sub_qos = 1;
	MQTTMessage message;
	message.qos = 1;
	message.retained = 0;

	pwd_leng = sprintf(str_pwd,"%s%s%s",MqttDevice.ProdectKey,MqttDevice.DevGuid,MqttDevice.Licesence);
	str_pwd[pwd_leng] = '\0';
	sprintf(str_clientID,"device:%s:%s:%s",MqttDevice.ProdectKey,MqttDevice.DevGuid,MqttDevice.SdkVersion);
	mbedtls_sha1( str_pwd, pwd_leng, pwd_sha1_out);
	to_hex_str((unsigned char*)pwd_sha1_out, pwd_to_hex, 20);

	printf("pwd_leng = %d\n",pwd_leng);
	printf("str_pwd = %s\n",str_pwd);
	printf("pwd_to_hex = %s\n",pwd_to_hex);
	printf("str_clientID = %s\n",str_clientID);

	//connect
	connectData.MQTTVersion 		= 4;
	connectData.keepAliveInterval 	= 80;
	connectData.cleansession		= 0;
	connectData.clientID.cstring 	= str_clientID;
	connectData.username.cstring 	= "luowq_senor";
   	connectData.password.cstring 	= pwd_to_hex;

	while(MQTT_ctrl)
	{
		OS_MSleep(200);

		if(cal_set.MqttCon == MQTT_CACK)
		{
			NewNetwork(&network);
			MQTTClient(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
			rc = ConnectNetwork(&network, MqttDevice.address, MqttDevice.port);
			while (rc  != 0) {
				printf("Return code from network connect is %d\n", rc);
			}
			if ((rc = MQTTConnect(&client, &connectData)) != 0)
				printf("Return code from MQTT connect is %d\n", rc);
			else
				printf("MQTT Connected\n");
			cal_set.MqttCon = MQTT_DICACK;
		}
		if(cal_set.MqttSub == MQTT_CACK)
		{
			rc = MQTTSubscribe(&client, sub_topic, sub_qos, messageArrived);
			if (rc != 0)
				printf("Return code from MQTT subscribe is %d\n", rc);
			else
				printf("MQTT Subscrible is success\n");
			cal_set.MqttSub = MQTT_DICACK;
		}
		if(cal_set.MqttPub == MQTT_CACK)
		{
			message.payload = BbcPubSet;
			message.payloadlen = strlen(BbcPubSet);

			rc = MQTTPublish(&client, pub_topic, &message);
			if (rc != 0)
				printf("Return code from MQTT publish is %d\n", rc);
			else
				printf("MQTT publish is success\n");
			memset(BbcPubSet, 0, mqtt_buff_size);
			cal_set.MqttPub = MQTT_DICACK;
		}
		if(cal_set.MqttQuit== MQTT_CACK)
		{
			rc = MQTTUnsubscribe(&client, sub_topic);
			if (rc  != 0)
				printf("Return code from MQTT unsubscribe is %d\n", rc);
			rc = MQTTDisconnect(&client);
			if (rc != 0)
				printf("Return code from MQTT disconnect is %d\n", rc);
			cal_set.MqttQuit = MQTT_DICACK;
		}
		rc = MQTTYield(&client, 1000);
		if (rc != 0)
			printf("Return code from yield is %d\n", rc);
	}

	printf("mqtt_work_set end\n");
	OS_ThreadDelete(&MQTT_ctrl_task_thread);

}

int mqtt_ctrl_task_init()
{
	MQTT_ctrl = 1;
	if (OS_ThreadCreate(&MQTT_ctrl_task_thread,
		                "mqtt_build",
		                mqtt_work_set,
		               	NULL,
		                OS_THREAD_PRIO_APP,
		                MQTT_THREAD_STACK_SIZE) != OS_OK) {
		printf("thread create error\n");
		return -1;
	}
	printf("end\n");
	return 0;
}


