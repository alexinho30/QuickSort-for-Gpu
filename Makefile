#definitions of flags
CC=gcc
LDLIBS=-lOpenCL -lm
DEPFLAGS=-MP -MD
CFLAGS=-Wall -g -I. -I./include/ $(DEPFLAGS)

#definitions of libraries
BINARY=main
CODEDIRS=. src
#INCLUDEDIRS=. ./include/

#definition of files
CFILES=$(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))
OBJECTS=$(patsubst %.c, %.o, $(CFILES))
DEPFILES=$(patsubst %.c, %.d, $(CFILES))

############
############

#rules
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDLIBS)

.PHONY : clean
clean: 
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)