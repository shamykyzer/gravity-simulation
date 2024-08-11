CC = gcc
CFLAGS = -Wall -g -Iinclude

SRC = src/main.o src/particle.o src/quadtree.o
OBJ = $(SRC:.c=.o)
TARGET = gravity_simulation

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lGL -lGLEW -lglfw -lm

clean:
	rm -f $(OBJ) $(TARGET)
