#include "cweb.h"

int cweb_connect(struct cweb_connection* conn, char* address)
{
	int sock, e;
	struct sockaddr_in addr;
	struct addrinfo hints, *ainfo;

	memset(&hints, 0, sizeof (struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (e = getaddrinfo(address, "http", &hints, &ainfo))
	{
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

int cweb_close(struct cweb_connection* conn)
{
	return close(conn->socket);
}