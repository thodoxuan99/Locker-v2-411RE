/*
 * mqtt.c
 *
 *  Created on: Jun 10, 2021
 *      Author: thodo
 */

#include <app_mqtt.h>

//Result of respone from MQTT message

//State of MQTT
//MQTT_State prev_state;
//MQTT_State curr_state;

// AT command for MQTT
char atcommand[250];
//MQTT_Machine_TypeDef mqtt_state_machine[]={
//		{MQTT_START					, 		MQTT_Start				},
//		{MQTT_STOP					, 		MQTT_Stop				},
//		{MQTT_ACCQUIRE_CLIENT		, 		MQTT_Accquire_Client	},
//		{MQTT_RELEASE_CLIENT		, 		MQTT_Release_Client		},
//		{MQTT_SET_SSL				, 		MQTT_Will_Topic			},
//		{MQTT_WILL_TOPIC			, 		MQTT_Will_Message		},
//		{MQTT_CONNECT				, 		MQTT_Connect			},
//		{MQTT_DISCONNECT			, 		MQTT_Disonnect			},
//		{MQTT_TOPIC					, 		MQTT_Topic				},
//		{MQTT_PAYLOAD				, 		MQTT_Payload			},
//		{MQTT_PUBLISH				, 		MQTT_Publish			},
//		{MQTT_SUBCRIBE_TOPIC		, 		MQTT_Subcribe_Topic		},
//		{MQTT_SUBCRIBE				, 		MQTT_Subcribe			},
//		{MQTT_UNSUBCRIBE_TOPIC		, 		MQTT_UnSubcribe_Topic	},
//		{MQTT_UNSUBCRIBE			, 		MQTT_UnSubcribe			}
//};

// Default config for MQTT Client

void MQTT_Start(){
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit("AT+CMQTTSTART\r\n");
	Wait_For_Respone();
	return;
}

void MQTT_Stop(){
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit("AT+CMQTTSTOP\r\n");
	Wait_For_Respone();
	return;
}

void MQTT_Accquire_Client(MQTTClient_TypeDef* _mqtt_client){
	sprintf(atcommand,"AT+CMQTTACCQ=%d,%s\r\n",_mqtt_client->client_index,_mqtt_client->clientid);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Release_Client(MQTTClient_TypeDef* _mqtt_client){
	sprintf(atcommand,"AT+CMQTTREL=%d\r\n",_mqtt_client->client_index);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Set_Ssl(MQTTClient_TypeDef* _mqtt_client,SSL_Typedef *ssl){
	sprintf(atcommand,"AT+CMQTTSSLCFG=%d,%d\r\n",0,ssl->ssl_ctx_index);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Will_Topic(MQTTClient_TypeDef * _mqtt_client ,char* topic){
	sprintf(atcommand,"AT+CMQTTWILLTOPIC=%d,%d\r\n",_mqtt_client->client_index,strlen(topic));
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	switch (Get_AT_Result().at_result) {
		case AT_INPUT:
			Clear_Reiceive_Buffer();
			UART_SIM7600_Transmit(topic);
			Wait_For_Respone();
			break;
		default:
			break;
	}
	return;
}

void MQTT_Will_Message(MQTTClient_TypeDef *_mqtt_client  , char * message){
	uint8_t qos = 1;
	sprintf(atcommand,"AT+CMQTTWILLMSG=%d,%d,%d\r\n",_mqtt_client->client_index,strlen(message),qos);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	switch (Get_AT_Result().at_result) {
		case AT_INPUT:
			Clear_Reiceive_Buffer();
			UART_SIM7600_Transmit(message);
			Wait_For_Respone();
			break;
		default:
			break;
	}
	return;
}

void MQTT_Connect(MQTTClient_TypeDef *mqtt_client){
	sprintf(atcommand,"AT+CMQTTCONNECT=%d,%s,%d,%d,%s,%s\r\n",mqtt_client->client_index,mqtt_client->server_address,
			mqtt_client->keepAlive,mqtt_client->clean_session,mqtt_client->user,mqtt_client->pass);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Disonnect(MQTTClient_TypeDef *mqtt_client, uint8_t time_out){
	sprintf(atcommand,"AT+CMQTTDISC=%d,%d\r\n",mqtt_client->client_index,time_out);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Topic(MQTTClient_TypeDef *mqtt_client,  char * topic){
	sprintf(atcommand,"AT+CMQTTTOPIC=%d,%d\r\n",mqtt_client->client_index,strlen(topic));
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	switch (Get_AT_Result().at_result) {
		case AT_INPUT:
			Clear_Reiceive_Buffer();
			UART_SIM7600_Transmit(topic);
			Wait_For_Respone();
			break;
		default:
			break;
	}
	return;
}

void MQTT_Payload(MQTTClient_TypeDef *mqtt_client , char * payload){
	sprintf(atcommand,"AT+CMQTTPAYLOAD=%d,%d\r\n",mqtt_client->client_index,strlen(payload));
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	switch (Get_AT_Result().at_result) {
		case AT_INPUT:
			Clear_Reiceive_Buffer();
			UART_SIM7600_Transmit(payload);
			Wait_For_Respone();
			break;
		default:
			break;
	}
	return;
}

void MQTT_Publish(MQTTClient_TypeDef *mqtt_client , MESSAGE_TypeDef *message ){

	MQTT_Topic(mqtt_client, message->topic);
	MQTT_Payload(mqtt_client, message->payload);
	sprintf(atcommand,"AT+CMQTTPUB=%d,%d,%d,%d,%d\r\n",mqtt_client->client_index,message->qos,
				message->pub_timeout, message->retain, message->dup);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Subcribe_Topic(MQTTClient_TypeDef *mqtt_client , char *topic ){
	uint8_t qos = 1;
	sprintf(atcommand,"AT+CMQTTSUBTOPIC=%d,%d,%d\r\n",mqtt_client->client_index,strlen(topic),qos);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	switch (Get_AT_Result().at_result) {
		case AT_INPUT:
			Clear_Reiceive_Buffer();
			UART_SIM7600_Transmit(topic);
			Wait_For_Respone();
			break;
		default:
			break;
	}
	return;
}

void MQTT_Subcribe(MQTTClient_TypeDef *mqtt_client ){
	sprintf(atcommand,"AT+CMQTTSUB=%d\r\n",mqtt_client->client_index);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

uint8_t* MQTT_Get_Message_Subcribe(char* topic ,uint8_t* match_length){
	char pattern[250];
//	sprintf(pattern,"+CMQTTRXSTART: \\d+,\\d+,\\d+\r\n+CMQTTRXTOPIC: \\d+,\\d+\r\n\\W++CMQTTRXPAYLOAD: \\d+,\\d+\r\n\\W++CMQTTRXEND: \\d+\r\n");
	sprintf(pattern,"*");
	uint8_t *result = Get_Uart_Pattern(pattern,match_length);
	return result;
}

void MQTT_UnSubcribe_Topic(MQTTClient_TypeDef *mqtt_client , char *topic ){
	sprintf(atcommand,"AT+CMQTTUNSUBTOPIC=%d,%d\r\n",mqtt_client->client_index,strlen(topic));
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	switch (Get_AT_Result().at_result) {
		case AT_INPUT:
			UART_SIM7600_Transmit(topic);
			Wait_For_Respone();
			break;
		default:
			break;
	}
	return;
}

void MQTT_UnSubcribe(MQTTClient_TypeDef *mqtt_client){
	uint8_t dup = 0;
	sprintf(atcommand,"AT+CMQTTUNSUB=%d,%d\r\n",mqtt_client->client_index,dup);
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

