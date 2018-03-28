#include <stdio.h>

#include "../lib/cweb.h"

struct cweb_connection conn;
struct cweb_request req;
struct cweb_response res;

int main(int argc, char *argv[])
{
	int n;

	if (cweb_connect(&conn, argv[1]))
	{
		perror("Failed http connect");
		return 1;
	}

	cweb_request(&conn, &req, "GET", "/");
	cweb_header(&req, "Connection", "close");

	while ((n = cweb_send(&conn, &req)) > 0);
	if (n < 0)
	{
		perror("Failed sending the http request");
		return 1;
	}

	while ((n = cweb_receive(&conn, &res)) > 0);
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

	if (cweb_close(&conn)) {
		perror("Failed closing the http connection");
		return 1;
	}

	return 0;
}