CC = g++
MAIN = kscope
SRCS = src/*
FLAGS = -Wall

$(MAIN):
	$(CC) $(SRCS) $(FLAGS) -o $(MAIN)
