#include "cweb.h"

char* cweb_header(void* r, char* name, char* val)
{
	struct cweb_request* re = (struct cweb_request*) r;

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

int cweb_headers_pack(struct cweb_request* req, unsigned int pos)
{
	char* buf_next = req->buf + pos;

	for (int i = 0; i < req->headers_num; i++)
	{
		sprintf(buf_next, "%s: %s\r\n", req->headers[i].name, req->headers[i].value);
		buf_next += strlen(buf_next);
	}

	sprintf(buf_next, "\r\n");

	return buf_next - req->buf + 2;
}

int cweb_headers_unpack(struct cweb_response* res, unsigned int pos)
{
	char* buf_next = res->buf + pos;

	while (*buf_next != '\r')
	{
		for (res->headers[res->headers_num].name = buf_next; *buf_next != ':'; buf_next++);
		*buf_next = 0;
		buf_next+=2; // skip space

		for (res->headers[res->headers_num].value = buf_next; *buf_next != '\r'; buf_next++);
		*buf_next = 0;
		buf_next+=2; // skip \n

		res->headers_num++;
	}

	return buf_next - res->buf + 2;
}