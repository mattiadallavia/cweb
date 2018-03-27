client:
	gcc -o build/client src/client.c src/dhttp.c

clean:
	rm build/*