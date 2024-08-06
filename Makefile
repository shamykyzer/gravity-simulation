# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -Iinclude

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = .

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Executable
EXECUTABLE = gravity_simulation

# Libraries
LIBS = -lGL -lGLEW -lglfw -lm

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINDIR)/$(EXECUTABLE) $(OBJECTS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)/$(EXECUTABLE)
