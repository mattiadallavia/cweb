#include "cweb.h"

int cweb_receive_head(struct cweb_connection* conn, struct cweb_response* res)
{
	int r;

	if (res->buf_len >= CWEB_BUF_MAX)
	{
		errno = EMSGSIZE; // message too long
		return -1;
	}

	r = read(conn->socket, res->buf + res->buf_len, 1);

	if (!r) return -1; // early end

	res->buf_len++;

	return ((res->buf[res->buf_len-1] != '\n')) || ((res->buf[res->buf_len-3] != '\n'));
}

int cweb_receive_body(struct cweb_connection* conn, struct cweb_response* res)
{
	int r;

	if (res->body_len >= res->body_size)
	{
		errno = EMSGSIZE; // message too long
		return -1;
	}

	r = read(conn->socket, res->body + res->body_len, res->body_size - res->body_len);
	res->body_len += r;

	res->body[res->body_len] = 0; // tappo

	return r;
}

int cweb_receive_chunk_head(struct cweb_connection* conn, struct cweb_response* res)
{
	int r;
	char* chunk_len_str = res->buf + res->buf_len;

	while (((r = read(conn->socket, res->buf + res->buf_len, 1)) > 0) &&
		    res->buf[res->buf_len] != '\n') res->buf_len++;

	if (!r) return -1; // early end

	res->buf[res->buf_len - 1] = 0; // tappo

	return strtol(chunk_len_str, 0, 16);
}

int cweb_receive_chunk(struct cweb_connection* conn, struct cweb_response* res, size_t n)
{
	int r;

	if (res->body_len >= res->body_size)
	{
		errno = EMSGSIZE; // message too long
		return -1;
	}

	r = read(conn->socket, res->body + res->body_len, n);
	if (!r) return -1; // early end

	res->body_len += r;
	res->body[res->body_len] = 0; // tappo

	return r;
}

int cweb_response_unpack(struct cweb_response* res)
{
	char* buf_next = res->buf;

	for (res->version = buf_next; *buf_next != ' '; buf_next++) ;
	*buf_next = 0; // tappo
	buf_next++;

	for (res->status = buf_next; *buf_next != ' '; buf_next++);
	*buf_next = 0; // tappo
	buf_next++;

	for (res->status_phrase = buf_next; *buf_next != '\r'; buf_next++);
	*buf_next = 0; // tappo
	buf_next+=2; // skip \n

	cweb_headers_unpack(res, buf_next - res->buf) + res->buf;

	return 0;
}