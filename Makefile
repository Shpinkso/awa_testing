all:
	$(CC) client-tutorial.c -o client-tutorial -I$(AWA_INSTALL_PATH)/include -L$(AWA_INSTALL_PATH)/lib -lawa
