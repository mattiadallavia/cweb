#include <stdio.h>

#include "../lib/dhttp.h"

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

	printf("%s %s %s\n", res.version, res.status, res.status_phrase);
	printf("--------------------------------------------------------------------------------\n");
	for (int i = 0; i < res.headers_num; i++) printf("%s: %s\n", res.headers[i].name, res.headers[i].value);
	printf("--------------------------------------------------------------------------------\n");
	printf("%s\n", res.body);

	if (dhttp_close(&conn)) {
		perror("Failed closing the http connection");
		return 1;
	}

	return 0;
}