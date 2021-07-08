/*
 * ssl.h
 *
 *  Created on: Jun 10, 2021
 *      Author: thodo
 */

#ifndef INC_APP_SSL_H_
#define INC_APP_SSL_H_

#include "main.h"
#include "app_uart.h"

// Define type
typedef struct {
	uint8_t ssl_ctx_index;
	uint8_t sslversion;
	uint8_t authmode;
	uint8_t ignorelocaltime;
	uint8_t negotiatetime;
	char 	*ca_file;
	char	*clientcert_file;
	char	*clientkey_file;
}SSL_Typedef;

typedef enum{
	SSL3 = 0,
	TLS1_0,
	TLS1_1,
	TLS1_2,
	ALL
}SSL_Version;


typedef enum {
	SSL_INIT,
	SSL_CONFIGURE_CTX,
	SSL_CERT_DOWN,
	SSL_CERT_LIST,
	SSL_CERT_DELETE,
	SSL_IDLE
}SSL_State;





void SSL_Init();

void SSL_Configure_Ctx(SSL_Typedef *ssl);

void SSL_Cert_Down(char *filename , char *certificate_data);

void SSL_Cert_List();

void SSL_Cert_Delete(char *filename );

char* createGCPJWT(const char* projectId , uint32_t iat ,uint32_t exp );

SSL_Typedef Get_Ssl_Config();
#endif /* INC_APP_SSL_H_ */
