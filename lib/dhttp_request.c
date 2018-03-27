#include "dhttp.h"

int dhttp_request(struct dhttp_connection* conn, struct dhttp_request* req, char* method, char* path)
{
	req->version = "HTTP/1.0";
	req->method = method;
	req->path = path;

	dhttp_header(req, "Host", conn->host);
	
	return 0;
}

int dhttp_send(struct dhttp_connection* conn, struct dhttp_request* req)
{
	int w;

	if (!req->packed) dhttp_request_pack(req); // auto pack before sending

	w = write(conn->socket, req->buf + req->sent, req->buf_len - req->sent);

	req->sent += w;
	return w;
}

int dhttp_request_pack(struct dhttp_request* req)
{
	sprintf(req->buf, "%s %s %s\r\n", req->method, req->path, req->version);

	req->buf_len = dhttp_headers_pack(req, strlen(req->buf));

	req->packed = 1;
	return 0;
}