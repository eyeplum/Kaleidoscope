CC = g++
MAIN = kscope
SRCS = lexer.cpp
FLAGS = -Wall

$(MAIN):
	$(CC) $(SRCS) $(FLAGS) -o $(MAIN)
