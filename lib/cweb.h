#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define CWEB_HEADERS_MAX 100
#define CWEB_BUF_MAX 100000

struct cweb_connection {
	int socket;
	char* host;
};

struct cweb_header {
	char* name;
	char* value;
};

struct cweb_request {
	char* version;
	struct cweb_header headers[CWEB_HEADERS_MAX];
	size_t headers_num;
	char* method;
	char* path;
	char buf[CWEB_BUF_MAX];
	size_t buf_len;
	size_t sent;
	char packed;
};

struct cweb_response {
	char* version;
	struct cweb_header headers[CWEB_HEADERS_MAX];
	size_t headers_num;
	char* status;
	char* status_phrase;
	char* body;
	char buf[CWEB_BUF_MAX];
	size_t buf_len;
	char unpacked;
};

struct cweb_client {
	struct cweb_connection conn;
	struct cweb_request req;
	struct cweb_response res;
};

struct cweb_client* cweb_client_init();
struct cweb_response* cweb_client_get(struct cweb_client* client, char* host, char* uri);
int cweb_client_destroy(struct cweb_client* client);

int cweb_connect(struct cweb_connection* conn, char* address);
int cweb_connection_close(struct cweb_connection* conn);

int cweb_request(struct cweb_connection* conn, struct cweb_request* req, char* method, char* path);
int cweb_request_pack(struct cweb_request* req);
int cweb_send(struct cweb_connection* conn, struct cweb_request* req);

int cweb_response_unpack(struct cweb_response* res);
int cweb_receive(struct cweb_connection* conn, struct cweb_response* res);

char* cweb_header(void* r, char* name, char* val);
int cweb_headers_pack(struct cweb_request* req, unsigned int pos);
int cweb_headers_unpack(struct cweb_response* res, unsigned int pos);