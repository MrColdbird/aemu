CC = gcc
CFLAGS = -fpack-struct -I.
OBJ = main.o user.o status.o
TARGET = pspnet_adhocctl_server

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	mv $(TARGET) ../dist/server
	cp index.php ../dist/server

clean:
#	rm -rf $(TARGET) *.o *~
	rm -rf *.o *~
