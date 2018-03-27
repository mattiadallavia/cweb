#include <stdio.h>

#include "dhttp.h"

struct dhttp_connection conn;
struct dhttp_request req;
struct dhttp_response res;

int main(int argc, char const *argv[])
{
	int s;

	if (dhttp_connect(&conn, "www.google.com"))
	{
		perror("Failed http connect");
		return 1;
	}

	dhttp_request(&conn, &req, "GET", "/");
	dhttp_header(&req, "Connection", "close");

	while ((s = dhttp_send(&conn, &req)) > 0);
	if (s < 0)
	{
		perror("Failed sending the http request");
		return 1;
	}

	if (dhttp_close(&conn)) {
		perror("Failed closing the http connection");
		return 1;
	}

	// printf("%s\n", req.buf);
	// for (int i = 0; i < req.headers_num; i++) printf("%s: %s\n", req.headers[i].name, req.headers[i].value);

	return 0;
}