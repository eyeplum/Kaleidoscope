CC = clang++
MAIN = kscope
SRCS = src/*
FLAGS = -Wall -g -std=c++11 -stdlib=libc++

$(MAIN):
	$(CC) $(SRCS) $(FLAGS) -o $(MAIN)
