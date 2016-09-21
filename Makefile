all:
	$(CC) client-tutorial.c -o client-tutorial.x -I$(AWA_INSTALL_PATH)/include -L$(AWA_INSTALL_PATH)/lib -lawa
