#include "cweb.h"

int cweb_request(struct cweb_connection* conn, struct cweb_request* req, char* method, char* path)
{
	req->method = method;
	req->path = path;

	return 0;
}

int cweb_send(struct cweb_connection* conn, struct cweb_request* req)
{
	int w = write(conn->socket, req->buf + req->sent, req->buf_len - req->sent);

	req->sent += w;
	return w;
}

int cweb_request_pack(struct cweb_request* req)
{
	sprintf(req->buf, "%s %s %s\r\n", req->method, req->path, req->version);

	req->buf_len = cweb_headers_pack(req, strlen(req->buf));

	return 0;
}