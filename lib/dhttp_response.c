#include "dhttp.h"

int dhttp_receive(struct dhttp_connection* conn, struct dhttp_response* res)
{
	int r = read(conn->socket, res->buf + res->buf_len, DHTTP_BUF_MAX - res->buf_len);
	res->buf_len += r;

	if (res->buf_len >= DHTTP_BUF_MAX)
	{
		errno = EMSGSIZE; // message too long
		return -1;
	}

	if (!res->unpacked) dhttp_response_unpack(res); // auto unpack when finished
	
	return r;
}

int dhttp_response_unpack(struct dhttp_response* res)
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

	res->body = dhttp_headers_unpack(res, buf_next - res->buf) + res->buf;

	res->unpacked = 1;
	return 0;
}