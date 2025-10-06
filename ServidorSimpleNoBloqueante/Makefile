# Variables
CC = gcc
CFLAGS = -Wall -g -I.

# Regla por defecto: compila ambos ejecutables
all: server client

# Compilar server
server: server.c
	$(CC) $(CFLAGS) -o $@ $<

# Compilar client
client: client.c
	$(CC) $(CFLAGS) -o $@ $<

# Limpiar archivos generados
clean:
	rm -f server client *.o
