#include "cweb.h"

int cweb_request(struct cweb_connection* conn, struct cweb_request* req, char* method, char* path)
{
	req->version = "HTTP/1.0";
	req->method = method;
	req->path = path;

	cweb_header(req, "Host", conn->host);
	
	return 0;
}

int cweb_send(struct cweb_connection* conn, struct cweb_request* req)
{
	int w;
	
	if (!req->packed) cweb_request_pack(req); // auto pack before sending

	w = write(conn->socket, req->buf + req->sent, req->buf_len - req->sent);

	req->sent += w;
	return w;
}

int cweb_request_pack(struct cweb_request* req)
{
	sprintf(req->buf, "%s %s %s\r\n", req->method, req->path, req->version);

	req->buf_len = cweb_headers_pack(req, strlen(req->buf));

	req->packed = 1;
	return 0;
}