#include "cweb.h"

int cweb_receive(struct cweb_connection* conn, struct cweb_response* res)
{
	int r = read(conn->socket, res->buf + res->buf_len, CWEB_BUF_MAX - res->buf_len);
	res->buf_len += r;

	if (res->buf_len >= CWEB_BUF_MAX)
	{
		errno = EMSGSIZE; // message too long
		return -1;
	}

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

	res->body = cweb_headers_unpack(res, buf_next - res->buf) + res->buf;

	return 0;
}