client:
	gcc -o build/client src/client.c src/dhttp.c src/dhttp_headers.c

clean:
	rm build/*