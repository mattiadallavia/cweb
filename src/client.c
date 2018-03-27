#include <stdio.h>

#include "dhttp.h"

struct dhttp_connection conn;
struct dhttp_request req;
struct dhttp_response res;

int main(int argc, char *argv[])
{
	int n;

	if (dhttp_connect(&conn, argv[1]))
	{
		perror("Failed http connect");
		return 1;
	}

	dhttp_request(&conn, &req, "GET", "/");
	dhttp_header(&req, "Connection", "close");

	while ((n = dhttp_send(&conn, &req)) > 0);
	if (n < 0)
	{
		perror("Failed sending the http request");
		return 1;
	}

	while ((n = dhttp_receive(&conn, &res)) > 0);
	if (n < 0)
	{
		perror("Failed receiving the http response");
		return 1;
	}

	printf("%s\n", res.buf);

	if (dhttp_close(&conn)) {
		perror("Failed closing the http connection");
		return 1;
	}

	// printf("%n\n", req.buf);
	// for (int i = 0; i < req.headers_num; i++) printf("%s: %s\n", req.headers[i].name, req.headers[i].value);

	return 0;
}