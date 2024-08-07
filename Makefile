CC = gcc
CFLAGS = -Wall -g -Iinclude

OBJDIR = obj
SRCDIR = src

OBJS = $(OBJDIR)/main.o $(OBJDIR)/particle.o $(OBJDIR)/quadtree.o

all: gravity_simulation

$(OBJDIR)/main.o: $(SRCDIR)/main.c include/particle.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(OBJDIR)/particle.o: $(SRCDIR)/particle.c include/particle.h include/quadtree.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/particle.c -o $(OBJDIR)/particle.o

$(OBJDIR)/quadtree.o: $(SRCDIR)/quadtree.c include/quadtree.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/quadtree.c -o $(OBJDIR)/quadtree.o

gravity_simulation: $(OBJS)
	$(CC) $(CFLAGS) -o gravity_simulation $(OBJS) -lGL -lGLEW -lglfw -lm

clean:
	rm -rf $(OBJDIR) gravity_simulation
