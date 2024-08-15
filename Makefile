CC = gcc
CFLAGS = -Wall -g -Iinclude
OBJS = src/main.o src/particle.o src/quadtree.o src/shader_utils.o src/controls.o src/integral_image.o
TARGET = gravity_simulation

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lGL -lGLEW -lglfw -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
