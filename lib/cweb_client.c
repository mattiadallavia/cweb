#include "cweb.h"

struct cweb_client* cweb_client_init()
{
	return (struct cweb_client*) calloc(1, sizeof (struct cweb_client));
}

int cweb_client_destroy(struct cweb_client* client)
{
	if (client->res.body) free(client->res.body);
	free(client);
	return 0;
}

struct cweb_response* cweb_client_get(struct cweb_client* client, char* host, char* uri)
{
	int n, chunk_len, chunk_eff_len, chunk_read;
	char chunked = 0;
	char *body_len_str, *encoding_str;

	if (cweb_connect(&client->conn, host)) return 0;

	cweb_request(&client->conn, &client->req, "GET", uri);
	client->req.version = CWEB_HTTP_DEF;

	cweb_header(&client->req, "Host", client->conn.host);
	cweb_header(&client->req, "Accept-Encoding", "chunked");
	cweb_header(&client->req, "Connection", "close");

	cweb_request_pack(&client->req);

	while ((n = cweb_send(&client->conn, &client->req)) > 0);
	if (n < 0) return 0;

	while ((n = cweb_receive_head(&client->conn, &client->res)) > 0);
	if (n < 0) return 0;

	cweb_response_unpack(&client->res);

	body_len_str = cweb_header(&client->res, "Content-Length", 0);
	encoding_str = cweb_header(&client->res, "Transfer-Encoding", 0);
	chunked = encoding_str && !strcmp(encoding_str, "chunked");

	if (body_len_str) client->res.body_size = atoi(body_len_str) + 1; // tappo
	else client->res.body_size = CWEB_BODY_SIZE_DEF;

	if (chunked)
	{
		client->res.body_size = 0;

		while (1)
		{
			chunk_len = cweb_receive_chunk_head(&client->conn, &client->res);
			chunk_eff_len = chunk_len + 2; // additional /r/n
			client->res.body_size += chunk_eff_len;

			if (chunk_len == 0) break;
			if (chunk_len < 0) return 0;

			client->res.body = realloc(client->res.body, client->res.body_size);

			for (chunk_read = 0;
				(chunk_read < chunk_eff_len) &&
				((n = cweb_receive_chunk(&client->conn, &client->res, chunk_eff_len - chunk_read)) > 0);
				chunk_read += n);
			if (n < 0) return 0;

			printf("%d\n", chunk_len);
		}
	}
	else
	{
		client->res.body = malloc(client->res.body_size);

		while ((n = cweb_receive_body(&client->conn, &client->res)) > 0);
		if (n < 0) return 0;
	}

	if (cweb_close(&client->conn)) return 0;

	return &client->res;
}