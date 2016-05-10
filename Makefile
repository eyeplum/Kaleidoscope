CC = clang++
MAIN = kscope
SRCS = src/*.cpp
FLAGS = -Wall -g -std=c++11 -stdlib=libc++
INCS = -I/usr/local/include/

$(MAIN):
	$(CC) $(SRCS) $(FLAGS) $(INCS) -o $(MAIN)

