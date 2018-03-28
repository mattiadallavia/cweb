client:
	gcc -o build/client src/client.c \
	       lib/cweb.c lib/cweb_client.c \
	       lib/cweb_request.c lib/cweb_response.c lib/cweb_headers.c

clean:
	rm build/*