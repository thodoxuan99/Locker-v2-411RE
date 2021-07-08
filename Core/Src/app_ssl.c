/*
 * ssl.c
 *
 *  Created on: Jun 10, 2021
 *      Author: thodo
 */

#include <app_ssl.h>

#include "mbedtls/rsa.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/pk.h"
#include "mbedtls/error.h"

#include "base64.h"

#include "time.h"



// Declare variable
uint8_t ssl_at_command[5000];

char input[100];
char *output;

mbedtls_rsa_context rsa;
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_mpi N, E;

char *pers = "rsa-encrypt";

char cacert[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBxTCCAWugAwIBAgINAfD3nVndblD3QnNxUDAKBggqhkjOPQQDAjBEMQswCQYD\r\n"
"VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzERMA8G\r\n"
"1UEAxMIR1RTIExUU1IwHhcNMTgxMTAxMDAwMDQyWhcNNDIxMTAxMDAwMDQyWjBE\r\n"
"MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\r\n"
"QzERMA8GA1UEAxMIR1RTIExUU1IwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATN\r\n"
"8YyO2u+yCQoZdwAkUNv5c3dokfULfrA6QJgFV2XMuENtQZIG5HUOS6jFn8f0ySlV\r\n"
"eORCxqFyjDJyRn86d+Iko0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUw\r\n"
"AwEB/zAdBgNVHQ4EFgQUPv7/zFLrvzQ+PfNA0OQlsV+4u1IwCgYIKoZIzj0EAwID\r\n"
"SAAwRQIhAPKuf/VtBHqGw3TUwUIq7TfaExp3bH7bjCBmVXJupT9FAiBr0SmCtsuk\r\n"
"miGgpajjf/gFigGM34F9021bCWs1MbL0SA==\r\n"
"-----END CERTIFICATE-----\r\n"
;

char  clientcert[] ="-----BEGIN CERTIFICATE-----\r\n"
"MIIBeDCCAR2gAwIBAgIUGT2Vn1OyIltRkDwlG2XMdzLIezUwCgYIKoZIzj0EAwIw\r\n"
"ETEPMA0GA1UEAwwGdW51c2VkMB4XDTIxMDcwODAyMTg1MloXDTIxMDgwNzAyMTg1\r\n"
"MlowETEPMA0GA1UEAwwGdW51c2VkMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE\r\n"
"z38SVM0D+eLaRf/7WvvvN8xd0z3QOVo8Ux0FgLSChdOSINVpxL5RfwoQ263frzOv\r\n"
"lIVz7TVay9HOJoi1W5Zt2aNTMFEwHQYDVR0OBBYEFBBSRcML4b3UAHR//MX19nEj\r\n"
"PpixMB8GA1UdIwQYMBaAFBBSRcML4b3UAHR//MX19nEjPpixMA8GA1UdEwEB/wQF\r\n"
"MAMBAf8wCgYIKoZIzj0EAwIDSQAwRgIhAI+wtiFLYdr0my4l2WXpxqiOeLTbnXxC\r\n"
"lJpUBlrEJdfTAiEAtLSABy1f45gzmoI2md/4sLcgqCgvQHk7S5nCm0/VVws=\r\n"
"-----END CERTIFICATE-----\r\n";

char clientkey[] = "-----BEGIN EC PRIVATE KEY-----\r\n"
"MHcCAQEEIHi0YGM4hpHQM5mV2DyUvQST2hcjrPa9jM1s+N6q08X/oAoGCCqGSM49\r\n"
"AwEHoUQDQgAEz38SVM0D+eLaRf/7WvvvN8xd0z3QOVo8Ux0FgLSChdOSINVpxL5R\r\n"
"fwoQ263frzOvlIVz7TVay9HOJoi1W5Zt2Q==\r\n"
"-----END EC PRIVATE KEY-----\r\n";



SSL_Typedef ssl_config = {
		.ssl_ctx_index = 0,
		.sslversion = ALL,
		.authmode = 2,
		.ignorelocaltime = 1,
		.negotiatetime = 200,
		.ca_file ="\"ca_file.pem\"",
		.clientcert_file="\"clientcert_file.pem\"",
		.clientkey_file="\"clientkey_file.pem\""
};

void SSL_Init(){
	char test[10];
	MX_MBEDTLS_Init();
	SSL_Cert_List();
	SSL_Cert_Down("\"ca_file.pem\"", cacert);
	SSL_Cert_Down("\"clientcert_file.pem\"", clientcert);
	SSL_Cert_Down("\"clientkey_file.pem\"", clientkey);
	SSL_Configure_Ctx(&ssl_config);
}

void SSL_Configure_Ctx(SSL_Typedef *ssl){
	sprintf(ssl_at_command,"AT+CSSLCFG=\"sslversion\",%d,%d\r\n",ssl->ssl_ctx_index,(uint8_t)ssl->sslversion);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();

	sprintf(ssl_at_command,"AT+CSSLCFG=\"authmode\",%d,%d\r\n",ssl->ssl_ctx_index,(uint8_t)ssl->authmode);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();

	sprintf(ssl_at_command,"AT+CSSLCFG=\"ignorelocaltime\",%d,%d\r\n",ssl->ssl_ctx_index,(uint8_t)ssl->ignorelocaltime);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();

	sprintf(ssl_at_command,"AT+CSSLCFG=\"negotiatetime\",%d,%d\r\n",ssl->ssl_ctx_index,(uint8_t)ssl->negotiatetime);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();

	sprintf(ssl_at_command,"AT+CSSLCFG=\"cacert\",%d,%s\r\n",ssl->ssl_ctx_index,ssl->ca_file);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();

	sprintf(ssl_at_command,"AT+CSSLCFG=\"clientcert\",%d,%s\r\n",ssl->ssl_ctx_index,ssl->clientcert_file);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();

	sprintf(ssl_at_command,"AT+CSSLCFG=\"clientkey\",%d,%s\r\n",ssl->ssl_ctx_index,ssl->clientkey_file);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();
	return;
}

void SSL_Cert_Down(char *filename , char *certificate_data ){
	sprintf(ssl_at_command,"AT+CCERTDOWN=%s,%d\r\n",filename,strlen(certificate_data));
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();
	switch (Get_AT_Result().at_result) {
		case AT_INPUT:
			Clear_Reiceive_Buffer();
			UART_SIM7600_Transmit(certificate_data);
			Wait_For_Respone();
			break;
		default:
			break;
	}
}

void SSL_Cert_List(){
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit("AT+CCERTLIST\r\n");
	Wait_For_Respone();
	return;

}

void SSL_Cert_Delete(char *filename){
	sprintf(ssl_at_command,"AT+CCERTDELE=%s\r\n",filename);
	Clear_Reiceive_Buffer();
	UART_SIM7600_Transmit(ssl_at_command);
	Wait_For_Respone();
	return;
}

SSL_Typedef Get_Ssl_Config(){
	return ssl_config;
}

static char* mbedtlsError(int errnum) {
    static char buffer[200];
    mbedtls_strerror(errnum, buffer, sizeof(buffer));
    return buffer;
} // mbedtlsError


char* createGCPJWT(const char* projectId, uint32_t iat ,uint32_t exp ) {
	char _rc[100];
    char base64Header[100];
    const char header[] = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
    base64url_encode(
        (unsigned char *)header,   // Data to encode.
        strlen(header),            // Length of data to encode.
        base64Header);             // Base64 encoded data.

    char payload[100];
    sprintf(payload, "{\"iat\":%d,\"exp\":%d,\"aud\":\"%s\"}", iat, exp, projectId);

    char base64Payload[100];
    base64url_encode(
        (unsigned char *)payload,  // Data to encode.
        strlen(payload),           // Length of data to encode.
        base64Payload);            // Base64 encoded data.

    uint8_t headerAndPayload[800];
    sprintf((char*)headerAndPayload, "%s.%s", base64Header, base64Payload);

    // At this point we have created the header and payload parts, converted both to base64 and concatenated them
    // together as a single string.  Now we need to sign them using RSASSA

    mbedtls_pk_context pk_context;
    mbedtls_pk_init(&pk_context);
    int rc = mbedtls_pk_parse_key(&pk_context, clientkey, sizeof(clientkey), NULL, 0);
//    int rc = mbedtls_pk_parse_public_key(&pk_context, clientkey, strlen(clientkey));
    if (rc != 0) {
        sprintf(_rc,"Failed to mbedtls_pk_parse_key: %d (-0x%x): %s\n", rc, -rc, mbedtlsError(rc));
        UART_DEBUG_Transmit(_rc);
        return NULL;
    }
    uint8_t oBuf[5000];
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    const char* pers="MyEntropy";

    mbedtls_ctr_drbg_seed(
        &ctr_drbg,
        mbedtls_entropy_func,
        &entropy,
        (const unsigned char*)pers,
        strlen(pers));


    uint8_t digest[32];
    rc = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), headerAndPayload, strlen((char*)headerAndPayload), digest);
    if (rc != 0) {
    	sprintf(_rc,"Failed to mbedtls_md: %d (-0x%x): %s\n", rc, -rc, mbedtlsError(rc));
    	UART_DEBUG_Transmit(_rc);
        return NULL;
    }

    size_t retSize;
    rc = mbedtls_pk_sign(&pk_context, MBEDTLS_MD_SHA256, digest, sizeof(digest), oBuf, &retSize, mbedtls_ctr_drbg_random, &ctr_drbg);
    if (rc != 0) {
    	sprintf(_rc,"Failed to mbedtls_sign: %d (-0x%x): %s\n", rc, -rc, mbedtlsError(rc));
    	UART_DEBUG_Transmit(_rc);
        return NULL;
    }

    char base64Signature[600];
    base64url_encode((unsigned char *)oBuf, retSize, base64Signature);


    output = (char*)malloc(strlen((char*)headerAndPayload) + 1 + strlen((char*)base64Signature) + 1);

    sprintf(output, "\"%s.%s\"", headerAndPayload, base64Signature);

    mbedtls_pk_free(&pk_context);
    return output;
}

