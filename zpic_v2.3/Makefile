# MPI Compiler
CC = mpicc

# FLAGS
CFLAGS = -Ofast -g -std=c99 -pedantic -Wall -ffast-math

LDFLAGS = -lm

# Source files in src/ directory
SOURCE = src/current.c src/emf.c src/particles.c src/random.c \
         src/timer.c src/main.c src/simulation.c src/zdf.c

TARGET = zpic_mpi
OBJ = $(SOURCE:.c=.o)

all :  $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

src/%.o : src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean