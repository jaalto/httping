/* Released under GPLv2 with exception for the OpenSSL library. See license.txt */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifndef NO_SSL
#include <openssl/ssl.h>
#include "mssl.h"
#endif

#include "gen.h"
#include "http.h"
#include "io.h"
#include "str.h"
#include "utils.h"

int get_HTTP_headers(int socket_h, SSL *ssl_h, char **headers, int *overflow, int timeout)
{
	int len_in=0, len=4096;
	char *buffer = (char *)malloc(len);
	int rc = RC_OK;

	*headers = NULL;

	memset(buffer, 0x00, len);

	for(;;)
	{
		int rrc;
		int now_n = (len - len_in) - 1;

#ifndef NO_SSL
		if (ssl_h)
			rrc = SSL_read(ssl_h, &buffer[len_in], now_n);
		else
#endif
			rrc = read_to(socket_h, &buffer[len_in], now_n, timeout);
		if (rrc == 0 || rrc == RC_SHORTREAD)	/* socket closed before request was read? */
		{
			rc = RC_SHORTREAD;
			break;
		}
		else if (rrc == RC_TIMEOUT)		/* timeout */
		{
			free(buffer);
			return RC_TIMEOUT;
		}

		len_in += rrc;

		buffer[len_in] = 0x00;
		if (strstr(buffer, "\r\n\r\n") != NULL)
			break;

		if (len_in == (len - 1))
		{
			len <<= 1;
			buffer = (char *)realloc(buffer, len);
		}
	}

	*headers = buffer;

	char *term = strstr(buffer, "\r\n\r\n");
	if (term)
		*overflow = len_in - (term - buffer + 4);
	else
		*overflow = 0;

	return rc;
}
