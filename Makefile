all: main

main: main.cpp
	g++ -std=c++23 -Wall -Werror -o mf main.cpp myfind314/Overseer.cpp

clean:
	rm -f mf
