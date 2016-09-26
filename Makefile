# Requires libsnd and fftw
TARGET=./bin/onset

OBJS=  \
objs/myfft.o \
objs/bands.o \
objs/processwav.o \
objs/ocsound.o \
objs/changes.o \
objs/onset.o

CC=g++
FLAGS_COMPILER= -c -O3 -Wall -Wno-deprecated
FLAGS_LINKER= -lm -lsndfile -lfftw3

all: $(OBJS) 
	@echo 'Linking: $@'
	$(CC) $(OBJS) $(FLAGS_LINKER) -o $(TARGET)


$(OBJS): ./objs/%.o : ./src/%.cpp
	@echo 'Compiling $< ...'
	$(CC) $(FLAGS_COMPILER) -o "./$@" "./$<"

clean:
	@echo 'Cleaning '
	rm -f ./objs/*
	@echo 'Cleaning '
	rm -f $(TARGET)
