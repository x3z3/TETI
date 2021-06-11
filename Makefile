CC = gcc

en:
	$(CC) src/utils.c src/main_encrypt.c -o encrypt

de:
	$(CC) src/utils.c src/main_decrypt.c -o decrypt
