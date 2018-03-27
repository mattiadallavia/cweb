client:
	gcc -o build/client src/client.c src/dhttp.c src/dhttp_request.c src/dhttp_response.c src/dhttp_headers.c

clean:
	rm build/*