#include <stdio.h>

#include "../lib/cweb.h"

int main(int argc, char *argv[])
{
	struct cweb_client* client;
	struct cweb_response* res;
	char *host, *uri;

	if (argc != 3)
	{
		fprintf(stderr, "usage: client <host> <uri>\n");
		return 1;
	}

	host = argv[1];
	uri = argv[2];

	client = cweb_client_init();

	if (!(res = cweb_client_get(client, host, uri)))
	{
		perror("Client failed to perform get");
		return 1;
	}

	printf("%s %s %s\n", res->version, res->status, res->status_phrase);
	printf("--------------------------------------------------------------------------------\n");
	for (int i = 0; i < res->headers_num; i++) printf("%s: %s\n", res->headers[i].name, res->headers[i].value);
	printf("--------------------------------------------------------------------------------\n");
	// printf("%s\n", res->body);

	cweb_client_destroy(client);

	return 0;
}
