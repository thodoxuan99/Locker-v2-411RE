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

char cacert[] ="-----BEGIN CERTIFICATE-----\r\n"
"MIIDZTCCAk2gAwIBAgIUCy5rmkXjWODdmEsVfGWVW1nWUccwDQYJKoZIhvcNAQEL\r\n"
"BQAwQjELMAkGA1UEBhMCQ04xIjAgBgNVBAoMGUVNUSBUZWNobm9sb2dpZXMgQ28u\r\n"
"LCBMdGQxDzANBgNVBAMMBkVNUSBDQTAeFw0yMTA3MTgxNDQ0MjVaFw0zMTA3MTYx\r\n"
"NDQ0MjVaMEIxCzAJBgNVBAYTAkNOMSIwIAYDVQQKDBlFTVEgVGVjaG5vbG9naWVz\r\n"
"IENvLiwgTHRkMQ8wDQYDVQQDDAZFTVEgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IB\r\n"
"DwAwggEKAoIBAQDVPRPkxxyxDbNVPE4Jhi8DhP5GPccfhRGMHVQw/Q/S0e5iBCT0\r\n"
"uNDDwZbPOR5aNzg4JrYjpYxs5YHHdR+iIC6O/tyXT9ffixet2JaSvcSS5ZM9fdOg\r\n"
"j89RfNmm8h5MnukkoJWx0HawLr7sLeUN6d9Yt9Xlsa6OWvFTRui261nRJi5RGKUV\r\n"
"NTd7zcacchBHKQnOxvcibmuXLH7U+iDMhffJ9z1IVhR6owvQOfbzsuj43+hhCuBn\r\n"
"1Tw20w/ODLLS4U7COwKOHg/t9WqZSPUaL/+4FRm3zHJIpjae7qvQpeJFyGpqYdf1\r\n"
"pQqf7vEZFOWfYKFtOgLFxHr6HEp6ghGXsMEDAgMBAAGjUzBRMB0GA1UdDgQWBBRs\r\n"
"zIHcI745cc9KHF2HjI2V6noEzzAfBgNVHSMEGDAWgBRszIHcI745cc9KHF2HjI2V\r\n"
"6noEzzAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQCJVMKeBn8c\r\n"
"1sY4BueUCJZZn2TfXOvzv2ae7g2wsUzxyc5HSSNyVEKl4xpvFxGFu3WzQ54bdNKs\r\n"
"F+XcjrnC5XJFoI7bweDa1Slkf+mtNAWEBI+3+hQYJGupRp0YmucQ0xbPd9cSFQ2+\r\n"
"jnNh7lrRjR4frCBegRPOZJfH71ubyXvTcj1Rk3hoQu4GPZtlFM6qQrDJnGzswAmb\r\n"
"7tcPB7Skt7EZOtDQ70eVBZOdhKS0KoWGytqxKXTg8vH2SZcYVZPXL96uHF1P0pHB\r\n"
"zA+xVF5vABXkINYBqWYDUj7Hczt54CE5uDGNJI899ehbZwPQro8QBSCosS0zrAzE\r\n"
"7gZb72yKS1Ne\r\n"
"-----END CERTIFICATE-----\r\n";


SSL_Typedef ssl_config = {
		.ssl_ctx_index = 0,
		.sslversion = ALL,
		.authmode = 1,
		.ignorelocaltime = 1,
		.negotiatetime = 200,
		.ca_file ="\"ca_file.pem\"",
		.clientcert_file="\"\"",
		.clientkey_file="\"\""
};

void SSL_Init(){
	MX_MBEDTLS_Init();
	SSL_Cert_Down("\"ca_file.pem\"", cacert);
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


//char* createGCPJWT(const char* projectId, unsigned long iat ,unsigned long exp ) {
//	char _rc[100];
//    char base64Header[100];
//    const char header[] = "{\"alg\":\"ES256\",\"typ\":\"JWT\"}";
//    base64url_encode(
//        (unsigned char *)header,   // Data to encode.
//        strlen(header),            // Length of data to encode.
//        base64Header);             // Base64 encoded data.
//
//    char payload[100];
//    sprintf(payload, "{\"iat\":%ld,\"exp\":%ld,\"aud\":\"%s\"}", iat, exp, projectId);
//    UART_DEBUG_Transmit(payload);
//
//    char base64Payload[100];
//    base64url_encode(
//        (unsigned char *)payload,  // Data to encode.
//        strlen(payload),           // Length of data to encode.
//        base64Payload);            // Base64 encoded data.
//
//    uint8_t headerAndPayload[800];
//    sprintf((char*)headerAndPayload, "%s.%s", base64Header, base64Payload);
//
//    // At this point we have created the header and payload parts, converted both to base64 and concatenated them
//    // together as a single string.  Now we need to sign them using RSASSA
//
//    mbedtls_pk_context pk_context;
//    mbedtls_pk_init(&pk_context);
//    int rc = mbedtls_pk_parse_key(&pk_context, clientkey, sizeof(clientkey), NULL, 0);
////    int rc = mbedtls_pk_parse_public_key(&pk_context, clientkey, strlen(clientkey));
//    if (rc != 0) {
//        sprintf(_rc,"Failed to mbedtls_pk_parse_key: %d (-0x%x): %s\n", rc, -rc, mbedtlsError(rc));
//        UART_DEBUG_Transmit(_rc);
//        return NULL;
//    }
//    uint8_t oBuf[5000];
//    mbedtls_entropy_context entropy;
//    mbedtls_ctr_drbg_context ctr_drbg;
//    mbedtls_ctr_drbg_init(&ctr_drbg);
//    mbedtls_entropy_init(&entropy);
//
//    const char* pers="MyEntropy";
//
//    mbedtls_ctr_drbg_seed(
//        &ctr_drbg,
//        mbedtls_entropy_func,
//        &entropy,
//        (const unsigned char*)pers,
//        strlen(pers));
//
//
//    uint8_t digest[32];
//    rc = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), headerAndPayload, strlen((char*)headerAndPayload), digest);
//    if (rc != 0) {
//    	sprintf(_rc,"Failed to mbedtls_md: %d (-0x%x): %s\n", rc, -rc, mbedtlsError(rc));
//    	UART_DEBUG_Transmit(_rc);
//        return NULL;
//    }
//
//    size_t retSize;
//    rc = mbedtls_pk_sign(&pk_context, MBEDTLS_MD_SHA256, digest, sizeof(digest), oBuf, &retSize, mbedtls_ctr_drbg_random, &ctr_drbg);
//    if (rc != 0) {
//    	sprintf(_rc,"Failed to mbedtls_sign: %d (-0x%x): %s\n", rc, -rc, mbedtlsError(rc));
//    	UART_DEBUG_Transmit(_rc);
//        return NULL;
//    }
//
//    char base64Signature[600];
//    base64url_encode((unsigned char *)oBuf, retSize, base64Signature);
//
//
//    output = (char*)malloc(strlen((char*)headerAndPayload) + 1 + strlen((char*)base64Signature) + 1);
//
//    sprintf(output, "\"%s.%s\"", headerAndPayload, base64Signature);
//
//    mbedtls_pk_free(&pk_context);
//    return output;
//}

