#include "dhttp.h"

#include <stdio.h>

int dhttp_connect(struct dhttp_connection* conn, char* address)
{
	int sock;
	struct sockaddr_in addr;
	struct addrinfo hints, *ainfo;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(address, "http", &hints, &ainfo))
	{
		freeaddrinfo(ainfo);
		errno = ENXIO; // No such device or address
		return -1;
	}
	
	if ((sock = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol)) < 0)
	{
		freeaddrinfo(ainfo);
		return -1;
	}

	if ((connect(sock, ainfo->ai_addr, ainfo->ai_addrlen)) < 0)
	{
		freeaddrinfo(ainfo);
		return -1;
	}

	freeaddrinfo(ainfo);
	conn->socket = sock;
	conn->host = address;

	return 0;
}

int dhttp_close(struct dhttp_connection* conn)
{
	return close(conn->socket);
}

int dhttp_request(struct dhttp_connection* conn, struct dhttp_request* req, char* method, char* path)
{
	req->version = "HTTP/1.0";
	req->method = method;
	req->path = path;
	req->buf_len = 0;
	req->sent = 0;
	req->packed = 0;

	dhttp_header(req, "Host", conn->host);
	
	return 0;
}

int dhttp_request_pack(struct dhttp_request* req)
{
	sprintf(req->buf + req->buf_len, "%s %s %s\r\n", req->method, req->path, req->version);
	req->buf_len += strlen(req->buf + req->buf_len);

	for (int i = 0; i < req->headers_num; i++)
	{
		sprintf(req->buf + req->buf_len, "%s: %s\r\n", req->headers[i].name, req->headers[i].value);
		req->buf_len += strlen(req->buf + req->buf_len);
	}

	sprintf(req->buf + req->buf_len, "\r\n");
	req->buf_len += strlen(req->buf + req->buf_len);

	req->packed = 1;

	return 0;
}

int dhttp_send(struct dhttp_connection* conn, struct dhttp_request* req)
{
	if (!req->packed) dhttp_request_pack(req);

	int w = write(conn->socket, req->buf, req->buf_len - req->sent);

	req->sent += w;
	return w;
}

int dhttp_receive(struct dhttp_connection* conn, struct dhttp_response* res)
{
	return 0;
}

char* dhttp_header(void* r, char* name, char* val)
{
	struct dhttp_request* re = (struct dhttp_request*) r;

	for (int i = 0; i < re->headers_num; i++)
	{
		if (!strcmp(name, re->headers[i].name))
		{
			if (val) return re->headers[i].value = val;
			else return re->headers[i].value;
		}
	}

	// header not already present
	if (val)
	{
		re->headers[re->headers_num].name = name;
		return re->headers[re->headers_num++].value = val;
	} else return 0;
}