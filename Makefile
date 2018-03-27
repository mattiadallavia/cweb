client:
	gcc -o build/client src/client.c lib/dhttp.c lib/dhttp_request.c lib/dhttp_response.c lib/dhttp_headers.c

clean:
	rm build/*