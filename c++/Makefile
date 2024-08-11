LDFLAGS=-lSDL2 -lSDL2main
CFLAGS=-ggdb
CC=g++
OBJDIR=./obj
SRCDIR=./src

EXEC = snake

all: $(OBJDIR) $(EXEC)

$(EXEC): $(OBJDIR)/main.o $(OBJDIR)/game.o $(OBJDIR)/snake.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJDIR)/game.o: $(SRCDIR)/game.cpp $(SRCDIR)/game.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/snake.o: $(SRCDIR)/snake.cpp $(SRCDIR)/snake.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean: 
	rm obj/*

.PHONY:
	clean


