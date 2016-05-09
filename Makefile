CC = g++
MAIN = kscope
SRCS = src/lexer.cpp
FLAGS = -Wall

$(MAIN):
	$(CC) $(SRCS) $(FLAGS) -o $(MAIN)
