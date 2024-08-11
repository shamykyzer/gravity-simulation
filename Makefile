CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lGL -lGLEW -lglfw -lm

SRCS = src/main.c src/particle.c src/quadtree.c src/shader_utils.c
OBJS = $(SRCS:.c=.o)

gravity_simulation: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) gravity_simulation
