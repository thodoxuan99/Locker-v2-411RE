/*
 * mqtt.h
 *
 *  Created on: Jun 10, 2021
 *      Author: thodo
 */

#ifndef INC_APP_MQTT_H_
#define INC_APP_MQTT_H_


#include <app_ssl.h>
#include "app_uart.h"

#define kClientidSize 	2
#define kTopicSize 		2

typedef struct {
	  uint8_t 	client_index;
	  char  	*clientid;
	  char		*server_address;
	  char 		*user;
	  char 		*pass;
	  uint8_t 	clean_session;
	  uint8_t 	keepAlive;
}MQTTClient_TypeDef;


typedef struct {
	  char *topic;
	  char *payload;
	  uint8_t qos;
	  uint8_t pub_timeout;
	  uint8_t retain;
	  uint8_t dup;
}MESSAGE_TypeDef;




//typedef enum{
//	MQTT_START=0,
//	MQTT_STOP,
//	MQTT_ACCQUIRE_CLIENT,
//	MQTT_RELEASE_CLIENT,
//	MQTT_SET_SSL,
//	MQTT_WILL_TOPIC,
//	MQTT_CONNECT,
//	MQTT_DISCONNECT,
//	MQTT_TOPIC,
//	MQTT_PAYLOAD,
//	MQTT_PUBLISH,
//	MQTT_SUBCRIBE_TOPIC,
//	MQTT_SUBCRIBE,
//	MQTT_UNSUBCRIBE_TOPIC,
//	MQTT_UNSUBCRIBE
//}MQTT_State;

//typedef struct{
//	MQTT_State mqtt_state;
//	void(*func)(void);
//}MQTT_Machine_TypeDef;




void MQTT_Start();

void MQTT_Stop();

void MQTT_Accquire_Client(MQTTClient_TypeDef *mqtt_client);

void MQTT_Release_Client(MQTTClient_TypeDef *mqtt_client);

void MQTT_Set_Ssl(MQTTClient_TypeDef *mqtt_client,SSL_Typedef *ssl);

void MQTT_Will_Topic(MQTTClient_TypeDef *mqtt_client ,  char * topic);

void MQTT_Will_Message( MQTTClient_TypeDef *mqtt_client , char * message);

void MQTT_Connect(MQTTClient_TypeDef *mqtt_client);

void MQTT_Disonnect(MQTTClient_TypeDef *mqtt_client , uint8_t time_out);

void MQTT_Topic(MQTTClient_TypeDef *mqtt_client,  char * topic);

void MQTT_Payload(MQTTClient_TypeDef *mqtt_client ,  char * payload);

void MQTT_Publish(MQTTClient_TypeDef *mqtt_client, MESSAGE_TypeDef *message );

void MQTT_Subcribe_Topic(MQTTClient_TypeDef *mqtt_client , char *topic );

void MQTT_Subcribe(MQTTClient_TypeDef *mqtt_client );

uint8_t* MQTT_Get_Message_Subcribe(char* topic ,uint8_t* match_length);

void MQTT_UnSubcribe_Topic(MQTTClient_TypeDef *mqtt_client, char *topic );

void MQTT_UnSubcribe(MQTTClient_TypeDef *mqtt_client);




#endif /* INC_APP_MQTT_H_ */
