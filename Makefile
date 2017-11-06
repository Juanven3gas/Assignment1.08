main_program: main.o Dungeon.o
	g++ main.o Dungeon.o -o main_program -lncurses

main.o: main.cpp Dungeon.h
	g++ -Wall -Werror -ggdb main.cpp -c

Dungeon.o: Dungeon.cpp Dungeon.h
	g++ -Wall -Werror -ggdb Dungeon.cpp -c

clean:
	rm -f *.o main_program