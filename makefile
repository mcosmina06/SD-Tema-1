#***NUME Mandru, PRENUME Cosmina***
#****GRUPA 313CB, SERIA CB**********
CC = gcc
CFLAGS = -g -Wall 
HEADERS = 
OBJECTS = SD.o
EXEC = atm

.PHONY = default build clean

default: build

build: atm

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

run_task1: build
	./$(EXEC) ${ARGS}

run_task2: build
	./$(EXEC) ${ARGS}
run_task3: build
	./$(EXEC) ${ARGS}	



clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXEC)
