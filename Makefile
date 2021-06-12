CC = gcc

en:
	$(CC) src/helper.c src/encrypt.c -o encrypt
	./encrypt
	rm -rf encrypt

de:
	$(CC) src/helper.c src/decrypt.c -o decrypt
	./decrypt
	rm -rf decrypt

clean:
	rm -rf encrypt decrypt
	> input.txt
