/*
 * mqtt.c
 *
 *  Created on: Jun 10, 2021
 *      Author: thodo
 */

#include <app_mqtt.h>

/*Server_type
Default : TCP
1		: SSL/TLS*/
#define SERVER_TYPE 1

#define DEFAULT_ID 	123456
//State of MQTT
//MQTT_State prev_state;
//MQTT_State curr_state;

// AT command for MQTT
char atcommand[250];

//Topic for subcribe
char sub_topic1[50];

//Topic for Publish
char pub_topic[50];

//Message from Subcribe
uint8_t buffer_reiceive_message[RXBUFFERSIZE];
uint8_t mess_receive_subcribe[LEN_MESSAGE_SUBCRIBE];

//Payload Pattern for finding subcribe message
char payload_pattern[50];
char payload_cmp[50];

MQTTClient_TypeDef mqtt_client = {
		.client_index = 0,
		.clientid = "\"Locker-v2\"",
		.server_address = "\"tcp://e6eb5b88-internet-facing-dafbf1a33e7f1f74.elb.us-east-1.amazonaws.com:8883\"",
		.user = "\"logUser\"",
		.pass ="\"logPwd\"",
		.keepAlive = 60,
		.clean_session = 0
};

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
void MQTT_Init(){
	sprintf(sub_topic1,"%s/set/states",DEFAULT_ID);
	sprintf(pub_topic,"%s/get/events",DEFAULT_ID);
}

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

void MQTT_Accquire_Client(){
	// SSL Server
	sprintf(atcommand,"AT+CMQTTACCQ=%d,%s,%d\r\n",mqtt_client.client_index,mqtt_client.clientid,SERVER_TYPE);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Release_Client(){
	sprintf(atcommand,"AT+CMQTTREL=%d\r\n",mqtt_client.client_index);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Set_Ssl(){
	SSL_Typedef ssl = Get_Ssl_Config();
	sprintf(atcommand,"AT+CMQTTSSLCFG=%d,%d\r\n",mqtt_client.client_index,ssl.ssl_ctx_index);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Will_Topic(char* topic){
	sprintf(atcommand,"AT+CMQTTWILLTOPIC=%d,%d\r\n",mqtt_client.client_index,strlen(topic));
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

void MQTT_Will_Message( char * message){
	uint8_t qos = 1;
	sprintf(atcommand,"AT+CMQTTWILLMSG=%d,%d,%d\r\n",mqtt_client.client_index,strlen(message),qos);
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

void MQTT_Connect(){
	sprintf(atcommand,"AT+CMQTTCONNECT=%d,%s,%d,%d,%s,%s\r\n",mqtt_client.client_index,mqtt_client.server_address,
			mqtt_client.keepAlive,mqtt_client.clean_session,mqtt_client.user,mqtt_client.pass);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Disonnect(uint8_t time_out){
	sprintf(atcommand,"AT+CMQTTDISC=%d,%d\r\n",mqtt_client.client_index,time_out);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Topic( char * topic){
	sprintf(atcommand,"AT+CMQTTTOPIC=%d,%d\r\n",mqtt_client.client_index,strlen(topic));
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

void MQTT_Payload( char * payload){
	sprintf(atcommand,"AT+CMQTTPAYLOAD=%d,%d\r\n",mqtt_client.client_index,strlen(payload));
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

void MQTT_Publish( MESSAGE_TypeDef *message ){

	MQTT_Topic(message->topic);
	MQTT_Payload(message->payload);
	sprintf(atcommand,"AT+CMQTTPUB=%d,%d,%d,%d,%d\r\n",mqtt_client.client_index,message->qos,
				message->pub_timeout, message->retain, message->dup);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void MQTT_Subcribe_Topic( char *topic ){
	uint8_t qos = 1;
	sprintf(atcommand,"AT+CMQTTSUBTOPIC=%d,%d,%d\r\n",mqtt_client.client_index,strlen(topic),qos);
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

void MQTT_Subcribe(){
	sprintf(atcommand,"AT+CMQTTSUB=%d\r\n",mqtt_client.client_index);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

FlagStatus MQTT_Message_Subcribe_Checking(char *topic){
	sprintf(payload_pattern,"%s\r\n+CMQTTRXPAYLOAD: %d,%d\r\n",topic,mqtt_client.client_index,LEN_MESSAGE_SUBCRIBE);
	uint8_t size = strlen(payload_pattern);
	if(UART_SIM7600_Received_Buffer_Available()){
		for (int var = 1; var < size; ++var) {
			payload_cmp[var-1]=payload_cmp[var];
		}
		payload_cmp[size-1]=UART_SIM7600_Read_Received_Buffer();
//		UART_DEBUG_Transmit_Size(payload_cmp+size-1, 1);
		for (int var = 0; var < size; ++var) {
			if(payload_cmp[var]!=payload_pattern[var]){
				return RESET;
			}
		}
		return SET;
	}
	else{
		return RESET;
	}
}

uint8_t* MQTT_Get_Message_Subcribe(uint8_t * payload_length){
	uint8_t var = 0;
	for (; var < LEN_MESSAGE_SUBCRIBE;) {
		if(UART_SIM7600_Received_Buffer_Available()){
			mess_receive_subcribe[var++]=UART_SIM7600_Read_Received_Buffer();
		}
	}
	*payload_length = LEN_MESSAGE_SUBCRIBE;
	return mess_receive_subcribe;
}

void MQTT_UnSubcribe_Topic( char *topic ){
	sprintf(atcommand,"AT+CMQTTUNSUBTOPIC=%d,%d\r\n",mqtt_client.client_index,strlen(topic));
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

void MQTT_UnSubcribe(){
	uint8_t dup = 0;
	sprintf(atcommand,"AT+CMQTTUNSUB=%d,%d\r\n",mqtt_client.client_index,dup);
	UART_SIM7600_Transmit(atcommand);
	Wait_For_Respone();
	return;
}

void Clear_Buffer_Receive_Message(){
	memset(buffer_reiceive_message,0,RXBUFFERSIZE);
	return;
}
