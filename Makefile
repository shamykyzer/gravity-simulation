OBJECTS = src/main.o src/particle.o src/quadtree.o src/shader_utils.o src/controls.o

gravity_simulation: $(OBJECTS)
	 gcc -o gravity_simulation $(OBJECTS) -lGL -lGLEW -lglfw -lm

src/main.o: src/main.c include/particle.h include/shader_utils.h include/controls.h
	 gcc -Wall -g -Iinclude -c -o src/main.o src/main.c

src/particle.o: src/particle.c include/particle.h include/quadtree.h include/particle_struct.h
	 gcc -Wall -g -Iinclude -c -o src/particle.o src/particle.c

src/quadtree.o: src/quadtree.c include/quadtree.h include/particle_struct.h
	 gcc -Wall -g -Iinclude -c -o src/quadtree.o src/quadtree.c

src/shader_utils.o: src/shader_utils.c include/shader_utils.h
	 gcc -Wall -g -Iinclude -c -o src/shader_utils.o src/shader_utils.c

src/controls.o: src/controls.c include/controls.h include/particle.h
	 gcc -Wall -g -Iinclude -c -o src/controls.o src/controls.c

clean:
	 rm -f src/*.o gravity_simulation
