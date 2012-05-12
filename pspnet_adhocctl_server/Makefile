CC = gcc
CFLAGS = -fpack-struct -I.
OBJ = main.o user.o status.o
TARGET = pspnet_adhocctl_server

LIBS = -lsqlite3

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)
	mv $(TARGET) ../dist/server
	cp database.db ../dist/server
	cp -r www ../dist/server

clean:
#	rm -rf $(TARGET) *.o *~
	rm -rf *.o *~
