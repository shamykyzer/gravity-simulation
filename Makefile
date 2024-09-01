CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lGL -lGLEW -lglfw -lm -lmpi
OBJ = src/main.o src/particle.o src/quadtree.o src/shader_utils.o src/controls.o

gravity_simulation: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

src/main.o: src/main.c include/particle.h include/shader_utils.h include/controls.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/particle.o: src/particle.c include/particle.h include/quadtree.h include/particle_struct.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/quadtree.o: src/quadtree.c include/quadtree.h include/particle_struct.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/shader_utils.o: src/shader_utils.c include/shader_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/controls.o: src/controls.c include/controls.h include/particle.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o gravity_simulation