# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -Iinclude

# Libraries
LIBS = -lGL -lGLEW -lglfw -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Executable
EXEC = gravity_simulation

# Default target
all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXEC)
