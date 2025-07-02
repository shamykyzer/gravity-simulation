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

TEST_OBJ = tests/test_quadtree.o src/quadtree.o
TEST_BIN = tests/test_quadtree
TEST_LDFLAGS = -lm

$(TEST_BIN): $(TEST_OBJ)
	$(CC) -o $@ $(TEST_OBJ) $(TEST_LDFLAGS)

tests/test_quadtree.o: tests/test_quadtree.c include/quadtree.h include/particle_struct.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f src/*.o $(OBJ) gravity_simulation $(TEST_BIN) tests/*.o
