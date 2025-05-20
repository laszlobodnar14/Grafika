CC = gcc
CFLAGS = -Wall -std=c11 -I.
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32

SRC = main.c camera.c texture.c room.c
OBJ = $(SRC:.c=.o)
TARGET = szoba.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(OBJ) $(TARGET)
